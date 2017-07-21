// SysEventLog.cpp : CSysEventLog ��ʵ��

#include "stdafx.h"
#include "SysEventLog.h"
#include <time.h>
#include "ParseEventLogXML.h "

// CSysEventLog




BOOL GetUserNameForSID (TCHAR* StringSid, TCHAR* strUserName, long nUserNameLen)
{
	BOOL bRet = FALSE;
	DWORD dwSize = 0;
	SID_NAME_USE SidType;
	TCHAR lpName[MAX_PATH];
	TCHAR lpDomain[MAX_PATH];
	PSID pSID = NULL;

	if (StringSid == NULL || strUserName == NULL || nUserNameLen <= 0)
	{
		return bRet;
	}

	// Convert String to SID 

	ConvertStringSidToSid(StringSid,&pSID);

	// Lookup the account name and print it.
	if (pSID != NULL)
	{

		dwSize = MAX_PATH;
		if( LookupAccountSid( NULL, pSID,
			lpName, &dwSize, lpDomain, 
			&dwSize, &SidType ) ) 
		{
			bRet = TRUE;

			_tcscpy_s ( strUserName, nUserNameLen, lpName);
		}

	}

	if (pSID)
	{
		LocalFree(pSID);
	}

	return bRet;
}

DWORD GetEventClass(ATL::CString EventName)
{
	DWORD eventclass = 0xffffffff;
	if (EventName.CompareNoCase(_T("Security")) == 0)
	{
		eventclass = 0;
	}
	else if (EventName.CompareNoCase(_T("System")) == 0)
	{
		eventclass = 1;
	}
	else if (EventName.CompareNoCase(_T("Application")) == 0)
	{
		eventclass = 2;
	}
	else if (EventName.CompareNoCase(_T("Setup")) == 0)
	{
		eventclass = 3;
	}

	return eventclass;
}


BOOL CSysEventLog::ReadSystemEventInfo(TCHAR* eventLogSourceName)
{
	//setlocale(LC_ALL,"Chinese-simplified");	// ���õ�ǰ�ĳ���Ϊ��������

	if(m_hSysInfo == NULL)
	{
		return FALSE;
	}

	DWORD	read_len;	// ��ȡ��Ϣ����
	DWORD	next_len;	// ��һ����Ϣ
	char	Data[BUFFER_SIZE] = {0};	// �¼��鿴����Ϣ
	int	iNum = 0;
	tagSystemEventLogInfo	tagSysEventLogInfo;	// ϵͳ�¼���־���

	while(ReadEventLog(m_hSysInfo,
		EVENTLOG_BACKWARDS_READ |	// ��ǰ��
		EVENTLOG_SEQUENTIAL_READ,	// ѭ���
		0,Data,sizeof(Data),&read_len,&next_len))
		//while(ReadEventLog(m_hSysInfo,
		//	EVENTLOG_FORWARDS_READ |	// ��ǰ��
		//	EVENTLOG_SEEK_READ,	// ��������
		//	(*nLogIndex),Data,sizeof(Data),&read_len,&next_len))
	{

		for (unsigned int i = 0; i < read_len;)
		{

			EVENTLOGRECORD *ptr = (EVENTLOGRECORD *)(Data + i);

			//	����״̬
			tagSysEventLogInfo.Sys_EventClass = GetEventClass(eventLogSourceName);


			//int iEventType = GetCategory(ptr->EventType, strEventCategory);

			UINT	uOffset = 0;	// ƫ����
			uOffset = sizeof(EVENTLOGRECORD);

			// ����
			tagSysEventLogInfo.Sys_EventType = ptr->EventType;

			// ����

			//time_t tm_t(ptr->TimeWritten);
			//tm *event_time = localtime(&tm_t);
			////time = (CTime)tm;	
			//tagSysEventLogInfo.Sys_EventDate.Format(_T("%.4d-%.2d-%.2d"),event_time->tm_year+1900,event_time->tm_mon+1,event_time->tm_mday);

			//// ʱ��	
			//tagSysEventLogInfo.Sys_EventTime.Format(_T("%.2d:%.2d:%.2d"),event_time->tm_hour,event_time->tm_min,event_time->tm_sec);
			tagSysEventLogInfo.Sys_EventTime = ptr->TimeGenerated;

			// �¼�ID	
			//tagSysEventLogInfo.Sys_EventID.Format(_T("%u"),(unsigned short)ptr->EventID);
			tagSysEventLogInfo.Sys_EventID = (0x0000FFFF & ptr->EventID);

			// �¼���Դ
			tagSysEventLogInfo.Sys_EventSource.Format(_T("%s"),(LPTSTR)(LPBYTE)ptr + uOffset/sizeof(TCHAR));
			uOffset += wcslen(tagSysEventLogInfo.Sys_EventSource) * sizeof(TCHAR) + sizeof(TCHAR);

			// �����
			tagSysEventLogInfo.Sys_ComputerName.Format(_T("%s"),(LPTSTR)(LPBYTE)ptr + uOffset/sizeof(TCHAR));
			uOffset += wcslen(tagSysEventLogInfo.Sys_ComputerName) + sizeof(TCHAR);

			// �û�	
			if (ptr->UserSidLength > 0)
			{	
				TCHAR	lpszRefDomainName[MAX_PATH + 1];
				TCHAR	lpszUserName[MAX_PATH + 1];
				SID_NAME_USE _SidNameUse = (SID_NAME_USE)(SidTypeUser - 1);
				PSID	pUserSID = 0;
				DWORD	cbName;
				DWORD	cbRefDomainName;
				BOOL	bRetVal = FALSE;
				pUserSID = (SID *)GlobalAlloc(GPTR,ptr->UserSidLength);
				memcpy(pUserSID,(PSID)((LPBYTE)ptr + ptr->UserSidOffset),ptr->UserSidLength);	

				cbName = cbRefDomainName = MAX_PATH + 1;	
				*lpszRefDomainName = *lpszUserName = '\0';	

				bRetVal = LookupAccountSid(0,pUserSID,lpszUserName,&cbName,lpszRefDomainName,
					&cbRefDomainName,&_SidNameUse);	
				if (bRetVal)
				{
					tagSysEventLogInfo.Sys_ComputerUser.Format(_T("%s/%s"),lpszRefDomainName,lpszUserName);
				}
				else
				{
					tagSysEventLogInfo.Sys_ComputerUser = _T("N/A");
				}
				GlobalFree((TCHAR*)pUserSID );

			}
			else
			{
				tagSysEventLogInfo.Sys_ComputerUser = _T("N/A");
			}

			// ����
			UINT	uSize = 0;	// �ı���С
			UINT	uStringOffset;	// ����ƫ����	
			uStringOffset = ptr->StringOffset;
			uSize = ptr->DataOffset - ptr->StringOffset;
			if (uSize > 0)
			{	
				LPBYTE	pStrings = 0;	// ��¼�¼�
				TCHAR	*szExpandedString = 0;	// �����ı�
				UINT	uStepOfString = 0;	// ƫ����	

				pStrings = (LPBYTE)GlobalAlloc(GPTR,uSize * sizeof(BYTE) + 1);
				memcpy(pStrings,(TCHAR *)ptr + uStringOffset/sizeof(TCHAR),uSize);
				pStrings[uSize] = '\0';

				uStepOfString = 0;
				szExpandedString = (TCHAR *)GlobalAlloc(GPTR,(uSize + MAX_MSG_LENGTH) * sizeof(TCHAR) + 1);
				for (UINT i = 0; i < ptr->NumStrings; i++)
				{
					if (i == 0)
					{	
						wcscpy(szExpandedString,(TCHAR *)pStrings + uStepOfString);	
						if (i < (UINT)ptr->NumStrings - 1)
						{
							wcscat(szExpandedString,_T(" "));
						}	
					}
					else
					{
						wcscat(szExpandedString,(TCHAR *)pStrings + uStepOfString);
						if (i < (UINT)ptr->NumStrings - 1)
						{
							wcscat(szExpandedString,_T(" "));
						}	
					}
					uStepOfString += (wcslen((TCHAR *)pStrings + uStepOfString) + 1);
				}	
				tagSysEventLogInfo.Sys_EventDesc.Format(_T("%s"),szExpandedString);

				GlobalFree(szExpandedString );
				GlobalFree((TCHAR*)pStrings );

			}

			i += ptr->Length;
			m_SysEventInfo.push_back(tagSysEventLogInfo);	
		}

	}	

	return TRUE;	
}


BOOL CSysEventLog::StartEventLog(TCHAR* eventLogSourceName)
{
	BOOL bRet = TRUE;
	HANDLE hToken = NULL;

	//����Ȩ��
	bRet = OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken);
	if(!bRet)
	{
		return bRet;
	}
	SetPrivilege( hToken, SE_SECURITY_NAME, TRUE);

	if (_tcsicmp(eventLogSourceName, _T("Setup")) == 0)
	{
#ifndef TDHXKJ_VERSION_XP2K
		//Setup�����־
		if (IsVistaOrLater())
		{
			if (!m_bLoadSetupEventlog)
			{



				if(!GetEventInfo(eventLogSourceName))
				{
					bRet = FALSE;
				}
				SetSubscription();
				m_bLoadSetupEventlog = TRUE;


			}
		}
		else
		{
			bRet = FALSE;
		}
#endif

	}
	else
	{
		// ���¼���־
		m_hSysInfo = OpenEventLog(NULL,eventLogSourceName);
		if (NULL == m_hSysInfo)
		{
			DWORD dwError = GetLastError();
			return FALSE;
		}

		// ��ȡϵͳ�¼���־��Ϣ
		if(!ReadSystemEventInfo(eventLogSourceName))
		{

			bRet = FALSE;
		}

		// �����¼���־���
		CloseEventLog(m_hSysInfo);
		m_hSysInfo = NULL;
	}

	return bRet;
}

STDMETHODIMP CSysEventLog::GetEventList(LONG Index, LONG* EventLog)
{
	// TODO: �ڴ����ʵ�ִ���
	if (EventLog == NULL || Index < 0)
	{
		return S_FALSE;
	}

	tagSystemEventLogInfo* tagSysName = (tagSystemEventLogInfo*)EventLog;

	tagSystemEventLogInfo tagSys = m_SysEventInfo.at(Index);
	tagSysName->Sys_EventClass = tagSys.Sys_EventClass;
	tagSysName->Sys_ComputerName = tagSys.Sys_ComputerName;
	tagSysName->Sys_ComputerUser = tagSys.Sys_ComputerUser;
	tagSysName->Sys_EventDesc = tagSys.Sys_EventDesc;
	tagSysName->Sys_EventID = tagSys.Sys_EventID;
	tagSysName->Sys_EventSource = tagSys.Sys_EventSource;
	tagSysName->Sys_EventTime = tagSys.Sys_EventTime;
	tagSysName->Sys_EventType = tagSys.Sys_EventType;


	return S_OK;
}


STDMETHODIMP CSysEventLog::GetEventLogSize(BSTR strFileName, LONG* nLogNum)
{
	// TODO: �ڴ����ʵ�ִ���
	if (strFileName == NULL || nLogNum == NULL)
	{
		return S_FALSE;
	}

	//TCHAR eventLogSourceName[256] = _T("System");
	TCHAR eventLogSourceName[256] = {0};

	_tcscpy_s(eventLogSourceName, strFileName);


	if(!StartEventLog(eventLogSourceName))
	{
		return S_FALSE;
	}

	(*nLogNum) = m_SysEventInfo.size();

	return S_OK;
}

BOOL CSysEventLog::SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
	)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if ( !LookupPrivilegeValue( 
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid ) )        // receives LUID of privilege
	{
		return FALSE; 
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if ( !AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL) )
	{
		return FALSE; 
	} 

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		return FALSE;
	} 

	return TRUE;
}

STDMETHODIMP CSysEventLog::ClearEventLogList()
{
	//����б�
	m_SysEventInfo.clear();	// �����������Ԫ��

	return 0;
}

#ifndef TDHXKJ_VERSION_XP2K

BOOL CSysEventLog::GetEventInfo(TCHAR* chChannelName)
{
	BOOL bReturn = FALSE;

	EVT_HANDLE hQueryResult = NULL;      // Handle to the loq query results.
	TCHAR *szChannel = NULL;			// Channel.
	TCHAR *szQuery   = L"*";           // XPATH Query (Level 2 events).

	if (chChannelName == NULL)
	{
		return bReturn;
	}

	szChannel = chChannelName;

	// Query the log.
	hQueryResult = EvtQuery(NULL, //session
		szChannel,                   //Path (channel)
		szQuery,                     //Query
		EvtQueryChannelPath | EvtQueryReverseDirection          //Flags
		);
	if( !hQueryResult )
	{
		return bReturn;
	}

	EVT_HANDLE hEvent = NULL;  // Events array.
	DWORD dwReturned = 0;      // Number of events returned.
	DWORD dwNumofEvents = 0;

	// Retrieve each event in the QueryResult and display the 
	// results on the console.
	while (EvtNext( 
		hQueryResult,   // QueryResult.
		1,              // BatchSize.
		&hEvent,        // EventArray.
		QUERY_TIMEOUT,  // TimeOut.
		0,              // dwFlags.
		&dwReturned) )  // Returned.
	{
		WCHAR *pBuff = NULL;         //Buffer.
		DWORD dwBuffSize = 0;        //BufferSize.
		DWORD dwBuffUsed = 0;        //Buffersize used or required.
		DWORD dwPropertyCount = 0;

		//Get the buffer size needed to render the event.
		BOOL bRet = EvtRender(  
			NULL,               // Context.
			hEvent,             // HANDLE.
			EvtRenderEventXml,  // Flags.                                                
			dwBuffSize,         // BufferSize.
			pBuff,              //Buffer
			&dwBuffUsed,        //Buffersize used or required.
			&dwPropertyCount);

		if (!bRet)
		{
			DWORD dwRes = GetLastError();
			if( dwRes == ERROR_INSUFFICIENT_BUFFER )
			{
				//Allocate the BufferSize needed
				dwBuffSize = dwBuffUsed;
				pBuff = new WCHAR[dwBuffSize/sizeof(WCHAR)];

				//Render the Event
				bRet = EvtRender(   
					NULL,               // Context.
					hEvent,             // HANDLE.
					EvtRenderEventXml,  // Flags.                                                
					dwBuffSize,         // BufferSize.
					pBuff,              // Buffer
					&dwBuffUsed,        // Buffersize used or required.
					&dwPropertyCount);

				if( !bRet )
				{
					if (pBuff != NULL)
					{
						delete[] pBuff;
						pBuff = NULL;
					}

					if (hQueryResult != NULL)
					{
						EvtClose(hQueryResult);
						hQueryResult = NULL;
					}
					if (hEvent != NULL)
					{
						EvtClose(hEvent);
						hEvent = NULL;
					}
					return bReturn;
				}

				//Display the Event XML on console
				//�������XML��ʽ����־��Ϣ
				bReturn = TRUE;
				ParseEventXML(pBuff);


				//����ѻ���
				if (pBuff != NULL)
				{
					delete[] pBuff;
					pBuff = NULL;
				}
			}
			else
			{

				if (hQueryResult != NULL)
				{
					EvtClose(hQueryResult);
					hQueryResult = NULL;
				}
				if (hEvent != NULL)
				{
					EvtClose(hEvent);
					hEvent = NULL;
				}
				return bReturn;
			}
		}

		if (hEvent != NULL)
		{
			EvtClose(hEvent);
			hEvent = NULL;
		}
	}

	//Close the Handles
	if (hQueryResult != NULL)
	{
		EvtClose(hQueryResult);
		hQueryResult = NULL;
	}

	return bReturn;
}

BOOL CSysEventLog::IsVistaOrLater()
{
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	return ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 0));
}

BOOL CSysEventLog::ParseEventXML(TCHAR* pBuff)
{
	BOOL bRet = FALSE;

	if (pBuff == NULL)
	{
		return bRet;
	}

	CParseEventLogXML eventlog;

	if(!eventlog.LoadFromString(pBuff))
	{
		tagSystemEventLogInfo	tagSysEventLogInfo;
		//���سɹ�����ʼ�����ַ���
		if(eventlog.ParseSetupEventLog(tagSysEventLogInfo))
		{
			m_SysEventInfo.push_back(tagSysEventLogInfo);
		}

	}

	return bRet;
}

DWORD WINAPI CSysEventLog::SubscriptionCallBack( 
	EVT_SUBSCRIBE_NOTIFY_ACTION Action, 
	PVOID Context, 
	EVT_HANDLE Event )
{

	CSysEventLog* pThis = (CSysEventLog*)Context;
	if (pThis == NULL)
	{
		return 0;
	}

	WCHAR *pBuff = NULL;
	DWORD dwBuffSize = 0;
	DWORD dwBuffUsed = 0;
	DWORD dwRes = 0;
	DWORD dwPropertyCount = 0;

	// Get the XML EventSize to allocate the buffer size.
	BOOL bRet = EvtRender(  
		NULL,                // Session.
		Event,               // HANDLE.
		EvtRenderEventXml,   // Flags.                                              
		dwBuffSize,          // BufferSize.
		pBuff,               // Buffer.
		&dwBuffUsed,         // Buffersize that is used or required.
		&dwPropertyCount);

	if (!bRet)
	{
		dwRes = GetLastError();
		if( dwRes == ERROR_INSUFFICIENT_BUFFER )
		{
			// Allocate the buffer size needed to for the XML event.
			dwBuffSize = dwBuffUsed;
			pBuff = new WCHAR[dwBuffSize/sizeof(WCHAR)];

			//Get the Event XML
			bRet = EvtRender(   
				NULL,                 // Session.
				Event,                // HANDLE.
				EvtRenderEventXml,    // Flags.                                              
				dwBuffSize,           // BufferSize.
				pBuff,                // Buffer.
				&dwBuffUsed,          // Buffer size that is used or required.
				&dwPropertyCount);

			if( !bRet )
			{
				if (pBuff != NULL)
				{
					delete[] pBuff;
					pBuff = NULL;
				}

				return dwRes;
			}
			else
			{
				//����setup��־��XML�ַ���
				pThis->ParseEventXML(pBuff);

				//֪ͨ�¼�
				HANDLE hEvent = ::CreateEvent(NULL,FALSE,FALSE,_T("Global\\HXSetupEvent"));
				if(NULL != hEvent)
				{
					::SetEvent(hEvent);
					CloseHandle(hEvent);
				}
			}
		}
	}

	//Cleanup
	if (pBuff != NULL)
	{
		delete[] pBuff;
		pBuff = NULL;
	}

	return dwRes;
}


BOOL CSysEventLog::SetSubscription()
{
	BOOL bRet = TRUE;


	wchar_t *szChannel = L"Setup";      // Channel.
	wchar_t *szQuery = L"*";  // XPATH Query to specify which events to subscribe to.

	// Register the subscription.
	m_hSub = EvtSubscribe(  NULL,  // Session
		NULL,                    // Used for pull subscriptions.
		szChannel,               // Channel.
		szQuery,                 // XPath query.
		NULL,                    // Bookmark.
		this,              // Callback context.
		(EVT_SUBSCRIBE_CALLBACK) SubscriptionCallBack,  // Callback.
		EvtSubscribeToFutureEvents   // Flags.
		);

	if( !m_hSub )
	{
		return FALSE;
	}  

	// Close the subscriber handle.
	//EvtClose(hSub);

	return bRet;
}

#endif
