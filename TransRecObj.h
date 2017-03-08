#ifndef __TRANSRECOBJ_H__
#define __TRANSRECOBJ_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CTransRecObj : public CRecObj
{
DECLARE_DYNAMIC(CTransRecObj)
protected:
	BOOL m_bNAVLock;
	BOOL m_bCashLock;
	BOOL m_bPayInstructLock;
	BOOL m_bMonthlyLock;

public:
	void SetNAVLock(BOOL bNAVLock) { m_bNAVLock = bNAVLock; }
	void SetCashLock(BOOL bCashLock) { m_bCashLock = bCashLock; }
	void SetPayInstructLock(BOOL bPayInstructLock) { m_bPayInstructLock = bPayInstructLock; }
	void SetMonthlyLock(BOOL bMonthlyLock) { m_bMonthlyLock = bMonthlyLock; }

	BOOL GetNAVLock() { return m_bNAVLock; }
	BOOL GetCashLock() { return m_bCashLock; }
	BOOL GetPayInstructLock() { return m_bPayInstructLock; }
	BOOL GetMonthlyLock() { return m_bMonthlyLock; }

	virtual void Copy(CTransRecObj &TransRecObj);
	const CTransRecObj &operator=(CTransRecObj &TransRecObj) { Copy(TransRecObj); return *this; }
	virtual void ToDBRec(CDBRec &Rec) { }
	virtual BOOL AddRec(COraLoader &OraLoader) { return TRUE; }
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
};

class CTransRecObjArray : public CRecObjArray
{
DECLARE_DYNAMIC(CTransRecObjArray)

public:
	CTransRecObjArray() : CRecObjArray() { };
	CTransRecObjArray(LPCTSTR ID) { SetID(ID); }

	int Add(CTransRecObj &TransRecObj);
	CTransRecObj *GetAt(int Index);
	void RemoveAll();
	virtual void Copy(CTransRecObjArray &TransRecObjArray);
	const CTransRecObjArray &operator=(CTransRecObjArray &TransRecObjArray) { Copy(TransRecObjArray); return *this; }
	virtual void ToDBRec(CDBRec &Rec, int Index) { }
	virtual BOOL AddRec(COraLoader &OraLoader) { return TRUE; }
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE) { return TRUE; }
};
#endif
