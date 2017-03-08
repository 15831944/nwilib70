#ifndef __DATAOBJ_H__
#define __DATAOBJ_H__

#ifndef __PORTFOLIOARRAY_H__
#include "portfolioarray.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CDataObj : public CObject
{
DECLARE_DYNAMIC(CDataObj)

protected:
	COraLoader m_OraLoader;
	int m_nUserStatus;
	CString	m_Date;
	CString	m_Plus3Date;
	CString m_Plus2Date;
	CString m_Plus1Date;
	CString	m_User;
	CString m_OraUser;
	CPortfolioArray m_PortfolioArr;
	
public:
	CDataObj();
	CDataObj(COraLoader &OraLoader);
	~CDataObj() {}

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	CString &GetDate() { return m_Date; }
	void SetDate(LPCTSTR Date) { m_Date = Date; }

	CString &GetPlus3Date() { return m_Plus3Date; }
	void SetPlus3Date(LPCTSTR Plus3Date) { m_Plus3Date = Plus3Date; }

	CString &GetPlus2Date() { return m_Plus2Date; }
	void SetPlus2Date(LPCTSTR Plus2Date) { m_Plus2Date = Plus2Date; }

	CString &GetPlus1Date() { return m_Plus1Date; }
	void SetPlus1Date(LPCTSTR Plus1Date) { m_Plus1Date = Plus1Date; }

	CString &GetUser() { return m_User; }
	int GetUserStatus() { return m_nUserStatus; }
	void SetUser(LPCTSTR User) { m_User = User; }	
	
	CString &GetOraUser() { return m_OraUser; }
	void SetOraUser(LPCTSTR OraUser) { m_OraUser = OraUser; }	

	BOOL IsReady();
		
	int LoadDBRecArray(CDBRecArray &DataArray, UINT SQL_ID, BOOL Refresh = FALSE);
	int LoadDBRecArray(CDBRecArray &DataArray, LPCTSTR Sql, BOOL Refresh = FALSE);

	virtual void LoadAssetData(BOOL Refresh = FALSE) {};
	virtual void LoadSupportData(BOOL Refresh = FALSE) {};

	CPortfolioArray &GetPortfolioArr() { return m_PortfolioArr; }
};
#endif // __DATAOBJ_H__