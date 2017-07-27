#ifndef __COMBOX_H__
#define __COMBOX_H__

class COptComboBox	: public CComboBox
{
DECLARE_DYNAMIC(COptComboBox)
protected:
	BOOL  m_bWithNone;
	BOOL  m_bAdd;

public:
	COptComboBox() {}
	COptComboBox(CComboBox *ComboBox, BOOL WithNone = FALSE, BOOL bAdd = FALSE);
	COptComboBox(CWnd *Parent, int ID, BOOL WithNone = FALSE, BOOL bAdd = FALSE) { Setup(Parent, ID, WithNone, bAdd); }
	~COptComboBox() {};

	BOOL Setup(CWnd *Parent, UINT ID, BOOL WithNone = FALSE, BOOL bAdd = FALSE);
	void SetWithNone(BOOL WithNone = TRUE) { m_bWithNone = WithNone; }
	BOOL GetWithNone() { return m_bWithNone; }
	void SetAdd(BOOL bAdd) { m_bAdd = bAdd; }
	BOOL GetAdd() { return m_bAdd; }

	void SetData(const CString Data);
	virtual const CString GetData() { CString Data; GetSelString(Data); return Data; }

	LPCTSTR GetSelString(CString &Text);
	void GetLBText(int Index, CString &Text);
	int GetCurSel();
	int SetCurSel(int Index);
	int FindString(int nStartAfter, LPCTSTR Key);
	int SelectString(int nStartAfter, LPCTSTR Key);
	void SetReadOnly(BOOL bReadOnly = TRUE);

protected:   
	// Generated message map functions
	//{{AFX_MSG(COptComboBox)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
};

#endif // __COMBOX_H__






