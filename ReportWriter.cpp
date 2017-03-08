#include "stdafx.h"
#include "reportwriter.h"


CReportWriter::CReportWriter()
{
	SetLines(0);
	SetMaxLines(64);
	SetPages(1);
	SetAFlag(FALSE);
	SetBFlag(FALSE);
	SetTFlag(FALSE);
	SetSFlag(FALSE);
}

BOOL CReportWriter::SetupReportWriter(LPCTSTR FileName, LPCTSTR LineFmt, LPCTSTR STotalFmt, 
									  LPCTSTR TotalFmt, LPCTSTR BreakFmt)
{
	SetLines(0);
	SetMaxLines(64);
	SetPages(1);
	SetAFlag(FALSE);
	SetBFlag(FALSE);
	SetTFlag(FALSE);
	SetSFlag(FALSE);
	
	SetLineFmt(LineFmt);
	SetSTotalFmt(STotalFmt);
	SetTotalFmt(TotalFmt);
	SetBreakFmt(BreakFmt);
	return m_File.Open(FileName, CFile::modeWrite);
}

BOOL CReportWriter::WritePage()
{
	if(GetLines() >= GetMaxLines())
	{
		m_File.WriteString("\f");
		SetLines(1);
		return TRUE;
	}

	return FALSE;
}
