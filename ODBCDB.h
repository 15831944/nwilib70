#ifndef __ODBCDB_H__
#define __ODBCDB_H__

#include<afxdb.h>

#ifndef __DBSHEET_H__
#include "DBSheet.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __MCCOMBOBOX_H__
#include "mccombobox.h"
#endif

class CODBCDB : public CDatabase
{
	DECLARE_DYNAMIC(CODBCDB)
protected:
	CString m_DSN;

public:
	CODBCDB() : CDatabase() {}
	
	~CODBCDB() {}

protected:
	CString GetOracleDSN(const CString UserName, const CString Password);
	CString GetExcelDSN(const CString FileName);

public:
	CString &GetDSN() { return m_DSN; }
	
	virtual BOOL Open(BOOL bExclusive = FALSE, BOOL bReadOnly = FALSE, BOOL bUserCursorLib = FALSE);
	virtual BOOL Open(const CString FileName, BOOL bExclusive = FALSE, BOOL bReadOnly = FALSE, BOOL bUserCursorLib = FALSE);
	virtual BOOL Open(const CString UserName, const CString Password, BOOL bExclusive = FALSE, BOOL bReadOnly = FALSE, BOOL bUserCursorLib = FALSE);
	virtual BOOL OpenEx(DWORD dwOptions = 0);
	virtual BOOL OpenEx(const CString FileName, DWORD dwOptions = 0);
	virtual BOOL OpenEx(const CString UserName, const CString Password, DWORD dwOptions = 0);

	int LoadDBSheet(CRecordset *pDBRec, CDBSheet &DBSheet);
	int LoadComboBox(CRecordset *pDBRec, COptComboBox &CBox);
	int LoadMCComboBox(CRecordset *pDBRec, CMCComboBox &CBox);
};
#endif // __ODBCDSET_H__