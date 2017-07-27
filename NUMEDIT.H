/****************************************************************************
  HEADER FILE: NumEdit.H

  DESCRIPTION: Class for CNumEdit.
****************************************************************************/

#ifndef __NUMEDIT_H__
#define __NUMEDIT_H__

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

class CNumEdit : public CCntlEdit
{
DECLARE_DYNAMIC(CNumEdit)

protected:
	int m_nDigits;
	BOOL m_bWithComma;
	CString m_ValidString;

public:
	CNumEdit() {};
	CNumEdit(CWnd *Parent, UINT ID, LPCTSTR ErrMsg = "Invalid Number", int Digits = -1, 
			BOOL bWithComma = TRUE);
	void Setup(CWnd *Parent, UINT ID, LPCTSTR ErrMsg = "Invalid Number", int Digits = -1, 
				BOOL bWithComma = TRUE);

	virtual BOOL ProcessData();
	int GetDigits() { return m_nDigits; }
	void SetDigits(int nDigits) { m_nDigits = nDigits; }
	BOOL WithComma() { return m_bWithComma; }
	void SetWithComma(BOOL bWithComma);
	void SetData(const CString Num);
	void SetData(double Num);
	void SetData(int Num);

	virtual const CString GetData();
};

#endif // __NUMEDIT_H__
