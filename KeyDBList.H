#ifndef __KEYDBLIST_H__
#define __KEYDBLIST_H__

#ifndef __DBRECARRAY_H__
#include "dbrecarray.h"
#endif

#ifndef __HLBOX_H__
#include "hlbox.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __DBSHEET_H__
#include "dbsheet.h"
#endif

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

class CKeyDBRecArray : public CDBRecArray
{
DECLARE_DYNAMIC(CKeyDBRecArray)

protected:
	CString m_Key;

public:
	CKeyDBRecArray() { };
	CKeyDBRecArray(CKeyDBRecArray &RecArray);
	~CKeyDBRecArray() { RemoveAll(); }

	LPCTSTR GetKey() { return (LPCTSTR) m_Key; }
	CString &GetKeyCtrl() { return m_Key; }
	void SetKey(LPCTSTR Key) { m_Key = Key; }
	void Copy(CKeyDBRecArray &RecArray) 
			{ SetKey(RecArray.GetKey()); CDBRecArray::Copy(RecArray); }
	const CKeyDBRecArray &operator = (CKeyDBRecArray &KeyDBRecArray) { Copy(KeyDBRecArray); return *this; }
};


class CKeyDBList : public CObArray
{
DECLARE_DYNAMIC(CKeyDBList)

protected:
	BOOL m_bModified;

public:
	CKeyDBList() { }
	CKeyDBList(CKeyDBList &KeyDBList);
	~CKeyDBList() { RemoveAll(); }

	int	Add(CKeyDBRecArray *RecArray);
	int Add(CKeyDBRecArray &RecArray);
	void SetAt(int Index, CKeyDBRecArray *RecArray);
	void SetAt(int Index, CKeyDBRecArray &RecArray);
	void InsertAt(int Index, CKeyDBRecArray *RecArray);
	void InsertAt(int Index, CKeyDBRecArray &RecArray);
	void RemoveAt(int Index);
	void RemoveAll();
	CKeyDBRecArray *GetAt(int Index);
	CKeyDBRecArray *GetAt(LPCTSTR Key);
    
	void Modified(BOOL bModified = TRUE);

	BOOL IsModified();
	int Find(LPCTSTR Key);

	int CopyDataToDBRecArray(LPCTSTR Key, CDBRecArray &DBRecArray);
	int CopyKeyToComboBox(COptComboBox &ComboBox);
	int CopyDataToComboBox(LPCTSTR Key, COptComboBox &ComboBox);
	int CopyToMCComboBox(LPCTSTR Key, CMCComboBox &ComboBox, int KeyCol = 0);
	int CopyKeyToListBox(CHLBox &ComboBox);
	int CopyDataToListBox(LPCTSTR Key, CHLBox &ComboBox);
	int CopyDataToDBSheet(LPCTSTR Key, CDBSheet &SS);
	int CopyDataToRowCtrl(LPCTSTR Key, CRowCtrl &RowCtrl);
};
#endif // __KEYDBLIST_H__