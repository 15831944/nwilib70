#include "stdafx.h"
#include "combox.h"
#include "nwivar.h"

IMPLEMENT_DYNAMIC(COptComboBox, CComboBox)

BEGIN_MESSAGE_MAP(COptComboBox, CComboBox)
	//{{AFX_MSG_MAP(COptComboBoxEdit)
	ON_WM_ENABLE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COptComboBox::COptComboBox(CComboBox *ComboBox, BOOL WithNone /* = FASLE */, BOOL bAdd)
{
	SetWithNone(WithNone);
	SetAdd(bAdd);
 //   SubclassDlgItem(ComboBox->GetDlgCtrlID(), ComboBox->GetParent());
}

BOOL COptComboBox::Setup(CWnd *Parent, UINT ID, BOOL WithNone /* = FASLE */, BOOL bAdd) 
{
	BOOL bSub;

	bSub = this->SubclassDlgItem(ID, Parent);
	if(!bSub)
	{
		CString Text;

		Text.Format("There is a problem when create %d Control", ID);
		MessageBox(Text);
	}

	SetWithNone(WithNone);
	SetAdd(bAdd);
    
	return bSub;
}

void COptComboBox::GetLBText(int Index, CString &Text)
{
	Text.Empty();
	if(Index >= 0 && Index < GetCount())
		CComboBox::GetLBText(Index, Text);
	
	if(m_bWithNone && Text == NONE)
	{
		Text.Empty();
		CComboBox::SetCurSel(-1);
	}	
}

const char* COptComboBox::GetSelString(CString &Buffer)
{
	GetLBText(GetCurSel(), Buffer);
	return (const char*) Buffer;
}

int COptComboBox::GetCurSel()
{
	if(m_bWithNone)
	{
		CString Text;
		GetLBText(CComboBox::GetCurSel(), Text);
	}

	return CComboBox::GetCurSel();
}

int COptComboBox::SetCurSel(int Index)
{
	if(Index < CComboBox::GetCount())
	{
		CString	Text;
		GetLBText(Index, Text);
		if(Text.IsEmpty())
			Index = -1;
		
	   	return CComboBox::SetCurSel(Index);
	}

	return CB_ERR;
}

void COptComboBox::SetData(const CString Data)
{
	int CurSel;

	CurSel = CComboBox::FindStringExact(0, Data);
	CComboBox::SetCurSel(CurSel);
}

int COptComboBox::FindString(int nStartAfter, LPCTSTR Key)
{
	if(Key && strlen(Key) > 0)
	{
		if(m_bWithNone && strcmp(Key, NONE) == 0)
			return CB_ERR;
		
		return CComboBox::FindString(nStartAfter, Key);
	}
	
	return CB_ERR;
}

int COptComboBox::SelectString(int nStartAfter, LPCTSTR Key)
{
	if(Key && strlen(Key) > 0)
	{
		if(m_bWithNone && strcmp(Key, NONE) == 0)
			return SetCurSel(-1);
		
		return CComboBox::SelectString(nStartAfter, Key);
	}
	
	return SetCurSel(-1);
}

void COptComboBox::SetReadOnly(BOOL bReadOnly)
{
	EnableWindow(!bReadOnly);
}

void COptComboBox::OnEnable(BOOL bEnable)
{
	CComboBox::OnEnable(bEnable);

	CEdit *pEdit;

	pEdit = (CEdit*)GetWindow(GW_CHILD);

	if(pEdit)
	{
		pEdit->EnableWindow(TRUE);

		if((GetStyle() & 3) != CBS_DROPDOWNLIST)
			pEdit->SetReadOnly(!bEnable);
	}
}

void COptComboBox::OnKillFocus(CWnd *pNewWnd)
{
	GetCurSel();
	if(m_bAdd)
	{
		CString Text;
		GetWindowText(Text);
		if(FindStringExact(-1, Text) == CB_ERR)
			AddString(Text);
	}

	CComboBox::OnKillFocus(pNewWnd);
}
