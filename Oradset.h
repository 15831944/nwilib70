#ifndef __ORADSET_H__
#define __ORADSET_H__

#ifndef ORACL_ORACLE
#include "oracl.h"
#endif

#ifndef __ORAFIELD_H__
#include "orafield.h"
#endif

class COraDset : public ODynaset
{
private:
	static LPCTSTR m_OraDsetError;

protected:
	OSession m_Session;
	ODatabase *m_pDB;
	BOOL m_bBegin;
	BOOL m_bShowError;
	CString m_Sql;
	CString	m_ErrMsg;
	CFieldArray m_FieldArray;

public:
	COraDset(ODatabase *Database);
	COraDset(COraDset &OraDset) { Copy(OraDset); }
	COraDset();
	~COraDset() {}

protected:
	BOOL GetFieldInfo();
	void SetBegin(BOOL bBegin) { m_bBegin = bBegin; }
	BOOL GetBegin() { return m_bBegin; }

public: // Data Interfaces
	void Copy(COraDset &OraDset);
	const COraDset &operator = (COraDset &OraDset) { Copy(OraDset); }
	void SetDB(ODatabase *pDB);
	ODatabase *GetDB() { return m_pDB; }
	CFieldArray &GetFieldArray() { return m_FieldArray; }
	void CopyFieldArray(CFieldArray &FieldArray);
	void UpdateFieldArray(CFieldArray &FieldArray);

	void SetSql(LPCTSTR Sql = NULL);
	CString &GetSql() { return m_Sql; }	
	CString &GetErrMsg() { return m_ErrMsg; }
	CString &GetErrMsg(int rc);

public: // Dynamic Set Operation	
	void BeginTransaction();
	void Commit();
	void Rollback();
	BOOL ExecuteSql(LPCTSTR Sql);
	BOOL ExecuteSql();
	BOOL ExecuteSql(UINT SqlID);
	int	IsExist(LPCTSTR Sql);
	int IsExist();
	int IsExist(UINT SqlID);
	void SetShowError(BOOL bShowError = TRUE) { m_bShowError = bShowError; }
	BOOL GetShowError() { return m_bShowError; }
 	BOOL Open(CString &Sql, int Options = ODYNASET_NOBIND|ODYNASET_READONLY|ODYNASET_NOCACHE);
	BOOL Open(LPCTSTR Sql = NULL, int Options = ODYNASET_NOBIND|ODYNASET_READONLY|ODYNASET_NOCACHE);
	BOOL Open(UINT SqlID, int Options = ODYNASET_NOBIND|ODYNASET_READONLY|ODYNASET_NOCACHE);
	BOOL CheckOpen();
};
#endif // __ORADSET_H__