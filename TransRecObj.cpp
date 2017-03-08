#include "stdafx.h"
#include "transrecobj.h"


IMPLEMENT_DYNAMIC(CTransRecObj, CRecObj)
void CTransRecObj::Copy(CTransRecObj &TransRecObj)
{
	SetID(TransRecObj.GetID());
	SetNAVLock(TransRecObj.GetNAVLock());
	SetCashLock(TransRecObj.GetCashLock());
	SetPayInstructLock(TransRecObj.GetPayInstructLock());
	SetMonthlyLock(TransRecObj.GetMonthlyLock());
}



IMPLEMENT_DYNAMIC(CTransRecObjArray, CRecObjArray)
CTransRecObj* CTransRecObjArray::GetAt(int Index)
{
	if(Index >= 0 && Index < GetSize())
		return (CTransRecObj*) CObArray::GetAt(Index);
	return NULL;
}

int CTransRecObjArray::Add(CTransRecObj &TransRecObj)
{
	return CObArray::Add(new CTransRecObj(TransRecObj));
}

void CTransRecObjArray::RemoveAll()
{
	CTransRecObj *pTransRecObj;

	for(int i = 0; i < GetSize(); i ++)
	{
		pTransRecObj = GetAt(i);
		if(pTransRecObj)
			delete pTransRecObj;
	}
	
	CObArray::RemoveAll();
}

void CTransRecObjArray::Copy(CTransRecObjArray &TransRecObjArray)
{
	SetID(TransRecObjArray.GetID());
	RemoveAll();
	for (int i = 0; i < TransRecObjArray.GetSize(); i ++)
		Add(*TransRecObjArray.GetAt(i));
}