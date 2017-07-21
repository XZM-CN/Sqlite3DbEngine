// SysEventLog.h : CSysEventLog 的声明

#pragma once
#include "resource.h"       // 主符号



#include "LogEngine_i.h"


#include <vector>
#include "HXVersionDef.h"


#ifndef TDHXKJ_VERSION_XP2K
#include <winevt.h> //EventLog Header
# pragma comment(lib, "wevtapi.lib")
#endif

#include "EventStructDef.h"

#define  MAX_MSG_LENGTH 4096
#define  BUFFER_SIZE 4096*4
#define QUERY_TIMEOUT 5000



using namespace std;

typedef std::vector<tagSystemEventLogInfo> SYS_EVENT;



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CSysEventLog

class ATL_NO_VTABLE CSysEventLog :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSysEventLog, &CLSID_SysEventLog>,
	public IDispatchImpl<ISysEventLog, &IID_ISysEventLog, &LIBID_LogEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSysEventLog()
	{
		m_hSysInfo = NULL;
#ifndef TDHXKJ_VERSION_XP2K
		m_hSub = NULL;
#endif
		m_bLoadSetupEventlog = FALSE;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
#ifndef TDHXKJ_VERSION_XP2K
		if (m_hSub != NULL)
		{
			EvtClose(m_hSub);
		}
#endif
		return S_OK;
	}

	void FinalRelease()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SYSEVENTLOG)

	DECLARE_NOT_AGGREGATABLE(CSysEventLog)

	BEGIN_COM_MAP(CSysEventLog)
		COM_INTERFACE_ENTRY(ISysEventLog)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

private:
	HANDLE  m_hSysInfo;

#ifndef TDHXKJ_VERSION_XP2K
	EVT_HANDLE m_hSub;
#endif

	BOOL m_bLoadSetupEventlog;
public:
	SYS_EVENT m_SysEventInfo;

	// ISysEventLog
public:
	STDMETHOD(GetEventList)(LONG Index, LONG* EventLog);
	STDMETHOD(GetEventLogSize)(BSTR strFileName, LONG* nLogNum);
	STDMETHODIMP ClearEventLogList();

	BOOL ParseEventXML(TCHAR* pBuff);

#ifndef TDHXKJ_VERSION_XP2K
	static DWORD WINAPI SubscriptionCallBack(EVT_SUBSCRIBE_NOTIFY_ACTION Action,PVOID Context,EVT_HANDLE Event);
#endif

private:


	BOOL StartEventLog(TCHAR* eventLogSourceName);
	BOOL ReadSystemEventInfo(TCHAR* eventLogSourceName);
	BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege); 
	BOOL GetEventInfo(TCHAR* chChannelName);	//获取Setup日志信息，支持VISTA之后的操作系统
	BOOL IsVistaOrLater();


	BOOL SetSubscription();

};

OBJECT_ENTRY_AUTO(__uuidof(SysEventLog), CSysEventLog)
