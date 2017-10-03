#pragma once

#include "spreadsheet.h"
#include "ssocx.h"

class CWorkSheet : public CSpreadSheet
{
protected:
	DECLARE_DYNCREATE(CWorkSheet)
public:
	CWorkSheet();
	~CWorkSheet() {};

protected:
	int m_nRows;
	int m_nCols;
	int m_nCurRow;
	int m_nCurCol;

public:
	int GetSheetCols() { return m_nCols; }
	void SetSheetCols(int Cols) { m_nCols = Cols; }
	int GetSheetRows() { return m_nRows; }
	void SetSheetRows(int Rows) { m_nRows = Rows; }
	int GetSheetCurCol() { return m_nCurCol; }
	void SetSheetCurCol(int CurCol) { m_nCurCol = CurCol; }
	int GetSheetCurRow() { return m_nCurRow; }
	void SetSheetCurRow(int CurRow) { m_nCurRow = CurRow; }
	void SetSheetActiveCell(int Col, int Row);
	void ClearSheet(int Col, int Row, int Col2, int Row2);
	void CopySheet(int Col, int Row, int Col2, int Row2, CWorkSheet &DestSS);
	void SetSheetColHeaders(int Cols, LPCTSTR Headers);
	void SetSheetColHeaders(CStringArray &Headers);
	void LockSheet(int Col, int Row, int Col2, int Row2, BOOL Locked = TRUE);
	void SetSheetRow(int Col, int Row, int Col2, int Row2, LPCTSTR Data);
	void SetSheetRow(int Col, int Row, int Col2, int Row2, CStringArray &Data);
	void SetSheetFontBold(BOOL bBold = TRUE);
 	CString GetSheetText(int Col, int Row);
	void SetSheetText(int Col, int Row, LPCTSTR Text);
	void SetSheetToFit(int VisibleCols, int VisibleRows);
	void StringArrayToString(CString &Str, CStringArray &StrArr);
	
	BOOL GetSheetColHidden(int Col);
	void SetSheetColHidden(int Col, BOOL Hidden = TRUE);
	BOOL GetSheetRowHidden(int Row);
	void SetSheetRowHidden(int Row, BOOL Hidden = TRUE);
};

