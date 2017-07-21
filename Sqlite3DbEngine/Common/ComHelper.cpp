#include "stdafx.h"
#include "ComHelper.h"
#include "BaseFuncLib.h"

STRINGHANDLE_MAP CComHelper::m_mapInstance;
SHORT            CComHelper::m_nCodeProtected = -1;

CString CComHelper::GetComTempPath(BOOL bCreateFlag /*= TRUE*/)
{
	TCHAR szPath[MAX_PATH];
	memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	::GetTempPath(MAX_PATH,szPath);
	CString strPath = szPath;
	if(0 != strPath.Right(1).CollateNoCase(_T("\\")))
		strPath += _T("\\");
	strPath += PRODUCT_REGNODENAME;
	if(bCreateFlag)
		CBaseFuncLib::CreatePath(strPath);
	strPath += _T("\\");
	return strPath;
}

CString CComHelper::GetAppInsPath()
{
	CString strInsPath;
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	regKey.GetRegStringVal(PRODUCT_COMMAN_INSPATH,strInsPath);
	if(strInsPath.IsEmpty())
		CBaseFuncLib::GetAppDir();
	return strInsPath;
}

HINSTANCE CComHelper::GetComHandle(const ATL::CString& strModelName)
{
	HINSTANCE hHandle = NULL;
	STRINGHANDLE_MAP::iterator it = m_mapInstance.find(strModelName);
	if(it == m_mapInstance.end())
		return hHandle;///不存在
	hHandle = it->second;
	return hHandle;
}

void CComHelper::FreeHandle(const ATL::CString& strModelName)
{
	HINSTANCE hHandle = GetComHandle(strModelName);
	if(NULL == hHandle)
		return;
	::FreeLibrary(hHandle);
	hHandle = NULL;
}

void CComHelper::FreeAllHandle()
{
	STRINGHANDLE_MAP::iterator it = m_mapInstance.begin();
	while(it != m_mapInstance.end())
	{
		HINSTANCE hHandle = it->second;
		if(NULL != hHandle)
		{
			::FreeLibrary(hHandle);
			hHandle = NULL;
		}
		it++;
	}
	m_mapInstance.clear();
}

SHORT CComHelper::GetSourceProtected()
{
#ifdef _DEBUG
	return 0;/// 代码不保护
#endif

	if(-1 != m_nCodeProtected)
		return m_nCodeProtected;
#ifdef APP_COPYRIGHT_CODE
	ATL::CString strDeviceID(_T(""));
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	if(!regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID))
		return 0;
	if(0 == strDeviceID.CompareNoCase(_T("C2927BBBA9FABC620B2764DAFFE832A8")))
		m_nCodeProtected = 0;/// 代码不保护
	else 
		m_nCodeProtected = 1;
#else
	m_nCodeProtected = 0;/// 代码不保护
#endif
	return m_nCodeProtected;
}
