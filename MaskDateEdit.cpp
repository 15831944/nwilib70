#include "stdafx.h"
#include "maskdateedit.h"
#include "qdata.h"
#include <math.h>


IMPLEMENT_DYNCREATE(CMaskDateEdit, CMaskEdit)

CMaskDateEdit::CMaskDateEdit(CWnd *Parent, unsigned int ID, BOOL bFindMode, 
							 LPCTSTR ErrMsg, CMaskDateEdit *pCompareDate)
{
	Setup(Parent, ID, bFindMode, ErrMsg, pCompareDate);
}

BOOL CMaskDateEdit::ConvertDate(CString Date, int &Year, int &Month, int &Day)
{
	CQData QData;

	if(Date.IsEmpty() || Date.Find("%") >= 0)
		return FALSE;

	Month = atoi(Date.Left(2));
	Day = atoi(((const char *) Date) + 3);

	if(Date.GetLength() == 8)
		Year = QData.YYToYYYY(atoi(Date.Right(2)));
	else
		if(Date.GetLength() == 10)
			Year = atoi(Date.Right(4));

	return TRUE;
}

void CMaskDateEdit::Setup(CWnd *Parent, unsigned int ID, BOOL bFindMode, 
						  LPCTSTR ErrMsg, CMaskDateEdit *pCompareDate)
{
	CString DateMask = "[01][0-9]/[0-3][0-9]/[12][0189][0-59][0-9]";

	CMaskEdit::Setup(Parent, ID, DateMask, bFindMode, "__/__/____", UPPERCASE, TRUE, ErrMsg);
	m_pCompareDate = pCompareDate;
}

BOOL CMaskDateEdit::ValidData()
{
	CString Date;
	int dd, dd2, mm, mm2, yy, yy2;
	CQData QData;

	Date = GetData();

	if(Date.GetLength() == 0)
		return TRUE;

	if(GetFindMode())
	{
		if(Date.Find("%") >= 0)
			return TRUE;
	}

	if(!ConvertDate(Date, yy, mm, dd))
		return FALSE;

	COleDateTime mydate;
	
	mydate.SetDate(yy, mm, dd);

	if(mydate.GetStatus() == COleDateTime::valid)
	{
		if(!m_pCompareDate && m_CompareDate.IsEmpty())
			return TRUE;

		if(m_pCompareDate)
			Date = m_pCompareDate->GetData();
		else
			Date = m_CompareDate;

		if(Date.Find("%") >= 0)
			return TRUE;

		ConvertDate(Date, yy2, mm2, dd2);

		if(yy > yy2 || yy == yy2 && mm > mm2 || yy == yy2 && mm == mm2 && dd >= dd2)
			return TRUE;
	}

	return FALSE;
}

COleDateTime &CMaskDateEdit::GetDate()
{
	int yy, mm, dd;

	m_Date.SetStatus(COleDateTime::null);

	if(GetData().Find("%", 0) < 0)
		if(ConvertDate(GetData(), yy, mm, dd))
			m_Date.SetDate(yy, mm, dd);

	return m_Date;
}