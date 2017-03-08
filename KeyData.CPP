#include "stdafx.h"
#include "keydata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CKeyData, CObject)

CKeyData::CKeyData()
{
	IsModified() = FALSE;
	SetKey(EMPTYSTRING);
}

void CKeyData::Copy(CKeyData &KeyData)
{
	IsModified() = KeyData.IsModified();
	SetKey(KeyData.GetKey());
}



IMPLEMENT_DYNAMIC(CKeyDataArray, CObArray)
    
int	CKeyDataArray::Add(CKeyData *KeyData)
{
	int i, j = -1;
	CKeyData *pKeyData;

	if(!KeyData)
		return -1;

	for(i = 0; i < GetSize(); i++)
	{
		pKeyData = GetAt(i);
		if(pKeyData)
		{
			if(pKeyData->GetKey() == KeyData->GetKey())
				return i;
			else
				if(pKeyData->GetKey() < KeyData->GetKey())
					j = i;
				else
					if(pKeyData->GetKey() > KeyData->GetKey())
					{
						if(j < 0)
							InsertAt(0, KeyData);
						else
							InsertAt(j, KeyData);
						return j;
					}
		}
	}
	
	return CObArray::Add(KeyData);
}

void CKeyDataArray::SetAt(int Index, CKeyData *KeyData)
{
	if(FreePointer(Index))
		CObArray::SetAt(Index, KeyData);
}

void CKeyDataArray::RemoveAt(int Index)
{
	if(FreePointer(Index))
		CObArray::RemoveAt(Index);
}

void CKeyDataArray::RemoveAll()
{
	for(int i = 0; i < GetSize(); i++)
		FreePointer(i);
    
    CObArray::RemoveAll();
}

CKeyData *CKeyDataArray::GetAt(int Index)
{
	if(Index < 0 || Index >= GetSize())
		return NULL;

	return (CKeyData*) CObArray::GetAt(Index);
}

void CKeyDataArray::Copy(CKeyDataArray &KeyDataArray)
{
	RemoveAll();
	for(int i = 0; i < KeyDataArray.GetSize(); i++)
		CObArray::Add(new CKeyData(*KeyDataArray.GetAt(i)));
}

BOOL CKeyDataArray::IsModified()
{
	CKeyData *pData;
	for(int i = 0; i < GetSize(); i ++)
	{
		pData = GetAt(i);
		if(pData && pData->IsModified())
			return TRUE;
	}

	return FALSE;
}

int CKeyDataArray::Find(const CString Key)
{
	int Start = 0, End, Idx;
	CKeyData *pData;
	
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

BOOL CKeyDataArray::FreePointer(int Index)
{
	CKeyData *pOriKeyData;

	pOriKeyData = GetAt(Index);
	if(pOriKeyData)
	{
		delete pOriKeyData;
		return TRUE;
	}

	return FALSE;
}