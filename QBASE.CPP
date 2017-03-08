#include "stdafx.h"
#include "qbase.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CQBase, CAvCost)
CQBase::CQBase(COraLoader &OraLoader, LPCTSTR FileSet, LPCTSTR PrevDate, 
			   LPCTSTR StartDate, LPCTSTR Date, CStringArray &Portfolios, 
			   CStringArray &Conditions, BOOL bAll)
{
	Setup(OraLoader, FileSet, PrevDate, StartDate, Date, Portfolios, Conditions, bAll);
}

void CQBase::Setup(COraLoader &OraLoader, LPCTSTR FileSet, LPCTSTR PrevDate, 
				   LPCTSTR StartDate, LPCTSTR Date, CStringArray &Portfolios, 
				   CStringArray &Conditions, BOOL bAll)
{
	SetOraLoader(OraLoader);
	SetFileSet(FileSet);
	SetPrevDate(PrevDate);
	SetStartDate(StartDate);
	SetDate(Date);
	ToInStmt(m_Portfolios, Portfolios);
	ToInStmt(m_Conditions, Conditions);
	SetAll(bAll);
}

void CQBase::ToInStmt(CString &InStmt, CStringArray &List)
{
	int i;
	CQData QData;

	InStmt = "IN (";
	for(i = 0; i < List.GetSize(); i ++)
	{
		if(i > 0)
			InStmt += COMMA;
		InStmt += QData.GetQueryText(List.GetAt(i));
	}

	if(i > 0)
		InStmt += ")";
	else
		InStmt.Empty();
}

char CQBase::GetCharVal(char& Val, OValue& Value, char Default)
{
	Val = Value.IsNull() ? Default : ((LPCTSTR) Value)[0];
	return Val;
}

BOOL CQBase::GetBOOLVal(BOOL& Flag, OValue& Value, LPCTSTR YesStr, BOOL Default)
{
	Flag = Value.IsNull() ? Default : strcmp((LPCTSTR) Value, YesStr) == 0 ? TRUE : FALSE;
	return Flag;
}

double CQBase::GetFactor(OValue& Value, double Default)
{
	return(Value.IsNull() ? Default : strcmp((LPCTSTR)Value, "EQUITY TYPE CAL") == 0 ? 1 : 0.01);
}

double CQBase::GetFxrate(double &Fxrate, LPCTSTR Currency, LPCTSTR Date)
{
	LPCTSTR pSql = "SELECT EXRATE FROM SEMAM.NW_EXRATES A "
						"WHERE A.CURRENCY = '%s' "
						"AND A.INDATE = (SELECT MAX(B.INDATE) FROM SEMAM.NW_EXRATES B "
										"WHERE A.CURRENCY = B.CURRENCY "
										"AND B.EXRATE IS NOT NULL AND B.INDATE <= %s) ";
	OValue Value;
	COraLoader OraLoader(GetOraLoader());
	CQData QData;

	if(strcmp(Currency, "USD") == 0)
	{
		Fxrate = 1;
		return Fxrate;
	}

	OraLoader.GetSql().Format(pSql, Currency, QData.GetQueryDate(Date)); // On date
	if(OraLoader.Open())
	{	
		OraLoader.GetFieldValue(0, &Value);
		if(!Value.IsNull())
			Fxrate = (double) Value;
	}

	if(Fxrate <= 0) // default
		Fxrate = 1;

	return Fxrate;
}

BOOL CQBase::Process()
{
	if(!GetOraLoader().GetDB())
		return FALSE;

	if(!PreProcess())
		return FALSE;

	if(!ClearTable())
		return FALSE;

	if(SetupSql())
		return FALSE;

	if(!GetOraLoader().Open())
		return FALSE;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		DoWork();
		GetOraLoader().MoveNext();
	}

	if(GetOraLoader().GetRecordCount() > 0)
		UpdateTable();
	return TRUE;
}