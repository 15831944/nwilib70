#ifndef __DBRECARRAY_H__
#define __DBRECARRAY_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __HLBOX_H__
#include "hlbox.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __MCCOMBOBOX_H__
#include "mccombobox.h"
#endif

class CDBRecArray : public CObArray
{
DECLARE_DYNAMIC(CDBRecArray)
public:
	BOOL m_bModified;

	CDBRecArray() { m_bModified = FALSE; };
	CDBRecArray(CDBRecArray &RecArray) { m_bModified = FALSE; Copy(RecArray); }
	~CDBRecArray() { RemoveAll(); }

	int	Add(CDBRec *Rec);
	void SetAt(int Index, CDBRec *Rec);
	void InsertAt(int Index, CDBRec *Rec);
	int Add(CDBRec &Rec);
	void SetAt(int Index, CDBRec &Rec);
	void InsertAt(int Index, CDBRec &Rec);
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CDBRecArray &DBRecArray);

	CDBRec *GetAt(int Index);
	CDBRec *GetAt(LPCTSTR ID) { return GetAt(Find(ID)); }
	int FindListed(int Index);
	CDBRec *GetListedAt(int Index);

	void Modified(BOOL bModified = TRUE) { m_bModified = bModified; }
	
	BOOL IsModified() { return m_bModified; }
	int Find(LPCTSTR ID, int FieldID = -1);	
	const CDBRecArray &operator = (CDBRecArray &DBRecArray) { Copy(DBRecArray); return *this; }
    int CopyToComboBox(COptComboBox &CBox, int Col = 0);
	int CopyToMCComboBox(CMCComboBox &CBox, int KeyCol = 0);
    int CopyToListBox(CHLBox &HLBox, int Col = 0);	
};
#endif // __DBRECARRAY_H__