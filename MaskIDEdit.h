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
	
	void SetData(const CString ID) { CMaskEdit::SetData(ID); }
	void SetData(unsigned int ID) { CString sID; sID.Format("%d", ID); CMaskEdit::SetData(sID); }
	virtual const CString GetData();
	virtual unsigned int GetID();
	virtual BOOL ValidData();
};

#endif
