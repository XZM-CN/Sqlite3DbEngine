#include "stdafx.h"
#include "SKFHelper.h"
#include "BaseFuncLib.h"

/// È«¾Ö¾ä±ú
extern HANDLE g_hInstance;

ATL::CString CSKFHelper::m_strModelName = TDHXKJ_ENCRYENGINEDLL;

static const char hex_chars[] = "0123456789ABCDEF";

CString convert_hex(unsigned char *md,int nLen)  
{  
	CString strSha1(_T(""));
	int i = 0;
	unsigned int c = 0;

	for (i = 0; i < nLen; i++)
	{
		c = (md[i] >> 4) & 0x0f;
		strSha1 += hex_chars[c];
		strSha1 += hex_chars[md[i] & 0x0f];
	}
	return strSha1;
} 

CComPtr <ISafeCard> CSKFHelper::GetSafeCard()
{
	CComPtr <ISafeCard> spiSafeCard = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(SafeCard),__uuidof(ISafeCard),(VOID **)&spiSafeCard);
		ATLASSERT(spiSafeCard);
		return spiSafeCard;
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
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(SafeCard),__uuidof(ISafeCard),(VOID **)&spiSafeCard);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiSafeCard);
	return spiSafeCard;
}

CComPtr <ICryptoStor> CSKFHelper::GetCryptoStor()
{
	CComPtr <ICryptoStor> spiCryptoStor = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(CryptoStor),__uuidof(ICryptoStor),(VOID **)&spiCryptoStor);
		ATLASSERT(spiCryptoStor);
		return spiCryptoStor;
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
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(CryptoStor),__uuidof(ICryptoStor),(VOID **)&spiCryptoStor);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiCryptoStor);
	return spiCryptoStor;
}

CComPtr <ISafeService> CSKFHelper::GetSafeService()
{
	CComPtr <ISafeService> spiSafeService = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(SafeService),__uuidof(ISafeService),(VOID **)&spiSafeService);
		ATLASSERT(spiSafeService);
		return spiSafeService;
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
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(SafeService),__uuidof(ISafeService),(VOID **)&spiSafeService);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiSafeService);
	return spiSafeService;
}

CComPtr <IWMIDevice> CSKFHelper::GetWMIDevice()
{
	CComPtr <IWMIDevice> spiWMIDevice = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(WMIDevice),__uuidof(IWMIDevice),(VOID **)&spiWMIDevice);
		ATLASSERT(spiWMIDevice);
		return spiWMIDevice;
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
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(WMIDevice),__uuidof(IWMIDevice),(VOID **)&spiWMIDevice);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiWMIDevice);
	return spiWMIDevice;
}

CComPtr <IUSBDevice> CSKFHelper::GetUSBDevice()
{
	CComPtr <IUSBDevice> spiUSBDevice = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(USBDevice),__uuidof(IUSBDevice),(VOID **)&spiUSBDevice);
		ATLASSERT(spiUSBDevice);
		return spiUSBDevice;
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
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(USBDevice),__uuidof(IUSBDevice),(VOID **)&spiUSBDevice);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiUSBDevice);
	return spiUSBDevice;
}