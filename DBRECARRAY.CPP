#include "stdafx.h"
#include "dbrecarray.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CDBRecArray, CObArray)
int	CDBRecArray::Add(CDBRec *Rec)
{
	return CObArray::Add(Rec);
}

int CDBRecArray::Add(CDBRec &Rec)
{
	return CObArray::Add(new CDBRec(Rec));
}

void CDBRecArray::SetAt(int Index, CDBRec *Rec)
{
	CDBRec *pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, Rec);
	}
}

void CDBRecArray::SetAt(int Index, CDBRec &Rec)
{
	CDBRec *pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, new CDBRec(Rec));
	}
}

void CDBRecArray::RemoveAt(int Index)
{
	CDBRec *pRec = GetAt(Index);
	if(pRec)
	{
		CObArray::RemoveAt(Index);
		delete pRec;		
	}
}

void CDBRecArray::RemoveAll()
{
	CDBRec *pRec;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pRec = GetAt(i);
		if(pRec)
			delete pRec;
    }
    
    CObArray::RemoveAll();
}

CDBRec *CDBRecArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CDBRec*) CObArray::GetAt(Index);
}

void CDBRecArray::InsertAt(int Index, CDBRec *Rec)
{
	CObArray::InsertAt(Index, Rec);	
}

void CDBRecArray::InsertAt(int Index, CDBRec &Rec)
{
	CObArray::InsertAt(Index, new CDBRec(Rec));
}

int CDBRecArray::FindListed(int Index)
{
	CDBRec *pRec;

	for(int i = Index; i < GetSize(); i ++)
	{
		pRec = GetAt(i);
		if(pRec && pRec->GetListed())
			return i;
	}

	return -1;
}

CDBRec *CDBRecArray::GetListedAt(int Index)
{
	CDBRec *pRec;
	
	for(int i = Index; i < GetSize(); i ++)
	{
		pRec = GetAt(i);
		if(pRec && pRec->GetListed())
			return pRec;
	}
	
	return NULL;
}

int CDBRecArray::Find(LPCTSTR ID, int FieldID)
{
	// If FieldID == -1, Use RecID.  Otherwise, use the field in m_Rec accordingly
	int i;
	CDBRec* Data;

	if(!ID || strlen(ID) == 0)
		return -1;
	
	if(GetSize() <= 0)
		return -1;

	if(GetAt(0)->GetSize() <= FieldID) // if Col is out of range, set default to zero
		FieldID = 0;

	for(i = 0; i < GetSize(); i++)
	{
		Data = GetAt(i);
		if(Data)
		{
			if(FieldID < 0)
			{
				if(strcmp(Data->GetRecID(), ID) == 0)
					return i;
			}
			else
			{
				if(strcmp(Data->GetAt(FieldID), ID) == 0)
					return i;
			}
		}
	}

	return -1;
}

void CDBRecArray::Copy(CDBRecArray &DBRecArray)
{
	RemoveAll();
	for(int i = 0; i < DBRecArray.GetSize(); i ++)
		Add(*DBRecArray.GetAt(i));
}

int CDBRecArray::CopyToComboBox(COptComboBox &CBox, int Col)
{
	CBox.ResetContent();
	if(GetSize() <= 0)
		return 0;

	if(CBox.GetWithNone())
		CBox.AddString(NONE);

	if(GetAt(0)->GetSize() <= Col) // if Col is out of range, set default to zero
		Col = 0;
	
	for(int i = 0; i < GetSize(); i++)
		CBox.AddString(GetAt(i)->GetAt(Col));

	return CBox.GetCount();
}

int CDBRecArray::CopyToMCComboBox(CMCComboBox &CBox, int KeyCol)
{
	int i, j;
	CStringArray StringArray;

	if(CBox.GetCount() > 0)
		CBox.ResetContent();

	if(GetSize() <= 0)
		return 0;

	if(CBox.GetColumnCount() < GetAt(0)->GetSize())
	{
		CBox.SetColumnCount(GetAt(0)->GetSize());
	}
	if(CBox.GetWithNone())
		CBox.AddStringNone();

	if(GetAt(0)->GetSize() <= KeyCol) // if Col is out of range, set default to zero
		KeyCol = 0;

	for(i = 0; i < GetSize(); i++)
	{
		StringArray.RemoveAll();
		
		StringArray.Add(GetAt(i)->GetRec().GetAt(KeyCol));
		for(j = 0; j < GetAt(i)->GetRec().GetSize(); j++)
		{
			if(j != KeyCol)
				StringArray.Add(GetAt(i)->GetRec().GetAt(j));
		}

		CBox.AddString(StringArray);
	}

	return CBox.GetCount();
}

int CDBRecArray::CopyToListBox(CHLBox &HLBox, int Col)
{
	HLBox.ResetContent();
	if(GetSize() <= 0)
		return 0;

	if(HLBox.GetWithNone())
		HLBox.AddString(NONE);
	
	if(GetAt(0)->GetSize() <= Col) // if Col is out of range, set default to zero
		Col = 0;

	for(int i = 0; i < GetSize(); i++)
		HLBox.AddString(GetAt(i)->GetAt(Col));

	return HLBox.GetCount();
}

