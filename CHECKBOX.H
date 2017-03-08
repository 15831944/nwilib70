#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

class CCheckBox : public CButton
{
DECLARE_DYNAMIC(CCheckBox)
protected:
	CString m_CheckString;
	CString m_UnCheckString;
	BOOL m_bReadOnly;

public:
	CCheckBox();
	CCheckBox(CWnd *Parent, unsigned int ID, LPCTSTR CheckString = Y, LPCTSTR UnCheckString = EMPTYSTRING);
	~CCheckBox() {}

	LPCTSTR GetCheckString();
	void SetCheck(LPCTSTR TextString);
	void SetCheck(int nCheck = 1);
	void SetStrings(LPCTSTR CheckString = Y, LPCTSTR UnCheckString = EMPTYSTRING);

	void SetReadOnly(BOOL bReadOnly = TRUE) { m_bReadOnly = bReadOnly; }
	BOOL GetReadOnly() { return m_bReadOnly; } 

protected:
	// {{ AFX_MSG(CBindCheckBox)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nReCnt, UINT nFlags);
	// }} AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // __CHECKBOX_H__