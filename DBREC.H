#ifndef __DBREC_H__
#define __DBREC_H__

#ifndef __DBBASE_H__
#include "dbbase.h"
#endif

class CDBRec : public CDBBase
{
DECLARE_DYNAMIC(CDBRec)

public:
    CStringArray m_Rec;

public:
	CDBRec();
	CDBRec(CDBRec &DBRec);
	~CDBRec() {}

	void Copy(CDBRec &DBRec) { CDBBase::Copy(DBRec); m_Rec.Copy(DBRec.GetRec()); }
	const CDBRec &operator = (CDBRec &DBRec) { Copy(DBRec); return *this; }
	CStringArray &GetRec() { return m_Rec; }
	void SetRec(CStringArray &Rec);
	void SetRec(CDBRec &Rec);

	LPCTSTR GetAt(int Index);
	int GetSize() { return GetRec().GetSize(); }
	int Add(LPCTSTR Field);
	void SetAt(int Index, LPCTSTR Field);
	void RemoveAt(int Index);
	void RemoveAll();
	void InsertAt(int Index, LPCTSTR Field);

	CString &GetRecID() { return CDBBase::GetID(); }
	void SetRecID(LPCTSTR RecID) { CDBBase::SetID(RecID); }
	CString &ToString(CString &Str);
};
#endif // __DBREC_H__