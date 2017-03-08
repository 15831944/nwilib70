#ifndef __EDITCHAR_H__
#define __EDITCHAR_H__

class CEditChar : public CObject 
{
DECLARE_DYNAMIC(CEditChar)

protected:
	CString m_strValids;
	CString m_strData;
	BOOL m_bStaticChar;

public:
	CEditChar();
	CEditChar(CString &StrValidChars);
	CEditChar(CString &StrValidChars, BOOL bStaticChar);
	CEditChar(CEditChar &EditChar);
	virtual ~CEditChar() { }

	void Setup(CString &StrValidChars, BOOL bStaticChar);
	void SetData(LPCTSTR Data) { m_strData = Data; }

	BOOL IsStaticChar() { return m_bStaticChar; }
	CString &GetData() { return m_strData; }
	CString &GetValids() { return m_strValids; }
	BOOL IsValid(CString Data);
};


class CEditCharArray : public CObArray
{
DECLARE_DYNAMIC(CEditCharArray)

public:
	CEditCharArray() {}
	~CEditCharArray();

	int	Add(CEditChar *EditChar);
	void SetAt(int Index, CEditChar *EditChar);
	void InsertAt(int Index, CEditChar *EditChar);
	int Add(CEditChar &EditChar);
	void SetAt(int Index, CEditChar &EditChar);
	void InsertAt(int Index, CEditChar &EditChar);
	void RemoveAt(int Index);
	void RemoveAll();
	void Copy(CEditCharArray &EditCharArray);
	CEditChar *GetAt(int Index);
		
	const CEditCharArray &operator=(CEditCharArray &EditCharArray) { Copy(EditCharArray); return *this; }
};
#endif
