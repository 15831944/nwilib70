#ifndef __MASKIDEDIT_H__
#define __MASKIDEDIT_H__

#ifndef __MASKEDIT_H__
#include "maskedit.h"
#endif

class CMaskIDEdit : public CMaskEdit
{
DECLARE_DYNCREATE(CMaskIDEdit)

protected:
	
public:
	CMaskIDEdit() {};
	CMaskIDEdit(CWnd *Parent, unsigned int ID, int Length = 6, BOOL bFindMode = TRUE, 
				LPCTSTR ErrMsg = "Invalid Data"); 
	
	virtual ~CMaskIDEdit() {}
	void Setup(CWnd *Parent, unsigned int ID, int Length = 6, BOOL bFindMode = TRUE, 
				LPCTSTR ErrMsg = "Invalid Data");
	virtual CString GetData();
	virtual BOOL ValidData();
};

#endif
