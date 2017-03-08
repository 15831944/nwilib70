#include "stdafx.h"
#include "assetlev.h"
#include "oradate.h"
#include "nwivar.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CAssetLev, CCapBond)
CAssetLev::CAssetLev(CAssetLev &AssetLev) : CCapBond(AssetLev)
{
	SetupLevInfo(AssetLev.GetValueDate(), AssetLev.GetDate(), AssetLev.GetLevRateBasis(), 
				AssetLev.GetLevRate(), AssetLev.GetFormula());
}

void CAssetLev::SetupLevInfo(LPCTSTR ValueDate, LPCTSTR Date, LPCTSTR LevRateBasis, 
							 double LevRate, LPCTSTR Formula)
{
	CQData QData;

	m_LevInterest = 0;
	m_LevAmount = 0;
	m_bLev = TRUE;
	if(Formula && strstr(Formula, DAILYREPO))
		QData.SetString(m_Formula, Formula);
	else
		m_Formula.Empty();

	QData.SetString(m_LevRateBasis, LevRateBasis);
	SetLevRate(m_Formula.IsEmpty() ? LevRate : 0);

	if(m_Formula.IsEmpty()) // Not Daily Float Repo
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

	if(strlen(GetFormula()) > 0)
	{
		// Weighted Lev Rate
		Interest = GetOutput(CBond::ComputeInterest(GetLevAmount(), LevRate, FromDate, ToDate, 
							GetLevRateBasis(), &Days, &Days2));
		SetLevRate((m_LevRate*m_nLevDays + LevRate*(Days + Days2))/(m_nLevDays + (Days + Days2))); 
		AddLevDays(Days);
		AddLevDays(Days + Days2);
		AddLevInterest(Interest);
		if(m_Formula == DAILYREPOA)
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
		SetupLevInfo(FromDate, ToDate, GetLevRateBasis(), LevRate, GetFormula());
	
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
	if(m_bLev && strlen(GetFormula()) == 0 && GetLevRate() != 0)
		SetLevInterest(GetOutput(CBond::ComputeInterest(GetLevAmount(), GetLevRate(), 
						GetLevDays(), GetLevYearBase())));
	else
		SetLevInterest();

	return m_LevInterest;
}
