#include "stdafx.h"
#include "BaseFuncLib.h"
#include "BaseHelper.h"

/// È«¾Ö¾ä±ú
extern HANDLE g_hInstance;

ATL::CString CBaseHelper::m_strModelName = TDHXKJ_BASEENGINEDLL;

CComPtr <INetRequest> CBaseHelper::GetNetRequest()
{
	CComPtr <INetRequest> spiNetRequest = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(NetRequest),__uuidof(INetRequest),(VOID **)&spiNetRequest);
		ATLASSERT(spiNetRequest);
		return spiNetRequest;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),m_strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),m_strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),m_strModelName);
	}
	if(GetSourceProtected())
	{
		/// ·À·¶ÆÆ½â
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*200)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(NetRequest),__uuidof(INetRequest),(VOID **)&spiNetRequest);
	if(NULL != hHandle)
		m_mapInstance[m_strModelName] = hHandle;
	strDllPath.Empty();
	ATLASSERT(spiNetRequest);
	return spiNetRequest;
}

CComPtr <IAccountMgr> CBaseHelper::GetAccountMgr()
{
	CComPtr <IAccountMgr> spiAccountMgr = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(AccountMgr),__uuidof(IAccountMgr),(VOID **)&spiAccountMgr);
		ATLASSERT(spiAccountMgr);
		return spiAccountMgr;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),m_strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),m_strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),m_strModelName);
	}
	if(GetSourceProtected())
	{
		/// ·À·¶ÆÆ½â
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(AccountMgr),__uuidof(IAccountMgr),(VOID **)&spiAccountMgr);
	if(NULL != hHandle)
		m_mapInstance[m_strModelName] = hHandle;
	strDllPath.Empty();
	ATLASSERT(spiAccountMgr);
	return spiAccountMgr;
}

CComPtr <IServiceControl> CBaseHelper::GetServiceControl()
{
	CComPtr <IServiceControl> spiServiceControl = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(ServiceControl),__uuidof(IServiceControl),(VOID **)&spiServiceControl);
		ATLASSERT(spiServiceControl);
		return spiServiceControl;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),m_strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),m_strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),m_strModelName);
	}
	if(GetSourceProtected())
	{
		/// ·À·¶ÆÆ½â
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(ServiceControl),__uuidof(IServiceControl),(VOID **)&spiServiceControl);
	if(NULL != hHandle)
		m_mapInstance[m_strModelName] = hHandle;
	strDllPath.Empty();
	ATLASSERT(spiServiceControl);
	return spiServiceControl;
}

CComPtr <ISysEventLog> CBaseHelper::GetSysEventLog()
{
	CComPtr <ISysEventLog> spiSysEventLog = NULL;
	HINSTANCE hHandle = GetComHandle(TDHXKJ_GETSYSEVENTLOGDLL);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(SysEventLog),__uuidof(ISysEventLog),(VOID **)&spiSysEventLog);
		ATLASSERT(spiSysEventLog);
		return spiSysEventLog;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),TDHXKJ_GETSYSEVENTLOGDLL);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),TDHXKJ_GETSYSEVENTLOGDLL);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),TDHXKJ_GETSYSEVENTLOGDLL);
	}
	if(GetSourceProtected())
	{
		/// ·À·¶ÆÆ½â
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(SysEventLog),__uuidof(ISysEventLog),(VOID **)&spiSysEventLog);
	if(NULL != hHandle)
		m_mapInstance[TDHXKJ_GETSYSEVENTLOGDLL] = hHandle;
	strDllPath.Empty();
	ATLASSERT(spiSysEventLog);
	return spiSysEventLog;
}