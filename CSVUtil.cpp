#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "csvutil.h"
#include "qdata.h"

BOOL CCSVUtil::Setup(const CString FileName, BOOL bRead)
{
	CString Path;
	char Buffer[2000];
	CFileException e;

	GetCSVHeaders().RemoveAll();
	GetCSVData().RemoveAll();

	m_bRead = bRead;
	m_nRowCount = 0;

	if(FileName.IsEmpty())
		return FALSE;

	GetEnvironmentVariable("FMSDATA", Buffer, sizeof(Buffer));
	Path = Buffer;
	if(Path.GetLength() > 0 && Path.GetAt(Path.GetLength() - 1) == '\\')
		Path += FileName;
	else
		Path += "\\" + FileName;

	if(!m_File.Open(Path, m_bRead ? CFile::modeRead : CFile::modeCreate|CFile::modeWrite, &e))
	{
		e.GetErrorMessage(Buffer, sizeof(Buffer) - 1);
		MessageBox(NULL, Buffer, "File could not be opened ", MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CCSVUtil::ReadCSVHeaders(int &nRow)
{
	if(!m_bRead)
		return FALSE;

	if(nRow <= 0)
		return FALSE;

	ReadCSV(m_CSVHeaders, nRow);

	return GetCSVHeaders().GetSize() > 0;
}

BOOL CCSVUtil::ReadCSVData()
{
	int nRow;

	if(!m_bRead)
		return FALSE;

	nRow = m_nRowCount + 1;
	ReadCSV(m_CSVData, nRow);
	
	return GetCSVData().GetSize() > 0;
}

BOOL CCSVUtil::ReadCSV(CStringArray &DataArray, int &nRow)
{
	int i, QuoteCnt;
	char DataChar;
	CString DataLine, Data;

	for(i = m_nRowCount; i < nRow; i++)
		m_File.ReadString(DataLine);

	m_nRowCount = nRow;

	if(DataLine.IsEmpty())
		return FALSE;

	DataArray.RemoveAll();
	for(i = 0; i < DataLine.GetLength(); i++)
	{
		DataChar = DataLine.GetAt(i);
		switch(DataChar)
		{
			case ',':
				if(QuoteCnt % 2 == 1)
					Data += DataChar;
				else
				{
					DataArray.Add(Data);
					Data.Empty();
					QuoteCnt = 0;
				}
				break;
			case '"':
				QuoteCnt ++;
				break;
			default:
				Data += DataChar;
				break;
		}
	}
	
	DataArray.Add(Data);
	return DataArray.GetSize() > 0;
}

BOOL CCSVUtil::WriteCSVHeader(const CString Header, BOOL bWithQuote)
{
	if(m_bRead)
		return FALSE;
	
	return WriteCSV(m_CSVHeaders, Header, bWithQuote);
}

BOOL CCSVUtil::WriteCSVData(const CString Data, BOOL bWithQuote)
{
	if(m_bRead)
		return FALSE;

	return WriteCSV(m_CSVData, Data, bWithQuote);
}

BOOL CCSVUtil::WriteCSV(CStringArray &DataArray, const CString Data, BOOL bWithQuote)
{
	DataArray.Add(bWithQuote ? "\"" + Data + "\"" : Data);
	return TRUE;
}

BOOL CCSVUtil::WriteCSVLine(BOOL bHeader)
{
	CString DataLine;
	int i;

	if(bHeader)
	{
		for(i = 0; i < m_CSVHeaders.GetSize(); i++)
		{
			if(!DataLine.IsEmpty())
				DataLine += ",";
			DataLine += m_CSVHeaders.GetAt(i);
		}
	}
	else
	{
		for(i = 0; i < m_CSVData.GetSize(); i++)
		{
			if(!DataLine.IsEmpty())
				DataLine += ",";
			DataLine += m_CSVData.GetAt(i);
		}
	}

	m_File.WriteString(DataLine);
	return TRUE;
}

CString CCSVUtil::GetCSVNumber(const CString SrcNum)
{
	CString Buf, Buf2;
	CQData QData;

	Buf = QData.RemoveComma(SrcNum);

	if(Buf.Find("(") >0)
	{
		Buf = Buf.SpanExcluding("()");
		Buf = "-" + Buf;
	}

	Buf.TrimLeft();
	Buf.TrimRight();

	Buf2.Empty();
	for(int i = 0; i < Buf.GetLength(); i++)  // Remove invisible char
	{
		if(strchr("-0123456789.", Buf.GetAt(i)))
			Buf2 += Buf.GetAt(i);
	}

	return Buf2;
}
