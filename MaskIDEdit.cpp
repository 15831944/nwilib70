#include "stdafx.h"
#include "maskidedit.h"


IMPLEMENT_DYNCREATE(CMaskIDEdit, CMaskEdit)

CMaskIDEdit::CMaskIDEdit(CWnd *Parent, unsigned int ID, int Length, BOOL bFindMode, LPCTSTR ErrMsg)
{
	Setup(Parent, ID, Length, bFindMode, ErrMsg);
}

void CMaskIDEdit::Setup(CWnd *Parent, unsigned int ID, int Length, BOOL bFindMode, LPCTSTR ErrMsg)
{
	CString IDMask, NULLString;
	int i;
	
	for(i = 0; i < Length; i ++)
	{
		if(i == 0)
		{
			IDMask = "[1-9]";
			NULLString = "_";
		}
		else
		{
			IDMask += "[0-9]";
			NULLString += "_";
		}
	}
	
	SetWithMask(FALSE);
	CMaskEdit::Setup(Parent, ID, IDMask, FALSE, NULLString, UPPERCASE, TRUE, ErrMsg); // ID is not searchable
	SetFindMode(bFindMode);
}

BOOL CMaskIDEdit::ValidData()
{
	CString ID;

	ID = GetData();

	if(ID.GetLength() == 0)
		return TRUE;

	if(GetFindMode())
	{
		if(ID.Find("%") >= 0)
			return TRUE;
	}

	if(atoi(ID) > 0)
		return TRUE;

	return FALSE;
}

const CString CMaskIDEdit::GetData()
{
	CString Data;

	Data = CMaskEdit::GetData();

    Data = Data.SpanExcluding("_");
	return Data;
}

unsigned int CMaskIDEdit::GetID()
{
	unsigned nID = 0;

	if(GetData().Find("%", 0) >= 0)
		nID = atoi(GetData());

	return nID;
}