#include "stdafx.h"
#include "orafield.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(COraField, CObject)

COraField::COraField()
{
	Setup(EMPTYSTRING, -1, 0, 0, 0);
}

COraField::COraField(COraField &Field)
{
	Setup(Field.GetName(), Field.GetType(), Field.GetSize(), Field.GetScale(), 
			Field.GetPrecision(), Field.GetVisible(), Field.WithComma());
}

COraField::COraField(LPCTSTR Name, short Type, int Size, int Scale, int Precision,
					 BOOL Visible, BOOL WithComma)
{
	Setup(Name, Type, Size, Scale, Precision, Visible, WithComma);
}

void COraField::Setup(LPCTSTR Name, short Type, int Size, int Scale, int Precision,
					 BOOL Visible, BOOL WithComma)
{
	SetName(Name);
	SetType(Type);
	SetSize(Size);
	SetScale(Scale);
	SetPrecision(Precision);
	SetVisible(Visible);
	SetWithComma(WithComma);
}

void COraField::SetName(LPCTSTR Name)
{
	if(!Name)
		m_Name = EMPTYSTRING;
	else
		m_Name = Name;
}



IMPLEMENT_DYNAMIC(CFieldArray, CObArray)
int	CFieldArray::Add(COraField &Field)
{
	return CObArray::Add(new COraField(Field));
}

int	CFieldArray::FindAt(LPCTSTR FieldName)
{
	for(int i = 0; i < GetSize(); i++)
		if(strcmp(GetAt(i)->GetName(), FieldName) == 0)
			return i;

	return -1;
}

COraField *CFieldArray::GetAt(int Index)
{
	if(Index >= 0 && Index < GetSize())
		return (COraField*) CObArray::GetAt(Index);
	
	return NULL;
}

COraField* CFieldArray::GetAt(LPCTSTR FieldName)
{
	int Index;

	Index = FindAt(FieldName);
	if(Index >= 0)
		return GetAt(Index);
	
	return NULL;
}

void CFieldArray::InsertAt(int Index, COraField &Field)
{
	if(Index >= 0)
		CObArray::InsertAt(Index, new COraField(Field));
}

void CFieldArray::RemoveAll()
{
	int i;
	COraField* pOraField;
	
	for(i = 0; i < GetSize(); i++)
	{
		pOraField = GetAt(i);
		if(pOraField)
			delete pOraField;	
	}
	
	CObArray::RemoveAll();		
}

void CFieldArray::RemoveAt(int Index)
{
	COraField* pOraField;

	pOraField = GetAt(Index);
	if(pOraField)
	{
		delete pOraField;
		CObArray::RemoveAt(Index);
	}
}

void CFieldArray::SetAt(int Index, COraField &Field)
{
	COraField* pOldOraField;
	 	
	pOldOraField = GetAt(Index);
	if(pOldOraField)
	{
		delete pOldOraField;
		CObArray::SetAt(Index, new COraField(Field));
	}
}

void CFieldArray::Copy(CFieldArray &FieldArray)
{
	RemoveAll();
	for(int i = 0; i < FieldArray.GetSize(); i ++)
		Add(*FieldArray.GetAt(i));
}