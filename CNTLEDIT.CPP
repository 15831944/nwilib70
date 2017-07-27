/****************************************************************************

  SOURCE FILE: CNTLEDIT.CPP

****************************************************************************/

// Windows header files:
#include "stdafx.h"
#include <ctype.h>
#include <string.h>
#include "nwivar.h"
#include "cntledit.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CCntlEdit, CEdit)
// CCntlEdit Edit
BEGIN_MESSAGE_MAP(CCntlEdit, CEdit)
	//{{AFX_MSG_MAP(CCntlEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCntlEdit::CCntlEdit() : CEdit()
{
	SetCharCase(CASESENSITIVE);    // Default is case sensitive
	SetValid(FALSE);
}

void CCntlEdit::Setup(CWnd *Parent, unsigned int ID, LPCTSTR ValidChars, int CharCase, 
					  UINT nLimitText, LPCTSTR ErrMsg)
{	
	SetValidChars(ValidChars);
	SetCharCase(CharCase);
	SetErrMsg(ErrMsg);
	if(Parent && ID > 0)
		CEdit::SubclassDlgItem(ID, Parent);
	if(nLimitText > 0)
		CEdit::SetLimitText(nLimitText);
	SetValid(FALSE);
}

void CCntlEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
/****************************************************************************
  WMChar message. 
****************************************************************************/
{
	if(IsCharVisible(nChar))
	{
		if(IsCharOk(nChar))
			AdjustCharCase(nChar);
		else
		{
			MessageBeep(-1);
			return;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CCntlEdit::OnKillFocus(CWnd *pNewWnd)
{
	if(!ProcessData())
		CEdit::SetFocus();
	else
		CEdit::OnKillFocus(pNewWnd);
}

BOOL CCntlEdit::ProcessData()
{
	CString Text;

	CEdit::GetWindowText(Text);
	SetValid(IsValid(Text));
	if(!GetValid())
		MessageBox(GetErrMsg(), "Input Error");
	
	CEdit::SetWindowText(Text);
	return GetValid();
}

const CString CCntlEdit::GetData()
{
	CString Text;

	CEdit::GetWindowText(Text);
	SetValid(IsValid(Text));
	if(!GetValid())
		Text.Empty();

	return Text;
}

void CCntlEdit::SetData(const CString lpText)
{
	CString Text;

	Text = lpText;
	SetValid(IsValid(Text));
	if(GetValid())
		CEdit::SetWindowText(Text);
	else
		MessageBeep(0);
}

BOOL CCntlEdit::IsCharOk(UINT nChar)
{
	return m_ValidChars.IsEmpty() || m_ValidChars.Find((char) nChar) >= 0 ? TRUE : FALSE;
}

BOOL CCntlEdit::IsValid(LPCTSTR lpText)
{
	CQData QData;
	
	return QData.IsValid(lpText, m_ValidChars);
}

BOOL CCntlEdit::IsCharVisible(UINT nChar)
{
	if(isalnum(nChar))
		return TRUE;

	if(ispunct(nChar))
		return TRUE;
   
	if(isspace(nChar))
		return TRUE;
         
	return FALSE;   
}

void CCntlEdit::AdjustCharCase(UINT &nChar)
{
	switch(m_nCharCase)
	{
		case CASESENSITIVE:
		case NOCASE:
			break;
		case UPPERCASE:
			nChar = toupper(nChar);
			break;
		case LOWERCASE:
			nChar = tolower(nChar);
			break;
		default:
			break;
	}
}

BOOL CCntlEdit::PreTranslateMessage(MSG *pMsg)
{
	if(pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_CONTROL) < 0)
	{
		switch(pMsg->wParam)
		{
			case 'A':
				SetSel(0, -1);
				return TRUE;
			case 'C':
				Copy();
				return TRUE;
			case 'V':
				Paste();
				return TRUE;
			case 'X':
				Cut();
				return TRUE;
			case 'Z':
				Undo(); 
				return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}