#include "stdafx.h"
#include "hlbox.h"
#include "nwivar.h"

IMPLEMENT_DYNAMIC(CHLBox, CListBox)

BEGIN_MESSAGE_MAP(CHLBox, CListBox)
	//{{AFX_MSG_MAP(CHLBox)
	ON_WM_RBUTTONDOWN()
	ON_WM_MENUSELECT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CHLBox::CHLBox(int nHMaxExtent, UINT MenuID /* = 0 */, BOOL WithNone /* = FASLE */)
{
	// Get the width of the listbox
	m_bWithNone = WithNone;
	m_nMaxHExtent = nHMaxExtent;
	m_nOrigMaxHExtent = nHMaxExtent;
	AddMenu(MenuID);
}

CHLBox::CHLBox(CListBox *ListBox, UINT MenuID /* = 0 */, BOOL WithNone /* = FASLE */)
{
    RECT Rect;
    	
	m_bWithNone = WithNone;
    ListBox->GetClientRect(&Rect);
	m_nMaxHExtent = Rect.right;
	m_nOrigMaxHExtent = Rect.right;
	AddMenu(MenuID);
//   SubclassDlgItem(ListBox->GetDlgCtrlID(), ListBox->GetParent());
}

CHLBox::CHLBox(CWnd *Parent, int ID, UINT MenuID /* = 0 */, BOOL WithNone /* = FASLE */)
{
    RECT      Rect;
    CListBox* ListBox;
    
	m_bWithNone = WithNone;
    ListBox = (CListBox*) Parent->GetDlgItem(ID);
    ListBox->GetClientRect(&Rect);
	m_nMaxHExtent = Rect.right;
	m_nOrigMaxHExtent = Rect.right;
	AddMenu(MenuID);
	if(Parent && ID > 0)
		CListBox::SubclassDlgItem(ID, Parent);
}

CHLBox::~CHLBox()
{
	if(m_Menu.GetSafeHmenu())
		m_Menu.DestroyMenu();
}

void CHLBox::AddMenu(UINT MenuID /* = 0 */)
{
	if(MenuID)
		m_Menu.LoadMenu(MenuID);
}

int CHLBox::GetItemHExtent(LPCTSTR lpszItem)
// Get the text extent
{
	CDC *pCdc;
	int TextHExtent; 
   
	pCdc = (CDC*) new CClientDC(this);
	TextHExtent = pCdc->GetTextExtent(lpszItem, lstrlen(lpszItem)).cx;
	delete pCdc;
   
	return(TextHExtent);
}
   
void CHLBox::ResetLBHExtent()
// Reset the horizontal extent of the listbox to its original width
{
	m_nMaxHExtent = m_nOrigMaxHExtent;
	SetHorizontalExtent(m_nMaxHExtent);
}

void CHLBox::SetLBHExtent(int HExtent)
// Set the horizontal extent if necessary
{
	if(HExtent > m_nMaxHExtent)
	{	
		m_nMaxHExtent = HExtent;
		SetHorizontalExtent(m_nMaxHExtent);
	}  
}
   
int CHLBox::AddString(LPCTSTR String)
// Add a string to the listbox
{
	SetLBHExtent(GetItemHExtent(String));
	return(CListBox::AddString(String));
}

int CHLBox::InsertString(int nIndex, LPCTSTR String)
// Insert a string to the listbox
{
	SetLBHExtent(GetItemHExtent(String));
	return(CListBox::InsertString(nIndex, String));
}

void CHLBox::ResetContent()
// Clear the content of the listbox
{
	ResetLBHExtent();
	CListBox::ResetContent();
}

void CHLBox::OnRButtonDown(UINT nFlags, CPoint point)
// Display a menu at the clicked point if it is existed
{
	if(m_Menu.GetSafeHmenu() != NULL)
	{
		// If a popup menu is defined, then put up the popupmenu.
		// Note that the messages are sent to the parent 
		ClientToScreen(&point);
		m_Menu.GetSubMenu(0)->TrackPopupMenu(nFlags, point.x, point.y, this, NULL);
	}
}

void CHLBox::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
// Make sure the message is sent
{
	unsigned int i;	
	   
	for(i = 0; i < (UINT) m_Menu.GetSubMenu(0)->GetMenuItemCount(); i ++)
	{
		if(m_Menu.GetSubMenu(0)->GetMenuItemID(i) == nItemID)
		{
			GetParent()->SendMessage(WM_USER + nItemID % 32767, nItemID, GetDlgCtrlID());
			break;
		}
	}
   
	CListBox::OnMenuSelect(nItemID, nFlags, hSysMenu);
}

void CHLBox::OnKillFocus(CWnd *pNewWnd)
{
	CListBox::OnKillFocus(pNewWnd);
	SetCurSel();
}

CString &CHLBox::GetSelString(CString &Buffer)
{
	int Index;
	
	Buffer.Empty();
	Index = CListBox::GetCurSel();
	if(Index >= 0)
	{
		CListBox::GetText(Index, Buffer);
		if(m_bWithNone && Buffer == NONE)
			Buffer.Empty();
	}

	return Buffer;
}

int CHLBox::SetCurSel()
{
	if(m_bWithNone)
	{
		int		CurSel;
		CString	Buffer;
		Buffer.Empty();
		CurSel = CListBox::GetCurSel();
		if(CurSel >= 0)
			CListBox::GetText(CurSel, Buffer);

		if(m_bWithNone && Buffer == NONE)
	   		CListBox::SetCurSel(-1);
	}

	return CListBox::GetCurSel();
}

int CHLBox::SetCurSel(int Index)
{
	if(Index == -1)
		return CListBox::SetCurSel(Index);

	CString	Buffer;
	if(Index >= 0 && Index < CListBox::GetCount())
	{
		if(m_bWithNone)
		{
			CListBox::GetText(Index, Buffer);
			if(Buffer == NONE)
				Index = -1;
		}

	   	return CListBox::SetCurSel(Index);
	}

	return LB_ERR;
}

int CHLBox::FindString(int nStartAfter, LPCTSTR Key)
{
	if(Key && strlen(Key) > 0)
	{
		if(m_bWithNone && strcmp(Key, NONE) == 0)
			return LB_ERR;
		
		return CListBox::FindString(nStartAfter, Key);
	}
	
	return LB_ERR;
}

int CHLBox::SelectString(int nStartAfter, LPCTSTR Key)
{
	if(Key && strlen(Key) > 0)
	{
		if(m_bWithNone && strcmp(Key, NONE) == 0)
			return SetCurSel(-1);
		
		return CListBox::SelectString(nStartAfter, Key);
	}
	
	return SetCurSel(-1);
}

int CHLBox::EditFind(CWnd &Wnd)
{
	CString Buffer;
	int		Index;
	
	Wnd.GetWindowText(Buffer);
	Index = FindString(-1, Buffer);
	if(Index != -1)
		SetCurSel(Index);
	
	return GetCurSel();
}

int CHLBox::EditSelect(CWnd &Wnd)
{
	CString Buffer;
	int		Index;
	
	Index = GetCurSel();
	if(Index != -1)
	{
		GetText(Index, Buffer);
		Wnd.SetWindowText(Buffer);
	}
	
	return Index;
}
