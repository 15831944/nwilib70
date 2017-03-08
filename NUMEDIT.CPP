/****************************************************************************

  SOURCE FILE: DATEEDIT.CPP
   This Edit will accept only DD-MON-YY input format
****************************************************************************/

// Windows header files:
#include "stdafx.h"
#include <ctype.h>
#include "numedit.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CNumEdit, CCntlEdit)
// CNumEdit Edit

CNumEdit::CNumEdit(CWnd *Parent, UINT ID, LPCTSTR ErrMsg, int nDigits, BOOL bWithComma)
{	
	Setup(Parent, ID, ErrMsg, nDigits, bWithComma);
}

void CNumEdit::Setup(CWnd *Parent, UINT ID, LPCTSTR ErrMsg, int nDigits, BOOL bWithComma) 
{	
	CCntlEdit::Setup(Parent, ID, EMPTYSTRING, NOCASE, 0, ErrMsg);
	SetDigits(nDigits);
	SetWithComma(bWithComma);
}

void CNumEdit::SetWithComma(BOOL bWithComma)
{
	m_bWithComma = bWithComma;
	if(m_bWithComma)
	{
		if(GetDigits() == 0)
			SetValidChars("-0123456789,/ "); // Integer Only
		else
			SetValidChars("-0123456789,. /"); 
	}
	else
	{
		if(GetDigits() == 0)
			SetValidChars("-0123456789/ "); // Integer Only
		else
			SetValidChars("-0123456789. /");
	}
}

BOOL CNumEdit::ProcessData()
{
	CString Text;
	CQData QData;

	CEdit::GetWindowText(Text);
	if(Text.IsEmpty())
		SetValid(TRUE);
	else
		if(IsValid(Text))
		{
			Text = QData.WriteNumber(Text, WithComma(), GetDigits());
			SetValid(!Text.IsEmpty());
		}
		else
			SetValid(FALSE);
	
	if(!GetValid())
		MessageBox(GetErrMsg());
	else
		CEdit::SetWindowText(Text);		

	return GetValid();
}

void CNumEdit::SetWindowText(LPCTSTR Num)
{
	CString Text;
	CQData QData;

	Text = Num;
	if(IsValid(Text))
	{
		Text = QData.WriteNumber(Text, WithComma(), GetDigits());
		CEdit::SetWindowText(Text);
	}
}
