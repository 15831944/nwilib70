#include "stdafx.h"
#include "dbbase.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CDBBase, CObject)

CDBBase::CDBBase()
{
	SetModify(FALSE);
	SetDeleted(FALSE);
	SetInDatabase(FALSE);
	SetListed(FALSE);
	SetLocked(FALSE);
	SetID(EMPTYSTRING);
}

void CDBBase::Copy(CDBBase &DBBase)
{
	SetModify(DBBase.GetModify());
	SetDeleted(DBBase.GetDeleted());
	SetInDatabase(DBBase.GetInDatabase());
	SetListed(DBBase.GetListed());
	SetLocked(DBBase.GetLocked());
	SetID(DBBase.GetID());	
}





IMPLEMENT_DYNAMIC(CDBBaseArray, CObArray)
CDBBaseArray::CDBBaseArray()
{
}

CDBBaseArray::~CDBBaseArray()
{
	RemoveAll();
}
    
int	CDBBaseArray::Add(CDBBase *Rec)
{
	return CObArray::Add(Rec);
}

int CDBBaseArray::Add(CDBBase &Rec)
{
	return CObArray::Add(new CDBBase(Rec));
}

void CDBBaseArray::SetAt(int Index, CDBBase *Rec)
{
	CDBBase *pRec;

	pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, Rec);
	}
}

void CDBBaseArray::SetAt(int Index, CDBBase &Rec)
{
	CDBBase *pRec;

	pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, new CDBBase(Rec));
	}
}

void CDBBaseArray::RemoveAt(int Index)
{
	CDBBase* Data;
	
	Data = GetAt(Index);
	if(Data)
	{
		CObArray::RemoveAt(Index);
		delete Data;		
	}
}

void CDBBaseArray::RemoveAll()
{
	CDBBase*	Data;
	
	for(int i = 0; i < GetSize(); i++)
	{
		Data = GetAt(i);
		if(Data)
			delete Data;				
    }
    
    CObArray::RemoveAll();
}

CDBBase *CDBBaseArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CDBBase*) CObArray::GetAt(Index);
}

void CDBBaseArray::InsertAt(int Index, CDBBase *Rec)
{
	CObArray::InsertAt(Index, Rec);	
}

void CDBBaseArray::InsertAt(int Index, CDBBase &Rec)
{
	CObArray::InsertAt(Index, new CDBBase(Rec));
}

void CDBBaseArray::Copy(CDBBaseArray &DBBaseArray)
{
	RemoveAll();
	for(int i = 0; i < DBBaseArray.GetSize(); i++)
		Add(*DBBaseArray.GetAt(i));
}