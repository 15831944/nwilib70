#include "stdafx.h"
#include "blmrefdataobj.h"

extern ODatabase theDB;

CBlmRefDataObj::CBlmRefDataObj()
{
	m_pSession = NULL;
	m_bAvail = FALSE;
	m_bReady = FALSE;
	GetOraLoader().SetDB(&theDB);
}


CBlmRefDataObj::~CBlmRefDataObj()
{
	if(IsReady())
		StopSession();

	if(m_pSession)
		delete m_pSession;
}


void CBlmRefDataObj::ShowErrorMsg()
{
	m_fs << m_BlmErrorMsg;
//	MessageBox(NULL, m_BlmErrorMsg, "Bloomberg", MB_OK);
}


void CBlmRefDataObj::GetErrorMsg(Element errElement)
{
	m_BlmErrorMsg += "SOURCE: ";
	m_BlmErrorMsg += errElement.getElementAsString("source"); 
	m_BlmErrorMsg += "\n";
	m_BlmErrorMsg += "CODE: ";
	m_BlmErrorMsg += errElement.getElementAsString("code");
	m_BlmErrorMsg += "\n";
	m_BlmErrorMsg += "CATEGORY: ";
	m_BlmErrorMsg += errElement.getElementAsString(CATEGORY);
	m_BlmErrorMsg += "\n";
	m_BlmErrorMsg += "MESSAGE: ";
	m_BlmErrorMsg += errElement.getElementAsString(MESSAGE);
	m_BlmErrorMsg += "\n";
	m_BlmErrorMsg += "SUBCATEGORY: ";
	m_BlmErrorMsg += errElement.getElementAsString("subcategory");

	ShowErrorMsg();
}

BOOL CBlmRefDataObj::IsAvailable()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile("blpapi3_32.dll", &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		m_bAvail = FALSE;
		return FALSE;
	}

	return m_bAvail = TRUE;
}

BOOL CBlmRefDataObj::StartSession()
{
	SessionOptions SessionOpts;

	if(IsReady())
		return TRUE;

	m_fs.open("%FMSDATA%\\FMSTrackRefData.log", std::fstream::out | std::fstream::app);	

	if(!IsAvailable()) 
		return FALSE;

/*	if(!GetBlmSecArr().IsOk())
	{
		m_BlmErrorMsg = "Either Security or Security Field not has been defined";
		return FALSE;
	} */

	SessionOpts.setServerHost("localhost");
	SessionOpts.setServerPort(8194);

	m_pSession = new Session(SessionOpts);

	if(!m_pSession)
	{
		m_BlmErrorMsg = "Fail to create Bloomberg session";
		ShowErrorMsg();
		return FALSE;
	}

	if(!m_pSession->start())
	{
		m_BlmErrorMsg = "Fail to start Bloomberg session";
		ShowErrorMsg();
		return FALSE;
	}

	if(!m_pSession->openService("//blp/refdata"))
	{
		m_BlmErrorMsg = "Fail to open refdata";
		ShowErrorMsg();
		return FALSE;
	}

	m_bReady = TRUE;

	return m_bReady;
}


void CBlmRefDataObj::ProcessRefData(CString &security, Element element)
{

}


void CBlmRefDataObj::ProcessRefDataEvent(Event evt)
{
    BOOL bError, bFieldError;
	MessageIterator Iter(evt);
	CString SecString;
	size_t i, error_idx;

	while (Iter.next()) 
	{
		Message msg = Iter.message();
		
		if(msg.hasElement(SECURITY_DATA))
		{
			Element SecurityDataArray = msg.getElement(SECURITY_DATA);

			for(i = 0; i < SecurityDataArray.numValues(); i++)
			{
				Element secData = SecurityDataArray.getValueAsElement(i);
			
				Element security = secData.getElement(SECURITY);
				SecString = security.getValueAsString();

				bFieldError = secData.hasElement(FIELD_EXCEPTIONS, true);

				if(bFieldError)
				{
					Element FieldErrors = secData.getElement(FIELD_EXCEPTIONS);
					for(error_idx = 0; error_idx < FieldErrors.numValues(); error_idx ++)
					{
						Element fieldError = FieldErrors.getValueAsElement(error_idx);
					
						m_BlmErrorMsg = SecString + "\n";
						m_BlmErrorMsg = "Field ID: ";
						m_BlmErrorMsg += fieldError.getElementAsString(FIELD_ID);
						m_BlmErrorMsg += "\n";
						Element errorInfo = fieldError.getElement(ERROR_INFO);
						GetErrorMsg(errorInfo);
					}
				}
			
				bError = secData.hasElement(SECURITY_ERROR, TRUE);

				if(bError)
				{
					Element secError = secData.getElement(SECURITY_ERROR);
					m_BlmErrorMsg = SecString + "\n";
					GetErrorMsg(secError);
				}
				else
				{
					Element FieldData = secData.getElement(FIELD_DATA);
					ProcessRefData(SecString, FieldData);
				}
			}
		}
	}
}


BOOL CBlmRefDataObj::PrepareRefDataRequest(Request &request)
{
	int i;

	for (i = 0; i < GetBlmSecArr().GetSize(); i++)
		request.append("securities", GetBlmSecArr().GetAt(i)->GetTicker());

	for (i = 0; i < GetFields().GetSize(); i++)
		request.append("fields", GetFields().GetAt(i));

	return TRUE;
}


BOOL CBlmRefDataObj::BlmRefDataRequest(const CString RefDataServiceType, int nCoID)
/* 	Valid RefDataServiceType: "HistoricalDataRequest"
							  "IntradayBarRequest"
							  "IntradayTickRequest"
							  "ReferenceDataRequest"       */
{
	BOOL bOk = TRUE;
	
	if(!StartSession())
		return FALSE;

	Service refDataSvc = m_pSession->getService("//blp/refdata");

	Request request = refDataSvc.createRequest(RefDataServiceType);

	if(!PrepareRefDataRequest(request))
		return FALSE;

	CorrelationId requestId(nCoID);

	m_pSession->sendRequest(request, requestId);

	while (bOk)
	{
		Event evt(m_pSession->nextEvent());

		switch(evt.eventType())
		{
			case Event::RESPONSE:
				bOk = FALSE;
			case Event::PARTIAL_RESPONSE:
				ProcessRefDataEvent(evt);
				break;
		}
	}

	return TRUE;
}



BOOL CBlmRefDataObj::StopSession()
{
	if(m_pSession)
		m_pSession->stop();

	if(m_fs)
		m_fs.close();

	return TRUE;
}
