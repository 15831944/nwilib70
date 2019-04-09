#include "stdafx.h"
#include "assetlev.h"
#include "oradate.h"
#include "nwivar.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CAssetLev, CCapBond)
CAssetLev::CAssetLev(CAssetLev &AssetLev) : CCapBond(AssetLev)
{
	SetupLevInfo(AssetLev.GetValueDate(), AssetLev.GetDate(), AssetLev.GetLevRateBasis(), 
				AssetLev.GetLevRate(), AssetLev.GetRepoFormula());
}

void CAssetLev::SetupLevInfo(LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR LevRateBasis, 
							 double LevRate, LPCTSTR RepoFormula)
{
	CQData QData;

	m_LevInterest = 0;
	m_LevAmount = 0;
	m_bLev = TRUE;
	if(RepoFormula && strstr(RepoFormula, DAILYREPO))
		QData.SetString(m_RepoFormula, RepoFormula);
	else
		m_RepoFormula.Empty();

	QData.SetString(m_LevRateBasis, LevRateBasis);
	SetLevRate(m_RepoFormula.IsEmpty() ? LevRate : 0);

	if(m_RepoFormula.IsEmpty()) // Not Daily Float Repo
		m_nLevDays = ComputeDays(ValueDate, Date, LevRateBasis, m_nLevYearBase);
	else
		m_nLevDays = 0;

	ComputeLevAmount(TRUE);
	ComputeLevInterest();
}

double CAssetLev::ComputeValue(double NomAmount, double Price)
{
	if(strcmp(GetType(), LEVERAGE) == 0)
	{
		if(NomAmount > 0)
			return NomAmount;
		else
			return GetNomAmount();		
	}
	else
		return CAmortBond::ComputeValue(NomAmount, Price);
}

double CAssetLev::GetDailyLevInterest(LPCTSTR FromDate, LPCTSTR ToDate, double LevRate)
{
	double Interest = 0;
	int Days = 0, Days2 = 0;

	if(!m_bLev)
		return 0;

	if(!GetRepoFormula().IsEmpty())
	{
		// Weighted Lev Rate
		Interest = GetOutput(CBond::ComputeInterest(GetLevAmount(), LevRate, FromDate, ToDate, GetLevRateBasis(), Days, Days2));
		SetLevRate((m_LevRate*m_nLevDays + LevRate*(Days + Days2))/(m_nLevDays + (Days + Days2))); 
		AddLevDays(Days);
		AddLevDays(Days + Days2);
		AddLevInterest(Interest);
		if(m_RepoFormula == DAILYREPOA)
			SetLevAmount(GetLevAmount() + Interest);
	}

	return Interest;
}

double CAssetLev::GetDailyLevInterest(double Amount, LPCTSTR FromDate, LPCTSTR ToDate, double LevRate)
{
	SetLevAmount(Amount);
	return GetDailyLevInterest(FromDate, ToDate, LevRate);
}

double CAssetLev::GetDailyLevInterest(COraLoader &OraLoader, LPCTSTR Portfolio, 
									  LPCTSTR TransNum, LPCTSTR InvNum)
{
	CQData QData;
	CString QPort, QTransNum, QInvNum;

	QPort = QData.GetQueryText(Portfolio);
	QTransNum = QData.GetQueryNumber(TransNum);
	QInvNum = QData.GetQueryNumber(InvNum);
	OraLoader.GetSql().Format("SELECT INT_FROM, "
				"LEAST(NVL(INT_TO, NVL(ACTUAL_VDATE, MATURITY_DATE)), NVL(ACTUAL_VDATE, MATURITY_DATE)), RATE "
				"FROM SEMAM.ALL_TICKET_INV_V A "
				"JOIN SEMAM.NW_REPO_RATES B ON (A.TRANS_NUM = B.TRANS_NUM) "
				"WHERE A.INV_NUM = %s AND A.TRANS_NUM = %s AND A.PORTFOLIO = %s "
				"AND A.TR_DESC = 'MATURITY' "
				"AND LEAST(NVL(INT_TO, NVL(ACTUAL_VDATE, MATURITY_DATE)), NVL(ACTUAL_VDATE, MATURITY_DATE)) "
				"<= NVL(ACTUAL_VDATE, MATURITY_DATE) ", (LPCTSTR) QInvNum, (LPCTSTR) QTransNum, (LPCTSTR) QPort);

	if(!OraLoader.Open())
		return 0;

	CString FromDate, ToDate;
	double Rate;

	SetLevRate();
	SetLevDays();
	SetLevInterest();
	while(!OraLoader.IsEOF())
	{				
		FromDate = OraLoader.GetDBString(0);
		ToDate = OraLoader.GetDBString(1);
		Rate = atof(OraLoader.GetDBString(2));
		GetDailyLevInterest(FromDate, ToDate, Rate);
		OraLoader.MoveNext();
	}

	return GetLevInterest();
}

double CAssetLev::GetDailyLevInterest(COraLoader &OraLoader, LPCTSTR Portfolio, 
						LPCTSTR TransNum, LPCTSTR FromDate, LPCTSTR ToDate)
{
	CQData QData;
	CString QFDate, QTDate, QPort, QTransNum;

	QFDate = QData.GetQueryDate(FromDate);
	QTDate = QData.GetQueryDate(ToDate);
	QPort = QData.GetQueryText(Portfolio);
	QTransNum = QData.GetQueryNumber(TransNum);

	OraLoader.GetSql().Format("SELECT "
		"DECODE(SIGN(INT_FROM - TO_DATE(%s)), 1, INT_FROM, TO_DATE(%s)), "
		"DECODE(SIGN(NVL(INT_TO, NVL(A.MATURITY_DATE, TO_DATE(%s) + 1)) - TO_DATE(%s)), "
		"1, NVL(A.MATURITY_DATE, TO_DATE(%s) + 1), INT_TO), RATE "
		"FROM SEMAM.NW_REPO_RATES A "
		"JOIN SEMAM.NW_TR_TICKETS B ON (A.TRANS_NUM = B.TRANS_NUM) "
		"WHERE A.PORTFOLIO = %s AND A.TRANS_NUM = %s "
		"AND DECODE(SIGN(INT_FROM - TO_DATE(%s)), 1, INT_FROM, TO_DATE(%s)) >= %s "
		"AND INT_FROM <= %s UNION ALL "
		"SELECT DECODE(SIGN(INT_FROM - TO_DATE(%s)), 1, INT_FROM, TO_DATE(%s)), "
		"DECODE(SIGN(NVL(INT_TO, NVL(A.MATURITY_DATE, TO_DATE(%s) + 1)) - TO_DATE(%s)), "
		"1, NVL(A.MATURITY_DATE, TO_DATE(%s) + 1), INT_TO), RATE "
		"FROM MNGACC.NW_REPO_RATES A "
		"JOIN MNGACC.NW_TR_TICKETS B ON (A.TRANS_NUM = B.TRANS_NUM) "
		"WHERE A.PORTFOLIO = %s AND A.TRANS_NUM = %s "
		"AND DECODE(SIGN(INT_FROM - TO_DATE(%s)), 1, INT_FROM, TO_DATE(%s)) >= %s "
		"AND INT_FROM <= %s ORDER BY 1 ", (LPCTSTR) QFDate, (LPCTSTR) QFDate, 
		(LPCTSTR) QTDate, (LPCTSTR) QTDate, (LPCTSTR) QTDate, (LPCTSTR) QPort, 
		(LPCTSTR) QTransNum, (LPCTSTR) QFDate, (LPCTSTR) QFDate, (LPCTSTR) QTDate, 
		(LPCTSTR) QFDate, (LPCTSTR) QFDate, (LPCTSTR) QTDate, (LPCTSTR) QTDate, 
		(LPCTSTR) QTDate, (LPCTSTR) QPort, (LPCTSTR) QTransNum, (LPCTSTR) QFDate, 
		(LPCTSTR) QFDate, (LPCTSTR) QTDate);

	if(!OraLoader.Open())
		return 0;

	CString From, To;
	double Rate;

	SetLevRate();
	SetLevDays();
	SetLevInterest();
	while(!OraLoader.IsEOF())
	{				
		From = OraLoader.GetDBString(0);
		To = OraLoader.GetDBString(1);
		Rate = atof(OraLoader.GetDBString(2));
		GetDailyLevInterest(From, To, Rate);
		OraLoader.MoveNext();
	}

	return GetLevInterest();
}

double CAssetLev::GetDailyLevInterest(COraLoader &OraLoader, double Amount, 
							LPCTSTR Portfolio, LPCTSTR TransNum, LPCTSTR InvNum)
{
	SetLevAmount(Amount);
	return GetDailyLevInterest(OraLoader, Portfolio, TransNum, InvNum);
}

double CAssetLev::GetDailyLevInterest(COraLoader &OraLoader, double Amount, 
					LPCTSTR Portfolio, LPCTSTR TransNum, LPCTSTR FromDate, LPCTSTR ToDate)
{
	SetLevAmount(Amount);
	return GetDailyLevInterest(OraLoader, Portfolio, TransNum, FromDate, ToDate);
}

double CAssetLev::GetLevInterest(LPCTSTR FromDate, LPCTSTR ToDate, double LevRate)
{
	if(!m_bLev)
		return 0;

	if((FromDate && strlen(FromDate) > 0) && (ToDate && strlen(ToDate) > 0))
		SetupLevInfo(FromDate, ToDate, GetLevRateBasis(), LevRate, GetRepoFormula());
	
	return ComputeLevInterest();
}

double CAssetLev::GetLevInterest(double Amount, LPCTSTR LevRateBasis, 
						LPCTSTR FromDate, LPCTSTR ToDate, double LevRate)
{
	CQData QData;

	SetLevAmount(Amount);
	QData.SetString(m_LevRateBasis, LevRateBasis);
	return GetLevInterest(FromDate, ToDate, LevRate);
}

double CAssetLev::ComputeLevAmount(BOOL bRefresh)
{
	if(!m_bLev)
		m_LevAmount = 0;
	else	
		if(bRefresh || m_LevAmount == 0)
		{
			m_LevAmount = GetValue();
/*			m_LevAmount += GetAccFlag() ? GetPrePaidInt() : 0;
			m_LevAmount += GetAccFlag() ? GetPlusAmount() : 0;
			m_LevAmount += GetAccFlag() ? GetPrePaidAccretionValue() : 0;  */
		}

	return m_LevAmount;
}

double CAssetLev::GetLevRepayAmount()
{
	return m_bLev ? GetLevAmount() + GetLevInterest() : 0;
}

int CAssetLev::GetnDir(LPCTSTR Dir)
{
	return CEquity::GetnDir(Dir)*(m_Type == REPO ? -1 : 1);
}

double CAssetLev::ComputeLevInterest()
{
	if(m_bLev && strlen(GetRepoFormula()) == 0 && GetLevRate() != 0)
		SetLevInterest(GetOutput(CBond::ComputeInterest(GetLevAmount(), GetLevRate(), 
						GetLevDays(), GetLevYearBase())));
	else
		SetLevInterest();

	return m_LevInterest;
}

void CAssetLev::PreInfBondAccrual(COraLoader &OraLoader, const CString Asset, LPCTSTR Date)
{
	CQData QData;
	CString sDate, QDate;

	OraLoader.ExecuteSql("DELETE SEMAM.NW_INF_BOND_LIMITS");

	sDate = Date;
	QDate = QData.GetQueryDate(Date);
	OraLoader.ExecuteSql("INSERT INTO SEMAM.NW_INF_BOND_LIMITS (INDATE, ASSET_CODE, REPT_DATE, FROM_DATE) "
						"SELECT TRUNC(SYSDATE), ASS_CODE, " + sDate + ", MAX(NVL(START_DATE, ASS_FROM)) "
							"FROM SEMAM.NW_ASS_PERIODS A "
							"WHERE A.ASSET_CODE = " + QData.GetQueryText(Asset) + 
							"AND NVL(START_DATE, ASS_FROM) <= " + QDate + 
							" GROUP BY ASS_CODE ");

	OraLoader.ExecuteSql("UPDATE SEMAM.NW_INF_BOND_LIMITS A "
							"SET (TO_DATE_MAX) = (SELECT MIN(NVL(END_DATE, ASS_TO)) "
												"FROM SEMAM.NW_ASS_PERIODS C "
												"WHERE C.ASS_CODE = A.ASSET_CODE "
												"AND NVL(END_DATE, ASS_TO) > REPT_DATE) "
							"WHERE TICKET_NUM IS NOT NULL "); 
			

	OraLoader.ExecuteSql("UPDATE SEMAM.NW_INF_BOND_LIMITS A "
							"SET (TO_DATE_MIN) = (SELECT MIN(ASS_TO) "
												"FROM SEMAM.NW_ASS_PERIODS C "
												"WHERE C.ASS_CODE = A.ASSET_CODE "
												"AND NVL(START_DATE, ASS_FROM) = A.FROM_DATE) "
							"WHERE TICKET_NUM IS NOT NULL "); 
}

double CAssetLev::GetPrePaidInt(COraLoader *pOraLoader, LPCTSTR Asset)
{
	CString FromDate, ToDate;
	OValue Value;
	int IntDays, IntDays2;
	double Interest = 0, Rate, AmortFact;

	if(!pOraLoader || !Asset || strlen(Asset) == 0 || GetFormula() != "INFLATION")
		return CBond::GetPrePaidInt();

	PreInfBondAccrual(*pOraLoader, Asset, GetPreIntDate());

	if(pOraLoader->Open("SELECT ASS_FROM, ASS_TO, RATE, NVL(AMORT_FACT, 1) "
						"FROM SEMAM.NW_ASS_PERIODS A, SEMAM.NW_INF_BOND_LIMITS B "
						"WHERE B.ASSET_CODE = A.ASS_CODE "
						"AND NVL(START_DATE, ASS_FROM) = B.FROM_DATE "
						"AND ASS_TO BETWEEN B.TO_DATE_MIN AND B.TO_DATE_MAX "
						"ORDER BY 1 "))
	{
		Interest = 0;
		pOraLoader->MoveFirst();
		while(!pOraLoader->IsEOF())
		{
			FromDate = pOraLoader->GetDBString(0);
			ToDate = pOraLoader->GetDBString(1);
			pOraLoader->GetFieldValue(2, &Value);
			Rate = (double) Value;
			pOraLoader->GetFieldValue(3, &Value);
			AmortFact = (double) Value;
			
			Interest += CAmortBond::ComputeInterest(GetNomAmount(), Rate, AmortFact, FromDate, ToDate, GetRateBasis(), IntDays, IntDays2);
			pOraLoader->MoveNext();
		}
	}

	return Interest;
}

double CAssetLev::GetAccrualInt(COraLoader *pOraLoader, LPCTSTR Asset)
{
	CString FromDate, ToDate;
	OValue Value;
	int IntDays, IntDays2;
	double Interest = 0, Rate, AmortFact;

	if(!pOraLoader || !Asset || strlen(Asset) == 0 || GetFormula() != "INFLATION")
		return CBond::GetAccrualInt();

	PreInfBondAccrual(*pOraLoader, Asset, GetDate());

	if(pOraLoader->Open("SELECT ASS_FROM, ASS_TO, RATE, NVL(AMORT_FACT, 1) "
						"FROM SEMAM.NW_ASS_PERIODS A, SEMAM.NW_INF_BOND_LIMITS B "
						"WHERE B.ASSET_CODE = A.ASS_CODE "
						"AND NVL(START_DATE, ASS_FROM) = B.FROM_DATE "
						"AND ASS_TO BETWEEN B.TO_DATE_MIN AND B.TO_DATE_MAX "
						"ORDER BY 1 "))
	{
		Interest = 0;
		pOraLoader->MoveFirst();
		while(!pOraLoader->IsEOF())
		{
			FromDate = pOraLoader->GetDBString(0);
			ToDate = pOraLoader->GetDBString(1);
			pOraLoader->GetFieldValue(2, &Value);
			Rate = (double) Value;
			pOraLoader->GetFieldValue(3, &Value);
			AmortFact = (double) Value;
			
			Interest += CAmortBond::ComputeInterest(GetNomAmount(), Rate, AmortFact, FromDate, ToDate, GetRateBasis(), IntDays, IntDays2);
			pOraLoader->MoveNext();
		}
	}

	return Interest;
}

double CAssetLev::GetAccretion(COraLoader *pOraLoader, LPCTSTR Asset)
{
	CString FromDate, ToDate;
	OValue Value;
	int IntDays, IntDays2;
	double CapInt = 0, AmortFact, CapRate;

	if(!pOraLoader || !Asset || strlen(Asset) == 0 || GetFormula() != "INFLATION")
		return CCapBond::GetAccretion();

	PreInfBondAccrual(*pOraLoader, Asset, GetDate());

	if(pOraLoader->Open("SELECT ASS_FROM, ASS_TO, CAPIT_RATE, NVL(AMORT_FACT, 1) "
						"FROM SEMAM.NW_ASS_PERIODS A, SEMAM.NW_INF_BOND_LIMITS B "
						"WHERE B.ASSET_CODE = A.ASS_CODE "
						"AND NVL(START_DATE, ASS_FROM) = B.FROM_DATE "
						"AND ASS_TO BETWEEN B.TO_DATE_MIN AND B.TO_DATE_MAX "
						"ORDER BY 1 "))
	{
		CapInt = 0;
		pOraLoader->MoveFirst();
		while(!pOraLoader->IsEOF())
		{
			FromDate = pOraLoader->GetDBString(0);
			ToDate = pOraLoader->GetDBString(1);
			pOraLoader->GetFieldValue(2, &Value);
			CapRate = (double) Value;
			pOraLoader->GetFieldValue(3, &Value);
			AmortFact = (double) Value;
		
			CapInt += CAmortBond::ComputeInterest(GetNomAmount(), CapRate, AmortFact, FromDate, ToDate, GetRateBasis(), IntDays, IntDays2);
			pOraLoader->MoveNext();
		}
	}

	return CapInt;
}