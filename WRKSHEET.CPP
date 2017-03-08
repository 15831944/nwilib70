#include "stdafx.h"
#include "wrksheet.h"
#include "nwivar.h"

IMPLEMENT_DYNCREATE(CWorkSheet, CSpreadSheet)

CWorkSheet::CWorkSheet()
{
	m_nRows = 0;
	m_nCols = 0;
	m_nCurCol = -1;
	m_nCurRow = -1;
	SetAutoClipboard(TRUE);
	SetClipboardOptions(SS_CLIP_COPYCOLHEADERS|SS_CLIP_PASTECOLHEADERS);
}

void CWorkSheet::ClearSheet(int Col, int Row, int Col2, int Row2)
{
	if(Row2 < 1)
		return;
	CSpreadSheet::ClearRange(Col, Row, Col2, Row2, TRUE);
}

void CWorkSheet::CopySheet(int Col, int Row, int Col2, int Row2, CWorkSheet &DestSS)
{
	int i, j;
	CString Text;

	for(i = Row; i <= Row2; i++)
		for(j = Col; j <= Col2; j++)
		{
			Text = GetSheetText(j, i);
			DestSS.SetSheetText(j, i, Text);
		}
	
	DestSS.SetSheetRows(GetSheetRows());
	DestSS.SetSheetCols(GetSheetCols());
}

void CWorkSheet::StringArrayToString(CString &Str, CStringArray &StrArr)
{
	Str.Empty();
	for(int i = 0; i < StrArr.GetSize(); i++)	
	{
		if(i > 0)
			Str += TAB;
		Str += StrArr.GetAt(i);			
	}
}
									
void CWorkSheet::SetSheetActiveCell(int Col, int Row)
{
	if(Col >= 1 && Col <= m_nCols && Row >= 1 && Row <= m_nRows)
		CSpreadSheet::SetActiveCell(Col, Row);
}

void CWorkSheet::SetSheetColHeaders(int Cols, LPCTSTR Headers)
{
	if(!Headers || strlen(Headers) == 0 || Cols < 1)
		return;
		
	SetSheetRow(1, 0, Cols, 0, Headers);
	CSpreadSheet::SetMaxCols(Cols);
}

void CWorkSheet::SetSheetFontBold(BOOL bBold)
{
	if(GetSheetCols() > 0 && GetSheetRows() > 0)
	{
		SetCol(-1);
		SetCol2(-1);
		SetRow(-1);
		SetRow2(-1);
		SetBlockMode(TRUE);
		SetFontBold(bBold);
		SetBlockMode(FALSE);
	}
}

void CWorkSheet::SetSheetColHeaders(CStringArray &Headers)
{
	CString Line;

	StringArrayToString(Line, Headers);
	SetSheetColHeaders(Headers.GetSize(), Line);
}

void CWorkSheet::SetSheetRow(int Col, int Row, int Col2, int Row2, CStringArray &Data)
{
	CString Line;

	StringArrayToString(Line, Data);
	SetSheetRow(Col, Row, Col2, Row2, Line);
}

void CWorkSheet::SetSheetRow(int Col, int Row, int Col2, int Row2, LPCTSTR Data)
{
	if(!Data || Col2 < Col || Row2 < Row)
		return;

	CSpreadSheet::SetCol(Col);
	CSpreadSheet::SetRow(Row);
	CSpreadSheet::SetCol2(Col2);
	CSpreadSheet::SetRow2(Row2);
	CSpreadSheet::SetClip(Data);
}

CString CWorkSheet::GetSheetText(int Col, int Row)
{
	CSpreadSheet::SetCol(Col);
	CSpreadSheet::SetRow(Row);

	return CSpreadSheet::GetText();
}

void CWorkSheet::SetSheetText(int Col, int Row, LPCTSTR Text)
{
	CSpreadSheet::SetCol(Col);
	CSpreadSheet::SetRow(Row);

	CSpreadSheet::SetText(Text);
}

void CWorkSheet::LockSheet(int Col, int Row, int Col2, int Row2, BOOL Locked)
{
	if(Col2 < Col || Row2 < Row)
		return;
	
	CSpreadSheet::SetCol(Col);
	CSpreadSheet::SetRow(Row);
	CSpreadSheet::SetCol2(Col2);
	CSpreadSheet::SetRow2(Row2);
	CSpreadSheet::SetBlockMode(TRUE);
	CSpreadSheet::SetLock(Locked);
	CSpreadSheet::SetBlockMode(FALSE);
}

void CWorkSheet::SetSheetToFit(int VisibleCols, int VisibleRows)
{
	int	i;
	double	FWidth;
	
	for(i = 0; i <= GetSheetCols(); i++)
	{
		CSpreadSheet::SetCol(i);
		FWidth = CSpreadSheet::GetMaxTextColWidth(i);
    	CSpreadSheet::SetColWidth(i, FWidth);
	}
		
	SetVisibleCols(VisibleCols);
	SetVisibleRows(VisibleRows);
}

BOOL CWorkSheet::GetSheetColHidden(int Col)
{
	if(Col >= 1 && Col <= m_nCols)
	{
		CSpreadSheet::SetCol(Col);
		return CSpreadSheet::GetColHidden();
	}

	return FALSE;
}

void CWorkSheet::SetSheetColHidden(int Col, BOOL Hidden)
{
	if(Col >= 1 && Col <= m_nCols)
	{
		CSpreadSheet::SetCol(Col);
		CSpreadSheet::SetColHidden(Hidden);
	}
}

BOOL CWorkSheet::GetSheetRowHidden(int Row)
{
	if(Row >= 1 && Row <= m_nRows)
	{
		CSpreadSheet::SetRow(Row);
		return CSpreadSheet::GetRowHidden();
	}

	return FALSE;
}

void CWorkSheet::SetSheetRowHidden(int Row, BOOL Hidden)
{
	if(Row >= 1 && Row <= m_nRows)
	{
		CSpreadSheet::SetRow(Row);
		CSpreadSheet::SetRowHidden(Hidden);
	}
}
