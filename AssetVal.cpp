#include "stdafx.h"
#include "assetval.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CAssetVal, CAssetLev)
void CAssetVal::Setup(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR Portfolio, LPCTSTR TransNum)
{
	CQData QData;
	CString sDate, sTransNum, sPort;

	sDate = QData.GetQueryDate(Date);
	sTransNum = QData.GetQueryNumber(TransNum);
	sPort = QData.GetQueryText(Portfolio);
	m_OraLoader = OraLoader;
	GetOraLoader().GetSql().Format("SELECT TRANS_TYPE, TRANS_DIRECTION, VALUE_DATE, NOM_AMOUNT, "
			"NVL(PRICE, 1), FXRATE, ASS_TYPE, ASS_RATE_BASIS, ASS_ACCRUABLE, ASS_PPAID_INT, "
			"ASS_FLOAT_FORMULA, NVL(RATE_BASIS, 'A/360'), NVL(TR_RATE, 0), FLOAT_RATE_FORMULA, "
			"NVL(START_DATE, ASS_FROM), ASS_TO, NVL(RATE, 0), NVL(AMORT_FACT, 1), NVL(CAPIT_RATE, 0), "
			"NVL(PLUS_AMOUNT, 0), ASS_TO - ADD_MONTHS(ASS_TO, ROUND(MONTHS_BETWEEN(ASS_FROM, ASS_TO), 0)) "
			"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_ASS_PERIODS C "
			"WHERE B.ASS_CODE = A.ASSET_CODE "
			"AND C.ASS_CODE(+) = B.ASS_CODE "
			"AND C.ASS_FROM(+) <= %s "
			"AND C.ASS_TO(+) + DECODE(NVL(C.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > %s "
			"AND A.PORTFOLIO = %s "
			"AND A.TRANS_NUM = %s ", (LPCTSTR) sDate, (LPCTSTR) sDate, (LPCTSTR) sPort, 
			(LPCTSTR) sTransNum);

	if(!GetOraLoader().Open())
		return;

	OValue Value;
	CString TransType, Dir, VDate, RateBasis, LevRateBasis, Formula, AssFrom, AssTo;
	CString AssetType, Accrual, PrePaid;
	double Amount, Price, Fxrate, Rate, LevRate, Amort, CapRate, Plus;
	int AADays;	

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		TransType = GetOraLoader().GetDBString(0);
		Dir = GetOraLoader().GetDBString(1);
		VDate = GetOraLoader().GetDBString(2);
		GetOraLoader().GetFieldValue(3, &Value);
		Amount = (double) Value;
		GetOraLoader().GetFieldValue(4, &Value);
		Price = (double) Value;
		GetOraLoader().GetFieldValue(5, &Value);
		Fxrate = (double) Value;
		AssetType = GetOraLoader().GetDBString(6);
		RateBasis = GetOraLoader().GetDBString(7);
		Accrual = GetOraLoader().GetDBString(8);
		PrePaid = GetOraLoader().GetDBString(9);
		GetFormula() = GetOraLoader().GetDBString(10);
		LevRateBasis = GetOraLoader().GetDBString(11);
		GetOraLoader().GetFieldValue(12, &Value);
		LevRate = (double) Value;
		Formula = GetOraLoader().GetDBString(13);
		AssFrom = GetOraLoader().GetDBString(14);
		AssTo = GetOraLoader().GetDBString(15);
		GetOraLoader().GetFieldValue(16, &Value);
		Rate = (double) Value;
		GetOraLoader().GetFieldValue(17, &Value);
		Amort = (double) Value;
		GetOraLoader().GetFieldValue(18, &Value);
		CapRate = (double) Value;
		GetOraLoader().GetFieldValue(19, &Value);
		Plus = (double) Value;
		GetOraLoader().GetFieldValue(20, &Value);
		AADays = (int) Value;
		GetOraLoader().MoveNext();
	}

	CAssetLev::Setup(OraLoader, TransType, Amount, Price, Fxrate, AssetType, Amort, Plus, Dir);

	if(Rate != 0)
		CAssetLev::SetupIntInfo(AssFrom, VDate, Date, AssTo, RateBasis, Rate, AADays, Accrual, PrePaid);

	if(CapRate != 0)
		CAssetLev::SetupCapInfo(AssFrom, VDate, Date, RateBasis, CapRate);

	if(TransType == REPO || TransType == LEVERAGE || TransType == INTSWAP)
		CAssetLev::SetupLevInfo(VDate, Date, LevRateBasis, LevRate, Formula);
}

void CAssetVal::Setup(COraLoader &OraLoader, LPCTSTR TransType, LPCTSTR Dir, LPCTSTR Asset, LPCTSTR ValueDate, 
						LPCTSTR Date, LPCTSTR NomAmount, LPCTSTR Price, LPCTSTR Fxrate, LPCTSTR Formula, 
						LPCTSTR LevRateBasis, LPCTSTR LevRate, LPCTSTR RepoFormula)
{
	double dNomAmount, dLevRate, dPrice, dFxrate;
	CQData QData;

	dNomAmount = atof(QData.RemoveComma(NomAmount));
	dPrice = atof(QData.RemoveComma(Price));
	dFxrate = atof(QData.RemoveComma(Fxrate));
	dLevRate = atof(QData.RemoveComma(LevRate));
	Setup(OraLoader, TransType, Dir, Asset, ValueDate, Date, dNomAmount, dPrice, dFxrate, 
			Formula, LevRateBasis, dLevRate, RepoFormula);		
}

void CAssetVal::Setup(COraLoader &OraLoader, LPCTSTR TransType, LPCTSTR Dir, LPCTSTR Asset, LPCTSTR ValueDate, 
					  LPCTSTR Date, double NomAmount, double Price, double Fxrate, LPCTSTR Formula, 
					  LPCTSTR LevRateBasis, double LevRate, LPCTSTR RepoFormula)
{
	CQData QData;
	double Amort = 1, Rate = 0, PlusAmount = 0, CapRate = 0;
	int AADays;
	CString	Text, AssetType, RateBasis, From, ToDate, PrePaid, Accruable;
	OValue Value;

	if(!Asset || strlen(Asset) == 0)
	{
		CAssetLev::Setup(OraLoader, TransType, NomAmount, Price, Fxrate, AssetType, 
						Amort, PlusAmount, Dir);
		return;
	}
	
	GetFormula() = Formula;
	Text = QData.GetQueryDate(ValueDate);
	OraLoader.GetSql().Format("SELECT NVL(START_DATE, ASS_FROM), ASS_TO, ASS_RATE_BASIS, "
							"ASS_ACCRUABLE, ASS_PPAID_INT, ASS_TYPE, RATE, NVL(AMORT_FACT, 1), "
							"NVL(CAPIT_RATE, 0), NVL(PLUS_AMOUNT, 0), "
							"ASS_TO - ADD_MONTHS(ASS_TO, ROUND(MONTHS_BETWEEN(ASS_FROM, ASS_TO), 0)) "
							"FROM SEMAM.NW_ASSETS A, SEMAM.NW_ASS_PERIODS B "
							"WHERE B.ASS_CODE(+) = A.ASS_CODE "
							"AND B.ASS_FROM(+) <= %s "
							"AND B.ASS_TO(+) + DECODE(NVL(B.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > %s "
							"AND A.ASS_CODE = %s ", (const char*) Text, (const char*) Text, QData.GetQueryText(Asset));
	if(!OraLoader.Open())
		return;
	
	OraLoader.MoveFirst();
	while(!OraLoader.IsEOF())
	{
		From = OraLoader.GetDBString(0);
		ToDate = OraLoader.GetDBString(1);
		RateBasis = OraLoader.GetDBString(2);
		Accruable = OraLoader.GetDBString(3);
		PrePaid = OraLoader.GetDBString(4);
		AssetType = OraLoader.GetDBString(5);
		OraLoader.GetFieldValue(6, &Value);
		Rate = (double) Value;
		OraLoader.GetFieldValue(7, &Value);
		Amort = (double) Value;
		OraLoader.GetFieldValue(8, &Value);
		CapRate = (double) Value;
		OraLoader.GetFieldValue(9, &Value);
		PlusAmount = (double) Value;
		OraLoader.GetFieldValue(10, &Value);
		AADays = (int) Value;
		
		OraLoader.MoveNext();
	}

	if(NomAmount == 0)
		NomAmount = AssetType == BONDTYPE ? 100 : 1;

	CAssetLev::Setup(OraLoader, TransType, NomAmount, Price, Fxrate, AssetType, Amort, 
					PlusAmount, Dir);

	if(Rate != 0)
		CAssetLev::SetupIntInfo(From, ValueDate, Date, ToDate, RateBasis, Rate, AADays, Accruable, PrePaid);

	if(CapRate != 0)
		CAssetLev::SetupCapInfo(From, ValueDate, Date, RateBasis, CapRate);
	
	if(strcmp(TransType, REPO) == 0 || strcmp(TransType, LEVERAGE) == 0)
		CAssetLev::SetupLevInfo(ValueDate, Date, LevRateBasis, LevRate, RepoFormula);
}
