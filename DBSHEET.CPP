#include "stdafx.h"
#include "dbsheet.h"
#include "nwivar.h"
#include "ssocx.h"


IMPLEMENT_DYNCREATE(CDBSheet, CWorkSheet)
CDBSheet::CDBSheet()
{

}

void CDBSheet::Copy(CDBSheet &DBSheet)
{
	GetFieldArray().Copy(DBSheet.GetFieldArray());
	ClearSheet();
	SetSheetColHeaders();
	
	int i, j;
	for(i = 1; i <= DBSheet.GetSheetRows(); i ++)
		for(j = 1; j <= DBSheet.GetSheetCols(); j ++)
			SetSheetText(j, i, DBSheet.GetSheetText(j, i));

	SetAutoClipboard(TRUE);
	SetClipboardOptions(SS_CLIP_COPYCOLHEADERS|SS_CLIP_PASTECOLHEADERS);
}


void CDBSheet::ClearSheet()
{
	CWorkSheet::ClearSheet(1, 1, GetSheetCols(), GetSheetRows());
	SetSheetRows(-1);
}

void CDBSheet::DeleteSheetRow(int Row)
{
	if(Row < 1 || Row > GetSheetRows())
		return;

	DeleteRows(Row, 1);

	SetSheetRows(GetSheetRows() - 1);
	SheetAutoFit();
}

void CDBSheet::GetSheetRow(int Row, CStringArray &Data)
{
	Data.RemoveAll();
	if(Row > 0 && Row <= GetSheetRows())
		for(int i = 1; i <= GetSheetCols(); i ++)
			Data.Add(GetSheetText(i, Row));
}

void CDBSheet::GetSheetRow(int Row, CDBRec &Data)
{
	Data.RemoveAll();
	if(Row > 0 && Row <= GetSheetRows())
		for(int i = 1; i <= GetSheetCols(); i ++)
			Data.Add(GetSheetText(i, Row));
}

void CDBSheet::SetSheetColHeaders()
{
	if(GetSheetCols() > 0)
		CWorkSheet::ClearSheet(1, 0, GetSheetCols(), 0);

	if(GetFieldArray().GetSize() > 0)
	{
		SetMaxCols(GetFieldArray().GetSize());
		SetSheetCols(GetFieldArray().GetSize());
	}

	for(int i = 0; i < GetFieldArray().GetSize(); i++)		
		SetSheetText(i + 1, 0, GetFieldArray().GetAt(i)->GetName());
}

void CDBSheet::SetMaxRows(int Rows)
{
	CWorkSheet::SetMaxRows(Rows > GetVisibleRows() ? Rows : GetVisibleRows());
}

void CDBSheet::AddSheetRow(CStringArray &Data)
{
	CString RowData;

	StringArrayToString(RowData, Data);
	AddSheetRow((const char*) RowData);
}

void CDBSheet::AddSheetRow(CDBRec &Data)
{
	CString RowData;
	
	AddSheetRow((const char*) Data.ToString(RowData));
}

void CDBSheet::AddSheetRow(LPCTSTR Data)
{
	if(GetMaxRows() <= GetSheetRows())
		SetMaxRows(GetSheetRows() + 1);
	
	if(GetSheetRows() <= 0)
		SetSheetRows(1);
	else
		SetSheetRows(GetSheetRows() + 1);
	SetSheetRow(1, GetSheetRows(), GetSheetCols(), GetSheetRows(), Data);
}

void CDBSheet::UpdateSheetRow(int Row, CStringArray &Data)
{
	CString RowData;

	StringArrayToString(RowData, Data);
	UpdateSheetRow(Row, (const char*) RowData);
}

void CDBSheet::UpdateSheetRow(int Row, CDBRec &Data)
{
	CString RowData;
	
	UpdateSheetRow(Row, Data.ToString(RowData));
}

void CDBSheet::UpdateSheetRow(int Row, LPCTSTR Data)
{
	if(Row >= 1 && Row <= GetSheetRows())
	{
		SetSheetRow(1, Row, GetSheetCols(), Row, (LPCTSTR) Data);
		SheetAutoFit();
	}
}

void CDBSheet::LockSheet(BOOL Locked)
{	
	CWorkSheet::LockSheet(1, 1, GetSheetCols(), GetMaxRows(), Locked);
}

void CDBSheet::LockSheetCol(int Col, BOOL Locked)
{
	if(Col >= 1 && Col <= GetSheetCols())
		CWorkSheet::LockSheet(Col, 1, Col, GetMaxRows(), Locked);
}

void CDBSheet::LockSheetRow(int Row, BOOL Locked)
{
	if(Row >= 1 && Row <= GetSheetRows())
		CWorkSheet::LockSheet(1, Row, GetSheetCols(), Row, Locked);
}

void CDBSheet::LockSheetCell(int Col, int Row, BOOL Locked)
{
	if(Col >= 1 && Col <= GetSheetCols() && Row >= 1 && Row <= GetSheetRows())
		CWorkSheet::LockSheet(Col, Row, Col, Row, Locked);
}

BOOL CDBSheet::GetSheetColHidden(LPCTSTR ColName)
{
	int Col = GetFieldArray().FindAt(ColName);
	if(Col > 0)
		return GetSheetColHidden(Col);
	return FALSE;
}

void CDBSheet::SetSheetColHidden(LPCTSTR ColName, BOOL Hidden)
{
	int Col = GetFieldArray().FindAt(ColName);
	if(Col > 0)
		SetSheetColHidden(Col, Hidden);
}

void CDBSheet::SheetAutoFit()
{
	SetSheetToFit(GetVisibleCols(), GetVisibleRows());
}

