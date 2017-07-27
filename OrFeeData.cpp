#include "stdafx.h"
#include "orfeedata.h"
#include "oradate.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(COrFeeData, CObject)

void COrFeeData::Copy(COrFeeData &OrFeeData)
{
	GetPB() = OrFeeData.GetPB();
	GetCP() = OrFeeData.GetCP();
	GetLowLimit() = OrFeeData.GetLowLimit();
	GetHighLimit() = OrFeeData.GetHighLimit();
	GetOrFee() = OrFeeData.GetOrFee();
	GetOCCFee() = OrFeeData.GetOCCFee();
}



IMPLEMENT_DYNAMIC(COrFeeDataArray, CObArray)

int	COrFeeDataArray::Add(COrFeeData *Rec)
{
	return CObArray::Add(Rec);
}

int COrFeeDataArray::Add(COrFeeData &Rec)
{
	return CObArray::Add(new COrFeeData(Rec));
}

void COrFeeDataArray::SetAt(int Index, COrFeeData *Rec)
{
	COrFeeData *pRec = GetAt(Index);
	
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, Rec);
	}
}

void COrFeeDataArray::SetAt(int Index, COrFeeData &Rec)
{
	COrFeeData *pRec = GetAt(Index);

	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, new COrFeeData(Rec));
	}
}

void COrFeeDataArray::RemoveAt(int Index)
{
	COrFeeData *pRec = GetAt(Index);
	
	if(pRec)
	{
		CObArray::RemoveAt(Index);
		delete pRec;		
	}
}

void COrFeeDataArray::RemoveAll()
{
	COrFeeData *pRec;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pRec = GetAt(i);
		if(pRec)
			delete pRec;
    }
    
    CObArray::RemoveAll();
}

COrFeeData *COrFeeDataArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;

	return (COrFeeData*) CObArray::GetAt(Index);
}

void COrFeeDataArray::InsertAt(int Index, COrFeeData *Rec)
{
	CObArray::InsertAt(Index, Rec);	
}

void COrFeeDataArray::InsertAt(int Index, COrFeeData &Rec)
{
	CObArray::InsertAt(Index, new COrFeeData(Rec));
}

int COrFeeDataArray::Find(const CString PB, const CString CP, double Contracts)
{
	// If FieldID == -1, Use RecID.  Otherwise, use the field in m_Rec accordingly
	int i, def_idx;
	COrFeeData *pData;
	CString Text;

	for(i = 0; i < GetSize(); i++)
	{
		pData = GetAt(i);
		if(pData && pData->GetPB().IsEmpty())
			def_idx = i;
	}

	for(i = 0; i < GetSize(); i++)
	{
		pData = GetAt(i);
		if(pData)
		{
			if(!pData->GetPB().IsEmpty())
			{
				if(pData->GetPB() == PB)
				{
					if(!pData->GetCP().IsEmpty())
					{
						if(pData->GetCP() == CP)
							return i;
					}
					else
						if(pData->GetLowLimit() >= 0)
						{
							if(pData->GetHighLimit() >= 0)
							{
								if(Contracts >= pData->GetLowLimit() && Contracts <= pData->GetHighLimit())
									return i;
							}
							else
								if(Contracts > pData->GetLowLimit())
									return i;
						}
				}
			}
		}
	}

	return def_idx;
}

void COrFeeDataArray::Copy(COrFeeDataArray &OrFeeDataArray)
{
	RemoveAll();
	
	for(int i = 0; i < OrFeeDataArray.GetSize(); i ++)
		Add(*OrFeeDataArray.GetAt(i));
}

int COrFeeDataArray::LoadData(COraLoader &OraLoader)
{
	OValue Value;
	COrFeeData OrFeeData;

	OraLoader.Open("SELECT PB_NAME, CP, LOW_LIMIT, HIGH_LIMIT, OR_FEE, OCC_FEE FROM SEMAM.NW_OR_FEE "
					"WHERE FROM_DATE <= TRUNC(SYSDATE) "
					"AND NVL(TO_DATE, SYSDATE) > TRUNC(SYSDATE) ");
	RemoveAll();
	OraLoader.MoveFirst();
	while(!OraLoader.IsEOF())
	{
		OraLoader.GetFieldValue(0, &Value); // PB
		if(Value.IsNull())
			OrFeeData.GetPB().Empty();
		else
			OrFeeData.GetPB() = OraLoader.GetDBString(0);

		OraLoader.GetFieldValue(1, &Value); // CP
		if(Value.IsNull())
			OrFeeData.GetCP().Empty();
		else
			OrFeeData.GetCP() = OraLoader.GetDBString(1);
		
		OraLoader.GetFieldValue(2, &Value); // LowLimit
		if(Value.IsNull())
			OrFeeData.GetLowLimit() = -1000000;
		else
			OrFeeData.GetLowLimit() = (double) Value;
		
		OraLoader.GetFieldValue(3, &Value); // HighLimit
		if(Value.IsNull())
			OrFeeData.GetHighLimit() = -1000000;
		else
			OrFeeData.GetHighLimit() = (double) Value;
	
		OraLoader.GetFieldValue(4, &Value); // OR_FEE
		OrFeeData.GetOrFee() = (double) Value;

		OraLoader.GetFieldValue(5, &Value); // OCC_FEE
		OrFeeData.GetOCCFee() = (double) Value;

		Add(OrFeeData);

		OraLoader.MoveNext();
	}

	return GetSize();
}