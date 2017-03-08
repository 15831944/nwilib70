#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <odbcinst.h>
#include "odbcdb.h"
#include "nwivar.h"

IMPLEMENT_DYNAMIC(CODBCDB, CDatabase)

CString CODBCDB::GetOracleDSN(const CString UserName, const CString Password)
{
	m_DSN = "ODBC;DSN=NWIDB;UID=" + UserName + ";PWD=" + Password + ";DBQ=NWIDB;DBA=W;";
	return m_DSN;
}

CString CODBCDB::GetExcelDSN(const CString FileName)
{
	m_DSN = "ODBC;DSN=Excel Files;DBQ=" + FileName + ";DriverID=1046;";
	return m_DSN;
}

BOOL CODBCDB::Open(BOOL bExclusive, BOOL bReadOnly, BOOL bUserCursorLib)
{
	if(GetDSN().IsEmpty())
		return FALSE;

	return CDatabase::Open("", bExclusive, bReadOnly, GetDSN(), bUserCursorLib);
}

BOOL CODBCDB::Open(const CString FileName, BOOL bExclusive, BOOL bReadOnly, BOOL bUserCursorLib)
{
	if(FileName.IsEmpty())
		return FALSE;

	GetExcelDSN(FileName);

	return Open(bExclusive, bReadOnly, bUserCursorLib);
}

BOOL CODBCDB::Open(const CString UserName, const CString Password, BOOL bExclusive, BOOL bReadOnly, BOOL bUserCursorLib)
{
	if(UserName.IsEmpty() || Password.IsEmpty())
		return FALSE;

	GetOracleDSN(UserName, Password);

	return Open(bExclusive, bReadOnly, bUserCursorLib);
}

BOOL CODBCDB::OpenEx(DWORD dwOptions)
{
	if(GetDSN().IsEmpty())
		return FALSE;

	return CDatabase::OpenEx(GetDSN(), dwOptions);
}

BOOL CODBCDB::OpenEx(const CString FileName, DWORD dwOptions)
{
	if(FileName.IsEmpty())
		return FALSE;

	GetExcelDSN(FileName);

	return OpenEx(dwOptions);
}

BOOL CODBCDB::OpenEx(const CString UserName, const CString Password, DWORD dwOptions)
{
	if(UserName.IsEmpty() || Password.IsEmpty())
		return FALSE;

	GetOracleDSN(UserName, Password);

	return OpenEx(dwOptions);
}

int CODBCDB::LoadDBSheet(CRecordset *pDBRec, CDBSheet &DBSheet)
{
	int i, FieldCount;
	CString Data;
	CStringArray Rec;

	DBSheet.ClearSheet();

	if(pDBRec)
	{	
		FieldCount = pDBRec->GetODBCFieldCount();
	
		DBSheet.SetSheetCols(FieldCount);
				
		while(!pDBRec->IsEOF())
		{
			Rec.RemoveAll();
			for(i = 0; i < FieldCount; i++)
			{
				pDBRec->GetFieldValue(i, Data);
				if(i == 0)
				{
					if(!Data.IsEmpty())
						Rec.Add(Data);
				}
				else
					if(Rec.GetCount() > 0)
						Rec.Add(Data);
			}
					
			if(Rec.GetCount() == FieldCount)
				DBSheet.AddSheetRow(Rec);
		
			pDBRec->MoveNext();
		}
	}

	return DBSheet.GetSheetRows();
}

int CODBCDB::LoadComboBox(CRecordset *pDBRec, COptComboBox &CBox)
{
	int i = 0, FieldCount;
	CString Data;

	CBox.ResetContent();

	if(pDBRec)
	{	
		FieldCount = pDBRec->GetODBCFieldCount();
					
		while(!pDBRec->IsEOF())
		{
			pDBRec->GetFieldValue(i, Data);

			if(!Data.IsEmpty())
				CBox.AddString(Data);
		
			pDBRec->MoveNext();
		}
	}

	return CBox.GetCount();
}

int CODBCDB::LoadMCComboBox(CRecordset *pDBRec, CMCComboBox &CBox)
{
	int i, FieldCount;
	CString Data;
	CStringArray Rec;

	CBox.ResetContent();

	if(pDBRec)
	{	
		FieldCount = pDBRec->GetODBCFieldCount();
	
		CBox.SetColumnCount(FieldCount);
				
		while(!pDBRec->IsEOF())
		{
			Rec.RemoveAll();
			for(i = 0; i < FieldCount; i++)
			{
				pDBRec->GetFieldValue(i, Data);
				if(i == 0)
				{
					if(!Data.IsEmpty())
						Rec.Add(Data);
				}
				else
					if(Rec.GetCount() > 0)
						Rec.Add(Data);
			}
					
			if(Rec.GetCount() == FieldCount)
				CBox.AddString(Rec);
		
			pDBRec->MoveNext();
		}
	}

	return CBox.GetCount();
}
