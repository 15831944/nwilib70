#ifndef __ORAFIELD_H__
#define __ORAFIELD_H__

class COraField : public CObject
{
DECLARE_DYNAMIC(COraField)
protected:
	CString	m_Name;	
	short m_nType;
	int	m_nScale;
	int	m_nPrecision;
	int m_nSize;
	BOOL m_bVisible;
	BOOL m_bWithComma;
    
public:
	COraField();
	COraField(COraField &Field);
	COraField(LPCTSTR Name, short Type, int Size, int Scale, int Precision, 
				BOOL Visible = TRUE, BOOL WithComma = FALSE);
	~COraField() {}

	void Setup(LPCTSTR Name, short Type, int Size, int Scale, int Precision, 
				BOOL Visible = TRUE, BOOL WithComma = FALSE);
	LPCTSTR GetName() { return (LPCTSTR) m_Name; }
	short GetType() { return m_nType; }
	int GetSize() { return m_nSize; }
	int GetScale() { return m_nScale; }
	int GetPrecision() { return m_nPrecision; }
	BOOL GetVisible() { return m_bVisible; }
	BOOL WithComma() { return m_bWithComma; }

	void SetName(LPCTSTR Name);
	void SetType(short Type) { m_nType = Type; }
	void SetSize(long Size) { m_nSize = Size; }
	void SetScale(int Scale) { m_nScale = Scale; }
	void SetPrecision(int Precision) { m_nPrecision = Precision; }
	void SetVisible(BOOL Visible = TRUE) { m_bVisible = Visible; }
	void SetWithComma(BOOL bWithComma = TRUE) { m_bWithComma = bWithComma; }
};



class CFieldArray : public CObArray
{
DECLARE_DYNAMIC(CFieldArray)
public:
	CFieldArray() {};
	CFieldArray(CFieldArray &FieldArray) { Copy(FieldArray); }
	~CFieldArray() { RemoveAll(); }
	
	int Add(COraField &Field);
	int	FindAt(LPCTSTR FieldName);
	COraField *GetAt(int i);
	COraField *GetAt(LPCTSTR FieldName);
	void InsertAt(int Index, COraField &Field);
	void RemoveAll();
	void RemoveAt(int Index);
	void SetAt(int Index, COraField &Field);
	void Copy(CFieldArray &FieldArray);
	const CFieldArray &operator = (CFieldArray &FieldArray) { Copy(FieldArray); return *this; }
};
#endif // __ORAFIELD_H__

