#ifndef __MASKMINTIMEEDIT_H__
#define __MASKMINTIMEEDIT_H__

#ifndef __MASKEDIT_H__
#include "maskedit.h"
#endif

class CMaskMinTimeEdit : public CMaskEdit
{
DECLARE_DYNCREATE(CMaskMinTimeEdit)

protected:
	COleDateTime m_Time;

	BOOL ConvertTime(const CString Time, int &hh, int &mm);

public:
	CMaskMinTimeEdit() {};
	CMaskMinTimeEdit(CWnd *Parent, unsigned int ID, BOOL bFindModeOn = TRUE, 
					const CString ErrMsg = "Invalid Data"); 
	
	virtual ~CMaskMinTimeEdit() {}

	virtual COleDateTime &GetTime();

	void Setup(CWnd *Parent, unsigned int ID, BOOL bFindMode = TRUE, 
				const CString ErrMsg = "Invalid Data");
	void SetProperty(BOOL bFindMode = TRUE, const CString ErrMsg = "Invalid Data");

	virtual BOOL ValidData();
};

#endif
