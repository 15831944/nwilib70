#include "stdafx.h"
#include "fifodata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CFifoData, CObject)
CFifoData::CFifoData()
{
}

void CFifoData::Copy(CFifoData &FifoData)
{
	SetTradeDate(FifoData.GetTradeDate());
	SetDir(FifoData.GetDir());
	SetTransNum(FifoData.GetTransNum());
	SetPrice(FifoData.GetPrice());
	SetFxrate(FifoData.GetFxrate());
	SetCost(FifoData.GetCost());
	SetProfit(FifoData.GetProfit());
	SetAmount(FifoData.GetAmount());
	SetRemain(FifoData.GetRemain());
	SetAmortFact(FifoData.GetAmortFact());
	SetHolding(FifoData.GetHolding());
	SetAddable(FifoData.GetAddable());
}





IMPLEMENT_DYNAMIC(CFifoDataArray, CObArray)
CFifoDataArray::CFifoDataArray()
{
}

CFifoDataArray::~CFifoDataArray()
{
	RemoveAll();
}
    
int	CFifoDataArray::Add(CFifoData *Rec)
{
	return CObArray::Add(Rec);
}

int CFifoDataArray::Add(CFifoData &Rec)
{
	return CObArray::Add(new CFifoData(Rec));
}

void CFifoDataArray::SetAt(int Index, CFifoData *Rec)
{
	CFifoData *pRec;

	pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, Rec);
	}
}

void CFifoDataArray::SetAt(int Index, CFifoData &Rec)
{
	CFifoData *pRec;

	pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, new CFifoData(Rec));
	}
}

void CFifoDataArray::RemoveAt(int Index)
{
	CFifoData* Data;
	
	Data = GetAt(Index);
	if(Data)
	{
		CObArray::RemoveAt(Index);
		delete Data;		
	}
}

void CFifoDataArray::RemoveAll()
{
	CFifoData* Data;
	
	for(int i = 0; i < GetSize(); i++)
	{
		Data = GetAt(i);
		if(Data)
			delete Data;				
    }
    
    CObArray::RemoveAll();
}

CFifoData *CFifoDataArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CFifoData*) CObArray::GetAt(Index);
}

void CFifoDataArray::InsertAt(int Index, CFifoData *Rec)
{
	CObArray::InsertAt(Index, Rec);	
}

void CFifoDataArray::InsertAt(int Index, CFifoData &Rec)
{
	CObArray::InsertAt(Index, new CFifoData(Rec));
}

void CFifoDataArray::Copy(CFifoDataArray &FifoDataArray)
{
	RemoveAll();
	for(int i = 0; i < FifoDataArray.GetSize(); i++)
		Add(*FifoDataArray.GetAt(i));
}
