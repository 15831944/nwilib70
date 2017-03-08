#ifndef __DBSHEET_H__
#define __DBSHEET_H__

#ifndef __WRKSHEET_H__
#include "wrksheet.h"
#endif

#ifndef __ORAFIELD_H__
#include "orafield.h"
#endif

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

class CDBSheet : public CWorkSheet
{
DECLARE_DYNCREATE(CDBSheet)
public:
	CDBSheet();
	CDBSheet(CDBSheet &DBSheet) { Copy(DBSheet); }
	~CDBSheet() {};

protected:
	CFieldArray m_FieldArray;

public:
	void Copy(CDBSheet &DBSheet);
	const CDBSheet &operator = (CDBSheet &DBSheet) { Copy(DBSheet); }

	void SetMaxRows(int Rows);
	void ClearSheet();
	void SetSheetColHeaders();
	void SheetAutoFit();
	void AddSheetRow(CStringArray &Data);
	void AddSheetRow(CDBRec &Data);
	void AddSheetRow(LPCTSTR Data);
	void DeleteSheetRow(int Row);
	void GetSheetRow(int Row, CStringArray &Data);
	void GetSheetRow(int, CDBRec &Data);
	void UpdateSheetRow(int Row, LPCTSTR Data);
	void UpdateSheetRow(int Row, CStringArray &Data);
	void UpdateSheetRow(int Row, CDBRec &Data);
	void LockSheet(BOOL Locked = TRUE);
	void LockSheetCol(int Col, BOOL Locked = TRUE);
	void LockSheetRow(int Row, BOOL Locked = TRUE);
	void LockSheetCell(int Col, int Row, BOOL Locked = TRUE);
	CFieldArray &GetFieldArray() { return m_FieldArray; }

	BOOL GetSheetColHidden(LPCTSTR ColName);
	void SetSheetColHidden(LPCTSTR ColName, BOOL Hidden = TRUE);
	BOOL GetSheetColHidden(int Col) { return CWorkSheet::GetSheetColHidden(Col); }
	void SetSheetColHidden(int Col, BOOL Hidden = TRUE) { CWorkSheet::SetSheetColHidden(Col, Hidden); }
};

#endif // __DBSHEET_H__
