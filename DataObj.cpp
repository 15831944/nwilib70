#include "stdafx.h"
#include "dataobj.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CDataObj, CObject)

CDataObj::CDataObj()
{
	m_Date.Empty();
	m_Plus3Date.Empty();
	m_Plus2Date.Empty();
	m_Plus1Date.Empty();
	m_User.Empty();
}

CDataObj::CDataObj(COraLoader &OraLoader)
{
	m_OraLoader = OraLoader;
	m_Date.Empty();
	m_Plus3Date.Empty();
	m_Plus2Date.Empty();
	m_Plus1Date.Empty();
	m_User.Empty();
}

BOOL CDataObj::IsReady()
{
	return GetOraLoader().GetDB() && !strlen(GetPortfolioArr().GetPortfolio()) == 0 ? TRUE : FALSE;
}

int CDataObj::LoadDBRecArray(CDBRecArray &DataArray, UINT SQL_ID, BOOL Refresh)
{
	CString Sql;

	Sql.LoadString(SQL_ID);
	return LoadDBRecArray(DataArray, Sql, Refresh);
}

int CDataObj::LoadDBRecArray(CDBRecArray &DataArray, LPCTSTR Sql, BOOL Refresh)
{
	if(!GetOraLoader().GetDB())
		return -1;

	if(Refresh || DataArray.IsModified())
		DataArray.RemoveAll();

	if(DataArray.GetSize() <= 0 && (Sql && strlen(Sql) > 0))
	{
		if(GetOraLoader().Open(Sql))
			GetOraLoader().LoadDBRecArray(DataArray);
	}

	return DataArray.GetSize();
}
