#ifndef __WNDCTRL_H__
#define __WNDCTRL_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __MASKIDEDIT_H__
#include "maskidedit.h"
#endif 

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __MASKMINTIMEEDIT_H__
#include "maskmintimeedit.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __HLBOX_H__
#include "hlbox.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

#ifndef __GROUP_H__
#include "group.h"
#endif

#ifndef __MCCOMBOBOX_H__
#include "mccombobox.h"
#endif

class CWndCtrl : public CObject
{
DECLARE_DYNAMIC(CWndCtrl)

protected:
	int m_nType;
	CString m_Text;
	CString *m_pData;
	CWnd *m_WndCtrl;

protected:
	CString &GetWndData(CString &Data, BOOL bFind = FALSE);

public:
	CWndCtrl() { m_nType = 0; m_WndCtrl = NULL; m_pData = NULL; }
	CWndCtrl(CWndCtrl &WndCtrl, CString *pData = NULL) 
			{ SetType(WndCtrl.m_nType); m_WndCtrl = WndCtrl.m_WndCtrl; m_pData = pData; }
	CWndCtrl(CCntlEdit *Edit, CString *pData = NULL) { Setup(Edit, pData); }
	CWndCtrl(CNumEdit *NumEdit, CString *pData = NULL) { Setup(NumEdit, pData); }
	CWndCtrl(CMaskIDEdit *IDEdit, CString *pData = NULL) { Setup(IDEdit, pData); }
	CWndCtrl(CMaskDateEdit *DateEdit, CString *pData = NULL) { Setup(DateEdit, pData); }
	CWndCtrl(CMaskMinTimeEdit *MinTimeEdit, CString *pData = NULL) { Setup(MinTimeEdit, pData); }
	CWndCtrl(COptComboBox *OptComboBox, CString *pData = NULL) { Setup(OptComboBox, pData); }
	CWndCtrl(CHLBox *HLBox, CString *pData = NULL) { Setup(HLBox, pData); }
	CWndCtrl(CCheckBox *CheckBox, CString *pData = NULL) { Setup(CheckBox, pData); }
	CWndCtrl(CGroupButton *GroupButton, CString *pData = NULL) { Setup(GroupButton, pData); }
	CWndCtrl(CMCComboBox *MCComboBox, CString *pData = NULL) { Setup(MCComboBox, pData); }
	CWndCtrl(CString *String) { Setup(String); }
	~CWndCtrl() {};

	void Setup(CCntlEdit *Edit, CString *pData = NULL)
			{ SetType(WNDCTRL_EDIT); m_WndCtrl = Edit; m_pData = pData; }
	void Setup(CNumEdit *NumEdit, CString *pData = NULL)
			{ SetType(WNDCTRL_NUMEDIT); m_WndCtrl = NumEdit;  m_pData = pData; }
	void Setup(CMaskIDEdit *IDEdit, CString *pData = NULL)
			{ SetType(WNDCTRL_IDEDIT); m_WndCtrl = IDEdit;  m_pData = pData; }
	void Setup(CMaskDateEdit *DateEdit, CString *pData = NULL)
			{ SetType(WNDCTRL_MASKDATEEDIT); m_WndCtrl = DateEdit;  m_pData = pData; }
	void Setup(CMaskMinTimeEdit *MinTimeEdit, CString *pData = NULL)
			{ SetType(WNDCTRL_MASKMINTIMEEDIT); m_WndCtrl = MinTimeEdit;  m_pData = pData; }
	void Setup(COptComboBox *OptComboBox, CString *pData = NULL) 
			{ SetType(WNDCTRL_COMBOBOX); m_WndCtrl = OptComboBox;  m_pData = pData; }
	void Setup(CHLBox *HLBox, CString *pData = NULL)
			{ SetType(WNDCTRL_LISTBOX); m_WndCtrl = HLBox;  m_pData = pData; }
	void Setup(CCheckBox *CheckBox, CString *pData = NULL)
			{ SetType(WNDCTRL_CHECKBOX); m_WndCtrl = CheckBox;  m_pData = pData; }
	void Setup(CGroupButton *GroupButton, CString *pData = NULL) 
			{ SetType(WNDCTRL_GROUP); m_WndCtrl = GroupButton;  m_pData = pData; }
	void Setup(CMCComboBox *MCComboBox, CString *pData = NULL) 
			{ SetType(WNDCTRL_MCCOMBOBOX); m_WndCtrl = MCComboBox;  m_pData = pData; }
	void Setup(CString *String) 
			{ SetType(WNDCTRL_STRING); m_WndCtrl = (CWnd*)String;  m_pData = NULL; }
	
	void SetType(int nType) { m_nType = nType; }
	int GetType() { return m_nType; }
	CWnd *GetWnd() { return m_WndCtrl; }

	LPCTSTR GetText() { return (LPCTSTR) m_Text; }
	LPCTSTR GetData(BOOL bFind = FALSE);

	void SetText(LPCTSTR Text) { m_Text = Text; }
	void SetData(LPCTSTR Data);
	void SetData() { SetData(GetText()); }
	void ShowWindow(int nCmdShow);
	void EnableWindow(BOOL bEnable);
	BOOL GetModify();
};



class CWndCtrlArray : public CObArray
{
DECLARE_DYNAMIC(CWndCtrlArray)
protected:
	CDBBase m_Base;

public:
	CWndCtrlArray() { m_Base.SetID(EMPTYSTRING); }
	CWndCtrlArray(LPCTSTR ID) { m_Base.SetID(ID); }
	CWndCtrlArray(CWndCtrlArray &WndCtrlArray);
	~CWndCtrlArray() { }

	int Add(CWndCtrl *WndCtrl) { return CObArray::Add(WndCtrl); }
	int Add(CCntlEdit *Edit, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(Edit, pData)); }
	int Add(CNumEdit *NumEdit, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(NumEdit, pData)); }
	int Add(CMaskIDEdit *IDEdit, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(IDEdit, pData)); }
	int Add(CMaskDateEdit *DateEdit, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(DateEdit, pData)); }
	int Add(CMaskMinTimeEdit *MinTimeEdit, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(MinTimeEdit, pData)); }
	int Add(COptComboBox *OptComboBox, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(OptComboBox, pData)); }
	int Add(CHLBox *HLBox, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(HLBox, pData)); }
	int Add(CCheckBox *CheckBox, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(CheckBox, pData)); }
	int Add(CGroupButton *GroupButton, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(GroupButton, pData)); }
	int Add(CMCComboBox *MCComboBox, CString *pData = NULL) 
			{ return CObArray::Add(new CWndCtrl(MCComboBox, pData)); }
	int Add(CString *String) { return CObArray::Add(new CWndCtrl(String)); }
	
	void SetAt(int Index, CWndCtrl *WndCtrl);
	void SetAt(int Index, CCntlEdit *Edit, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(Edit, pData)); }
	void SetAt(int Index, CNumEdit *NumEdit, CString *pData = NULL)  
				{ SetAt(Index, new CWndCtrl(NumEdit, pData)); }
	void SetAt(int Index, CMaskIDEdit *IDEdit, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(IDEdit, pData)); }
	void SetAt(int Index, CMaskDateEdit *DateEdit, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(DateEdit, pData)); }
	void SetAt(int Index, CMaskMinTimeEdit *MinEdit, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(MinEdit, pData)); }
	void SetAt(int Index, COptComboBox *OptComboBox, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(OptComboBox, pData)); }
	int SetAt(int Index, CHLBox *HLBox, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(HLBox, pData)); }
	int SetAt(int Index, CCheckBox *CheckBox, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(CheckBox, pData)); }
	int SetAt(int Index, CGroupButton *GroupButton, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(GroupButton, pData)); }
	int SetAt(int Index, CMCComboBox *MCComboBox, CString *pData = NULL) 
				{ SetAt(Index, new CWndCtrl(MCComboBox, pData)); }
	int SetAt(int Index, CString *String) { SetAt(Index, new CWndCtrl(String)); }

	void InsertAt(int Index, CWndCtrl *WndCtrl);
	void InsertAt(int Index, CCntlEdit *Edit, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(Edit, pData)); }
	void InsertAt(int Index, CNumEdit *NumEdit, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(NumEdit, pData)); }
	void InsertAt(int Index, CMaskIDEdit *IDEdit, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(IDEdit, pData)); }
	void InsertAt(int Index, CMaskDateEdit *DateEdit, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(DateEdit, pData)); }
	void InsertAt(int Index, CMaskMinTimeEdit *MinEdit, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(MinEdit, pData)); }
	void InsertAt(int Index, COptComboBox *OptComboBox, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(OptComboBox, pData)); }
	void InsertAt(int Index, CHLBox *HLBox, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(HLBox, pData)); }
	void InsertAt(int Index, CCheckBox *CheckBox, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(CheckBox, pData)); }
	void InsertAt(int Index, CGroupButton *GroupButton, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(GroupButton, pData)); }
	void InsertAt(int Index, CMCComboBox *MCComboBox, CString *pData = NULL) 
					{ InsertAt(Index, new CWndCtrl(MCComboBox, pData)); }
	void InsertAt(int Index, CString *String) { InsertAt(Index, new CWndCtrl(String)); }

	void RemoveAt(int Index);
	void RemoveAll();
	void Refresh();
	
	CWndCtrl *GetAt(int Index);

	CDBBase &GetBase() { return m_Base; }
	CString &GetID() { return GetBase().GetID(); }
	BOOL GetInDatabase() { return GetBase().GetInDatabase(); }
	BOOL GetDeleted() { return GetBase().GetDeleted(); }
	BOOL GetModify(BOOL bFromSource = TRUE);

	void SetID(LPCTSTR ID) { GetBase().SetID(ID); }
	void SetInDatabase(BOOL bInDatabase = TRUE) { GetBase().SetInDatabase(bInDatabase); }
	void SetDeleted(BOOL bDeleted = TRUE) { GetBase().SetDeleted(bDeleted); }
	void SetModify(BOOL bModify = TRUE) { GetBase().SetModify(bModify); }

	virtual void ToStringArray(CStringArray &StringArray, BOOL bFind = FALSE);
	virtual void ToDBRec(CDBRec &DBRec, BOOL bFind = FALSE);
	virtual void UpdateData(BOOL bFind = FALSE);
	virtual void UpdateWndCtrl();
};
#endif