#ifndef __SORTARRAY_H__
#define __SORTARRAY_H__


class CSortArray : public CObArray
{
DECLARE_DYNAMIC(CSortArray)

protected:
	BOOL m_bSortable;

protected:	
	int Search(CObject *Object);
	int BSearch(CObject *Object);
	int Order(CObject *Object);
	virtual int Compare(CObject *Obj1, CObject *Obj2) = 0;

public:	
	CSortArray(BOOL bSortable = TRUE) { SetSortable(bSortable); }
	~CSortArray() { RemoveAll(); }

	void SetSortable(BOOL bSortable = TRUE) { if(GetSize() == 0) m_bSortable = bSortable; }
	BOOL GetSortable() { return m_bSortable; }

	int	Add(CObject *Object);
};
#endif // __SQRTARRAY_H__