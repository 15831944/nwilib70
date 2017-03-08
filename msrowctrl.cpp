#include "stdafx.h"
#include "msrowctrl.h"


IMPLEMENT_DYNAMIC(CMSRowCtrl, CRowCtrl)
CMSRowCtrl::CMSRowCtrl(CMSRowCtrl &MSRowCtrl) : CRowCtrl(MSRowCtrl)
{
	SetOraLoader(MSRowCtrl.GetOraLoader());
	GetSRowCtrl().Setup(MSRowCtrl.GetSS());
	SetKeyIndex(-1);
}

void CMSRowCtrl::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex) 
{ 
	SetOraLoader(OraLoader);
	CRowCtrl::Setup(pSS);
	GetSRowCtrl().Setup(pSSS);
	SetKeyIndex(KeyIndex);
}

void CMSRowCtrl::LoadSheet(CDBRecArray &DBRecArray)
{
	if(!IsReady())
		return;
	
	if(GetCurrentRow() < 1)
		return;

	CDBRec *pDBRec;
	GetSRowCtrl().ClearSheet();
	for(int i = 0; i < DBRecArray.GetSize(); i ++)
	{
		pDBRec = DBRecArray.GetAt(i);
		if(pDBRec && !pDBRec->GetDeleted())
			GetSRowCtrl().AddSheetRow(*pDBRec);
	}

	GetSRowCtrl().GetSS()->LockSheet();
	GetSRowCtrl().GetSS()->SheetAutoFit();
}

void CMSRowCtrl::LoadSheet(CKeyDBList &KeyDBList)
{
	CKeyDBRecArray *pKeyDBRecArray;
	pKeyDBRecArray = KeyDBList.GetAt(GetKey());
	if(pKeyDBRecArray)
		LoadSheet(*pKeyDBRecArray);
}

void CMSRowCtrl::UpdateSheetRow(BOOL bFromSheet)
{
	CRowCtrl::UpdateSheetRow(bFromSheet);
	SetKey();
}

BOOL CMSRowCtrl::SetCurrentRow(int CurrentRow, BOOL UpdatePrevRow)
{
	if(!IsReady())
		return FALSE;

	if(CurrentRow > 0 && CurrentRow <= GetSS()->GetSheetRows())
	{
		if(UpdatePrevRow)
			GetSRowCtrl().UpdateSheetRow(FALSE);
		CRowCtrl::SetCurrentRow(CurrentRow, UpdatePrevRow);
		LoadSheet();
		if(GetSRowCtrl().GetSS()->GetSheetRows() > 0)
			GetSRowCtrl().SetCurrentRow(1);
		else
			GetSRowCtrl().SetCurrentRow(-1);
		return TRUE;
	}
	else
	{
		GetSS()->SetSheetCurRow(-1);
		GetSRowCtrl().ClearSheet();
		GetSRowCtrl().Refresh();
		return FALSE;
	}
}

BOOL CMSRowCtrl::SetCurrentRow(int CurrentRow, CKeyDBList &KeyDBList, BOOL UpdatePrevRow)
{
	if(!IsReady())
		return FALSE;

	if(CurrentRow > 0 && CurrentRow <= GetSS()->GetSheetRows())
	{
		if(UpdatePrevRow)
			GetSRowCtrl().UpdateSheetRow(FALSE);
		CRowCtrl::SetCurrentRow(CurrentRow, UpdatePrevRow);
		LoadSheet(KeyDBList);
		if(GetSRowCtrl().GetSS()->GetSheetRows() > 0)
			GetSRowCtrl().SetCurrentRow(1);
		else
			GetSRowCtrl().SetCurrentRow(-1);
		return TRUE;
	}
	else
	{
		GetSS()->SetSheetCurRow(-1);
		GetSRowCtrl().ClearSheet();
		GetSRowCtrl().Refresh();
		return FALSE;
	}
}


void CMSRowCtrl::SetKey()
{
	if(GetKeyIndex() < 0 || GetKeyIndex() >= GetSize())
		return;

	SetKey(GetAt(GetKeyIndex())->GetData());
}

void CMSRowCtrl::SetKeyIndex(int KeyIndex)
{
	if(KeyIndex >= 0 && KeyIndex < GetSize())
		m_KeyIndex = KeyIndex;
	else
		m_KeyIndex = -1;
}

void CMSRowCtrl::UpdateData(CRecObj *pRec, CRecObj *pInv)
{
	CWndCtrlArray::UpdateData();
	SetKey();
	GetSRowCtrl().UpdateData();

	if(pRec)
		pRec->SetID(GetID());
	if(pInv)
		pInv->SetID(GetSRowCtrl().GetID());
}
