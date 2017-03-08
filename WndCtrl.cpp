#include "stdafx.h"
#include "wndctrl.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CWndCtrl, CObject)
void CWndCtrl::SetData(LPCTSTR Data)
{
	if(!m_WndCtrl)
		return;

	if(Data && strlen(Data) > 0)
		SetText(Data);
	else
		m_Text.Empty();

	switch(GetType())
	{
		case WNDCTRL_EDIT:
			((CCntlEdit*) m_WndCtrl)->SetWindowText(GetText());
			break;
		case WNDCTRL_NUMEDIT:
			((CNumEdit*) m_WndCtrl)->SetWindowText(GetText());
			break;
		case WNDCTRL_IDEDIT:
			((CMaskIDEdit*) m_WndCtrl)->SetData(GetText());
			break;
		case WNDCTRL_MASKDATEEDIT:
			((CMaskDateEdit*) m_WndCtrl)->SetData(GetText());
			break;
		case WNDCTRL_MASKMINTIMEEDIT:
			((CMaskMinTimeEdit*) m_WndCtrl)->SetData(GetText());
			break;
		case WNDCTRL_COMBOBOX:
			if(((COptComboBox*) m_WndCtrl)->SelectString(-1, GetText()) < 0)
			{
				if(((COptComboBox*) m_WndCtrl)->GetAdd())
				{
					((COptComboBox*) m_WndCtrl)->AddString(GetText());
					((COptComboBox*) m_WndCtrl)->SelectString(-1, GetText());
				}
				else
					((COptComboBox*) m_WndCtrl)->SetCurSel(-1);
			}
			break;
		case WNDCTRL_LISTBOX:
			if(((CHLBox*) m_WndCtrl)->SelectString(-1, GetText()) < 0)
				((CHLBox*) m_WndCtrl)->SetCurSel(-1);
			break;
		case WNDCTRL_CHECKBOX:
			if(m_Text.IsEmpty())
				((CCheckBox*) m_WndCtrl)->SetCheck(0);
			else
				((CCheckBox*) m_WndCtrl)->SetCheck(GetText());
			break;
		case WNDCTRL_GROUP:
			if(m_Text.IsEmpty())
				((CGroupButton*) m_WndCtrl)->SetCheck(0, 0);
			else
				((CGroupButton*) m_WndCtrl)->SetCheck(GetText());
			break;
		case WNDCTRL_MCCOMBOBOX:
			if(((CMCComboBox*) m_WndCtrl)->SelectString(-1, GetText()) < 0)
			{
				if(((COptComboBox*) m_WndCtrl)->GetAdd())
				{
					((COptComboBox*) m_WndCtrl)->AddString(GetText());
					((COptComboBox*) m_WndCtrl)->SelectString(-1, GetText());
				}
				else
					((CMCComboBox*) m_WndCtrl)->SetCurSel(-1);
			}
			break;
		case WNDCTRL_STRING:
			*((CString *) m_WndCtrl) = GetText();
			break;
		default:
			break;
	}

	if(m_pData)
		*m_pData = GetText();
}

void CWndCtrl::EnableWindow(BOOL bEnable)
{
	if(!m_WndCtrl)
		return;

	switch(GetType())
	{
		case WNDCTRL_EDIT:
			((CCntlEdit*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_NUMEDIT:
			((CNumEdit*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_IDEDIT:
			((CMaskIDEdit*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_MASKDATEEDIT:
			((CMaskDateEdit*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_MASKMINTIMEEDIT:
			((CMaskMinTimeEdit*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_COMBOBOX:
			((COptComboBox*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_LISTBOX:
			((CHLBox*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_CHECKBOX:
			((CCheckBox*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_GROUP:
			((CGroupButton*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		case WNDCTRL_MCCOMBOBOX:
			((CMCComboBox*) m_WndCtrl)->EnableWindow(bEnable);
			break;
		default:
			break;
	}
}

void CWndCtrl::ShowWindow(int nCmdShow)
{
	if(!m_WndCtrl)
		return;

	switch(GetType())
	{
		case WNDCTRL_EDIT:
			((CCntlEdit*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_NUMEDIT:
			((CNumEdit*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_IDEDIT:
			((CMaskIDEdit*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_MASKDATEEDIT:
			((CMaskDateEdit*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_MASKMINTIMEEDIT:
			((CMaskMinTimeEdit*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_COMBOBOX:
			((COptComboBox*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_LISTBOX:
			((CHLBox*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_CHECKBOX:
			((CCheckBox*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_GROUP:
			((CGroupButton*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		case WNDCTRL_MCCOMBOBOX:
			((CMCComboBox*) m_WndCtrl)->ShowWindow(nCmdShow);
			break;
		default:
			break;
	}
}

CString &CWndCtrl::GetWndData(CString &Data, BOOL bFind)
{
	Data.Empty();
	if(!m_WndCtrl)
		return Data;

	switch(GetType())
	{
		case WNDCTRL_EDIT:
			((CCntlEdit*) m_WndCtrl)->GetWindowText(Data);
			break;
		case WNDCTRL_NUMEDIT:
			((CNumEdit*) m_WndCtrl)->GetWindowText(Data);
			break;
		case WNDCTRL_IDEDIT:
			Data = ((CMaskIDEdit*) m_WndCtrl)->GetData();
			break;
		case WNDCTRL_MASKDATEEDIT:
			Data = ((CMaskDateEdit*) m_WndCtrl)->GetData();
			break;
		case WNDCTRL_MASKMINTIMEEDIT:
			Data = ((CMaskMinTimeEdit*) m_WndCtrl)->GetData();
			break;
		case WNDCTRL_COMBOBOX:
			((COptComboBox*) m_WndCtrl)->GetSelString(Data);
			break;
		case WNDCTRL_LISTBOX:
			((CHLBox*) m_WndCtrl)->GetSelString(Data);
			break;
		case WNDCTRL_CHECKBOX:
			Data = ((CCheckBox*) m_WndCtrl)->GetCheckString();
			if(bFind && ((CCheckBox*) m_WndCtrl)->GetCheck() == 0)
				Data.Empty();
			break;
		case WNDCTRL_GROUP:
			Data = ((CGroupButton*) m_WndCtrl)->GetCheckString();
			break;
		case WNDCTRL_MCCOMBOBOX:
			((CMCComboBox*) m_WndCtrl)->GetSelString(Data);
			break;
		case WNDCTRL_STRING:
			Data = *((CString *) m_WndCtrl);
			break;
		default:
			break;
	}

	return Data;
}

LPCTSTR CWndCtrl::GetData(BOOL bFind)
{
	GetWndData(m_Text, bFind);

	if(m_pData)
		*m_pData = GetText();

	return GetText();
}

BOOL CWndCtrl::GetModify()
{
	CString Data;

	return strcmp(GetText(), GetWndData(Data)) ? TRUE : FALSE;
}




IMPLEMENT_DYNAMIC(CWndCtrlArray, CObArray)
CWndCtrlArray::CWndCtrlArray(CWndCtrlArray &WndCtrlArray)
{	
	GetBase() = WndCtrlArray.GetBase();
	for(int i = 0; i < WndCtrlArray.GetSize(); i ++)
		Add(WndCtrlArray.GetAt(i));
}

void CWndCtrlArray::SetAt(int Index, CWndCtrl *WndCtrl)
{
	CWndCtrl *pWndCtrl;
	 	
	pWndCtrl = GetAt(Index);
	if(pWndCtrl)
	{
		delete pWndCtrl;
		CObArray::SetAt(Index, WndCtrl);
	}
}

void CWndCtrlArray::InsertAt(int Index, CWndCtrl *WndCtrl)
{
	if(Index >= 0)
		CObArray::InsertAt(Index, WndCtrl);
}

void CWndCtrlArray::RemoveAt(int Index)
{
	CWndCtrl *pWndCtrl;

	pWndCtrl = GetAt(Index);
	if(pWndCtrl)
	{
		CObArray::RemoveAt(Index);
		delete pWndCtrl;
	}
}

void CWndCtrlArray::RemoveAll()
{
	CWndCtrl *pWndCtrl;

	for(int i = 0; i < GetSize(); i ++)
	{
		pWndCtrl = GetAt(i);
		if(pWndCtrl)
			delete pWndCtrl;
	}
	
	CObArray::RemoveAll();
}

void CWndCtrlArray::Refresh()
{
	SetID(EMPTYSTRING);
	for(int i = 0; i < GetSize(); i ++)
		GetAt(i)->SetData(EMPTYSTRING);
}

CWndCtrl *CWndCtrlArray::GetAt(int Index)
{
	if(Index >= 0 && Index < GetSize())
		return (CWndCtrl *) CObArray::GetAt(Index);

	return NULL;
}

BOOL CWndCtrlArray::GetModify(BOOL bFromSource)
{
	if(!bFromSource)
		return GetBase().GetModify();

	SetModify(FALSE);
	for(int i = 0; i < GetSize() && !GetBase().GetModify(); i ++)
	{
		if(GetAt(i)->GetModify())
			SetModify();
	}

	return GetBase().GetModify();
}

void CWndCtrlArray::ToStringArray(CStringArray &StringArray, BOOL bFind)
{
	StringArray.RemoveAll();
	for(int i = 0; i < GetSize(); i ++)
		StringArray.Add(GetAt(i)->GetData(bFind));
}

void CWndCtrlArray::ToDBRec(CDBRec &DBRec, BOOL bFind)
{
	DBRec.RemoveAll();
	for(int i = 0; i < GetSize(); i ++)
		DBRec.Add(GetAt(i)->GetData(bFind));
}

void CWndCtrlArray::UpdateData(BOOL bFind)
{
	if(!GetModify())
		return;

	for(int i = 0; i < GetSize(); i ++)
		GetAt(i)->GetData(bFind);
}

void CWndCtrlArray::UpdateWndCtrl()
{
	for(int i = 0; i < GetSize(); i ++)
		GetAt(i)->SetData();
}
