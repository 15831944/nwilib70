#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "qdata.h"


LPCTSTR CQData::GetQueryText(const char Src)
{
	m_DataBuf = Src;
	return GetQueryText((LPCTSTR) m_DataBuf);
}

LPCTSTR CQData::GetQueryText(LPCTSTR Src, LPCTSTR Extra)
// Produce a query Text.
// if ' is found in Src, make it ''; if it is empty, assign a "NULL". Add quote.
{
	if(!Src || strlen(Src) == 0)
	{
		m_DataBuf = DBNULL;
		return GetDataBuf();
	}

	CString Buf = Src;
	int Pos;
	
	m_DataBuf.Empty();
	Pos = Buf.Find(QUOTE);
	while(Pos >= 0)
	{		
		m_DataBuf += Buf.Left(++Pos);
		m_DataBuf += QUOTE;
		Buf = Buf.Right(Buf.GetLength() - Pos);
		Pos = Buf.Find(QUOTE);
	}

	if(!Buf.IsEmpty())
    	m_DataBuf += Buf;

	if(Extra && strlen(Extra) > 0 && !m_DataBuf.IsEmpty())
		m_DataBuf = Extra + m_DataBuf + Extra;
	
	m_DataBuf = QUOTE + m_DataBuf + QUOTE;

	return GetDataBuf();
}

LPCTSTR CQData::GetQueryText(CString &Src, LPCTSTR Extra)
{
	return GetQueryText((LPCTSTR) Src, Extra);
}

LPCTSTR CQData::GetQueryDate(LPCTSTR SrcDate)
{ // Convert MM/DD/YYYY to 'MM/DD/YYYY'

	if(!SrcDate || strlen(SrcDate) == 0)
	{
		m_DataBuf = DBNULL;
		return GetDataBuf();
	}

	m_DataBuf = SrcDate;
	m_DataBuf = QUOTE + m_DataBuf + QUOTE;
	
	return GetDataBuf();
}

LPCTSTR CQData::GetQueryDate(CString &SrcDate)
{ 
	return GetQueryDate((LPCTSTR) SrcDate);
}

LPCTSTR CQData::GetQueryNumber(LPCTSTR SrcNum)
{
	if(!SrcNum || strlen(SrcNum) == 0)
		m_DataBuf = DBNULL;
	else
	{
		RemoveComma(SrcNum);
		m_DataBuf = QUOTE + m_DataBuf + QUOTE;
	}

	return GetDataBuf();
}

LPCTSTR CQData::GetQueryNumber(CString &Src)
{
	return GetQueryNumber((LPCTSTR) Src);
}

LPCTSTR CQData::GetRecAt(CStringArray &QRec, int Index)
{
	if(Index >= 0 && Index < QRec.GetSize())
		return (LPCTSTR) QRec.GetAt(Index);
	return DBNULL;
}

LPCTSTR CQData::Abs(LPCTSTR Num)
{
	SetDataBuf(Num);

	return Abs(m_DataBuf);
}

LPCTSTR CQData::Abs(CString &Num)
{
	if(Num.IsEmpty())
		SetDataBuf();
	else
	{
		int Pos;

		Pos = Num.Find(MINUS);
		if(Pos >= 0)
			SetDataBuf(Num.Right(Num.GetLength() - Pos - 1));
		else
			SetDataBuf(Num);
	}

	return GetDataBuf();
}

LPCTSTR CQData::AddComma(LPCTSTR SrcNum)
{
	int Pos;
	BOOL bMinus = FALSE;
	CString Buf;

	if(SrcNum && strlen(SrcNum) > 0)
	{
		Buf = SrcNum;
		Buf.TrimLeft();
		Buf.TrimRight();
	}

	m_DataBuf.Empty();
	if(Buf.IsEmpty())		
		return GetDataBuf();

	Pos = Buf.Find(MINUS);
	if(Pos >= 0)
	{
		bMinus = TRUE;
		Buf = Buf.Right(Buf.GetLength() - Pos - 1);
	}
	
	Pos = Buf.Find(PERIOD);
	if(Pos >= 0)
	{		
		m_DataBuf = Buf.Right(Buf.GetLength() - Pos);
		Buf = Buf.Left(Pos);
	}

	while(Buf.GetLength() > 3)
	{
		m_DataBuf = COMMA + Buf.Right(3) + m_DataBuf;		
		Buf = Buf.Left(Buf.GetLength() - 3);
	}

	if(!Buf.IsEmpty())
		m_DataBuf = Buf + m_DataBuf;
	
	if(bMinus)
		m_DataBuf = MINUS + m_DataBuf;
	return GetDataBuf();
}

LPCTSTR CQData::RemoveComma(LPCTSTR SrcNum)
{
	CString Buf;
	
	Buf = SrcNum;

	return RemoveString(Buf, COMMA);
}

LPCTSTR CQData::RemoveString(CString &String, LPCTSTR RemoveString)
{
	CString Buf;
	int Pos;

	Buf = String;
	SetDataBuf();
	if(Buf.IsEmpty())
		return GetDataBuf();

	Pos = Buf.Find(RemoveString);
	while(Pos >= 0)
	{
		m_DataBuf += Buf.Left(Pos);
		Buf = Buf.Right(Buf.GetLength() - Pos - 1);
		Pos = Buf.Find(RemoveString);
	}

	if(!Buf.IsEmpty())
		m_DataBuf += Buf;

	return GetDataBuf();
}

double CQData::Round(double Number, int Precision)
{
	double num1 = 0, num2 = 0;

	if(Number < 0.0)
		return(-1*Round((-1.0*Number), Precision));

	num1 = Number*pow(10, (double) Precision);
	if (modf(num1, &num2) >= .5)
		num2++;

	return num2/pow(10, (double) Precision);
}

double CQData::Trunc(double Number, int Precision)
{
	double num1=0,num2=0;

	if(Number < 0.0)
		return(-1*Trunc((-1.0*Number),Precision));

	num1 = Number * pow(10, (double) Precision);
	modf(num1, &num2);
	return num2/pow(10, (double) Precision);
}

void CQData::GetCSVToken(CString &SrcData, CString &Data)
{
	CString Token;
	int Len;

	Data.Empty();
	if(SrcData.IsEmpty())
		return;

	if(SrcData.GetAt(0) == '"')
		Token = "\",";
	else	
		Token = ",";

	Len = SrcData.Find(Token);
	if(Len > 0)
		Data = SrcData.Left(Len);
	else
		if(Len < 0)
			Data = SrcData;

	if(SrcData.GetLength() > Len)
	{
		SrcData = SrcData.Right(SrcData.GetLength() - Len - 1);
		SrcData.Trim();
	}
	else
		SrcData.Empty();
}

LPCTSTR CQData::GetCSVString(LPCTSTR String)
{
	CString Buf;

	Buf = String;
	Buf = Buf.SpanExcluding("\"");

	Buf.TrimLeft();
	Buf.TrimRight();

	SetDataBuf(Buf);

	return GetDataBuf();
}

LPCTSTR CQData::GetCSVNumber(LPCTSTR SrcNum)
{
	CString Buf;

	Buf = GetCSVString(SrcNum);

	Buf = RemoveComma(Buf);

	if(Buf.Find("(") >0)
	{
		Buf = Buf.SpanExcluding("()");
		Buf = "-" + Buf;
	}

	Buf.TrimLeft();
	Buf.TrimRight();

	SetDataBuf();
	for(int i = 0; i < Buf.GetLength(); i++)  // Remove invisible char
	{
		if(strchr("-0123456789.", Buf.GetAt(i)))
			m_DataBuf += Buf.GetAt(i);
	}

	return GetDataBuf();
}

LPCTSTR CQData::WriteNumber(LPCTSTR Number, BOOL bComma, int Precision)
{
	CString Num;
	
	if(Number && strlen(Number) > 0)
		Num = Number;

	m_DataBuf.Empty();
	if(Num.IsEmpty())
		return GetDataBuf();

	return WriteNumber(Num, bComma, Precision);
}

LPCTSTR CQData::WriteNumber(int Number, BOOL bComma)
{
	m_DataBuf.Format("%d", Number);
	if(bComma)
		AddComma(m_DataBuf);

	return (const char*) m_DataBuf;
}

LPCTSTR CQData::WriteNumber(double Number, BOOL bComma, int Precision)
{
	CString Fmt;

	if(Precision < 0)
		m_DataBuf.Format("%.20lf", Number);
	else
		if(Precision == 0)
			m_DataBuf.Format("%.0lf", Number);
		else
		{
			Fmt.Format("%%.%dlf", Precision);
			m_DataBuf.Format((LPCTSTR) Fmt, Number);
		}
	
	if(Precision < 0)
		TrimZero(m_DataBuf);
	if(bComma)
		AddComma(m_DataBuf);
	return (LPCTSTR) m_DataBuf;
}

LPCTSTR CQData::WriteNumber(CString &Number, BOOL bComma, int Precision)
{	
	m_DataBuf.Empty();
	if(Number.IsEmpty()) // Blank is allowed
		return GetDataBuf();

	if(!IsValid(Number, "-0123456789 ,./")) // Invalid, return Empty		
		return GetDataBuf();

	m_DataBuf = RemoveComma(Number);
	if(m_DataBuf.Find(SLASH, 0) >= 0 || Precision >= 0)
		ReWriteFraction(m_DataBuf, bComma, Precision);
	else
	{
		if(bComma)
			AddComma(m_DataBuf);
	}

	return GetDataBuf();
}

LPCTSTR CQData::WriteNumber(CEdit &Ctrl, BOOL bComma, int Precision)
{
	CString Buffer;

	if(Ctrl.GetModify() && Ctrl.GetWindowTextLength() > 0)
		Ctrl.GetWindowText(Buffer);

	m_DataBuf.Empty();
	if(Buffer.IsEmpty())
		return GetDataBuf();

	WriteNumber(Buffer, bComma, Precision);
	if(m_DataBuf.IsEmpty())
	{
		MessageBox(NULL, "Only number are allowed", NULL, MB_OK);
		Ctrl.GetFocus();
		return GetDataBuf();
	}

	Ctrl.SetWindowText(m_DataBuf);
	Ctrl.SetModify(FALSE);
	
	return GetDataBuf();		
}

void CQData::ReWriteFraction(CString &Num, BOOL bComma, int Precision)
// Handle num with fraction.
{
	double Base, Frac, dNum = 0;
	CString Data;
	int Pos;
	
	Pos = Num.Find('/');
	if(Pos < 0)
		dNum = atof(Num);
	else
	{
		Base = atof(((LPCTSTR) Num) + Pos + 1); // Get the base portion
		Data = Num.Left(Pos);		// Get the Fraction portion
		Pos = Data.Find(' ');
		if(Pos < 0)
			Frac = atof(Data);
		else
		{
			atof(((LPCTSTR) Data) + Pos + 1);
			dNum = atof(Data.Left(Pos));
		}

		if(Base > 0)	// Compute Value
			dNum += Frac/Base;
	}

	Num = WriteNumber(dNum, bComma, Precision);
}

LPCTSTR CQData::TrimZero(CString &Num)
{
	while(Num.Find(PERIOD) >= 0 && Num.Right(1) == "0")
		Num = Num.Left(Num.GetLength() - 1);

	if(Num.GetAt(Num.GetLength() - 1) == '.')
		Num = Num.Left(Num.GetLength() - 1);
	return (LPCTSTR) Num;
}

LPCTSTR CQData::AddZero(CString &Num, int Precision)
{
	if(Precision <= 0)
		return (LPCTSTR) Num;

	if(Num.IsEmpty())
		return (LPCTSTR) Num;

	int Pos, Len;

	Pos = Num.Find(PERIOD);
	if(Pos < 0)
	{
		Num += PERIOD;
		Len = Precision;
	}
	else
		Len = Precision - strlen((LPCTSTR)Num + Pos + 1);
	
	while(Len-- > 0)
		Num += "0";

	return (LPCTSTR) Num;
}

BOOL CQData::IsValid(LPCTSTR Str, LPCTSTR ValidString)
{
	int i;
	
	if(!Str || strlen(Str) == 0)
		return TRUE;
    
    if(!ValidString || strlen(ValidString) == 0)
    	return TRUE;
	
	for(i = 0; i < (int) strlen(Str); i++)
	{
		if(!strchr(ValidString, Str[i]))
			return FALSE;
	}
	return TRUE;
}

LPCTSTR CQData::FormatedDate(LPCTSTR Date)
// From 5/2/2001 to 05/02/2001
{	
	CString Buf, t;
	int Pos, Year;

	m_DataBuf.Empty();
	if(!Date || strlen(Date) <= 0)
		return GetDataBuf();
	
	Buf = Date;
	Pos = Buf.Find(SLASH);

	while(Pos >= 0)
	{
		t = Buf.Left(Pos);		
		if(t.GetLength() == 1)
			t = "0" + t;

		if(!m_DataBuf.IsEmpty())
			m_DataBuf += SLASH;
		m_DataBuf += t;

		Buf = Buf.Right(Buf.GetLength() - Pos - 1);
		Pos = Buf.Find(SLASH);
	}

	Pos = Buf.Find(SPACE); // Remove Time
	if(Pos >= 0)
		Buf = Buf.Left(Pos);

	if(!Buf.IsEmpty())
	{
		if(Buf.GetLength() == 2)
			Year = YYToYYYY(atoi(Buf));
		else
			Year = atoi(Buf);
		Buf.Format("%d", Year);
		m_DataBuf += "/" + Buf;
	}

	return GetDataBuf();
}

BOOL CQData::SetSqlStmt(CString &Sql, CString &Value, LPCTSTR Field, BOOL bAnd)
{
	LPCTSTR Like = " LIKE ";
	LPCTSTR And = " AND ";
	CString Data;
	
	if(Value.IsEmpty())
		return FALSE;

	Data = GetQueryText(Value, "%");
	if(bAnd)
		Sql += And;
		
	Sql += Field;
	Sql += Like;
	Sql += Data;

	return TRUE;
}

BOOL CQData::SetSqlStmt(CString &Sql, CEdit &Edit, LPCTSTR Field, BOOL bAnd)
{
	CString Buffer;
	
	Edit.GetWindowText(Buffer);
	return SetSqlStmt(Sql, Buffer, Field, bAnd);
}

BOOL CQData::SetSqlStmt(CString &Sql, COptComboBox *Combo, LPCTSTR Field, BOOL bAnd)
{
	CString Buffer;
	
	Combo->GetSelString(Buffer);
	return SetSqlStmt(Sql, Buffer, Field, bAnd);
}

BOOL CQData::SetSqlStmt(CString &Sql, CHLBox *HLBox, LPCTSTR Field, BOOL bAnd)
{
	CString Buffer;
	
	HLBox->GetSelString(Buffer);
	return SetSqlStmt(Sql, Buffer, Field, bAnd);
}

BOOL CQData::GetFileName(CWinApp *theApp, CString &Path, LPCTSTR Section, LPCTSTR Entry, LPCTSTR DefExt)
{
	m_DataBuf = theApp->GetProfileString(Section, Entry);
	if(m_DataBuf.IsEmpty())
	{
		m_DataBuf = "*.";
		m_DataBuf += strlen(DefExt) ? DefExt : EMPTYSTRING;	
	}
	
	CFileDialog FDlg(TRUE, DefExt, m_DataBuf, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, Path);    
    
    if(FDlg.DoModal() == IDOK)
    {
		Path = FDlg.GetPathName();
 		theApp->WriteProfileString(Section, Entry, Path);
		return TRUE;
 	}

	return FALSE;
}

void CQData::SetString(CString &Str, LPCTSTR pStr)
{
	if(pStr && strlen(pStr) > 0)
		Str = pStr;
	else
		Str.Empty();
}

int CQData::CopyDBRecArrayToComboBox(CDBRecArray &DBRecArray, COptComboBox &ComboBox, int Col, BOOL bSetModified)
{
	if(ComboBox.GetCount() == 0 || DBRecArray.IsModified())
	{
		DBRecArray.CopyToComboBox(ComboBox, Col);
		if(bSetModified)
			DBRecArray.Modified(FALSE);
	}

	return ComboBox.GetCount();
}

int CQData::CopyKeyDBListKeyToComboBox(CKeyDBList &KeyDBList, COptComboBox &ComboBox, BOOL bSetModified)
{
	if(ComboBox.GetCount() == 0 || KeyDBList.IsModified())
	{
		KeyDBList.CopyKeyToComboBox(ComboBox);
		if(bSetModified)
			KeyDBList.Modified(FALSE);
	}

	return ComboBox.GetCount();
}

int CQData::CopyKeyDBListDataToComboBox(CKeyDBList &KeyDBList, LPCTSTR Key, COptComboBox &ComboBox, BOOL bSetModified)
{
	if(ComboBox.GetCount() == 0 || KeyDBList.IsModified())
	{
		KeyDBList.CopyDataToComboBox(Key, ComboBox);
		if(bSetModified)
			KeyDBList.Modified(FALSE);
	}
	
	return ComboBox.GetCount();
}

int CQData::YYToYYYY(int yy)
{
	yy %= 100;
	
	if(yy < 85)
		yy += 2000;
	else
		yy += 1900;

	return yy;
}

BOOL CQData::IsBloombergAvailbe()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile("blpapi3_32.dll", &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		BLOOMBERG_AVAILABLE = FALSE;
	else
		BLOOMBERG_AVAILABLE = TRUE;
	
	return BLOOMBERG_AVAILABLE;

}