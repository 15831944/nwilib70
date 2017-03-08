#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "oradset.h"
#include "nwivar.h"

LPCTSTR COraDset::m_OraDsetError = "ORACLE ERROR";

COraDset::COraDset(ODatabase *DB)
{
	m_ErrMsg = EMPTYSTRING;
	SetDB(DB);
	SetBegin(FALSE);
	SetShowError();
}

COraDset::COraDset()
{
	m_ErrMsg = EMPTYSTRING;
	SetDB(NULL);
	SetBegin(FALSE);
	SetShowError();
}

void COraDset::Copy(COraDset &OraDset)
{
	m_ErrMsg = EMPTYSTRING;
	SetDB(OraDset.GetDB());
	SetBegin(FALSE);
	SetShowError(OraDset.GetShowError());
}

void COraDset::SetDB(ODatabase *pDB)
{
	m_pDB = pDB;
}

BOOL COraDset::ExecuteSql(LPCTSTR Sql)
{
	SetSql(Sql);

	return ExecuteSql();
}

BOOL COraDset::ExecuteSql()
{
	if(!GetDB())
		return FALSE;

	int rc = GetDB()->ExecuteSQL(GetSql());
	if(rc == OSUCCESS)
		return TRUE;

	GetErrMsg(rc);
	return FALSE;
}

BOOL COraDset::ExecuteSql(UINT SqlID)
{
	m_Sql.LoadString(SqlID);

	return ExecuteSql();
}

BOOL COraDset::CheckOpen()
{
	if(!IsOpen())
	{
		m_ErrMsg = "Data Set is not open";
		if(GetShowError())
			MessageBox(NULL, m_ErrMsg, m_OraDsetError, MB_OK);
		return FALSE;
	}

	return TRUE;
}

void COraDset::SetSql(LPCTSTR Sql)
{
	if(Sql && strlen(Sql) > 0)
		m_Sql = Sql;
}

BOOL COraDset::Open(CString &Sql, int Options)
{
	if(!GetDB())
		return FALSE;

	SetSql(Sql);
	int rc = ODynaset::Open(*GetDB(), GetSql(), Options);
						
	if(rc == OSUCCESS && GetFieldInfo())
		return TRUE;

	GetErrMsg(rc);
	return FALSE;
}

BOOL COraDset::Open(LPCTSTR Sql, int Options)
{
	SetSql(Sql);
	return Open(GetSql(), Options);
}

BOOL COraDset::Open(UINT SqlID, int Options)
{
	GetSql().LoadString(SqlID);

	return Open(GetSql(), Options);
}

CString &COraDset::GetErrMsg(int rc)
{
	CStdioFile	F;

	m_ErrMsg.Empty();
	if(rc != OSUCCESS)
	{
		if(IsOpen())
		{
			m_ErrMsg = GetErrorText();
			if(m_ErrMsg.IsEmpty())
				m_ErrMsg = GetDB()->GetErrorText();
		}
		else
			m_ErrMsg = GetDB()->GetErrorText();

		if(m_ErrMsg.IsEmpty())
			m_ErrMsg = m_OraDsetError;

		CString Text;
		Text = GetErrMsg();
		Text += "\n" + GetSql();
		if(GetShowError())
			MessageBox(NULL, Text, m_OraDsetError, MB_OK);
       	F.Open("ORAOBJ.SQL", CFile::modeCreate | CFile::modeWrite);
		F.WriteString(Text);
		F.Close();
	}

	return m_ErrMsg;
}

int COraDset::IsExist(LPCTSTR Sql)
{
	SetSql(Sql);
	
	return IsExist();
}

int COraDset::IsExist()
{
	if(!Open())
		return -1;

	OValue Value;
	GetFieldValue(0, &Value);

	return (int) Value;
}

int COraDset::IsExist(UINT SqlID)
{
	GetSql().LoadString(SqlID);

	return IsExist();
}

BOOL COraDset::GetFieldInfo()
{
	OField OraField;
	COraField Field;

	GetFieldArray().RemoveAll();
	if(!CheckOpen())
		return FALSE;

	for(int i = 0; i < GetFieldCount(); i ++)
	{
		OraField = GetField(i);
		Field.SetName(OraField.GetName());
		Field.SetType(OraField.GetServerType());
		Field.SetScale(OraField.GetScale());
		Field.SetPrecision(-1);
		Field.SetWithComma(TRUE);
		Field.SetVisible();
		Field.SetSize(GetFieldSize(i) == 0 ? GetFieldServerSize(i) : GetFieldSize(i));
		GetFieldArray().Add(Field);
	}

	return TRUE;
}

void COraDset::CopyFieldArray(CFieldArray &FieldArray)
{
	COraField* pField;

	FieldArray.RemoveAll();
	for(int i = 0; i < GetFieldArray().GetSize(); i++)
	{
		pField = GetFieldArray().GetAt(i);
		FieldArray.Add(*pField);
	}
}

void COraDset::UpdateFieldArray(CFieldArray &FieldArray)
{
	COraField* pField;

	GetFieldArray().RemoveAll();
	for(int i = 0; i < FieldArray.GetSize(); i++)
	{
		pField = FieldArray.GetAt(i);
		GetFieldArray().Add(*pField);
	}
}

void COraDset::BeginTransaction()
{
	SetBegin(TRUE);
	m_Session = GetSession();
}

void COraDset::Commit()
{
	if(GetBegin())
	{
		SetBegin(FALSE);
		m_Session.Commit();
	}
}

void COraDset::Rollback()
{
	if(GetBegin())
	{
		SetBegin(FALSE);
		m_Session.Rollback();
	}
}