#ifndef __MASKEDIT_H__
#define __MASKEDIT_H__

#ifndef __EDITCHAR_H__
#include "editchar.h"
#endif

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

class CMaskEdit : public CEdit
{
DECLARE_DYNCREATE(CMaskEdit)

protected:
	CString m_Prompt;
	CString m_ErrMsg;
	CString m_NULLString;
	int m_nCurrentChar;
	int m_nCharCase;
	BOOL m_bInit;
	BOOL m_bShiftDown;
	BOOL m_bWithMask;
	BOOL m_bFindMode;
	CEditCharArray m_CharArray;

	virtual BOOL Parse(const CString &StrMask);
	CString GetRange(CString szLow, CString szHigh);
	int FindNextChar(int iStartPos, BOOL bStaticChar, BOOL bForward);

public:
	CMaskEdit();
	CMaskEdit(CWnd *Parent, unsigned int ID, CString strMask, BOOL bFindMode, 
				const CString NULLString, int CharCase = UPPERCASE, 
				BOOL bWithMask = TRUE, LPCTSTR ErrMsg = "Invalid Data") 
				{ Setup(Parent, ID, strMask, bFindMode, NULLString, CharCase, bWithMask, ErrMsg); }
	~CMaskEdit();

	void Setup(CWnd *Parent, unsigned int ID, CString strMask, BOOL bFindMode, 
				const CString NULLString, int CharCase = UPPERCASE, 
				BOOL bWithMask = TRUE, LPCTSTR ErrMsg = "Invalid Data");

	BOOL Init(CString strMask);
	BOOL SetPromptChar(CString PromptChar);

	virtual BOOL ValidData();
	BOOL SetMaskChars(const CString strMask);
	virtual void SetData(const CString Data);
	virtual const CString GetData();

	int  GetCharCase() { return m_nCharCase; }
	BOOL GetWithMask() { return m_bWithMask; }
	BOOL GetFindMode() { return m_bFindMode; }
	CString &GetErrMsg() { return m_ErrMsg; }
	CString &GetNULLString() { return m_NULLString; }

	void SetCharCase(int CharCase = UPPERCASE) { m_nCharCase = CharCase; }
	void SetWithMask(BOOL bWithMask = TRUE) { m_bWithMask = bWithMask; }
	void SetNULLString(const CString NULLString) { m_NULLString = NULLString; }
	void SetFindMode(BOOL bFindMode = TRUE) { m_bFindMode = bFindMode; }
	void SetErrMsg(LPCTSTR ErrMsg = "Invalid Data") { m_ErrMsg = ErrMsg; }
	BOOL PreTranslateMessage(MSG *pMsg);

protected:
	void AdjustCharCase(UINT &nChar);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
};

#endif
