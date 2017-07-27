#ifndef __HLBOX_H__
#define __HLBOX_H__

class CHLBox : public CListBox
{
DECLARE_DYNAMIC(CHLBox)
protected:
	CMenu m_Menu;
	BOOL  m_bWithNone;
	int   m_nMaxHExtent;
	int   m_nOrigMaxHExtent;

private:
	int  GetItemHExtent(LPCTSTR String);   
	void ResetLBHExtent();
	void SetLBHExtent(int HExtent);
	void AddMenu(UINT MenuID = 0);

public:
	CHLBox() {}
	CHLBox(int nHMaxExtent, UINT MenuID = 0, BOOL WithNone = FALSE);
	CHLBox(CListBox *ListBox, UINT MenuID = 0, BOOL WithNone = FALSE);
	CHLBox(CWnd *Parent, int ID, UINT MenuID = 0, BOOL WithNone = FALSE);
	~CHLBox();
   
	void Setup(CWnd *Parent, int ID, UINT MenuID = 0, BOOL WithNone = FALSE);
	int  AddString(LPCTSTR String);
	int  InsertString(int nIndex, LPCTSTR String);
	void ResetContent();
	void SetWithNone(BOOL WithNone = TRUE) { m_bWithNone = WithNone; }
	BOOL GetWithNone() { return m_bWithNone; }
	CString &GetSelString(CString &Buffer);
	int SetCurSel();
	int SetCurSel(int Index);
	int FindString(int nStartAfter, LPCTSTR Key);
	int SelectString(int nStartAfter, LPCTSTR Key);
	int EditFind(CWnd &Wnd);
	int	EditSelect(CWnd &Wnd);

	void SetData(const CString Data);
	virtual const CString GetData() { CString Buffer; GetSelString(Buffer); return Buffer; }

protected:
	// Generated message map functions
	//{{AFX_MSG(CCHLBox)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()	//}}AFX_MSG
};

#endif // __HLBOX_H__






