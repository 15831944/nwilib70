#include "stdafx.h"
#include "blmdata.h"

IMPLEMENT_DYNAMIC(CBlmSec, CKeyData)

void CBlmSec::Copy(CBlmSec &BlmSec)
{
	GetKey() = BlmSec.GetKey();
	GetAssetClass() = BlmSec.GetAssetClass();
	GetTicker() = BlmSec.GetTicker();
	GetBlmKey() = BlmSec.GetBlmKey();
	GetBlmCurr() = BlmSec.GetKey();
	GetIDType() = BlmSec.GetIDType();
	GetCountryCode() = BlmSec.GetCountryCode();
	GetPrevPrice() = BlmSec.GetPrevPrice();
	GetPrice() = BlmSec.GetPrice();
	GetFxrate() = BlmSec.GetFxrate();
	GetFxIdx() = BlmSec.GetFxIdx();
	IsUSDPrice() = BlmSec.IsUSDPrice();
	IsRev() = BlmSec.IsRev();
	IsModified() = BlmSec.IsModified();
}




IMPLEMENT_DYNAMIC(CBlmSecArray, CKeyDataArray)

int	CBlmSecArray::Add(CBlmSec *pSecData)
{
	int i, j = -1;
	CBlmSec *pData;

	if(!pSecData)
		return -1;

	for(i = 0; i < GetSize(); i++)
	{
		pData = GetAt(i);
		if(pData)
		{
			if(pData->GetKey() == pSecData->GetKey())
				return i;
			else
				if(pData->GetKey() < pSecData->GetKey())
					j = i;
				else
					if(pData->GetKey() > pSecData->GetKey())
					{
						if(j < 0)
							InsertAt(0, pSecData);
						else
							InsertAt(j, pSecData);
						return j;
					}
		}
	}
	
	return CObArray::Add(pSecData);
}

void CBlmSecArray::SetAt(int Index, CBlmSec *pBlmSec)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, pBlmSec);
}

void CBlmSecArray::InsertAt(int Index, CBlmSec *pSecData)
{
	if(Index >= 0 && Index < GetSize())
		CObArray::InsertAt(Index, pSecData);
}

CBlmSec *CBlmSecArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CBlmSec*) CObArray::GetAt(Index);
}

void CBlmSecArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CBlmSecArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

BOOL CBlmSecArray::FreePointer(int Index)
{
	CBlmSec *pOriBlmSec;

	pOriBlmSec = GetAt(Index);
	if(pOriBlmSec)
	{
		delete pOriBlmSec;
		return TRUE;
	}

	return FALSE;
}

void CBlmSecArray::Copy(CBlmSecArray &BlmSecArray)
{
	RemoveAll();

	GetFields().Copy(BlmSecArray.GetFields());

	for(int i = 0; i < GetSize(); i++)
		Add(*BlmSecArray.GetAt(i));
}

BOOL CBlmSecArray::IsModified()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i) && GetAt(i)->IsModified())
			return TRUE;
	
	return FALSE;
}

BOOL CBlmSecArray::IsFullyUpdated()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i) && !GetAt(i)->IsModified())
			return FALSE;

	return TRUE;
}

void CBlmSecArray::ResetData()
{
	for(int i = 0; i < GetSize(); i++)
		if(GetAt(i))
			GetAt(i)->IsModified() = FALSE;
}

int CBlmSecArray::Find(const CString Key)
{
	int Start = 0, End, Idx;
	CBlmSec *pData;
	
	if(GetSize() <= 0) // EmptyArray
		return -1;
	
	End = GetSize() - 1;

	for(;;)
	{
		switch(End - Start)
		{
			case 0:
				pData = GetAt(Start);
				if(pData && pData->GetKey() == Key)
					return Start;
				else
					return -1;
				break;
			case 1:
				pData = GetAt(Start);
				if(pData && pData->GetKey() == Key)
					return Start;
				else
				{
					pData = GetAt(End);
					if(pData && pData->GetKey() == Key)
						return End;
					else
						return -1;
				}
				break;
			default:
				Idx = (Start + End)/2;
				pData = GetAt(Idx);
				if(pData)
				{
					if(pData->GetKey() == Key)
						return Idx;
					else
						if(pData->GetKey() < Key)
							Start = Idx;
						else
							End = Idx;
				}
				break;
		}
	}

	return -1;
}


