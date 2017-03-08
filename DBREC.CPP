#include "stdafx.h"
#include "dbrec.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CDBRec, CDBBase)

CDBRec::CDBRec()
{
	RemoveAll();
}

CDBRec::CDBRec(CDBRec &DBRec) : CDBBase(DBRec)
{
	SetID(DBRec.GetRecID());
	SetRec(DBRec.GetRec());
}

void CDBRec::SetRec(CStringArray &Rec)
{
	RemoveAll();
	for(int i = 0; i < Rec.GetSize(); i++)
		Add(Rec.GetAt(i));
}

void CDBRec::SetRec(CDBRec &Rec)
{
	RemoveAll();
	for(int i = 0; i < Rec.GetSize(); i++)
		Add(Rec.GetAt(i));
}

LPCTSTR CDBRec::GetAt(int Index)
{
	if(Index >= 0 && Index < GetRec().GetSize())
		return (const char*) GetRec().GetAt(Index);
	
	return NULL;
}

int CDBRec::Add(LPCTSTR Field)
{
	return GetRec().Add(Field);
}

void CDBRec::SetAt(int Index, LPCTSTR Field)
{
	if(Index >= 0 && Index < GetRec().GetSize())
		GetRec().SetAt(Index, Field);
}

void CDBRec::RemoveAt(int Index)
{
	if(Index >= 0 && Index < GetRec().GetSize())
		GetRec().RemoveAt(Index);
}

void CDBRec::RemoveAll()
{
	GetRec().RemoveAll();
}

void CDBRec::InsertAt(int Index, LPCTSTR Field)
{
	if(Index >= 0 && Index < GetRec().GetSize())
		GetRec().InsertAt(Index, Field);
}

CString &CDBRec::ToString(CString &Str)
{
	Str.Empty();
	for(int i = 0; i < GetSize(); i++)	
	{
		if(i > 0)
			Str += TAB;
		Str += GetAt(i);			
	}

	return Str;
}
