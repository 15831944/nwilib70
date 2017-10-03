#pragma once

#include "oraloader.h"


class CRecObj : public CObject
{
DECLARE_DYNAMIC(CRecObj)
protected:
	CString m_ID;

public:
	CRecObj() { m_ID.Empty(); }
	CRecObj(CRecObj &RecObj) { SetID(RecObj.GetID()); }
	CRecObj(LPCTSTR ID) { SetID(ID); }

	void SetID(LPCTSTR ID);
	CString &GetID() { return m_ID; }

	virtual void Copy(CRecObj &RecObj) { SetID(RecObj.GetID()); }
	const CRecObj &operator=(CRecObj &RecObj) { Copy(RecObj); return *this; }
	virtual void ToDBRec(CDBRec &Rec) { }
	virtual BOOL AddRec(COraLoader &OraLoader) { return TRUE; }
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
};

class CRecObjArray : public CObArray
{
DECLARE_DYNAMIC(CRecObjArray)
protected:
	CString m_ID;

public:
	CRecObjArray() { m_ID.Empty(); }
	CRecObjArray(LPCTSTR ID) { SetID(ID); }

	void SetID(LPCTSTR ID);
	CString &GetID() { return m_ID; }

	int Add(CRecObj &RecObj);
	CRecObj *GetAt(int Index);
	void RemoveAll();
	virtual void Copy(CRecObjArray &RecObjArray);
	const CRecObjArray &operator=(CRecObjArray &RecObjArray) { Copy(RecObjArray); return *this; }
	virtual void ToDBRec(CDBRec &Rec, int Index) { }
	virtual BOOL AddRec(COraLoader &OraLoader) { return TRUE; }
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
};

