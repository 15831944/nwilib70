// ==========================================================================
// 					Class Specification : COXMultiComboBox
// ==========================================================================

// Header file : OXMultiComboBox.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (But in the case of no CBS_HASSTRINGS user has to handle DrawItem,CompareItem)
//	YES	Derived from CComboBox

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create() or Dialogtemplate and Subclass the window)
//	YES	Has a message map
//	NO	Needs a resource 

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This control implements a combo box with more than one column. Only one
//	 column will be transferred to the edit (or static) control. 
//	The other columns are shown in the dropdown listbox, but this
//	 is only extra information by which the user can exactly choose 
//	 the item he wants. This control also exists in the MS Access 
//	 product.


// Prerequisites (necessary conditions):
//
// The ComboBox should be created as either CBS_OWNERDRAWFIXED. If you want 
// variable height rows you can use CBS_OWNERDRAWVARIABLE. but you have to handle
// the MeasureItem . If you don't specify CBS_HASSTRINGS, have to handle 
// DrawItem,CompareItem yourself and maintain the each columns data in
// in an item yourself.
//
// If the COXMultiComboBox is used as a control in dialog box then set the 
// above said(ownerdrawn) styles in dialog templates. In OnInitDialog you to
// subclass the window handle of the combo to the COXMultiComboBox.
/////////////////////////////////////////////////////////////////////////////

#ifndef __MCCOMBOBOX_H__
#define __MCCOMBOBOX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// #include "OXDllExt.h"
// #include "OXMainRes.h"

#ifndef __OXMULTICOMBOBOX_H__
#include "OXMULTIComboBox.h"
#endif

class CMCComboBox : public COXMultiComboBox
{
	DECLARE_DYNAMIC(CMCComboBox)

public:
	CMCComboBox();
	~CMCComboBox() {}

	BOOL Setup(CWnd *wnd, UINT itemid, int ColumnCount = 2, BOOL WithNone = FALSE, BOOL bAddable = FALSE);

	virtual BOOL Refresh(LPCTSTR Val);
	int AddStringNone();
	void SetAdd(BOOL bAdd) { m_bAdd = bAdd; }
	BOOL GetAdd() { return m_bAdd; }

protected:
	BOOL m_bAdd;
};
#endif // MCCOMBOBOX_H
/////////////////////////////////////////////////////////////////////////////
