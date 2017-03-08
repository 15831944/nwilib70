
// ==========================================================================
//				inline functions for class : COXMultiComboBox
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#ifndef __MCCOMBOBOX_INL__
#define __MCCOMBOBOX_INL__

//Inline member functions
//public
inline BOOL CMCComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	ASSERT((dwStyle & CBS_OWNERDRAWFIXED) | (dwStyle & CBS_OWNERDRAWVARIABLE) );
	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}

inline CMCComboBox::~CMCComboBox()
{
	m_GridPen.DeleteObject();
	m_aColumnWidth.SetSize(0);
}

inline int CMCComboBox::GetColumnCount() const 
{ 
	return m_nColumnCount; 
}

inline int CMCComboBox::GetMasterColumn() const 
{ 
	return m_nMasterColumn; 
}

inline int CMCComboBox::GetColumnWidth(int nColIndex) const 
{ 
	return ((nColIndex < 0 || nColIndex >= m_nColumnCount) ?
		CB_ERR : m_aColumnWidth[nColIndex]);
}

inline int CMCComboBox::GetLBText(int nIndex, LPTSTR lpszText) const
{ 
	return GetLBText(nIndex, m_nMasterColumn, lpszText);
}

inline void CMCComboBox::GetLBText(int nIndex, CString& rString) const
{
	GetLBText(nIndex, m_nMasterColumn, rString);
}

inline 	int CMCComboBox::GetLBTextLen(int nIndex) const
{
	return GetLBTextLen(nIndex,m_nMasterColumn);
}

inline int CMCComboBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind, 
											 BOOL bCaseSensitive/*=FALSE*/) const
{
	return FindStringExact(nIndexStart, m_nMasterColumn, lpszFind, bCaseSensitive);
}

inline int CMCComboBox::FindString(int nStartAfter, LPCTSTR lpszString, 
										BOOL bCaseSensitive/*=FALSE*/) const
{
	return FindString(nStartAfter, m_nMasterColumn, lpszString, bCaseSensitive);
}

inline int CMCComboBox::SelectString(int nStartAfter, LPCTSTR lpszString)
{
	return SelectString(nStartAfter, m_nMasterColumn, lpszString);
}

//protected
inline CMCComboBox::CRowData::CRowData(int nColCount) 
// --- In  : nColCount : Column Count
// --- Out : 
// --- Returns : 
// --- Effect : constructs the object and sets the size of string array
{ 
	ASSERT(nColCount>=0);
	ColumnString.SetSize(nColCount); //sets the string array size
	ItemData = 0;
}

inline void CMCComboBox::CRowData::SetColumnCount(int nCount)
// --- In  : nColCount : Column Count
// --- Out : 
// --- Returns : 
// --- Effect : Sets the size of string array
{
	ASSERT(nCount>=0);
	ColumnString.SetSize(nCount); 
}

inline void CMCComboBox::CRowData::SetItemData(DWORD Data)
// --- In  : Data : Item Data to set
// --- Out : 
// --- Returns : 
// --- Effect : Sets the Data to ItemData
{
	ItemData = Data;
}

inline DWORD CMCComboBox::CRowData::GetItemData()	const
// --- In  : 
// --- Out : 
// --- Returns : Item data
// --- Effect : 
{
	return ItemData;
}

inline void CMCComboBox::CRowData::SetColumnString(int nColIndex,const CString& rString)
// --- In  : nColIndex : Column Index
//			 rString : String to set
// --- Out : 
// --- Returns : 
// --- Effect : Copies the given string into nColIndex string in string array
{ 
	ASSERT(nColIndex >= 0 && nColIndex < ColumnString.GetSize()); 
	ColumnString[nColIndex] = rString; 
}

inline CString CMCComboBox::CRowData::GetColumnString(int nColIndex) const 
// --- In  : nColCount : Column Count
// --- Out : 
// --- Returns : Returns the nColIndex'th string 
// --- Effect : 
{ 
	ASSERT(nColIndex >= 0 && nColIndex<ColumnString.GetSize());
	return ColumnString[nColIndex];
}

inline void CMCComboBox::CRowData::DeleteColumnString(int nColIndex)
// --- In  : nColCount : Column Count
// --- Out : 
// --- Returns : 
// --- Effect : Removes the string at nColIndex
{
	ASSERT(nColIndex >= 0 && nColIndex<ColumnString.GetSize());
	ColumnString.RemoveAt(nColIndex);
}


inline void CMCComboBox::CComboLBox::Init(CWnd* pCombo)
// --- In  : pCombo : Combobox CWnd pointer
// --- Out : 
// --- Returns : 
// --- Effect : Initializes the combo pointer 
{
	m_pWndCombo = pCombo;
}

inline void CMCComboBox::PreSubclassWindow()
// --- In  : 
// --- Out : 
// --- Returns : 
// --- Effect : Asserts the owner draw style and calls CComboBoxs 
//				PreSubclassWindow.
{
	DWORD dwStyle = GetStyle();
	dwStyle = dwStyle; // prevent level 4 warning in release mode
	ASSERT((dwStyle & CBS_OWNERDRAWFIXED) | (dwStyle & CBS_OWNERDRAWVARIABLE) );
	CComboBox::PreSubclassWindow();
}

#endif // CMCCOMBOBOX_INL

// //////////////////////////////////////////////////////////////////////////
