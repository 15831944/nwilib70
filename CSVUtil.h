#ifndef __CSVUTIL_H__
#define __CSVUTIL_H__

#ifndef __NWIVAR_H__
#include "nwivar.h"
#endif

class CCSVUtil
{
protected:
	BOOL m_bRead;
	int m_nRowCount;
	CStdioFile m_File;
	CStringArray m_CSVHeaders;
	CStringArray m_CSVData;

public:
	CCSVUtil() {}
	~CCSVUtil() { m_File.Close(); }

public: // Data Interfaces
	BOOL Setup(const CString FileName, BOOL bRead = TRUE);
	BOOL ReadCSVHeaders(int &nHeaderRow);
	BOOL ReadCSVData();
	BOOL WriteCSVHeader(const CString Data, BOOL bWithQuote);
	BOOL WriteCSVData(const CString Data, BOOL bWithQuote);
	BOOL WriteCSVLine(BOOL bHeader = FALSE);
	CStringArray &GetCSVHeaders() { return m_CSVHeaders; }
	CStringArray &GetCSVData() { return m_CSVData; }
	CString GetCSVNumber(const CString SrcNum);

protected:
	BOOL ReadCSV(CStringArray &DataArray, int &nRow);
	BOOL WriteCSV(CStringArray &DataArray, const CString Data, BOOL bWithQuote);
};
#endif // __CSVUtil_H__