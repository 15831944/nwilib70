#ifndef __ROWCTRL_H__
#define __ROWCTRL_H__

#ifndef __WNDCTRL_H__
#include "wndctrl.h"
#endif

#ifndef __DBSHEET_H__
#include "dbsheet.h"
#endif

class CRowCtrl : public CWndCtrlArray
{
DECLARE_DYNAMIC(CRowCtrl)
protected:
	CDBSheet *m_pSS;
	CDBBaseArray m_IDArray;

public:
	CRowCtrl() { SetSS(NULL); }
	CRowCtrl(CDBSheet *pSS) { SetSS(pSS); }
	CRowCtrl(CRowCtrl &RowCtrl);
	~CRowCtrl() { }

	void Setup(CDBSheet *pSS) { SetSS(pSS); }

	void SetSS(CDBSheet *SS) { m_pSS = SS; }
	CDBSheet *GetSS() { return m_pSS; }

	CDBBaseArray &GetIDArray() { return m_IDArray; }
	
	LPCTSTR GetID();
	virtual void AddSheetRow();
	virtual void AddSheetRow(CDBRec &DBRec);
	virtual void AddSheetRow(LPCTSTR ID, LPCTSTR Rec);
	virtual void DeleteSheetRow(int CurrentRow = -1);
	virtual void UpdateSheetRow(BOOL bFromSheet = TRUE);
	virtual void ClearSheet();

	void Refresh();
	virtual BOOL IsReady () { return m_pSS ? TRUE : FALSE; }
	int GetCurrentRow() { return m_pSS ? m_pSS->GetSheetCurRow() : -1; }
	BOOL SetCurrentRow(int CurrentRow, BOOL UpdatePrevRow = FALSE);
};
#endif