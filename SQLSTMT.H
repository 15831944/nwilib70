#ifndef __SQLSTMT_H__
#define __SQLSTMT_H__

class CSqlStmt : public CString
{
public:
	CSqlStmt() { }

	BOOL LoadString(UINT SQLID);
	BOOL LoadString(unsigned ID1, unsigned ID2);

	operator const char*() const;
	// overloaded assignment
	const CSqlStmt &operator=(const CSqlStmt &stringSrc);
	const CSqlStmt &operator=(const CString &stringSrc);
	const CSqlStmt &operator=(LPCTSTR psz);

	// string concatenation
	const CSqlStmt &operator+=(const CSqlStmt &string);
	const CSqlStmt &operator+=(const CString &string);
	const CSqlStmt &operator+=(LPCTSTR psz);
};
#endif /* __SQLSTMT_H__ */
