#include "stdafx.h"
#include "sortarray.h"


IMPLEMENT_DYNAMIC(CSortArray, CObArray)
int	CSortArray::Add(CObject *Object)
{
	int i = Order(Object);
	if(i < 0)
		return CObArray::Add(Object);
	else
		InsertAt(i, Object);	
	return -1;
}

int CSortArray::Search(CObject *Object)
{
	if(GetSortable())
		return BSearch(Object);
	
	CObject *Obj2;
	for(int i = 0; i < GetSize(); i ++)
	{
		Obj2 = GetAt(i);
		if(Compare(Object, Obj2) == 0)
			return i;
	}

	return -1;
}

int CSortArray::BSearch(CObject *Object)
{
	int Start = 0, Mid, End, Rc;
	CObject *Obj2;

	if(GetSize() <= 0)
		return -1;

	End = GetSize() - 1;
	while(End - Start >= 2)
	{
		Mid = (Start + End)/2;
		Obj2 = GetAt(Mid);
		Rc = Compare(Object, Obj2);
		if(Rc == 0) // Match Find
			return Mid;
		else
			if(Rc > 0) // Object > Obj2
				Start = Mid;
			else
				End = Mid; // Object < Obj
	}

	if(GetSize() == 1)
		Mid = 0;
	else
	{
		if(Rc > 0)
			Mid = End;
		else
			Mid = Start;
	}

	Obj2 = GetAt(Mid);
	if(Compare(Object, Obj2) == 0)
		return Mid;
	return -1;
}

int CSortArray::Order(CObject *Object)
{
	if(!GetSortable())
		return -1;

	CObject *Obj2;
	
	if(GetSize() == 0)
		return -1;

	for(int i = 0; i < GetSize(); i ++)
	{
		Obj2 = GetAt(i);
		if(Compare(Object, Obj2) >= 0)
			return i;
	}

	return -1;
}