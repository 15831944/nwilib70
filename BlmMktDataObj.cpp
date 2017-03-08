#include "stdafx.h"
#include "blmmktdataobj.h"

extern ODatabase theDB;

void CMktEventHandler::Initialize(int IntervalInSeconds)
{
	m_IntervalInSeconds = IntervalInSeconds;

	m_Fields.Empty();
	for (int i = 0; i < GetFields().GetSize(); i ++)
	{
		if(!m_Fields.IsEmpty())
			m_Fields += ", ";
		
		m_Fields += GetFields().GetAt(i);
	}
}

	//The Bloomberg API calls this function on a separate thread
bool CMktEventHandler::processEvent(const Event &event, Session *session)
{
	switch (event.eventType())
	{
		case Event::SESSION_STATUS:
			this->openService(event, session); //attempts to open the //blp/mktdata service
			break;
				
		case Event::SERVICE_STATUS:
			this->subscribe(event, session); //attempts to subscribe for market data
			break;

		case Event::SUBSCRIPTION_DATA:
			this->dataReceived(event, session); //handles market data events
			break;

		case Event::SUBSCRIPTION_STATUS:
			this->securityOrFieldError(event, session); //handles any security or field errors
			break;
	}
	return true;
}

void CMktEventHandler::openService(const Event& event, Session * session)
{
	MessageIterator messageIterator(event);
	while (messageIterator.next())
	{
		Message message = messageIterator.message();
		if(message.messageType() == "SessionStarted")
		{
			session->openServiceAsync("//blp/mktdata", CorrelationId(-9999));
		}
	}
}

void CMktEventHandler::subscribe(const Event& event, Session *session)
{
	int i;
	SubscriptionList subscriptions;
	CString Text, Interval;

	if(m_IntervalInSeconds > 0)
		Interval.Format("interval=%d", m_IntervalInSeconds);

	for(i = 0; i < GetBlmSecArr().GetSize(); i ++)
	{
		Text = GetBlmSecArr().GetAt(i)->GetTicker() + " " + GetBlmSecArr().GetAt(i)->GetBlmKey();

		if(m_IntervalInSeconds > 0)
			subscriptions.add(Text, m_Fields, Interval, CorrelationId(i));
		else
			subscriptions.add(Text, m_Fields, "", CorrelationId(i));
	}	

	session->subscribe(subscriptions);
}

void CMktEventHandler::dataReceived(const Event& event, Session *session)
{
	int idx, j;
	CString Security;

	MessageIterator messageIterator(event);
	while (messageIterator.next())
	{
		Message message = messageIterator.message();

		idx = (int) message.correlationId().asInteger();
		Security = GetBlmSecArr().GetAt(idx)->GetTicker();

		for(j = 0; j < GetFields().GetSize(); j ++)
		{
			if(message.hasElement(GetFields().GetAt(j), TRUE))
			{
				GetBlmSecArr().GetAt(idx)->GetPrice() = message.getElementAsFloat64(GetFields().GetAt(j));
				GetBlmSecArr().GetAt(idx)->IsModified() = TRUE;
			}
		}
	}
}

void CMktEventHandler::GetErrorMsg(Element errElement, BOOL bWithErrorCode)
{
	m_BlmErrorMsg += "SOURCE: ";
	m_BlmErrorMsg += errElement.getElementAsString("source");
	m_BlmErrorMsg += "\n";
	if(bWithErrorCode)
	{
		m_BlmErrorMsg += "ERROR CODE: ";
		m_BlmErrorMsg += errElement.getElementAsString("errorCode");
		m_BlmErrorMsg += "\n";
	}

	m_BlmErrorMsg += "CATEGORY: ";
	m_BlmErrorMsg += errElement.getElementAsString("category");
	m_BlmErrorMsg += "\n";
	m_BlmErrorMsg += "DESCRIPTION: ";
	m_BlmErrorMsg += errElement.getElementAsString("description");
	ShowErrorMsg();
}

void CMktEventHandler::securityOrFieldError(const Event& event, Session * session)
{
	MessageIterator messageIterator(event);

	while (messageIterator.next())
	{
		Message message = messageIterator.message();

		bool fieldExceptionsExist = message.messageType() == "SubscriptionStarted" && message.hasElement("exceptions", true);
		bool securityError = message.messageType() == "SubscriptionFailure" && message.hasElement("reason", true);
		bool subscriptionCancelled = message.messageType() == "SubscriptionTerminated";
		
		if(fieldExceptionsExist)
		{
			Element elmExceptions = message.getElement("exceptions");
			for (size_t i = 0; i < elmExceptions.numValues(); i++)
			{
				Element elmException = elmExceptions.getValueAsElement(i);
				
				m_BlmErrorMsg = elmException.getElementAsString(FIELD_ID);
				m_BlmErrorMsg += "\n";

				Element elmReason = elmException.getElement("reason");
				GetErrorMsg(elmReason, FALSE);
			}
		}
		else 
			if(securityError)
			{
				m_BlmErrorMsg = GetBlmSecArr().GetAt((int)message.correlationId().asInteger())->GetTicker();
				m_BlmErrorMsg += "\n"; 
				Element elmReason = message.getElement("reason");

				GetErrorMsg(elmReason, TRUE);
			}
			else 
				if(subscriptionCancelled)
				{
					m_BlmErrorMsg = GetBlmSecArr().GetAt((int)message.correlationId().asInteger())->GetTicker();
					m_BlmErrorMsg += "CANCELLED " + m_BlmErrorMsg + " subscription";
					ShowErrorMsg();
				}
	}
}





CBlmMktDataObj::CBlmMktDataObj()
{
	m_pSession = NULL;
	m_bReady = FALSE;
	GetOraLoader().SetDB(&theDB);
}


CBlmMktDataObj::~CBlmMktDataObj()
{
	if(IsReady())
		StopSession();

	if(m_pSession)
		delete m_pSession;
}


void CBlmMktDataObj::SetupSecurityData(int IntervalInSeconds)
{
	m_EventHandler.Initialize(IntervalInSeconds);

	/* define how to fill blmsecarray and field array */
}


BOOL CBlmMktDataObj::GetMarketData()
{
	if(!m_EventHandler.IsReady())
		return FALSE;

	if(!IsReady())
		if(!StartSession())
			return FALSE;

	if(!m_pSession->startAsync())
		return FALSE;

	return TRUE;
}


BOOL CBlmMktDataObj::StartSession()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	SessionOptions SessionOpts;
	
	m_fs.open("%FMS_DATA%\\FMSTrackMktData.log", std::fstream::out | std::fstream::app);

	SessionOpts.setServerHost("localhost");
	SessionOpts.setServerPort(8194);

	hFind = FindFirstFile("blpapi3_32.dll", &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	
	m_EventHandler.SetOutFile(&m_fs);
	m_pSession = new Session(SessionOpts, &m_EventHandler);

	if(m_pSession)
		m_bReady = TRUE;

	return m_bReady;
}


BOOL CBlmMktDataObj::StopSession()
{
	if(m_pSession)
	{
		m_pSession->stop();
		m_bReady = FALSE;
	}

	m_fs.close();

	return TRUE;
}
