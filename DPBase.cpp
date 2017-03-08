#include "stdafx.h"
#include "dpbase.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CDPBase, CObject)

void CDPBase::Setup(COraLoader &OraLoader, CStringArray &Portfolios, CStringArray &Conditions,
				LPCTSTR PrevDate, LPCTSTR StartDate, LPCTSTR Date)
{
	SetOraLoader(OraLoader);
	SetPortfolios(Portfolios);
	SetConditions(Conditions);
	SetPrevDate(PrevDate);
	SetStartDate(StartDate);
	SetDate(Date);
}

void CDPBase::Setup(COraLoader &OraLoader, LPCTSTR Portfolios, LPCTSTR Conditions,
				LPCTSTR PrevDate, LPCTSTR StartDate, LPCTSTR Date)
{
	SetOraLoader(OraLoader);
	SetPortfolios(Portfolios);
	SetConditions(Conditions);
	SetPrevDate(PrevDate);
	SetStartDate(StartDate);
	SetDate(Date);
}

void CDPBase::SetOraLoader(COraLoader &OraLoader)
{
	m_OraLoader = OraLoader;
	m_OraLoader2 = OraLoader;
	m_bReady = TRUE;
	SetFileSets();
}

BOOL CDPBase::SetFileSets()
{
	OValue Value;
	
	m_FileSets.RemoveAll();
	if(!GetOraLoader().Open("SELECT DISTINCT TABLESPACE_NAME FROM SEMAM.NW_COMBINED_PORTFOLIOS "))
		return FALSE;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{		
		GetOraLoader().GetFieldValue(0, &Value);
		m_FileSets.Add((LPCTSTR) Value);
		GetOraLoader().MoveNext();
	}
	
	return TRUE;
}

void CDPBase::ToInStmt(CString &InStmt, CStringArray &List)
{
	int i;
	CQData QData;
	
	for(i = 0; i < List.GetSize(); i ++)
	{
		if(i == 0)
			InStmt = "IN (";
		else
			InStmt += COMMA;
		InStmt += QData.GetQueryText(List.GetAt(i));
	}

	if(i > 0)
		InStmt += ") ";
	else
		InStmt.Empty();

	if(IsAll(InStmt))
		InStmt.Empty();
}

char CDPBase::GetCharVal(OValue &Value, char Default)
{
	return Value.IsNull() ? Default : ((LPCTSTR) Value) [0];	
}

BOOL CDPBase::GetBOOLVal(OValue &Value, LPCTSTR YesStr, BOOL Default)
{
	return Value.IsNull() ? Default : strcmp((LPCTSTR) Value, YesStr) == 0 ? TRUE : FALSE;	
}

double CDPBase::GetFactor(OValue &Value, double Default)
{
	return (Value.IsNull() ? Default : strcmp((LPCTSTR) Value, "EQUITY TYPE CAL") == 0 ? 1.0 : 0.01);
}

BOOL CDPBase::IsAll(LPCTSTR InStmt)
{
	return strstr(InStmt, "ALL") ? TRUE : FALSE;
}

double CDPBase::GetFxrate(double &Fxrate, LPCTSTR Currency, LPCTSTR Date)
{
	CQData QData;
	OValue Value;
	
	Fxrate = 1;
	if(!Currency || strlen(Currency) == 0)
		return Fxrate;

	if(strcmp(Currency, "USD") == 0)
		return Fxrate;

	GetOraLoader2().GetSql().Format("SELECT EXRATE FROM SEMAM.NW_EXRATES A WHERE A.CURRENCY = '%s' "
								"AND A.INDATE = (SELECT MAX(B.INDATE) FROM SEMAM.NW_EXRATES B "
												"WHERE B.CURRENCY = A.CURRENCY "
												"AND B.EXRATE IS NOT NULL AND B.INDATE <= %s) ",
								Currency, QData.GetQueryDate(Date));

	if(GetOraLoader2().Open())
	{
		GetOraLoader2().GetFieldValue(0, &Value);
		if(!Value.IsNull())
			Fxrate = (double) Value;
	}
	
	return Fxrate;
}

BOOL CDPBase::DoWork()
{
	if(!m_bReady)
		return FALSE;

	if(!SetupSql())
		return FALSE;

	if(!DeleteDataFromTable())
		return FALSE;

	if(!PreProcess())
		return FALSE;

	if(!GetOraLoader2().Open())
		return FALSE;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{		
		if(!PrepareData())
			return FALSE;
		if(!ComputeData())
			return FALSE;
		if(!AddDataToTable())
			return FALSE;
		GetOraLoader().MoveNext();
	}
			
	return TRUE;
}