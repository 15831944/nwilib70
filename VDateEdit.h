#ifndef __VDATEEDIT_H__
#define __VDATEEDIT_H__

const UINT WM_DATETIME_EDIT = ::RegisterWindowMessage("WM_DATETIME_EDIT");
#define DTE_DATETIMECHANGED 0x0001

class CVDateTimeEdit : public CEdit
{
// Construction
public:
    CVDateTimeEdit();
	void CVDateEdit(CWnd *Parent, unsigned int ID, LPCTSTR ErrMsg = "Invalid Date", CWnd *pCompareDate = NULL);
	void Setup(CWnd *Parent, unsigned int ID, LPCTSTR ErrMsg = "Invalid Date", CWnd *pCompareDate = NULL);

// Attributes
public:
	BOOL m_bFind;
	CWnd *m_pCompareDate;
	CString m_ErrMsg;
	COleDateTime m_CompareDate;

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(VDateTimeEdit)
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

// Implementation
public:
    CString GetMask() const {return m_strMask;}
    void SetMask(CString mask);
    COleDateTime GetDateTime();
    COleDateTime GetDateTime(CString str);
    void SetDateTime(COleDateTime dt);

    virtual ~CVDateTimeEdit();

    // Generated message map functions
protected:
    afx_msg void OnContextMenu(CWnd*, CPoint point);
	void SetCompareDate();

    CString m_strMask;
    //{{AFX_MSG(VDateTimeEdit)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
#endif