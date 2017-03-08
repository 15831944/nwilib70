#ifndef __BLMMKTDATAOBJ_H__
#define __BLMMKTDATAOBJ_H__

#ifndef __BLMAPI_H__
#include "BlmApi.h"
#endif

#ifndef __BLMDATA_H__
#include "BLMDATA.h"
#endif

#ifndef __ORALOADER_H__
#include "ORALOADER.H"
#endif

#include <fstream>

class CMktEventHandler  : public EventHandler
{
protected:
	CString m_BlmErrorMsg;
	CBlmSecArray m_BlmSecArr;
	CString m_Fields; //a comma-delimited string of requested fields
	int m_IntervalInSeconds;
	std::fstream *m_pfs;

private:
	void openService(const Event& event, Session * session);
	void subscribe(const Event& event, Session * session);
	void dataReceived(const Event& event, Session * session);
	void securityOrFieldError(const Event& event, Session * session);
	void GetErrorMsg(Element errElement, BOOL bWithErrorCode = FALSE);
	void ShowErrorMsg() { *m_pfs << (LPCTSTR) m_BlmErrorMsg << "\n"; /* MessageBox(NULL, m_BlmErrorMsg, "MktData", MB_OK); */ }

public:
	CMktEventHandler() { Initialize(-1000); }
	~CMktEventHandler() { }
	
	void Initialize(int IntervalInSeconds);
	virtual bool processEvent(const Event& event, Session *session); //The API calls this function when a market data event happens

	CBlmSecArray &GetBlmSecArr() { return m_BlmSecArr; }
	CString GetBlmErrorMsg() { return m_BlmErrorMsg; }
    CStringArray &GetFields() { return GetBlmSecArr().GetFields(); }
	BOOL IsReady() { return GetBlmSecArr().GetSize() > 0 && GetFields().GetSize() > 0 && m_IntervalInSeconds >= 0 ? TRUE: FALSE; }
	void SetOutFile(std::fstream *fs) { m_pfs = fs; } 
};


class CBlmMktDataObj
{
protected:
	CString m_BlmErrorMsg;
	BOOL m_bReady;
	CMktEventHandler m_EventHandler;
	Session *m_pSession;
	std::fstream m_fs;
	COraLoader m_OraLoader;

public:
	CBlmMktDataObj();
	~CBlmMktDataObj();
	
	virtual void SetupSecurityData(int IntervalInSeconds = 0);
	BOOL GetMarketData();
	CBlmSecArray &GetBlmSecArr() { return m_EventHandler.GetBlmSecArr(); }

	BOOL StartSession();
	BOOL StopSession();

	BOOL IsReady() { return m_bReady; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
};
#endif