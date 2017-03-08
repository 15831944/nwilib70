#include "stdafx.h"
#include "recobj.h"


IMPLEMENT_DYNAMIC(CRecObj, CObject)

void CRecObj::SetID(LPCTSTR ID)
{
	if(ID && strlen(ID) > 0)
		m_ID = ID;
	else
		m_ID.Empty();
}


IMPLEMENT_DYNAMIC(CRecObjArray, CObArray)
void CRecObjArray::SetID(LPCTSTR ID)
{
	if(ID && strlen(ID) > 0)
		m_ID = ID;
	else
		m_ID.Empty();
}

CRecObj *CRecObjArray::GetAt(int Index)
{
	if(Index >= 0 && Index < GetSize())
		return (CRecObj*) CObArray::GetAt(Index);
	return NULL;
}

int CRecObjArray::Add(CRecObj &RecObj)
{
	return CObArray::Add(new CRecObj(RecObj));
}

void CRecObjArray::RemoveAll()
{
	CRecObj *pRecObj;

	for(int i = 0; i < GetSize(); i ++)
	{
		pRecObj = GetAt(i);
		if(pRecObj)
			delete pRecObj;
	}
	
	CObArray::RemoveAll();
}

void CRecObjArray::Copy(CRecObjArray &RecObjArray)
{
	SetID(RecObjArray.GetID());
	RemoveAll();
	for (int i = 0; i < RecObjArray.GetSize(); i ++)
		Add(*RecObjArray.GetAt(i));
}