#include "stdafx.h"
#include "maskmintimeedit.h"
#include "qdata.h"


IMPLEMENT_DYNCREATE(CMaskMinTimeEdit, CMaskEdit)

CMaskMinTimeEdit::CMaskMinTimeEdit(CWnd *Parent, unsigned int ID, BOOL bFindMode, const CString ErrMsg)
{
	Setup(Parent, ID, bFindMode, ErrMsg);
}

void CMaskMinTimeEdit::Setup(CWnd *Parent, unsigned int ID, BOOL bFindMode, const CString ErrMsg)
{
	CString TimeMask = "[0-2][0-9]:[0-5][0-9]";

	CMaskEdit::Setup(Parent, ID, TimeMask, bFindMode, "__:__", UPPERCASE, TRUE, ErrMsg);
}

BOOL CMaskMinTimeEdit::ConvertTime(const CString Time, int &hh, int &mm)
{
	if(Time.IsEmpty(), Time.Find("%") >= 0)
		return FALSE;

	hh = atoi(Time.Left(2));
	mm = atoi(Time.Right(2));
	return TRUE;
}

COleDateTime &CMaskMinTimeEdit::GetTime()
{
	int hour, minute;

	m_Time.SetStatus(COleDateTime::null);
	
	if(GetData().Find("%", 0) < 0)
		if(ConvertTime(GetData(), hour, minute))
			m_Time.SetTime(hour, minute, 0);

	return m_Time;
}

BOOL CMaskMinTimeEdit::ValidData()
{
	CString Time;
	int mm, hh;

	Time = GetData();

	if(Time.IsEmpty())
		return TRUE;

	if(GetFindMode())
	{
		if(Time.Find("%") >= 0)
			return TRUE;
	}

	if(!ConvertTime(Time, hh, mm))
		return FALSE;

	COleDateTime mydate;
	
	mydate.SetTime(hh, mm, 0);

	return mydate.GetStatus() == COleDateTime::valid ? TRUE : FALSE;
}

