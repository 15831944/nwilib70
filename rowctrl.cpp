#include "stdafx.h"
#include "rowctrl.h"


IMPLEMENT_DYNAMIC(CRowCtrl, CWndCtrlArray)
CRowCtrl::CRowCtrl(CRowCtrl &RowCtrl) : CWndCtrlArray(RowCtrl)
{	
	SetCurrentRow(RowCtrl.GetCurrentRow());
	SetSS(RowCtrl.GetSS());
	GetIDArray() = RowCtrl.GetIDArray();
}

LPCTSTR CRowCtrl::GetID()
{
	if(GetCurrentRow() > 0)
		return GetIDArray().GetAt(GetCurrentRow() - 1)->GetID();
	return NULL;
}

void CRowCtrl::ClearSheet()
{
	if(!IsReady())
		return;

	GetSS()->ClearSheet();
	GetIDArray().RemoveAll();
}

void CRowCtrl::AddSheetRow(LPCTSTR ID, LPCTSTR Rec)
{ // Add Row From Database
	if(!IsReady())
		return;

	CDBBase Base;
	GetSS()->AddSheetRow(Rec);
	Base.SetID(ID);
	Base.SetInDatabase(TRUE);
	Base.SetDeleted(FALSE);
	GetIDArray().Add(Base);
}

void CRowCtrl::AddSheetRow(CDBRec &DBRec)
{
	if(!IsReady())
		return;

	GetSS()->AddSheetRow(DBRec);
	GetIDArray().Add(DBRec);
}

void CRowCtrl::AddSheetRow()
{ // Add Row From Window Ctrls
	if(!IsReady())
		return;

	CStringArray StringArray;

	UpdateData();
	ToStringArray(StringArray);	
	GetSS()->AddSheetRow(StringArray);
	GetIDArray().Add(GetBase());
}

void CRowCtrl::DeleteSheetRow(int CurrentRow)
{
	if(!IsReady())
		return;

	if(CurrentRow > 0)
		SetCurrentRow(CurrentRow);

	CurrentRow = GetCurrentRow();
	if(CurrentRow < 1)
		return;
		
	GetSS()->DeleteSheetRow(CurrentRow);
	GetIDArray().RemoveAt(CurrentRow - 1);
	SetCurrentRow(-1);
}

void CRowCtrl::UpdateSheetRow(BOOL bFromSheet)
{ // if bFrom Sheet, load data from a sheet row to window ctrls. else update row from window ctrls
	if(!IsReady() || GetCurrentRow() <= 0)
		return;
	
	int i;

	if(bFromSheet) // Load Data from a sheet Row to window ctrls
	{
		GetBase() = *GetIDArray().GetAt(GetCurrentRow() - 1);
		for(i = 0; i < GetSize(); i ++)
			GetAt(i)->SetData(GetSS()->GetSheetText(i + 1, GetCurrentRow()));
	}
	else
	{
		CDBBase *pBase;

		pBase = GetIDArray().GetAt(GetCurrentRow() - 1);
		*pBase = GetBase();
		pBase->SetModify(GetModify());
		for(i = 0; i < GetSize(); i ++) // Load Data from window ctrls to a sheet row
			m_pSS->SetSheetText(i + 1, GetCurrentRow(), GetAt(i)->GetData());
	}
}

void CRowCtrl::Refresh()
{
	CWndCtrlArray::Refresh();
	if(IsReady())
		GetSS()->SetSheetCurRow(-1);
}

BOOL CRowCtrl::SetCurrentRow(int CurrentRow, BOOL bUpdatePrevRow) 
{
	if(!IsReady())
		return FALSE;

	if(bUpdatePrevRow && GetCurrentRow() > 0) // Update Prev Row
		UpdateSheetRow(FALSE);

	if(CurrentRow > 0 && CurrentRow <= GetSS()->GetSheetRows()) // Update if in range
	{		
		GetSS()->SetSheetCurRow(CurrentRow);
		UpdateSheetRow();
		return TRUE;
	}

	GetSS()->SetSheetCurRow(-1);
	Refresh();
	return FALSE;
}

