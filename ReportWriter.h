#ifndef __REPORTWRITER_H__
#define __REPORTWRITER_H__

class CReportWriter : public CObject
{
DECLARE_DYNAMIC(CReportWriter)
protected:
	CStdioFile m_File;
	int m_MaxLines;
	int m_Lines;
	int m_Pages;
	BOOL m_bBFlag; // Break Flag
	BOOL m_bTFlag; // Total Flag
	BOOL m_bSFlag; // SubTotal Flag
	BOOL m_bAFlag; // Condition Flag
	CString m_LineFmt;
	CString m_STotalFmt;
	CString m_TotalFmt;
	CString m_BreakFmt;
	CString m_Date;
	CString m_Portfolio;

public:
	CReportWriter();
	CReportWriter(LPCTSTR FileName, LPCTSTR LineFmt, LPCTSTR STotalFmt, LPCTSTR TotalFmt, 
					LPCTSTR BreakFmt = NULL) 
				{ SetupReportWriter(FileName, LineFmt, STotalFmt, TotalFmt, BreakFmt); }
	BOOL SetupReportWriter(LPCTSTR FileName, LPCTSTR LineFmt, LPCTSTR STotalFmt, LPCTSTR TotalFmt, 
					LPCTSTR BreakFmt = NULL);
	~CReportWriter() { m_File.Close(); }

public:
	int GetLines() { return m_Lines; }
	void SetLines(int Lines) { m_Lines = Lines; }
	int GetMaxLines() { return m_MaxLines; }
	void SetMaxLines(int MaxLines) { m_MaxLines = MaxLines; }

	int GetPages() { return m_Pages; }
	void SetPages(int Pages) { m_Pages = Pages; }
	
	BOOL GetBFlag() { return m_bBFlag; }
	void SetBFlag(BOOL bBFlag = TRUE) { m_bBFlag = bBFlag; }
	BOOL GetTFlag() { return m_bTFlag; }
	void SetTFlag(BOOL bTFlag = TRUE) { m_bTFlag = bTFlag; }
	BOOL GetSFlag() { return m_bSFlag; }
	void SetSFlag(BOOL bSFlag = TRUE) { m_bSFlag = bSFlag; }
	BOOL GetAFlag() { return m_bAFlag; }
	void SetAFlag(BOOL bAFlag = TRUE) { m_bAFlag = bAFlag; }
	
	CString &GetLineFmt() { return m_LineFmt; }
	void SetLineFmt(LPCTSTR LineFmt) { m_LineFmt = LineFmt; }
	CString &GetSTotalFmt() { return m_STotalFmt; }
	void SetSTotalFmt(LPCTSTR STotalFmt) { m_STotalFmt = STotalFmt; }
	CString &GetTotalFmt() { return m_TotalFmt; }
	void SetTotalFmt(LPCTSTR TotalFmt) { m_TotalFmt = TotalFmt; }
	CString &GetBreakFmt() { return m_BreakFmt; }
	void SetBreakFmt(LPCTSTR BreakFmt) { m_BreakFmt = BreakFmt; }

	CString &GetDate() { return m_Date; }
	void SetDate(LPCTSTR Date) { m_Date = Date; }
	CString &GetPortfolio() { return m_Portfolio; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = m_Portfolio; }

	BOOL WritePage();
	void WritePageHeading(LPCTSTR Date, LPCTSTR Portfolio) { SetDate(Date); SetPortfolio(Portfolio); };
	void WriteLine(CStringArray &Data) {}
	void WriteBreakLine(CStringArray &Data) {}
	void WriteSTotalLine(CStringArray &Data) {}
	void WriteTotalLine(CStringArray &Data) {}
};
#endif 