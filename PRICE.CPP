#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include "price.h"
#include "disperr.h"
#include "qdata.h"
#include "sqlstmt.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CPriceData, CDBRec)

CPriceData::CPriceData()
{
	SetLock(FALSE);
	SetData();
}

CPriceData::CPriceData(CPriceData &PriceData) : CDBRec(PriceData)
{
	SetLock(PriceData.GetLock());	
}           

void CPriceData::SetData()
{
	if(GetSize() > 0)
		return;
	
	for(int i = 0; i < 3; i++)
		Add("");
}

void CPriceData::SetPrice(LPCTSTR Price, int DataRange)
{
	double Val;

	SetAt(2, (GetVal(Price, DataRange, Val)) ? Price : "");
}

BOOL CPriceData::GetVal(LPCTSTR Price, int DataRange, double &Val)
{
	CQData QData;
	CString Line;

	Val = -1;
	if(!Price || strlen(Price) == 0)
		return FALSE;

	if(QData.IsValid(Price, "-0123456789.,-"))
	{		
		Line = QData.RemoveComma(Price);    
		Val = atof(Line);

		switch(DataRange)
		{
			case 1:			
				return (Val <= 0) ? FALSE : TRUE;
			case 0:
				return (Val < 0) ? FALSE : TRUE;
			case -1:
				return TRUE;
			default:
				break;
		}
	}

	return FALSE;
}

BOOL CPriceData::GetVal(CString &Price, int DataRange, double &Val)
{
	return GetVal((LPCTSTR) Price, DataRange, Val);
}




IMPLEMENT_DYNAMIC(CPriceDataArray, CDBRecArray)
BOOL CPriceDataArray::Setup(LPCTSTR Date, double WarnLimit, int DataRange)
{
	SetWarnLimit(WarnLimit);
	SetDate(Date);
	SetDataRange(DataRange);
	SetModify(FALSE);
	return TRUE;
}

BOOL CPriceDataArray::Setup(LPCTSTR Date, LPCTSTR FName, int DataRange)
{
	char Text[1024];
	CFileException e;

	Setup(Date, 0.0, DataRange);
	if(!GetInFile().Open(FName, CFile::modeRead, &e))
	{
		e.GetErrorMessage(Text, sizeof(Text) - 1);
		MessageBox(NULL, Text, "File could not be opened ", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CPriceDataArray::Setup(COraLoader &OraLoader, LPCTSTR Date, double WarnLimit, int DataRange)
{
	SetOraLoader(OraLoader);
	return Setup(Date, WarnLimit, DataRange);
}

BOOL CPriceDataArray::Setup(COraLoader &OraLoader, LPCTSTR Date, LPCTSTR FName, int DataRange)
{
	SetOraLoader(OraLoader);
	return Setup(Date, FName, DataRange);	
}

CPriceData *CPriceDataArray::GetAt(LPCTSTR Asset, BOOL bSwap)
{
	if(bSwap)
	{
		CPriceData *PriceData;
		CString ID, TransNum;
		int Pos;

		if(!Asset || strlen(Asset) < 10)
			return NULL;

		for(int i = 0; i < GetSize(); i ++)
		{
			PriceData = GetAt(i);
			if(PriceData)
			{
				ID = PriceData->GetAsset();

				Pos = ID.Find("\t", 0);
				if(Pos >= 0)
				{
					TransNum = ID.Right(ID.GetLength() - Pos - 1);
					ID = ID.Left(Pos + 1);
					Pos = TransNum.Find("\\", 0);
					if(Pos >= 0)
					{
						TransNum = TransNum.Right(TransNum.GetLength() - Pos - 1);
						ID += TransNum;

						if(ID == Asset)
							return PriceData;
					}
				}
			}
		}

		return NULL;
	}
	else
		return (CPriceData*) CDBRecArray::GetAt(Find(Asset, 0));
}

void CPriceDataArray::RemoveAll()
{
	CPriceData *PriceData;
	
	for(int i = 0; i < GetSize(); i++)
	{
		PriceData = GetAt(i);
		if(PriceData)
			delete PriceData;
	}
	
	CObArray::RemoveAll();		
}

void CPriceDataArray::RemoveAt(int Index)
{
	CPriceData *PriceData;
	
	PriceData = GetAt(Index);
	if(PriceData)
	{
		delete PriceData;
		CObArray::RemoveAt(Index);
	}
}

void CPriceDataArray::SetAt(int Index, CPriceData &PriceData)
{
	CPriceData *OldPriceData;
	 	
	OldPriceData = GetAt(Index);
	if(OldPriceData)
	{
		CObArray::SetAt(Index, &PriceData);
		delete OldPriceData;
	}
}

void CPriceDataArray::ChangeChar(CString &Asset)
{
	char Text[1024], *p;

	strcpy_s(Text, Asset);
	p = strstr(Text, AT);
	if(p)
	{
		p[0] = '\t';
		Asset = Text;
	}
}

BOOL CPriceDataArray::ComputePriceChange(CPriceData &PriceData, double &Result, PriceStatus_t &Status)									
{
	double Price, PrevPrice;
	BOOL Valid = FALSE;

	Status.m_bOverLimit = FALSE;
	Status.m_bLock = PriceData.GetLock();
	Status.m_nOverUnderEqual = 0;
	Result = -1;

	if(PriceData.GetSize() <= 0)
		return FALSE;
	
	if(!PriceData.GetPriceVal(GetDataRange(), Price) ||
		!PriceData.GetPrevPriceVal(GetDataRange(), PrevPrice))
		return FALSE;

	if(PrevPrice != 0)
	{
		Result = (Price - PrevPrice)/PrevPrice*100;
		Valid = TRUE;
	}

	if(Valid)
	{
		if(GetWarnLimit() > 0)
			Status.m_bOverLimit = fabs(Result) >= GetWarnLimit() ? TRUE : FALSE;
		
		if(Price == PrevPrice)
			Status.m_nOverUnderEqual = 0;
		else
			if(Price > PrevPrice)
				Status.m_nOverUnderEqual = 1;
			else
				Status.m_nOverUnderEqual = -1;
	}

	return Valid;
}

LPCTSTR CPriceDataArray::GetPrevDate(BOOL bFromDB, LPCTSTR Sql)
// 1. Convert Date from "MM/DD/YY" to "'MM/DD/YY'", 
// 2. Get Previous Date Through an Sql Statement Based on SqlID
// 3. Convert Previous Date From "MM/DD/YY" to "'MM/DD/YY'"
{
	if(bFromDB)
	{
		CQData QData;

		SetQDate(QData.GetQueryDate(GetDate()));		
		if(!Sql || strlen(Sql) == 0)
			GetOraLoader().GetSql().Format("SELECT MAX(INDATE) FROM SEMAM.NW_EXRATES "
											"WHERE INDATE < %s", GetQDate());
		else
			GetOraLoader().GetSql().Format(Sql, GetQDate());

		GetOraLoader().Open();
		GetOraLoader().LoadTextString(m_PrevDate);
		SetPrevDate(m_PrevDate.IsEmpty() ? GetQDate() : QData.GetQueryDate(m_PrevDate));
	}

	return (LPCTSTR) m_PrevDate;
}

BOOL CPriceDataArray::SetupVisual(UINT SSSqlID, CDBSheet &SS)
{
	if(GetOraLoader().Open(SSSqlID))
	{
		GetOraLoader().CopyFieldArray(SS.GetFieldArray());
		SS.SetSheetColHeaders();		
		return TRUE;
	}

	return FALSE;
}

BOOL CPriceDataArray::SetupVisual(CString &Sql, CDBSheet &SS)
{
	if(GetOraLoader().Open(Sql))
	{
		GetOraLoader().CopyFieldArray(SS.GetFieldArray());
		SS.SetSheetColHeaders();		
		return TRUE;
	}

	return FALSE;
}

void CPriceDataArray::UpdateAssetPos(UINT DelSqlID, UINT UpdtSqlID)
{
	if(!GetOraLoader().ExecuteSql(DelSqlID))
		return;

	CString Sql;

	Sql.LoadString(UpdtSqlID);
	GetOraLoader().GetSql().Format((LPCTSTR)Sql, GetQDate(), GetQDate());
	GetOraLoader().ExecuteSql();
}

void CPriceDataArray::UpdateAssetPos(LPCTSTR DelSql, LPCTSTR UpdtSql)
{
	if(!GetOraLoader().ExecuteSql(DelSql))
		return;

	GetOraLoader().GetSql().Format((LPCTSTR) UpdtSql, GetQDate(), GetQDate());
	GetOraLoader().ExecuteSql();
}

void CPriceDataArray::FillPriceData(CString &Asset, BOOL bPrice)
{
	CPriceData* PriceData = NULL;
	OValue	Value;
	CString Cost;
	BOOL Lock = FALSE;

	GetOraLoader().GetFieldValue(0, &Value); // Asset
	if(!Value.IsNull() && (Asset != (LPCTSTR) Value)) // Asset
		PriceData = (CPriceData*) new CPriceData();

	if(!PriceData)
		return;
	
	Asset = (LPCTSTR)Value;
	PriceData->SetAsset(Asset);

	GetOraLoader().GetFieldValue(1, &Value); // PrevPrice
	PriceData->SetPrevPrice(!Value.IsNull() ? (LPCTSTR) Value : EMPTYSTRING);			

	GetOraLoader().GetFieldValue(2, &Value); // Price
	PriceData->SetModify(FALSE);
	PriceData->SetPrice(!Value.IsNull() ? (LPCTSTR) Value : EMPTYSTRING);				 
		
	PriceData->SetLock(FALSE);
	if(bPrice)
	{
		PriceData->SetInDatabase(!Value.IsNull()); // In Database ID
		GetOraLoader().GetFieldValue(3, &Value); // Cost;
		if(!Value.IsNull())
		{
			Cost = (LPCTSTR) Value;
			GetOraLoader().GetFieldValue(4, &Value); // Source
			if(!Value.IsNull())
				Lock = strstr((LPCTSTR) Value, "COST") ? TRUE : FALSE;

			if(strlen(PriceData->GetPrice()) <= 0)
			{
				PriceData->SetPrice(Cost, GetDataRange());
				if(Lock)
					PriceData->SetLock();
				PriceData->SetModify(TRUE);
			}
			else
				if(Cost == PriceData->GetPrice() && Lock)
					PriceData->SetLock();
		}
	}
	else
	{
		GetOraLoader().GetFieldValue(3, &Value); // In Database ID
		PriceData->SetInDatabase(((int) Value < 0) ? TRUE : FALSE);
	}

	Add(PriceData);
}

int CPriceDataArray::LoadPriceDataArray(CString &Sql, BOOL bPrice)
{
	CString Asset;

	RemoveAll();
	if(Sql.IsEmpty() || !GetOraLoader().Open(Sql))
		return -1;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{		
		FillPriceData(Asset, bPrice);		
		GetOraLoader().MoveNext();
	}		

	return GetSize();
}

BOOL CPriceDataArray::OpenInsertSql(UINT SqlID)
{
	return GetOraLoader().Open(SqlID, ODYNASET_DEFAULT);
}

BOOL CPriceDataArray::OpenInsertSql(LPCTSTR Sql)
{
	return GetOraLoader().Open(Sql, ODYNASET_DEFAULT);
}

void CPriceDataArray::InsertSwapData(UINT SqlID)
{
	CString Asset, ID, Price, TransNum;
	CQData QData;
	CPriceData *pPriceData;
	int Pos;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Pos = ID.Find("\\");
			if(Pos >= 0)
				TransNum = ID.Right(ID.GetLength() - Pos - 1);
			else
				TransNum = ID;
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			if(!Asset.IsEmpty())
			{
				GetOraLoader().GetSql().Format(SqlID, GetQDate(), (LPCTSTR) Price, (LPCTSTR) TransNum);

				GetOraLoader().BeginTransaction();
				if(GetOraLoader().ExecuteSql())
				{
					pPriceData->SetInDatabase(TRUE);
					pPriceData->SetModify(FALSE);
					GetOraLoader().Commit();
				}
				else
					GetOraLoader().Rollback();
			}
		}
	}
}

void CPriceDataArray::InsertSwapData(LPCTSTR Sql)
{
	CString Asset, ID, Price, TransNum;
	CQData QData;
	CPriceData *pPriceData;
	int Pos;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Pos = ID.Find("\\");
			if(Pos >= 0)
				TransNum = ID.Right(ID.GetLength() - Pos - 1);
			else
				TransNum = ID;
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			if(!Asset.IsEmpty())
			{
				GetOraLoader().GetSql().Format(Sql, GetQDate(), (LPCTSTR) Price, (LPCTSTR) TransNum);

				GetOraLoader().BeginTransaction();
				if(GetOraLoader().ExecuteSql())
				{
					pPriceData->SetInDatabase(TRUE);
					pPriceData->SetModify(FALSE);
					GetOraLoader().Commit();
				}
				else
					GetOraLoader().Rollback();
			}
		}
	}
}

CString CPriceDataArray::OptValueToPrice(CString TransNum, CString OptValue)
{
	CString Price;
	CQData QData;
	COraLoader OraLoader = GetOraLoader();

	if(atof(OptValue) == 0)
		return OptValue;

	OraLoader.GetSql().Format("SELECT %s/NOM_AMOUNT/NVL(AMORT_FACT, 1)*DECODE(ASS_TYPE, 'EQUITY TYPE CAL', 1, 100) "
								"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_ASSETS B, SEMAM.NW_ASS_PERIODS C, "
								"SEMAM.NW_PORTFOLIOS Z "
								"WHERE B.ASS_CODE = A.ASSET_CODE "
								"AND C.ASS_CODE(+) = A.ASSET_CODE "
								"AND C.ASS_FROM(+) <= %s "
								"AND C.ASS_TO(+) > %s) "
								"WHERE TRANS_NUM = %s ", (LPCTSTR) OptValue, GetQDate(), GetQDate(), TransNum);

	OraLoader.Open();
	OraLoader.LoadTextString(Price);
	Price = QData.RemoveComma(Price);
	return Price;
}

void CPriceDataArray::InsertOptionData(UINT SqlID)
{
	CString Asset, ID, Price;
	CQData QData;
	CDBRec Rec;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i ++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{						
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			GetOraLoader().GetSql().Format(SqlID);
			GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT);

			if(GetOraLoader().CheckOpen())
			{
				Rec.RemoveAll();
				Price = QData.RemoveComma(pPriceData->GetPrice());
				if(atof(Price) >= 0)
				{
					if(IsOptionValue())
						Price = OptValueToPrice(ID, Price);
					Rec.Add(Price);
					Rec.Add(ID);
					Rec.Add(GetDate());
					GetOraLoader().BeginTransaction();
					if(GetOraLoader().UpdateRecord(Rec, TRUE))
					{
						pPriceData->SetInDatabase(TRUE);
						pPriceData->SetModify(FALSE);
						GetOraLoader().Commit();
					}
					else
						GetOraLoader().Rollback();
				}
			}
		}
	}
}

void CPriceDataArray::InsertOptionData(LPCTSTR Sql)
{
	CString Asset, ID, Price;
	CQData QData;
	CDBRec Rec;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i ++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{						
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			GetOraLoader().Open(Sql, ODYNASET_DEFAULT);

			if(GetOraLoader().CheckOpen())
			{
				Rec.RemoveAll();
				Price = QData.RemoveComma(pPriceData->GetPrice());
				if(atof(Price) >= 0)
				{
					if(IsOptionValue())
						Price = OptValueToPrice(ID, Price);

					Rec.Add(Price);
					Rec.Add(ID);
					Rec.Add(GetDate());
					GetOraLoader().BeginTransaction();
					if(GetOraLoader().UpdateRecord(Rec, TRUE))
					{
						pPriceData->SetInDatabase(TRUE);
						pPriceData->SetModify(FALSE);
						GetOraLoader().Commit();
					}
					else
						GetOraLoader().Rollback();
				}
			}
		}
	}
}

void CPriceDataArray::InsertRiskFreeRateData(UINT SqlID)
{
	CString Asset, ID, Price;
	CQData QData;
	CDBRec Rec;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i ++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{						
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			GetOraLoader().GetSql().Format(SqlID);
			GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT);

			if(GetOraLoader().CheckOpen())
			{
				Rec.RemoveAll();
				Price = QData.RemoveComma(pPriceData->GetPrice());
				if(atof(Price) >= 0)
				{
					Rec.Add(Asset);
					Rec.Add(ID);
					Rec.Add(Price);
					Rec.Add(GetDate());
					GetOraLoader().BeginTransaction();
					if(GetOraLoader().UpdateRecord(Rec, TRUE))
					{
						pPriceData->SetInDatabase(TRUE);
						pPriceData->SetModify(FALSE);
						GetOraLoader().Commit();
					}
					else
						GetOraLoader().Rollback();
				}
			}
		}
	}
}

void CPriceDataArray::InsertRiskFreeRateData(LPCTSTR Sql)
{
	CString Asset, ID, Price;
	CQData QData;
	CDBRec Rec;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i ++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{						
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			GetOraLoader().Open(Sql, ODYNASET_DEFAULT);

			if(GetOraLoader().CheckOpen())
			{
				Rec.RemoveAll();
				Price = QData.RemoveComma(pPriceData->GetPrice());
				if(atof(Price) >= 0)
				{
					Rec.Add(Asset);
					Rec.Add(ID);
					Rec.Add(Price);
					Rec.Add(GetDate());
					GetOraLoader().BeginTransaction();
					if(GetOraLoader().UpdateRecord(Rec, TRUE))
					{
						pPriceData->SetInDatabase(TRUE);
						pPriceData->SetModify(FALSE);
						GetOraLoader().Commit();
					}
					else
						GetOraLoader().Rollback();
				}
			}
		}
	}
}

void CPriceDataArray::InsertPriceData()
{
	CString Asset, ID, Price;
	CDBRec Rec;
	CQData QData;
	CPriceData *pPriceData;

	if(!GetOraLoader().CheckOpen())
		return;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify() && !pPriceData->GetInDatabase())
		{			
			Rec.RemoveAll();
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Price = QData.RemoveComma(pPriceData->GetPrice());

			switch(GetOraLoader().GetFieldArray().GetSize())
			{
				case 3:
					Rec.Add(Price);
					Rec.Add(Asset);
					Rec.Add(GetDate());
					break;
				case 2:
					Rec.Add(Price);
					Rec.Add(Asset);
					break;
				case 4:				
					Rec.Add(Price);			
					Rec.Add(ID);
					Rec.Add(Asset);
					Rec.Add(GetDate()); // Indate
					break;
				default:
					break;
			}
			
			if(Rec.GetSize() > 0)
			{
				GetOraLoader().BeginTransaction();
				if(GetOraLoader().UpdateRecord(Rec, TRUE))
				{
					pPriceData->SetInDatabase(TRUE);
					pPriceData->SetModify(FALSE);
					GetOraLoader().Commit();
				}
				else
					GetOraLoader().Rollback();
			}
		}
	}
}

BOOL CPriceDataArray::DeletePriceData(UINT SqlID)
{
	CString Asset, ID;
	CQData QData;
	CPriceData *pPriceData;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetDeleted())
		{
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Asset = QData.GetQueryText(Asset);

			GetOraLoader().GetSql().Format(SqlID, (LPCTSTR) Asset);
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
				GetOraLoader().Commit();
			else
				GetOraLoader().Rollback();

			pPriceData->SetDeleted(FALSE);
			pPriceData->SetModify(TRUE);
		}
	}

	return TRUE;
}

BOOL CPriceDataArray::DeletePriceData(LPCTSTR Sql)
{
	CString Asset, ID;
	CQData QData;
	CPriceData *pPriceData;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetDeleted())
		{
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Asset = QData.GetQueryText(Asset);

			GetOraLoader().GetSql().Format(Sql, (LPCTSTR) Asset);
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
				GetOraLoader().Commit();
			else
				GetOraLoader().Rollback();

			pPriceData->SetDeleted(FALSE);
			pPriceData->SetModify(TRUE);
		}
	}

	return TRUE;
}

void CPriceDataArray::UpdatePriceData(UINT SqlID, BOOL bCost)
{
	CString Asset, ID, Price;
	CQData QData;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Asset = QData.GetQueryText(Asset);
			if(bCost)			
				GetOraLoader().GetSql().Format(SqlID, (LPCTSTR) Price, (LPCTSTR) Asset);
			else
				GetOraLoader().GetSql().Format(SqlID, (LPCTSTR) Price, (LPCTSTR) Asset, GetQDate());
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdatePriceData(LPCTSTR Sql, BOOL bCost)
{
	CString Asset, ID, Price;
	CQData QData;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Asset = QData.GetQueryText(Asset);
			if(bCost)			
				GetOraLoader().GetSql().Format(Sql, (LPCTSTR) Price, (LPCTSTR) Asset);			
			else
				GetOraLoader().GetSql().Format(Sql, (LPCTSTR) Price, (LPCTSTR)Asset, GetQDate());
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdateOptionData(UINT SqlID)
{
	CString Asset, ID, Price;
	CQData QData;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);

		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{				
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			
			Price = QData.RemoveComma(pPriceData->GetPrice());
			if(IsOptionValue())
				Price = OptValueToPrice(ID, Price);

			Price = QData.GetQueryNumber(Price);
			
			GetOraLoader().GetSql().Format(SqlID, (LPCTSTR) Price, (LPCTSTR) ID, GetQDate());
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdateOptionData(LPCTSTR Sql)
{
	CString Asset, ID, Price;
	CQData QData;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);

		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{				
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);

			Price = QData.RemoveComma(pPriceData->GetPrice());
			if(IsOptionValue())
				Price = OptValueToPrice(ID, Price);
			
			Price = QData.GetQueryNumber(Price);

			GetOraLoader().GetSql().Format(Sql, (LPCTSTR) Price, (LPCTSTR) ID, GetQDate());
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdateRiskFreeRateData(UINT SqlID)
{
	CString Asset, ID, Price;
	CQData QData;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);

		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{				
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);

			GetOraLoader().GetSql().Format(SqlID, (LPCTSTR) Price, (LPCTSTR) Asset, (LPCTSTR) ID, GetQDate());
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdateRiskFreeRateData(LPCTSTR Sql)
{
	CString Asset, ID, Price;
	CQData QData;
	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);

		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{				
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);

			GetOraLoader().GetSql().Format(Sql, (LPCTSTR) Price, (LPCTSTR) Asset, (LPCTSTR) ID, GetQDate());
			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdateSwapData(UINT SqlID)
{
	CString Asset, ID, Price, TransNum;
	CQData QData;
	CPriceData *pPriceData;
	int Pos;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{				
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Pos = ID.Find("\\", 0);
			if(Pos >= 0)
				TransNum = ID.Right(ID.GetLength() - Pos - 1);
			else
				TransNum = ID;
			GetOraLoader().GetSql().Format(SqlID, (LPCTSTR) Price, (LPCTSTR) TransNum, GetQDate());

			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

void CPriceDataArray::UpdateSwapData(LPCTSTR Sql)
{
	CString Asset, ID, Price, TransNum;
	CQData QData;
	CPriceData *pPriceData;
	int Pos;

	for(int i = 0; i < GetSize(); i++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetInDatabase() && pPriceData->GetModify())
		{				
			Price = QData.GetQueryNumber(pPriceData->GetPrice());
			ParseAssetData(pPriceData->GetAsset(), Asset, ID);
			Pos = ID.Find("\\", 0);
			if(Pos >= 0)
				TransNum = ID.Right(ID.GetLength() - Pos - 1);
			else
				TransNum = ID;
			GetOraLoader().GetSql().Format(Sql, (LPCTSTR) Price, (LPCTSTR) TransNum, GetQDate());

			GetOraLoader().BeginTransaction();
			if(GetOraLoader().ExecuteSql())
			{
				pPriceData->SetModify(FALSE);
				GetOraLoader().Commit();
			}
			else
				GetOraLoader().Rollback();
		}
	}

	SetModify(FALSE);
}

BOOL CPriceDataArray::ParsePriceData(LPCTSTR Data, CString &Asset, CString &Price, int PriceValue) 
{
	char Buf[1024], *p, *q;
	CQData QData;
	LPCTSTR ValidStr = "-.0123456789";

	Price.Empty();
	Asset.Empty();

	strcpy_s(Buf, Data);
	p = strstr(Buf, "\n");
	if(p)
		p[0] = 0;		

	p = strtok_s(Buf, COMMA, &q);
	while(p)
	{
		if(Asset.IsEmpty())
		{
			Asset = p;
			Asset.TrimLeft();
			Asset.TrimRight();
			if(Asset.IsEmpty())
				return FALSE;			
		}
		else
		{			
			Price = p;
			Price.TrimLeft();
			Price.TrimRight();

			if(!Price.IsEmpty())
			{
				if(!QData.IsValid(Price, ValidStr))
					Price.Empty();
				else
				{
					if(PriceValue == 1) // Positive Only
					{
						if(atof(Price) <= 0)
							Price.Empty();
					}
					else
						if(PriceValue == 0) // >= 0
						{
							if(atof(Price) < 0)
								Price.Empty();
						}
				}
			}	

			if(!Price.IsEmpty())
				return TRUE;
		}
		
		p = strtok_s(NULL, COMMA, &q);
	}

	return FALSE;
}


void CPriceDataArray::DisplayErrorFile()
{
	CDisplayErrorFile ErrDlg;

	ErrDlg.m_ErrF.Open(GetErrorFile().GetFileName(), CFile::modeRead);
	if(ErrDlg.m_ErrF.GetLength() == 0)
		ErrDlg.m_ErrF.Close();
    else
    {
		ErrDlg.m_Title = "Display Error";	
		ErrDlg.DoModal();		
	}
}

void CPriceDataArray::WriteError(LPCTSTR Text)
{
	CString Err;

	Err = Text;
	Err += " does not exist\n";
	GetErrorFile().WriteString(Err);
}

void CPriceDataArray::ProcessPriceData(CString &Asset, CString &Price, BOOL bSwap)
{
	CPriceData *pPriceData;

	Asset.TrimLeft("\"");
	Asset.TrimRight("\"");

	pPriceData = GetAt(Asset, bSwap);
	if(pPriceData)
	{
		if(!pPriceData->GetModify())
			pPriceData->SetInDatabase(strlen(pPriceData->GetPrice()) > 0 ? TRUE : FALSE);
		pPriceData->SetPrice(Price, GetDataRange());
		pPriceData->SetModify(TRUE);
		SetModify(TRUE);
	}
	else			
		WriteError(Asset);
}

void CPriceDataArray::UpdatePriceInfo()
{          
	char Buffer[256];
	CString	Asset, Price, MaxPrice, Text;
	double	dPrice;
	CPriceData* pPriceData;
	
	GetInFile().ReadString(Buffer, sizeof(Buffer));
	GetInFile().ReadString(Buffer, sizeof(Buffer));

	while(GetInFile().ReadString(Buffer, sizeof(Buffer)))
	{
		if(ParsePriceData(Buffer, Asset, Price, GetDataRange())) // Price From csv must be >= 0
		{
			ChangeChar(Asset);
			dPrice = atof(Price);
			if(dPrice >= 0)
			{
				pPriceData = GetAt(Asset);
				if(pPriceData)
				{
					pPriceData->SetPrice(Price, GetDataRange());
					pPriceData->SetModify();
				}
				else
					WriteError(Asset);				
			}
		}
	}
}

void CPriceDataArray::LoadFileData(BOOL bPrice, BOOL bSwap)
{
    char Buffer[256];
	CPriceData *pPriceData;
	CString	ErrFName, Price, Asset;
	CFileException e;
	int Pos;

	ErrFName = GetInFile().GetFileName();
	Pos = ErrFName.Find(PERIOD);
	if(Pos >= 0)
		ErrFName = ErrFName.Left(Pos);
	
	ErrFName += ".ERR";
	if(!GetErrorFile().Open(ErrFName, CFile::modeCreate | CFile::modeWrite, &e))
	{
		e.GetErrorMessage(Buffer, sizeof(Buffer) - 1);
		MessageBox(NULL, Buffer, "File could not be opened ", MB_OK);
		return;
	}

	if(bPrice)
		UpdatePriceInfo();
	else
	{
		GetInFile().ReadString(Buffer, sizeof(Buffer));
		GetInFile().ReadString(Buffer, sizeof(Buffer));

		while(GetInFile().ReadString(Buffer, sizeof(Buffer)))
		{
			if(ParsePriceData(Buffer, Asset, Price, GetDataRange())) // Exrate must > 0
			{
				ChangeChar(Asset);
				ProcessPriceData(Asset, Price, bSwap);
			}
		}
	}

	for(int i = 0; i < GetSize() && bSwap; i++) // IntSwap
	{		
		pPriceData = GetAt(i);
		if(strlen(pPriceData->GetPrice()) <= 0)
		{
			pPriceData->SetPrice("0", GetDataRange());
			pPriceData->SetModify();
		}
	}

	SetModify();
	GetErrorFile().Close();
}

void CPriceDataArray::ParseAssetData(LPCTSTR Data, CString &Asset, CString &ID)
{
	int Pos;
	CString Buf;

	Asset.Empty();
	ID.Empty();
	Buf = Data;

	Pos = Buf.Find(TAB);
	if(Pos >= 0)
	{
		Asset = Buf.Left(Pos);
		ID = Buf.Right(Buf.GetLength() - Pos - 1);
		ID.TrimLeft();
		ID.TrimRight();
	}
	else
		Asset = Buf;
	Asset.TrimLeft();
	Asset.TrimRight();
}

void CPriceDataArray::SetModify()
{
	if(GetModify())
		return;

	CPriceData *pPriceData;

	for(int i = 0; i < GetSize(); i ++)
	{
		pPriceData = GetAt(i);
		if(pPriceData && pPriceData->GetModify())
		{
			SetModify(TRUE);
			return;
		}
	}
}