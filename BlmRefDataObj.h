#ifndef __BLMREFDATAOBJ_H__
#define __BLMREFDATAOBJ_H__

#ifndef __BLMAPI_H__
#include "BlmApi.h"
#endif

#ifndef __BLMDATA_H__
#include "blmdata.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#include <fstream>

class CBlmRefDataObj
{
protected:
	BOOL m_bReady;
	BOOL m_bAvail;
	COraLoader m_OraLoader;
	CString m_BlmErrorMsg;
	CBlmSecArray m_BlmSecArr;
	Session *m_pSession;
	std::fstream m_fs;

protected:
	void GetErrorMsg(Element errElement);
	void ShowErrorMsg();

public:
	CBlmRefDataObj();
	~CBlmRefDataObj();

	CBlmRefDataObj(const CString RefDataServiceType);
	BOOL BlmRefDataRequest(const CString RefDataServiceType, int nCoID);
	virtual BOOL PrepareRefDataRequest(Request &request);
	virtual void ProcessRefDataEvent(Event evt);
	virtual void ProcessRefData(CString &security, Element element);
//	virtual BOOL PreparePriceData(Session &session);
	CBlmSecArray &GetBlmSecArr() { return m_BlmSecArr; }
	CString GetBlmErrorMsg() { return m_BlmErrorMsg; }
    CStringArray &GetFields() { return GetBlmSecArr().GetFields(); }

	BOOL StartSession();
	BOOL StopSession();

	BOOL IsReady() { return m_bReady; }
	BOOL IsAvailable();

	COraLoader &GetOraLoader() { return m_OraLoader; }
};
#endif