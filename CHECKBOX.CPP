#include "stdafx.h"
#include "checkbox.h"
#include "nwivar.h"

IMPLEMENT_DYNAMIC(CCheckBox, CButton)
// CCheckBox

CCheckBox::CCheckBox()
{
	m_CheckString.Empty();
	m_UnCheckString.Empty();
	SetReadOnly(FALSE);
}
	
void CCheckBox::Setup(CWnd *Parent, unsigned int ID, LPCTSTR CheckString, LPCTSTR UnCheckString)
{
	SetStrings(CheckString, UnCheckString);
    if(Parent && ID > 0)
		CButton::SubclassDlgItem(ID, Parent);
	SetReadOnly(FALSE);
}

BEGIN_MESSAGE_MAP(CCheckBox, CButton)
	// {{AFX_MSG_MAP(CCheckBox)
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	// }}AFX_MSG_MAP
END_MESSAGE_MAP()

LPCTSTR CCheckBox::GetCheckString()
{
	return (LPCTSTR) CButton::GetCheck() ? m_CheckString : m_UnCheckString; 	
}

void CCheckBox::SetStrings(LPCTSTR CheckString, LPCTSTR UnCheckString)
{
	m_CheckString.Empty();
	m_UnCheckString.Empty();
	if(CheckString && strlen(CheckString))
		m_CheckString = CheckString;
		
	if(UnCheckString && strlen(UnCheckString))
		m_UnCheckString = UnCheckString;
}

void CCheckBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	CButton::OnLButtonUp(nFlags, point);

	if(GetReadOnly())
		CButton::SetCheck(!CButton::GetCheck());
}

void CCheckBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CButton::OnKeyUp(nChar, nRepCnt, nFlags);

	if(GetReadOnly())
		CButton::SetCheck(!CButton::GetCheck());
}