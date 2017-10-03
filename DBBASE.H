#pragma once

class CDBBase : public CObject
{
DECLARE_DYNAMIC(CDBBase)
protected:	
	CString	m_ID;
	BOOL	m_bModified;
	BOOL	m_bDeleted;
	BOOL	m_bListed;
	BOOL	m_bInDatabase;
	BOOL	m_bLocked;
   
public:
	CDBBase();
	CDBBase(CDBBase &DBBase) { Copy(DBBase); }
	virtual ~CDBBase() {}
    	
	void Copy(CDBBase &DBBase);
	const CDBBase &operator=(CDBBase &DBBase) { Copy(DBBase); return *this; }

	CString &GetID() { return m_ID; }
	void SetID(LPCTSTR ID) { m_ID = ID; }
	
	BOOL GetModify() { return m_bModified; }
	void SetModify(BOOL Modified = TRUE) { m_bModified = Modified; }
	
	BOOL GetInDatabase() { return m_bInDatabase; }
	void SetInDatabase(BOOL InDb = TRUE) { m_bInDatabase = InDb; }
	
	BOOL GetDeleted() { return m_bDeleted; }
	void SetDeleted(BOOL Deleted = TRUE) { m_bDeleted = Deleted; }
	
	BOOL GetListed() { return m_bListed; }
	void SetListed(BOOL Listed = TRUE) { m_bListed = Listed; }

	BOOL GetLocked() { return m_bLocked; }
	void SetLocked(BOOL Locked = TRUE) { m_bLocked = TRUE; }
};


class CDBBaseArray : public CObArray
{
DECLARE_DYNAMIC(CDBBaseArray)

public:
	CDBBaseArray();
	~CDBBaseArray();

	int	Add(CDBBase *Rec);
	void SetAt(int Index, CDBBase *Rec);
	void InsertAt(int Index, CDBBase *Rec);
	int Add(CDBBase &Rec);
	void SetAt(int Index, CDBBase &Rec);
	void InsertAt(int Index, CDBBase &Rec);
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CDBBaseArray &DBBaseArray);
	CDBBase *GetAt(int Index);
		
	const CDBBaseArray &operator=(CDBBaseArray &DBBaseArray) { Copy(DBBaseArray); return *this; }
};
