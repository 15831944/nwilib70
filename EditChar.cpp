#include "stdafx.h"
#include "editchar.h"

IMPLEMENT_DYNAMIC(CEditChar, CObject)

CEditChar::CEditChar()
{
}

CEditChar::CEditChar(CString &StrValidChars)
{
	Setup(StrValidChars, FALSE);
}

CEditChar::CEditChar(CString &StrValidChars, BOOL bStaticChar)
{
	Setup(StrValidChars, bStaticChar);
}

CEditChar::CEditChar(CEditChar &EditChar)
{
	Setup(EditChar.GetValids(), EditChar.IsStaticChar());
}

void CEditChar::Setup(CString &StrValidChar, BOOL bStaticChar)
{
	m_strValids = StrValidChar;
	m_bStaticChar = bStaticChar;
	
	if(bStaticChar)
		m_strData = m_strValids;
	else
		m_strData.Empty();
}

BOOL CEditChar::IsValid(CString Data)
{
	return m_strValids.Find(Data) >= 0 ? TRUE : FALSE;
}



IMPLEMENT_DYNAMIC(CEditCharArray, CObArray)
CEditCharArray::~CEditCharArray()
{
	RemoveAll();
}
    
int	CEditCharArray::Add(CEditChar *EditChar)
{
	return CObArray::Add(EditChar);
}

int CEditCharArray::Add(CEditChar &EditChar)
{
	return CObArray::Add(new CEditChar(EditChar));
}

void CEditCharArray::SetAt(int Index, CEditChar *EditChar)
{
	if(Index >= 0 && Index < GetSize())
	{
		CEditChar *pEditChar = GetAt(Index);
		if(pEditChar)
		{
			delete pEditChar;
			CObArray::SetAt(Index, EditChar);
		}
	}
	else
		Add(*EditChar);
}

void CEditCharArray::SetAt(int Index, CEditChar &EditChar)
{
	if(Index >= 0 && Index < GetSize())
	{
		CEditChar *pEditChar = GetAt(Index);
		if(pEditChar)
		{
			delete pEditChar;
			CObArray::SetAt(Index, new CEditChar(EditChar));
		}
	}
	else
		Add(EditChar);
}

void CEditCharArray::RemoveAt(int Index)
{
	CEditChar* pData = GetAt(Index);
	
	if(pData)
	{
		CObArray::RemoveAt(Index);
		delete pData;
	}
}

void CEditCharArray::RemoveAll()
{
	CEditChar* pData;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pData = GetAt(i);
		if(pData)
			delete pData;
    }

    CObArray::RemoveAll();
}

CEditChar *CEditCharArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CEditChar*) CObArray::GetAt(Index);
}

void CEditCharArray::InsertAt(int Index, CEditChar *EditChar)
{
	if(Index < GetSize()) // if limit is not set or compare size if set.
		CObArray::InsertAt(Index, EditChar);
}

void CEditCharArray::InsertAt(int Index, CEditChar &EditChar)
{
	if(Index < GetSize()) // if limit is not set or compare size if set.
		CObArray::InsertAt(Index, new CEditChar(EditChar));
}

void CEditCharArray::Copy(CEditCharArray &EditCharArray)
{
	RemoveAll();

	for(int i = 0; i < EditCharArray.GetSize(); i++)
		Add(*EditCharArray.GetAt(i));
}
