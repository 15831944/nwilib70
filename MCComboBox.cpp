// ==========================================================================
//				Class Implementation : CMCComboBox
// ==========================================================================

// Source file : MCComboBox.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MCComboBox.h"
#include "qdata.h"

// #include "UTBStrOp.h"
// #include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMCComboBox, COXMultiComboBox)


CMCComboBox::CMCComboBox()
{
}

BOOL CMCComboBox::Setup(CWnd *Wnd, UINT ItemID, int nColumnCount, BOOL bWithNone, BOOL bAdd)
{
    if (!Wnd->GetDlgItem(ItemID))
    { // couldn't get textedit
        return FALSE;
    }
   
	SubclassDlgItem(ItemID, Wnd);
	COXMultiComboBox::SetColumnCount(nColumnCount);
	COXMultiComboBox::SetWithNone(bWithNone);
	SetAdd(bAdd);
	return TRUE;
}

BOOL CMCComboBox::Refresh(LPCTSTR Value)
{
	COXMultiComboBox::SelectString(0, Value);
	return TRUE;
}

int CMCComboBox::AddStringNone()
{
	int i;
	CStringArray StringArray;

	for(i = 0; i < GetColumnCount(); i++)
		StringArray.Add(NONE);

	return COXMultiComboBox::AddString(StringArray);
}