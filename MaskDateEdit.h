#ifndef __MASKDATEEDIT_H__
#define __MASKDATEEDIT_H__

#ifndef __MASKEDIT_H__
#include "maskedit.h"
#endif

class CMaskDateEdit : public CMaskEdit
{
DECLARE_DYNCREATE(CMaskDateEdit)

protected:
	CMaskDateEdit *m_pCompareDate;
	CString m_CompareDate;
	
	BOOL ConvertDate(CString Date, int &Year, int &Month, int &Day);
	
public:
	CMaskDateEdit() {};
	CMaskDateEdit(CWnd *Parent, unsigned int ID, BOOL bFindMode = TRUE, 
					LPCTSTR ErrMsg = "Invalid Data", CMaskDateEdit *pCompareDate = NULL); 
	
	virtual ~CMaskDateEdit() {}
	
	void Setup(CWnd *Parent, unsigned int ID, BOOL bFindMode = TRUE, 
				LPCTSTR ErrMsg = "Invalid Data", CMaskDateEdit *pCompareDate = NULL); 
	void SetCompareDate(CMaskDateEdit *pCompareDate) { m_pCompareDate = pCompareDate; }
	void SetCompareDate(CString CompareDate) { m_CompareDate = CompareDate; }
	virtual BOOL ValidData();
};

#endif
