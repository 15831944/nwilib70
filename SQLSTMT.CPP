#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include "sqlstmt.h"


BOOL CSqlStmt::LoadString(UINT SQLID)
{
	return CString::LoadString(SQLID);
}

BOOL CSqlStmt::LoadString(unsigned ID1, unsigned ID2)
{
	unsigned i;
	CString	Buffer;

	Empty();
	LoadString(ID1);
	for(i = ID1 + 1; i <= ID2; i++)
	{
		Buffer.LoadString(i);
	    if(!Buffer.IsEmpty())
			*this += Buffer;
	}

	return this->IsEmpty();
}

const CSqlStmt &CSqlStmt::operator =(const CSqlStmt &Src)
{
	CString::SetString(Src);
	return *this;
}

const CSqlStmt &CSqlStmt::operator =(LPCTSTR psz)
{
	CString::SetString(psz);
	return *this;
}

const CSqlStmt &CSqlStmt::operator+=(const CSqlStmt &string)
{
	CString Buffer = string;

	CString::SetString(CString::GetString() + Buffer);
	return *this;
}
	
const CSqlStmt &CSqlStmt::operator+=(const CString &string)
{
	CString Buffer = string;

	CString::SetString(CString::GetString() + Buffer);
	return *this;
}

const CSqlStmt &CSqlStmt::operator+=(LPCTSTR psz)
{
	CString Buffer = psz;

	CString::SetString(CString::GetString() + Buffer);
	return *this;
}

CSqlStmt::operator LPCTSTR() const
{
	return (LPCTSTR) CString::GetString();
}
