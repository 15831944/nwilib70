#ifndef __KEYDATA_H__
#define __KEYDATA_H__


class CKeyData : public CObject
{
DECLARE_DYNAMIC(CKeyData)
protected:	
	CString m_Key;
	BOOL	m_bModified;
   
public:
	CKeyData();
	CKeyData(const CString Key) { m_Key = Key; IsModified() = FALSE; }
	CKeyData(CKeyData &KeyData) { Copy(KeyData); }
	virtual ~CKeyData() {}
    	
	void Copy(CKeyData &KeyData);
	const CKeyData &operator=(CKeyData &KeyData) { Copy(KeyData); return *this; }

	CString &GetKey() { return m_Key; }
	void SetKey(const CString Key) { m_Key = Key; }
	
	BOOL &IsModified() { return m_bModified; }
};


class CKeyDataArray : public CObArray
{
DECLARE_DYNAMIC(CKeyDataArray)

public:
	CKeyDataArray() {}
	~CKeyDataArray() { RemoveAll(); }

	int	Add(CKeyData *pData);
	void SetAt(int Index, CKeyData *pKeyData);
	void InsertAt(int Index, CKeyData *pKeyData) { CObArray::InsertAt(Index, pKeyData);	 }
	int Add(CKeyData &KeyData) { return Add(new CKeyData(KeyData)); }
	void SetAt(int Index, CKeyData &KeyData) { SetAt(Index, new CKeyData(KeyData)); }
	void InsertAt(int Index, CKeyData &KeyData) { InsertAt(Index, new CKeyData(KeyData)); }
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CKeyDataArray &KeyDataArray);
	CKeyData *GetAt(int Index);
		
	const CKeyDataArray &operator=(CKeyDataArray &KeyDataArray) { Copy(KeyDataArray); return *this; }
	BOOL IsModified();
	virtual int Find(const CString Key);
	virtual BOOL FreePointer(int Index);
};
#endif
