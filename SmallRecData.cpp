#include "stdafx.h"
#include "smallrecdata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CSmallRecData, CRowCtrl)
void CSmallRecData::Setup(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR TableSpace, LPCTSTR Table, 
						  LPCTSTR PreLoad, LPCTSTR KeyName, LPCTSTR KeyName2)
{
	SetOraLoader(OraLoader);
	SetSS(pSS);
	CString T;
	T = TableSpace;
	T += PERIOD;
	T += Table;

	SetTable(T);
	SetPreLoad(PreLoad);
	SetKeyName(KeyName);
	SetKeyName2(KeyName2);
}

void CSmallRecData::SetKeyName2(LPCTSTR KeyName)
{
	if(KeyName && strlen(KeyName) > 0) 
		m_KeyName2 = KeyName;
	else
		m_KeyName2.Empty();
}

BOOL CSmallRecData::IsReady() 
{
	return (GetOraLoader().GetDB() && CRowCtrl::IsReady() && 
			strlen(GetTable()) && strlen(GetKeyName()));
}

BOOL CSmallRecData::SetSheetHeaders()
{
	if(!IsReady())
		return FALSE;
	
	GetOraLoader().GetSql().Format("SELECT * FROM %s WHERE 1 = 2", (LPCTSTR) GetTable());
	if(!GetOraLoader().Open())
		return FALSE;

	GetSS()->GetFieldArray() = GetOraLoader().GetFieldArray();
	return GetSS()->GetFieldArray().GetSize() > 0 ? TRUE : FALSE;
}

int CSmallRecData::LoadDBData()
{
	CDBRec Rec;
	CStringArray QRec;

	if(!IsReady())
		return FALSE;

	if(GetSS()->GetFieldArray().GetSize() <= 0)
		return -1;

	ToDBRec(Rec, TRUE);
	if(Rec.GetSize() <= 0)
		return -1;

	GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");
	GetOraLoader().GetSql().Format("SELECT * FROM %s WHERE ", (LPCTSTR) GetTable());
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), FALSE);
	GetOraLoader().GetSql() += " ORDER BY 1 ";

	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);

	return FALSE;
}

BOOL CSmallRecData::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	if(strlen(GetKey()) == 0)
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	GetOraLoader().GetSql().Format("SELECT * FROM %s", GetTable());
	GetOraLoader().BeginTransaction();
	if(!GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT))
	{
		GetOraLoader().Rollback();
		return FALSE;
	}

	if(!GetOraLoader().UpdateRecord(Rec, TRUE))
	{
		GetOraLoader().Rollback();
		return FALSE;
	}

	GetOraLoader().Commit();
	AddSheetRow();
	Refresh();
	return TRUE;
}

BOOL CSmallRecData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	if(strlen(GetKey()) == 0)
		return FALSE;
	
	CQData QData;
	CString Key, Key2;

	GetQueryInfo(m_KeyName, GetKey(), Key);
	GetQueryInfo(m_KeyName2, GetKey2(), Key2);
	if(m_KeyName2.IsEmpty())
		GetOraLoader().GetSql().Format("DELETE FROM %s WHERE %s = %s", GetTable(), GetKeyName(), 
										(LPCTSTR) Key);
	else
	{				
		GetOraLoader().GetSql().Format("DELETE FROM %s WHERE %s = %s AND %s = %s", GetTable(),
										GetKeyName(), (LPCTSTR) Key, GetKeyName2(),	(LPCTSTR) Key2);
	}

	GetOraLoader().BeginTransaction();
	if(GetOraLoader().ExecuteSql())
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		Refresh();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CSmallRecData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();

	if(strlen(GetKey()) == 0)
		return FALSE;

	if(!GetModify(FALSE))
		return TRUE;

	CQData QData;
	CString Key, Key2;

	CDBRec Rec;
	ToDBRec(Rec);

	GetQueryInfo(m_KeyName, GetKey(), Key);
	GetQueryInfo(m_KeyName2, GetKey2(), Key2);

	if(m_KeyName2.IsEmpty())
		GetOraLoader().GetSql().Format("SELECT * FROM %s WHERE %s = %s", GetTable(), GetKeyName(), 
										(LPCTSTR) Key);
	else
		GetOraLoader().GetSql().Format("SELECT * FROM %s WHERE %s = %s AND %s = %s", GetTable(), 
										GetKeyName(), (LPCTSTR) Key, GetKeyName2(), (LPCTSTR) Key2);

	GetOraLoader().BeginTransaction();
	if(!GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT))
	{
		GetOraLoader().Rollback();
		return FALSE;
	}

	if(!GetOraLoader().UpdateRecord(Rec))
	{
		GetOraLoader().Rollback();
		return FALSE;
	}

	GetOraLoader().Commit();
	UpdateSheetRow(FALSE);
	return TRUE;
}

void CSmallRecData::SetLimitText(CEdit &Edit, int Index)
{
	if(GetSS()->GetFieldArray().GetSize() > Index)
	{
		if(GetSS()->GetFieldArray().GetAt(Index)->GetType() == OTYPE_VARCHAR2)
			Edit.LimitText(GetSS()->GetFieldArray().GetAt(Index)->GetSize());
	}
}

LPCTSTR CSmallRecData::GetQueryInfo(CString &KeyName, LPCTSTR Key, CString &QKey)
{
	int i;
	CQData QData;

	for (i = 0; i < GetSS()->GetFieldArray().GetSize(); i ++)
	{
		if(KeyName == GetSS()->GetFieldArray().GetAt(i)->GetName())
		{
			switch(GetSS()->GetFieldArray().GetAt(i)->GetType())
			{
				case OTYPE_DATE:
					QKey = QData.GetQueryDate(Key);
					break;
				case OTYPE_FLOAT:
				case OTYPE_NUMBER:
				case OTYPE_SINT:
				case OTYPE_UINT:
					QKey = QData.GetQueryNumber(Key);
					break;
				default:
					QKey = QData.GetQueryText(Key);

			}
		}
	}

	if(QKey.IsEmpty())
		QKey = QData.GetQueryText(Key); // Default
	return (LPCTSTR) QKey;
}