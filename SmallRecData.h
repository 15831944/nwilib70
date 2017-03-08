#ifndef __SMALLRECDATA_H__
#define __SMALLRECDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif


class CSmallRecData: public CRowCtrl
{
DECLARE_DYNAMIC(CSmallRecData)
protected:
	CString m_Key;
	CString m_KeyName;
	CString m_Key2;
	CString m_KeyName2;
	CString m_Table;
	CString m_PreLoad;
	COraLoader m_OraLoader;

public:
	CSmallRecData() { }
	CSmallRecData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CSmallRecData(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR TableSpace, 
				LPCTSTR Table, LPCTSTR PreLoad, LPCTSTR KeyName, LPCTSTR KeyName2 = NULL)
				{ Setup(OraLoader, pSS, TableSpace, Table, PreLoad, KeyName, KeyName2); }
	~CSmallRecData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	void Setup(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR TableSpace, LPCTSTR Table, 
				LPCTSTR PreLoad, LPCTSTR KeyName, LPCTSTR KeyName2 = NULL);

	CString &GetKey() { return m_Key; }
	CString &GetKeyName() { return m_KeyName; }
	CString &GetKey2() { return m_Key2; }
	CString &GetKeyName2() { return m_KeyName2; }
	CString &GetTable() { return m_Table; }
	CString &GetPreLoad() { return m_PreLoad; }
	void SetKey(LPCTSTR Key) { m_Key = Key; }
	void SetKeyName(LPCTSTR KeyName) { m_KeyName = KeyName; }
	void SetKey2(LPCTSTR Key) { m_Key2 = Key; }
	void SetKeyName2(LPCTSTR KeyName);
	void SetTable(LPCTSTR Table) { m_Table = Table; }
	void SetPreLoad(LPCTSTR PreLoad) { m_PreLoad = PreLoad; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	LPCTSTR GetQueryInfo(CString &KeyName, LPCTSTR Key, CString &QKey);
	
	virtual BOOL IsReady();
	
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
	int LoadDBData();
	BOOL SetSheetHeaders();

	void SetLimitText(CEdit &Edit, int Index);
};
#endif