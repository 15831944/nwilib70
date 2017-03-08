#include "stdafx.h"
#include "keydblist.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CKeyDBRecArray, CDBRecArray)
CKeyDBRecArray::CKeyDBRecArray(CKeyDBRecArray &RecArray) : CDBRecArray(RecArray)
{
	SetKey(RecArray.GetKey());
}


IMPLEMENT_DYNAMIC(CKeyDBList, CObArray)
CKeyDBList::CKeyDBList(CKeyDBList &KeyDBList)
{
	RemoveAll();
	for(int i = 0; i < KeyDBList.GetSize(); i ++)
		Add(*KeyDBList.GetAt(i));
}	

int	CKeyDBList::Add(CKeyDBRecArray *RecArray)
{
	return CObArray::Add(RecArray);
}

int CKeyDBList::Add(CKeyDBRecArray &RecArray)
{
	return CObArray::Add(new CKeyDBRecArray(RecArray));
}

void CKeyDBList::SetAt(int Index, CKeyDBRecArray *RecArray)
{
	CKeyDBRecArray *pRecArray = GetAt(Index);
	if(pRecArray)
	{
		delete pRecArray;
		CObArray::SetAt(Index, RecArray);
	}
}

void CKeyDBList::SetAt(int Index, CKeyDBRecArray &RecArray)
{
	CKeyDBRecArray *pRecArray = GetAt(Index);
	if(pRecArray)
	{
		delete pRecArray;
		CObArray::SetAt(Index, new CKeyDBRecArray(RecArray));
	}
}

void CKeyDBList::InsertAt(int Index, CKeyDBRecArray *RecArray)
{
	CObArray::InsertAt(Index, RecArray);	
}

void CKeyDBList::InsertAt(int Index, CKeyDBRecArray &RecArray)
{
	CObArray::InsertAt(Index, new CKeyDBRecArray(RecArray));
}

void CKeyDBList::RemoveAt(int Index)
{
	CKeyDBRecArray *pRecArray = GetAt(Index);
	if(pRecArray)
	{
		delete pRecArray;
		CObArray::RemoveAt(Index);
	}
}

void CKeyDBList::RemoveAll()
{
	CKeyDBRecArray *pRecArray;
	for(int i = 0; i < GetSize(); i ++)
	{
		pRecArray = GetAt(i);
		if(pRecArray)
			delete pRecArray;
	}

	CObArray::RemoveAll();
}

CKeyDBRecArray *CKeyDBList::GetAt(int Index)
{
	if(Index >= 0 && Index < GetSize())
		return (CKeyDBRecArray *) CObArray::GetAt(Index);
	return NULL;
}

CKeyDBRecArray *CKeyDBList::GetAt(LPCTSTR Key)
{
	int i = Find(Key);
	return GetAt(i);
}

int CKeyDBList::Find(LPCTSTR Key)
{
	CKeyDBRecArray *pRecArray;

	if(!Key || strlen(Key) == 0)
		return -1;

	for(int i = 0; i < GetSize(); i ++)
	{
		pRecArray = GetAt(i);
		if(pRecArray && strcmp(pRecArray->GetKey(), Key) == 0)
			return i;
	}

	return -1;
}

int CKeyDBList::CopyDataToDBRecArray(LPCTSTR Key, CDBRecArray &DBRecArray)
{
	CKeyDBRecArray *pRecArray;

	DBRecArray.RemoveAll();
	pRecArray = GetAt(Key);
	if(!pRecArray)
		return -1;
	
	CDBRec *pDBRec;	
	for(int i = 0; i < pRecArray->GetSize(); i ++)
	{
		pDBRec = pRecArray->GetAt(i);
		if(pDBRec)
			DBRecArray.Add(*pDBRec);
	}

	return DBRecArray.GetSize();
}

int CKeyDBList::CopyKeyToComboBox(COptComboBox &ComboBox)
{
	ComboBox.ResetContent();
	
	if(ComboBox.GetWithNone())
		ComboBox.AddString(NONE);
	for(int i = 0; i < GetSize(); i ++)
		ComboBox.AddString(GetAt(i)->GetKey());

	return ComboBox.GetCount();
}

int CKeyDBList::CopyDataToComboBox(LPCTSTR Key, COptComboBox &ComboBox)
{
	CKeyDBRecArray *pRecArray;

	ComboBox.ResetContent();
	pRecArray = GetAt(Key);
	if(!pRecArray)
		return -1;

	CString Data;
	CDBRec *pDBRec;
	int i, j;
	
	if(ComboBox.GetWithNone())
		ComboBox.AddString(NONE);

	for(i = 0; i < pRecArray->GetSize(); i ++)
	{
		pDBRec = pRecArray->GetAt(i);
		if(pDBRec)
		{
			Data.Empty();
			for(j = 0; j < pDBRec->GetSize(); j ++)
			{
				if(j > 0)
					Data += SPACE;
				Data += pDBRec->GetAt(j);
			}
			ComboBox.AddString(Data);
		}
	}

	return ComboBox.GetCount();
}

int CKeyDBList::CopyToMCComboBox(LPCTSTR Key, CMCComboBox &ComboBox, int KeyCol)
{
	CKeyDBRecArray *pRecArray;
	CStringArray StringArray;

	ComboBox.ResetContent();
	pRecArray = GetAt(Key);
	if(!pRecArray)
		return -1;

	CDBRec *pDBRec;
	int i, j;
	
	if(pRecArray->GetSize() <= 0)
		return 0;
	
	if(pRecArray->GetAt(0)->GetSize() <= KeyCol)
		KeyCol = 0;

	if(ComboBox.GetColumnCount() < pRecArray->GetAt(0)->GetSize())
		ComboBox.SetColumnCount(pRecArray->GetAt(0)->GetSize());

	if(ComboBox.GetWithNone())
		ComboBox.AddStringNone();

	for(i = 0; i < pRecArray->GetSize(); i ++)
	{
		pDBRec = pRecArray->GetAt(i);
		if(pDBRec)
		{
			StringArray.RemoveAll();
			StringArray.Add(pDBRec->GetAt(KeyCol));
			for(j = 0; j < pDBRec->GetSize(); j ++)
			{
				if(j != KeyCol)
					StringArray.Add(pDBRec->GetAt(j));
			}
			ComboBox.AddString(StringArray);
		}
	}

	return ComboBox.GetCount();
}

int CKeyDBList::CopyKeyToListBox(CHLBox &ListBox)
{
	ListBox.ResetContent();
	if(ListBox.GetWithNone())
		ListBox.AddString(NONE);
	for(int i = 0; i < GetSize(); i ++)
		ListBox.AddString(GetAt(i)->GetKey());

	return ListBox.GetCount();
}

int CKeyDBList::CopyDataToListBox(LPCTSTR Key, CHLBox &ListBox)
{
	CKeyDBRecArray *pRecArray;

	ListBox.ResetContent();
	pRecArray = GetAt(Key);
	if(!pRecArray)
		return -1;

	CString Data;
	CDBRec *pDBRec;
	int i, j;
	
	if(ListBox.GetWithNone())
		ListBox.AddString(NONE);
	for(i = 0; i < pRecArray->GetSize(); i ++)
	{
		pDBRec = pRecArray->GetAt(i);
		if(pDBRec)
		{
			Data.Empty();
			for(j = 0; j < pDBRec->GetSize(); j ++)
			{
				if(j > 0)
					Data += SPACE;
				Data += pDBRec->GetAt(j);
			}
			
			ListBox.AddString(Data);
		}
	}

	return ListBox.GetCount();
}

int CKeyDBList::CopyDataToDBSheet(LPCTSTR Key, CDBSheet &SS)
{
	CKeyDBRecArray *pRecArray;

	SS.ClearSheet();
	pRecArray = GetAt(Key);
	if(!pRecArray)
		return -1;

	CStringArray DataArray;
	CDBRec *pDBRec;
	int i, j;

	for(i = 0; i < pRecArray->GetSize(); i ++)
	{
		pDBRec = pRecArray->GetAt(i);
		if(pDBRec)
		{
			DataArray.RemoveAll();
			for(j = 0; j < pDBRec->GetSize(); j ++)
				DataArray.Add(pDBRec->GetAt(j));
			
			SS.AddSheetRow(DataArray);
		}
	}

	SS.SheetAutoFit();

	return SS.GetSheetRows();
}

int CKeyDBList::CopyDataToRowCtrl(LPCTSTR Key, CRowCtrl &RowCtrl)
{
	CKeyDBRecArray *pRecArray;

	RowCtrl.ClearSheet();
	pRecArray = GetAt(Key);
	if(!pRecArray)
		return -1;

	CDBRec *pDBRec;

	for(int i = 0; i < pRecArray->GetSize(); i ++)
	{
		pDBRec = pRecArray->GetAt(i);
		if(pDBRec)			
			RowCtrl.AddSheetRow(*pDBRec);
	}

	RowCtrl.GetSS()->SheetAutoFit();

	return RowCtrl.GetSS()->GetSheetRows();
}

BOOL CKeyDBList::IsModified()
{
	if(m_bModified)
		return m_bModified;

	for(int i = 0; i < GetSize(); i ++)
		if(GetAt(i)->IsModified())
			return TRUE;
	
	return FALSE;
}

void CKeyDBList::Modified(BOOL bModified)
{
	m_bModified = bModified;
	
	if(!m_bModified)
	{
		for(int i = 0; i < GetSize(); i ++)
			GetAt(i)->Modified(m_bModified);

	}
}