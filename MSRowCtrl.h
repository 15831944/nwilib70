#ifndef __MSROWCTRL_H__
#define __MSROWCTRL_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CMSRowCtrl : public CRowCtrl
{
DECLARE_DYNAMIC(CMSRowCtrl)
protected:
	COraLoader m_OraLoader;
	CRowCtrl m_SRowCtrl;
	CString m_Key;
	int m_KeyIndex;

public:
	CMSRowCtrl() { }
	CMSRowCtrl(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS)
				{ Setup(OraLoader, pSS, pSSS, -1); }
	CMSRowCtrl(CMSRowCtrl &MSRowCtrl);
	~CMSRowCtrl() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex = -1);
	
	CRowCtrl &GetSRowCtrl() { return m_SRowCtrl; }
	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	virtual BOOL IsReady() { return m_OraLoader.GetDB() && CRowCtrl::IsReady() && GetSRowCtrl().IsReady(); }

	BOOL SetCurrentRow(int CurrentRow, BOOL UpdatePrevRow = FALSE);
	BOOL SetCurrentRow(int Row, CKeyDBList &KeyDBList, BOOL UpdatePrevRow = FALSE);
	
	int GetKeyIndex() { return m_KeyIndex; }
	CString &GetKey() { return m_Key; }
	void SetKey();
	void SetKey(LPCTSTR Key) { m_Key = Key; }
	void SetKeyIndex(int Index);

	virtual void LoadSheet() {}
	virtual void UpdateData(CRecObj *pRec = NULL, CRecObj *pInv = NULL);
	virtual void LoadSheet(CDBRecArray &DBRecArray);
	virtual void LoadSheet(CKeyDBList &KeyDBList);
	virtual void UpdateSheetRow(BOOL bFromSheet = TRUE);

	virtual BOOL AddRow() { return TRUE; }
	virtual BOOL DeleteRow() { return TRUE; }
	virtual BOOL UpdateRow() { return TRUE; }

	virtual BOOL AddSRow() { return TRUE; }
	virtual BOOL DeleteSRow(int CurrentRow = -1) { return TRUE; }
	virtual BOOL UpdateSRow() { return TRUE; }
};
#endif