#include "stdafx.h"
#include "group.h"
#include "nwivar.h"
 

CGroupButton::CGroupButton(CWnd *Parent)
{
	m_StringArr.RemoveAll();
	m_IDArr.RemoveAll();
	SetParent(Parent);
}

CGroupButton::CGroupButton()
{
	m_StringArr.RemoveAll();
	m_IDArr.RemoveAll();
}

LPCTSTR CGroupButton::GetCheckString()
{
	CButton *Button;
	
	for(int i = 0; i < m_IDArr.GetSize() && GetParent(); i++)
	{
		Button = (CButton *) GetParent()->GetDlgItem((unsigned int) m_IDArr.GetAt(i));
		if(Button && Button->GetCheck() == 1)
			return m_StringArr.GetAt(i);			
	}
	
	return NULL;
}

void CGroupButton::AddButton(unsigned int ID, LPCTSTR String)
{
	m_IDArr.Add(ID);
	if(String && strlen(String))
		m_StringArr.Add(String);
	else
		m_StringArr.Add(EMPTYSTRING);
}

void CGroupButton::SetCheck(LPCTSTR TextString)
{
	int State;
	CButton *Button;

	for(int i = 0; i < m_StringArr.GetSize() && GetParent(); i++)
	{
		State = m_StringArr.GetAt(i) == TextString ? 1 : 0;

		Button = (CButton *) GetParent()->GetDlgItem((unsigned int) m_IDArr.GetAt(i));
		if(Button)
			Button->SetCheck(State);
	}
}

void CGroupButton::SetCheck(unsigned int ID, int nCheck)
{
	int State;
	CButton *Button;

	for(int i = 0; i < m_IDArr.GetSize() && GetParent(); i++)
	{
		State = m_IDArr.GetAt(i) == ID ? 1 : 0;

		Button = (CButton *) GetParent()->GetDlgItem((unsigned int) m_IDArr.GetAt(i));
		if(Button)
			Button->SetCheck(State);
	}
}

void CGroupButton::EnableWindow(BOOL bEnable)
{
	CButton *Button;

	for(int i = 0; i < m_IDArr.GetSize() && GetParent(); i ++)
	{
		Button = (CButton *) GetParent()->GetDlgItem((unsigned int) m_IDArr.GetAt(i));
		if(Button)
			Button->EnableWindow(bEnable);
	}
}

void CGroupButton::ShowWindow(int nCmdShow)
{
	CButton *Button;

	for(int i = 0; i < m_IDArr.GetSize() && GetParent(); i ++)
	{
		Button = (CButton *) GetParent()->GetDlgItem((unsigned int) m_IDArr.GetAt(i));
		if(Button)
			Button->ShowWindow(nCmdShow);
	}
}