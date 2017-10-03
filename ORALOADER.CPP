#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "oraloader.h"
#include "nwivar.h"
#include "qdata.h"

COraLoader::COraLoader(ODatabase *pDB) : COraDset(pDB)
{
	SetCase();
	SetSeparator();
}

COraLoader::COraLoader(COraDset &OraDset) : COraDset(OraDset)
{	
	SetCase();
	SetSeparator();
}

COraLoader::COraLoader()
{	
	SetSeparator();
	SetCase();
}

void COraLoader::Copy(COraLoader &OraLoader)
{	
	COraDset::Copy(OraLoader);
	SetCase(OraLoader.GetCase());
	SetSeparator(OraLoader.GetSeparator());
}

int COraLoader::Compute()
{
	if(!CheckOpen())
		return -1;

	MoveFirst();
	while(!IsEOF())
	{
		SetupData();
		ProcessData();
		RetrieveData();
		MoveNext();
	}

	return GetRecordCount();
}

BOOL COraLoader::RecordToLine()
{
	int i;

	m_DataRec.Empty();
	for(i = 0; i < GetFieldCount(); i ++)
	{
		if(i > 0)
			m_DataRec += m_Separator;
		m_DataRec += GetDBString(i);
	}

	return !m_DataRec.IsEmpty();
}

CString &COraLoader::GetDBString(int Index)
// Support OTYPE_VARCHAR2, OTYPE_NUMBER, OTYPE_DATE, OTYPE_CHAR
{
	SetDataBuf();
	if(Index < 0 || Index >= GetFieldCount())
		return GetDataBuf();
	
	OValue Value;
	CQData QData;
	COraField* pField;	
	
	GetFieldValue(Index, &Value);
	if(Value.IsNull())
		return GetDataBuf();

	pField = GetFieldArray().GetAt(Index);
	switch(pField->GetType())
	{
		case OTYPE_SINT:
		case OTYPE_UINT:
			SetDataBuf((LPCTSTR) Value);
			if(pField->WithComma())
				m_DataBuf = QData.AddComma(m_DataBuf);
			break;
		case OTYPE_NUMBER:
		case OTYPE_FLOAT:
			SetDataBuf((LPCTSTR) Value);
			if(pField->WithComma() || pField->GetPrecision() >= 0)
				SetDataBuf(QData.WriteNumber(GetDataBuf(), pField->WithComma(), pField->GetPrecision()));
			break;
		case OTYPE_DATE:
			m_DataBuf = QData.FormatedDate((LPCTSTR) Value);
			break;
		case OTYPE_LONG: // Note: Long and long Raw fields are restricted to 64K
		case OTYPE_LONGRAW:			
		default:
			m_DataBuf = (LPCTSTR) Value;
			AdjustCharCase();
			break;
	}	
	
	return GetDataBuf();
}

void COraLoader::AdjustCharCase()
{
	if(GetCase() == UPPERCASE)
		m_DataBuf.MakeUpper();
	else
		if(GetCase() == LOWERCASE)
			 m_DataBuf.MakeLower();
}

LPCTSTR COraLoader::ToDateValue(CString &Date, OValue &Value)
{
	COraLoader OraLoader(GetDB());

	Value.Clear();
	if(!Date || strlen(Date) == 0)
		return (LPCTSTR) Value;

	if(strcmp(Date, DBSYSDATE) == 0)
	{
		OraLoader.GetSql() = "SELECT SYSDATE FROM DUAL";
		if(OraLoader.Open())
			OraLoader.GetFieldValue(0, &Value);
	}
	else
	{
		CQData QData;
		CString Buf;

		Buf = QData.GetQueryDate(Date);
		if(!Buf.IsEmpty())
		{
			Buf.TrimLeft(QUOTE);
			Buf.TrimRight(QUOTE);
			Value = (LPCTSTR) Buf;
		}
	}

	return (LPCTSTR) Value;
}

int COraLoader::LoadComboBox(COptComboBox &CBox)
{
	if(!CheckOpen())
		return -1;

	if(CBox.GetWithNone())
		CBox.AddString(NONE);

	SetSeparator(SPACE);
	MoveFirst();
	while(!IsEOF())
	{
		if(RecordToLine())
			CBox.AddString(GetDataRec());
		MoveNext();
	}

	SetSeparator(); // Reset To Default
	return CBox.GetCount();
}

int COraLoader::LoadMCComboBox(CMCComboBox &CBox, int KeyCol)
{
	if(!CheckOpen())
		return -1;

	if(CBox.GetCount() > 0)
		CBox.ResetContent();

	if(CBox.GetColumnCount() < GetFieldCount())
		CBox.SetColumnCount(GetFieldCount());

	if(CBox.GetWithNone())
		CBox.AddStringNone();

	int i, j;
	CStringArray StringArray;

	if(GetFieldCount() <= KeyCol) // if Col is out of range, set default to zero
		KeyCol = 0;

	MoveFirst();
	while(!IsEOF())
	{
		for(i = 0; i < GetFieldCount(); i ++)
		{
			StringArray.RemoveAll();
		
			StringArray.Add(GetDBString(KeyCol));
			for(j = 0; j < GetFieldCount(); j++)
			{
				if(j != KeyCol)
					StringArray.Add(GetDBString(j));
			}
		}
		
		CBox.AddString(StringArray);
		MoveNext();
	}

	return CBox.GetCount();
}

int COraLoader::LoadListBox(CHLBox &HLBox)
{
	if(!CheckOpen())
		return -1;

	if(HLBox.GetWithNone())
		HLBox.AddString(QUOTE);

	MoveFirst();
	while(!IsEOF())
	{
		if(RecordToLine())
			HLBox.AddString(GetDataRec());
		MoveNext();
	}

	return HLBox.GetCount();
}

int COraLoader::LoadSheet(CWorkSheet &SS, int StartCol, int StartRow, BOOL Locked)
{
	if(!CheckOpen())
		return -1;

	int Col2;
	int Row;

	SS.SetSheetCols(GetFieldCount());
	Row = StartRow;
	Col2 = StartCol + SS.GetSheetCols() - 1;

	MoveFirst();
	while(!IsEOF())
	{
		if(RecordToLine())
		{
			if(Row > SS.GetMaxRows())
				SS.SetMaxRows(Row);
			SS.SetSheetRow(StartCol, Row, Col2, Row, GetDataRec());
			SS.LockSheet(StartCol, Row, Col2, Row, Locked);
			Row++;
		}

		MoveNext();
	}
	
	SS.SetSheetFontBold(TRUE);
	SS.SetSheetRows(GetRecordCount());

	return SS.GetSheetRows();
}

int COraLoader::LoadKeyDBList(CKeyDBList &KeyDBList)
// 1st field must be Key of a subset. The 2nd field must id of a record. 3rd and above are data.
{
	if(!CheckOpen() || GetFieldCount() < 3)
		return FALSE;

	int i;
	CKeyDBRecArray *pRecArray;
	CDBRec DBRec;
	CString Key, OldKey;

	KeyDBList.RemoveAll();
	MoveFirst();
	while(!IsEOF())
	{		
		Key = GetDBString(0);
		if(OldKey != Key)
		{
			pRecArray = new CKeyDBRecArray();			
			pRecArray->SetKey(Key);
			KeyDBList.Add(pRecArray);
			OldKey = Key;
		}
	
		DBRec.RemoveAll();
		DBRec.SetID(GetDBString(1));
		DBRec.SetInDatabase(TRUE);
		for(i = 2; i < GetFieldCount(); i ++)
			DBRec.Add(GetDBString(i));
		pRecArray->Add(DBRec);

		MoveNext();
	}

	return KeyDBList.GetSize();
}

int COraLoader::LoadDBSheet(CDBSheet &SS, BOOL Locked)
{
	if(!CheckOpen())
		return -1;

	if(SS.GetSheetRows() > 0)
		SS.ClearSheet();
	SS.SetSheetRows(0);

	CopyFieldArray(SS.GetFieldArray());

	SS.SetSheetColHeaders();
	MoveFirst();
	while(!IsEOF())
	{
		if(RecordToLine())
			SS.AddSheetRow(GetDataRec());

		MoveNext();
	}
	
	if(SS.GetSheetRows() >= 1)
		SS.SetMaxRows(SS.GetSheetRows());
	else
		SS.SetMaxRows(500);

	if(Locked)
		SS.LockSheet();

	SS.SetSheetFontBold(TRUE);
	SS.SheetAutoFit();

	for(int i = 0; i < SS.GetFieldArray().GetSize(); i ++)
	{
		if(!SS.GetFieldArray().GetAt(i)->GetVisible())
			SS.SetSheetColHidden(i + 1, TRUE);
	}

	return SS.GetSheetRows();
}

int COraLoader::LoadRowCtrl(CRowCtrl &RowCtrl, BOOL Locked, BOOL withKey)
{
	int i, start;
	CString Data;
	CNumEdit *pEdit;
	CQData QData;
	COraField* pField;

	if(!CheckOpen())
		return FALSE;

	if(!RowCtrl.IsReady())
		return FALSE;

	RowCtrl.ClearSheet();
	CopyFieldArray(RowCtrl.GetSS()->GetFieldArray());

	if(withKey) // Keep Key in the spread sheet row
		start = 0;
	else
	{
		start = 1;
		RowCtrl.GetSS()->GetFieldArray().RemoveAt(0); // Remove ID Field
	}

	RowCtrl.GetSS()->SetSheetColHeaders();

	MoveFirst();
	while(!IsEOF())
	{				
		m_DataRec.Empty();
		for(i = start; i < GetFieldCount(); i ++)
		{
			if(i > start)
				m_DataRec += m_Separator;
			Data = GetDBString(i);
			pField = GetFieldArray().GetAt(i);
			if(pField->GetType() == OTYPE_SINT || pField->GetType() == OTYPE_UINT ||
				pField->GetType() == OTYPE_NUMBER || pField->GetType() == OTYPE_FLOAT)
			{
				if(RowCtrl.GetAt(i - start) && RowCtrl.GetAt(i - start)->GetType() == WNDCTRL_NUMEDIT)
				{
					pEdit = (CNumEdit*) RowCtrl.GetAt(i - start)->GetWnd();
					Data = QData.WriteNumber(Data, pEdit->WithComma(), pEdit->GetDigits());
				}
				else
					if(RowCtrl.GetAt(i - start) && RowCtrl.GetAt(i - start)->GetType() == WNDCTRL_IDEDIT)
						Data = QData.WriteNumber(Data, FALSE, 0);
			}			
			m_DataRec += Data;
		}
		
		RowCtrl.AddSheetRow(GetDBString(0), m_DataRec);// The first field is id
		MoveNext();
	}
	
	if(RowCtrl.GetSS()->GetSheetRows() >= 1)
		RowCtrl.GetSS()->SetMaxRows(RowCtrl.GetSS()->GetSheetRows());
	else
		RowCtrl.GetSS()->SetMaxRows(500);

	for(i = 0; i < RowCtrl.GetSS()->GetFieldArray().GetSize(); i ++)		
		RowCtrl.GetSS()->SetSheetColHidden(i + 1, !RowCtrl.GetSS()->GetFieldArray().GetAt(i)->GetVisible());

	if(Locked)
		RowCtrl.GetSS()->LockSheet();
	
	RowCtrl.GetSS()->SetSheetFontBold(TRUE);
	RowCtrl.GetSS()->SheetAutoFit();
	return RowCtrl.GetSS()->GetSheetRows();
}

int COraLoader::LoadStringArray(CStringArray &StrArr)
{
	if(!CheckOpen())
		return -1;

	MoveFirst();
	while(!IsEOF())
	{
		if(RecordToLine())
			StrArr.Add(GetDataRec());
		MoveNext();
	}

	return StrArr.GetSize();
}

int COraLoader::LoadDBRecArray(CDBRecArray &DBRecArr)
{
	int i;
	CDBRec* pDBRec;

	if(!CheckOpen())
		return -1;

	MoveFirst();
	while(!IsEOF())
	{
		pDBRec = (CDBRec*) new CDBRec();
		if(pDBRec)
		{
			for(i = 0; i < GetFieldCount(); i ++)
				pDBRec->Add(GetDBString(i));

			DBRecArr.Add(pDBRec);
		}

		MoveNext();
	}

	return DBRecArr.GetSize();
}

int COraLoader::LoadDBRec(CDBRec &DBRec)
{
	if(!CheckOpen())
		return -1;

	MoveFirst();
	if(!IsEOF())
	{
		for(int i = 0; i < GetFieldCount(); i ++)
			DBRec.Add(GetDBString(i));

		return 1;
	}

	return 0;
}

int COraLoader::LoadText(CWnd &Ctrl)
{
    LoadTextString(m_DataRec);
	Ctrl.SetWindowText(GetDataRec());

	return 1;
}

int COraLoader::LoadTextString(CString &String)
{
	String.Empty();
	if(!CheckOpen())
		return -1;

	MoveFirst();
	if(!IsEOF())
	{
		String = GetDBString(0);
		return 1;
	}

	return 0;
}

CString &COraLoader::DateStamp(CString &Date)
{	
	if(Open("SELECT TO_CHAR(SYSDATE, 'DD-MON-YY HH24:MI:SS') \"STAMP\" FROM DUAL"))
		LoadTextString(Date);

	return Date;
}

CString &COraLoader::Today(CString &Date)
{	
	if(Open("SELECT SYSDATE FROM DUAL"))
		LoadTextString(Date);		

	return Date;
}

int COraLoader::GetCount(LPCTSTR Sql)
{
	OValue Value;
	if(!Open(Sql))
		return -1;

	MoveFirst();
	GetFieldValue(0, &Value);
	return (int) Value;
}

int COraLoader::GetHolidays(LPCTSTR FromDate, LPCTSTR ToDate, LPCTSTR Currency)
{
	CString StrFromDate;
	CQData QData;

	StrFromDate = QData.GetQueryDate(FromDate);
	if(!Currency || strlen(Currency) == 0)
		Currency = "USD";
	GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_HOLIDAYS "
					"WHERE CURRENCY = '%s' AND HOLIDAY BETWEEN %s AND %s ", 
					Currency, (LPCTSTR) StrFromDate, QData.GetQueryDate(ToDate));
	return GetCount();
}

BOOL COraLoader::GetTPlusNDate(CString &Date, LPCTSTR SrcDate, int PlusDays, LPCTSTR Currency, BOOL bCDS)
{
	LPCTSTR pDate;
	int Days = 0, HDays = 0;
	CQData QData;
	OValue Value;

	pDate = QData.GetQueryDate(SrcDate);
	if(!Currency || strlen(Currency) == 0)
		Currency = "USD";

	if(bCDS)
	{
		GetSql().Format("SELECT TO_DATE(%s) + %d FROM DUAL ", pDate, PlusDays);
		if(!Open(GetSql()))
			return FALSE;
		LoadTextString(Date);
			return TRUE;
	}	
	
	if(PlusDays == 0)
		GetSql().Format("SELECT DISTINCT TO_DATE(%s) + INC, "
						"DECODE(SIGN(TO_DATE(%s) + INC - NVL(HOLIDAY, TO_DATE(%s) - 1)), 0, 0, "
							"DECODE(TO_CHAR(TO_DATE(%s) + INC, 'D'), '7', 0, '1', 0, 1)) "
						"FROM SEMAM.NW_HOLIDAYS A, SEMAM.MYSEQ B "
						"WHERE INC >= 0 "
						"AND HOLIDAY(+) >= %s "
						"AND HOLIDAY(+) <= TO_DATE(%s) + INC "
						"AND CURRENCY(+) = '%s' "
						"ORDER BY 1 ", pDate, pDate, pDate, pDate, pDate, pDate, Currency);
	else
		if(PlusDays > 0)
			GetSql().Format("SELECT DISTINCT TO_DATE(%s) + INC, "
						"DECODE(SIGN(TO_DATE(%s) + INC - NVL(HOLIDAY, TO_DATE(%s))), 0, 0, "
							"DECODE(TO_CHAR(TO_DATE(%s) + INC, 'D'), '7', 0, '1', 0, 1)) "
						"FROM SEMAM.NW_HOLIDAYS A, SEMAM.MYSEQ B "
						"WHERE INC >= 1 "
						"AND HOLIDAY(+) >= %s "
						"AND HOLIDAY(+) <= TO_DATE(%s) + INC "
						"AND CURRENCY(+) = '%s' "
						"ORDER BY 1 ", pDate, pDate, pDate, pDate, pDate, pDate, Currency);
		else
			GetSql().Format("SELECT DISTINCT TO_DATE(%s) - INC, "
						"DECODE(SIGN(TO_DATE(%s) - INC - NVL(HOLIDAY, TO_DATE(%s))), 0, 0, "
							"DECODE(TO_CHAR(TO_DATE(%s) - INC, 'D'), '7', 0, '1', 0, 1)) "
						"FROM SEMAM.NW_HOLIDAYS A, SEMAM.MYSEQ B "
						"WHERE INC >= 1 "
						"AND HOLIDAY(+) <= %s "
						"AND HOLIDAY(+) >= TO_DATE(%s) - INC "
						"AND CURRENCY(+) = '%s' "
						"ORDER BY 1 DESC ", pDate, pDate, pDate, pDate, pDate, pDate, Currency);

	Date.Empty();
	if(!Open())
		return FALSE;

	MoveFirst();
	while(!IsEOF())
	{		
		Date = GetDBString(0);

		GetFieldValue(1, &Value);
		if(PlusDays == 0)
		{
			if((int)Value > 0)
				return TRUE;
		}
		else
		{
			if(PlusDays > 0)
				Days += (int) Value;
			else
				if(PlusDays < 0)
					Days -= (int) Value;

			if(abs(Days) == abs(PlusDays))
				return TRUE;
		}

		MoveNext();
	}
	
	Date.Empty();
	return FALSE;
}

CString &COraLoader::TPlusN(CString &Date, LPCTSTR SrcDate, int Days, LPCTSTR Currency, BOOL bCDS)
{
	GetTPlusNDate(Date, SrcDate, Days, Currency, bCDS);
	return Date;	
}

CString &COraLoader::TPlus1(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency, BOOL bCDS)
{
	GetTPlusNDate(Date, SrcDate, 1, Currency, bCDS);
	return Date;
}

CString &COraLoader::TPlus2(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency, BOOL bCDS)
{
	GetTPlusNDate(Date, SrcDate, 2, Currency, bCDS);
	return Date;
}

CString &COraLoader::TPlus3(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency, BOOL bCDS)
{
	GetTPlusNDate(Date, SrcDate, 3, Currency, bCDS);
	return Date;
}

CString &COraLoader::TMinus1(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency, BOOL bCDS)
{
	GetTPlusNDate(Date, SrcDate, -1, Currency, bCDS);
	return Date;
}

CString &COraLoader::TMinus3(CString &Date, LPCTSTR SrcDate, LPCTSTR Currency, BOOL bCDS)
{
	GetTPlusNDate(Date, SrcDate, -3, Currency, bCDS);
	return Date;
}

CString &COraLoader::OptExpDate(CString &Date, LPCTSTR SrcDate)
{
	CQData QData;
	LPCTSTR MyDate;

	Date.Empty();
	MyDate = QData.GetQueryDate(SrcDate);
	GetSql().Format("SELECT TO_DATE(%s) + DECODE(TO_NUMBER(TO_CHAR(TO_DATE(%s), 'D')), "
					"7, 20, 20 - TO_NUMBER(TO_CHAR(TO_DATE(%s), 'D'))) FROM DUAL ", 
					MyDate, MyDate, MyDate);
	if(Open())
		LoadTextString(Date);
	return Date;
}

BOOL COraLoader::RecToQueryRec(CDBSheet &SS, CDBRec &DBRec)
{	
	CQData QData;
	COraField* pField;
	CString Text;

	if(SS.GetSheetRows() <= 0 || SS.GetFieldArray().GetSize() < SS.GetSheetCols())
		return FALSE;

	DBRec.RemoveAll();
	for(int i = 0; i < SS.GetFieldArray().GetSize(); i++)
	{
		pField = SS.GetFieldArray().GetAt(i);
		if(pField)
		{
			Text = SS.GetSheetText(i + 1, SS.GetSheetCurRow());
			switch(pField->GetType())
			{
				case OTYPE_CHAR:
				case OTYPE_CHARZ:
				case OTYPE_LONG:
				case OTYPE_LONGRAW:
				case OTYPE_MSLABEL:
				case OTYPE_RAW:
				case OTYPE_STRING:
				case OTYPE_VARCHAR:
				case OTYPE_VARCHAR2:
				case OTYPE_ROWID:
					Text = QData.GetQueryText(Text);
					break;
				case OTYPE_DATE:
					if(Text == DBSYSDATE)
						Text = DBSYSDATE;
					else
						Text = QData.GetQueryDate(Text);
					break;
				case OTYPE_FLOAT:
				case OTYPE_NUMBER:
				case OTYPE_SINT:
				case OTYPE_UINT:
					if(pField->WithComma())
						Text = QData.GetQueryNumber(Text);
					break;
				default:
					m_DataBuf = DBNULL;
					break;
			}

			DBRec.Add(GetDataBuf());
		}
	}

	return TRUE;
}

BOOL COraLoader::RecToQueryRec(CStringArray &QRec, CStringArray &Rec, 
							   CFieldArray *FieldArray, LPCTSTR Extra)
{
	int i;
	CDBRec DBRec;
	
	DBRec.RemoveAll();
	for(i = 0; i < Rec.GetSize(); i ++)
		DBRec.Add(Rec.GetAt(i));
	
	return RecToQueryRec(QRec, DBRec, FieldArray, Extra);
}

BOOL COraLoader::RecToQueryRec(CStringArray &QRec, CDBRec &Rec, 
							   CFieldArray *FieldArray, LPCTSTR Extra)
{
	CQData QData;
	COraField* pField;

	if(!FieldArray)
		FieldArray = &GetFieldArray();

	if(!FieldArray || FieldArray->GetSize() < Rec.GetSize())
		return FALSE;

	QRec.RemoveAll();
	for(int i = 0; i < Rec.GetSize(); i++)
	{
		pField = FieldArray->GetAt(i);
		if(pField)
		{
			m_DataBuf = Rec.GetAt(i);
			switch(pField->GetType())
			{
				case OTYPE_CHAR:
				case OTYPE_CHARZ:
				case OTYPE_LONG:
				case OTYPE_LONGRAW:
				case OTYPE_MSLABEL:
				case OTYPE_RAW:
				case OTYPE_STRING:
				case OTYPE_VARCHAR:
				case OTYPE_VARCHAR2:
				case OTYPE_ROWID:
					m_DataBuf = QData.GetQueryText(m_DataBuf, Extra);
					break;
				case OTYPE_DATE:				
					if(m_DataBuf == DBSYSDATE)
						m_DataBuf = DBSYSDATE;
					m_DataBuf = QData.GetQueryDate(m_DataBuf);
					break;
				case OTYPE_FLOAT:
				case OTYPE_NUMBER:
				case OTYPE_SINT:
				case OTYPE_UINT:
					m_DataBuf = QData.GetQueryNumber(m_DataBuf);
					break;
				default:
					m_DataBuf = DBNULL;
					break;
			}
		
			QRec.Add(m_DataBuf);
		}		
	}

	return TRUE;
}

BOOL COraLoader::SetSqlSearchInfo(CString &Sql, CStringArray &QRec, CFieldArray *FieldArray, 
								  BOOL bAllAnd, BOOL bFindExact)
{
	const char* Like = " LIKE ";
	const char* Equal = " = ";
	const char* And = " AND ";
	BOOL		bEq;
	BOOL		bAnd = FALSE;
	CString		Text;
	COraField*	pField;
	int i;

	if((!FieldArray && GetFieldArray().GetSize() < QRec.GetSize()) ||
		(FieldArray && FieldArray->GetSize() < QRec.GetSize()))
		return FALSE;

	bAnd = bAllAnd;
	for(i = 0; i < QRec.GetSize(); i++)
	{
		Text = QRec.GetAt(i);
		pField = FieldArray ? FieldArray->GetAt(i) : GetFieldArray().GetAt(i);
		if(!Text.IsEmpty() && Text != DBNULL && pField)
		{
			if(bFindExact)
				bEq = TRUE;
			else
				switch(pField->GetType())
				{
					case OTYPE_CHAR:
					case OTYPE_CHARZ:
					case OTYPE_DATE:
					case OTYPE_LONG:
					case OTYPE_LONGRAW:
					case OTYPE_MSLABEL:
					case OTYPE_RAW:
					case OTYPE_ROWID:
					case OTYPE_STRING:
					case OTYPE_VARCHAR2:
						bEq = FALSE;
						break;
					default:
						bEq = TRUE;
						break;
				}

			if(bAnd)
				Sql += And;
			
			if(bEq)
			{
				if(Text.Find('%', 0) < 0)
				{
					Sql += pField->GetName();
					Sql += Equal + Text;
				}
				else
					Sql += " 1 = 1 ";
			}
			else
			{
				Sql += pField->GetName();
				Sql += Like + Text;
			}

			bAnd = TRUE;
		}
	}

	return TRUE;
}

BOOL COraLoader::UpdateRecord(CDBRec &DBRec, BOOL bNew)
{
	CQData QData;
	OValue Value;
	CString Text;
	int i;
	
	if(!IsOpen())
		return FALSE;

	if(GetFieldArray().GetSize() > DBRec.GetSize())
	{
		MessageBox(NULL, "There are more fields than the data available", "Error", MB_OK);
		return FALSE;
	}
	
	if(bNew)
		AddNewRecord();
	else
		StartEdit();

	for(i = 0; i < GetFieldArray().GetSize(); i++)
	{
		Text = DBRec.GetAt(i);
		switch(GetFieldArray().GetAt(i)->GetType())
		{
			case OTYPE_DATE:
				ToDateValue(Text, Value);
				break;
			case OTYPE_NUMBER:
			case OTYPE_FLOAT:
				Text = QData.RemoveComma(Text);
				break;
			default:
				break;
		}

		if(GetFieldArray().GetAt(i)->GetType() != OTYPE_DATE)
		{
			if(Text.IsEmpty())
				Value.Clear();
			else
				Value = (const char*) Text;
		}

		SetFieldValue(i, Value);
	}

	i = Update();
	if(i != OSUCCESS)
		COraDset::GetErrMsg(i);

	return i == OSUCCESS ? TRUE : FALSE;
}

double COraLoader::GenerateUniqueID(LPCTSTR IDCode, CString &IDValue)
{
    if(!IDCode || strlen(IDCode) == 0)
		return -1;

	double idv;
	CQData QData;

	IDValue.Empty();

	GetSql().Empty();
	if(strcmp(IDCode, TRANSNUM) == 0)
	{
		GetSql().Format("UPDATE SEMAM.NW_IDS A SET "
			"ID_VALUE = (SELECT GREATEST(MAX(A.TRANS_NUM), ID_VALUE) FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_IDS B "
						"WHERE ID_CODE = '%s' GROUP BY ID_VALUE) "
						"WHERE ID_CODE = '%s' ", IDCode, IDCode);
	}
	else
		if(strcmp(IDCode, CASHNUM) == 0)
			GetSql().Format("UPDATE SEMAM.NW_IDS A SET "
				"ID_VALUE = (SELECT GREATEST(MAX(A.CASH_NUM), ID_VALUE) FROM SEMAM.NW_CASH_PMNTS A, SEMAM.NW_IDS B "
							"WHERE ID_CODE = '%s' GROUP BY ID_VALUE) "
				"WHERE ID_CODE = '%s' ", IDCode, IDCode);
		else
			if(strcmp(IDCode, PAYID) == 0)
				GetSql().Format("UPDATE SEMAM.NW_IDS A SET "
				"ID_VALUE = (SELECT GREATEST(MAX(A.PAY_ID), ID_VALUE) FROM SEMAM.NW_PAYMENTS A, SEMAM.NW_IDS B "
							"WHERE ID_CODE = '%s' GROUP BY ID_VALUE) "
				"WHERE ID_CODE = '%s' ", IDCode, IDCode);
			else
				if(strcmp(IDCode, TICKETNUM) == 0)
					GetSql().Format("UPDATE SEMAM.NW_IDS A SET "
					"ID_VALUE = (SELECT GREATEST(MAX(A.TICKET_NUM), ID_VALUE) FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_IDS B "
								"WHERE ID_CODE = '%s' GROUP BY ID_VALUE) "
					"WHERE ID_CODE = '%s' ", IDCode, IDCode);
	if(!GetSql().IsEmpty())
		ExecuteSql();

	GetSql().Format("SELECT ID_VALUE FROM SEMAM.NW_IDS WHERE ID_CODE = '%s' FOR UPDATE", IDCode);

	if(!Open(GetSql(), ODYNASET_DEFAULT)) // Get to id value and Set id + 1
		return -1;
	StartEdit();
	
	GetFieldValue(0, &idv);
	idv += 1;
	SetFieldValue(0, idv);
	Update();
	
	IDValue = QData.WriteNumber(idv, FALSE, 0);

	return idv;
}

BOOL COraLoader::InCurrentMonth(LPCTSTR Date)
{
	CQData QData;
	CString Status;

	if(strstr(Date, "%"))
		return FALSE;

	if(Date && strlen(Date) > 0)
	{
		GetSql().Format("SELECT DECODE(TO_CHAR(SYSDATE, 'MM/YYYY'), TO_CHAR(TO_DATE(%s), 'MM/YYYY'), 1, 0) FROM DUAL", 
						QData.GetQueryDate(Date));
		if(Open(GetSql()))
		{
			LoadTextString(Status);
			return atoi(Status);
		}
	}

	return FALSE;
}

CString &COraLoader::GetValueDate(LPCTSTR SrcDate, const CString TransType, const CString Asset, BOOL bListed)
{
	CString ValueDate;

	if(TransType == CDS) // CDS and Forex, Value Date is T + 1
		TPlus1(ValueDate, SrcDate, "USD", TRUE);
	else
		if(TransType == REPO || TransType == FOREX) // Repo T + 2
			TPlus2(ValueDate, SrcDate);
		else // Other T + 3
			TPlus3(ValueDate, SrcDate);

	if((TransType == SECURITIES || TransType == CALL || TransType == PUT) && !Asset.IsEmpty())
	{
		CQData QData;
		CString sAsset, AssetType, Future;
	
		sAsset = QData.GetQueryText(Asset);

		Open("SELECT ASS_INDUSTRY, ASS_CONV_TYPE FROM SEMAM.NW_ASSETS WHERE ASS_CODE = " + sAsset);
		AssetType = GetDBString(0);
		Future = GetDBString(1);

		if(Future == F && AssetType.Find("CURRENCY") < 0) // Future except currencys
			ValueDate = SrcDate;
		else
			if(TransType == CALL || TransType == PUT)
			{
				if(bListed) // Listed Options
					TPlus1(ValueDate, SrcDate);
				else 
					TPlus2(ValueDate, SrcDate);
			}
			else			
				if(AssetType == "US FIXED INCOME")
					TPlus1(ValueDate, SrcDate);
				else
				{
					if(AssetType == "ADR EQUITIES" || AssetType == "DOM EQTY-NEW" || AssetType == "DOM EQUITIES" || AssetType == "EQUITY FUNDS" || 
						AssetType == "EXCH TRADE FUND" || AssetType == "BRADY BONDS" || AssetType == "CONVERTIBLE" || AssetType == "CORPORATE" || 
						AssetType == "COUNTRY FUND" || AssetType == "DM BONDS" || AssetType == "EM BONDS" || AssetType == "EURO BONDS" || 
						AssetType == "FIXED INC FUNDS" || AssetType == "FIXED INCOME" || AssetType == "G7 BONDS" || AssetType == "G7 FIXED INCOME" || 
						AssetType == "SOVEREIGN")
						TPlus2(ValueDate, SrcDate);
					else
						if(AssetType == "CURRENCY FWDS")
						{
							Open("SELECT ASS_MATURITY_DATE FROM SEMAM.NW_ASSETS WHERE ASS_CODE = " + sAsset);
							LoadTextString(ValueDate);
										
							if(ValueDate.IsEmpty()) // if it is spot T + 1
								TPlus2(ValueDate, SrcDate);
						}
				}
	}

	m_DataBuf = ValueDate;

	return m_DataBuf;
}