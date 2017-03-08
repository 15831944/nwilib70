/****************************************************************************
  HEADER FILE: CNTLEDIT.H

  DESCRIPTION: Class for CCntlEdit.
****************************************************************************/

#ifndef __CNTLEDIT_H__
#define __CNTLEDIT_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

class CCntlEdit : public CEdit
{
DECLARE_DYNAMIC(CCntlEdit)
protected:
	int	m_nCharCase;
	CString m_ErrMsg;
	CString m_ValidChars;
	BOOL m_bValid;
	
private:
	BOOL IsCharVisible(UINT nChar);

protected:
    virtual BOOL IsCharOk(UINT nChar);
	void AdjustCharCase(UINT &nChar);
	void SetValid(BOOL Valid = TRUE) { m_bValid = Valid; }

public:
	CCntlEdit();
	CCntlEdit(CWnd *Parent, unsigned int ID, LPCTSTR ValidChars = EMPTYSTRING, 
				int CharCase = UPPERCASE, UINT nLimitText = 0, LPCTSTR ErrMsg = "Invalid Data") 
				{ Setup(Parent, ID, ValidChars, CharCase, nLimitText, ErrMsg); }
	void Setup(CWnd *Parent, unsigned int ID, LPCTSTR ValidChars = EMPTYSTRING, 
				int CharCase = UPPERCASE, UINT nLimitText = 0, LPCTSTR ErrMsg = "Invalid Data");
	void SetCharCase(int CharCase = UPPERCASE) { m_nCharCase = CharCase; }
	int  GetCharCase() { return m_nCharCase; }
	void SetWindowText(LPCTSTR lpText);
	virtual BOOL IsValid(LPCTSTR lpText);
	virtual BOOL ProcessData();
	CString &GetErrMsg() { return m_ErrMsg; }
	void SetErrMsg(LPCTSTR ErrMsg = "Invalid Data") { m_ErrMsg = ErrMsg; }
	void SetValidChars(LPCTSTR ValidChars) { m_ValidChars = ValidChars; }
	CString &GetValidChars() { return m_ValidChars; }
	BOOL GetValid() { return m_bValid; }
	BOOL PreTranslateMessage(MSG *pMsg);
    
protected:   
	// Generated message map functions
	//{{AFX_MSG(CCntlEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()
};

#endif // __CNTLEDIT_H__