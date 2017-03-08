#ifndef __OPENTRADE_H__
#define __OPENTRADE_H__

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class COpenTrade : public CObject
{
DECLARE_DYNAMIC(COpenTrade)
protected:
	COraLoader m_OraLoader;
	CString m_Date;

public:
	COpenTrade() { };
	COpenTrade(COraLoader &OraLoader, LPCTSTR Date) { m_OraLoader = OraLoader; m_Date = Date; }
	~COpenTrade() {}

protected:
	BOOL UpdateTable();
	BOOL ClearTable();
	BOOL PreProcess();
	BOOL SetupSql();
	BOOL DoWork();

public:
	BOOL Process();
};
#endif