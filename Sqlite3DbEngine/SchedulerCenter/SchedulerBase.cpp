#include "StdAfx.h"
#include "SchedulerBase.h"


CSchedulerBase::CSchedulerBase(void)
{
}


CSchedulerBase::CSchedulerBase(CString strModulerName)
{
	m_strModulerName = strModulerName;
}

CSchedulerBase::~CSchedulerBase(void)
{
}

HINSTANCE CSchedulerBase::CreateInstance(LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv)
{
	HINSTANCE hInst = ::LoadLibrary( lpszFilename ); 
	if(hInst == NULL)
	{
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			ATLASSERT( hInst );
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("创建对象时载入 %s 失败，错误码：%ld"),\
				lpszFilename,dwErrCode);
			WRITELOGTOFILE(strLogInfo);
#endif
		}
		return hInst;
	}
	CreateInterface(hInst,clsid,iid,ppv);
	if(NULL == *ppv)
	{
		FreeLibrary(hInst);
		hInst= NULL;
		return hInst;	
	}
	return hInst;
}

void CSchedulerBase::CreateInterface(HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv)
{
	if(hInst == NULL)
		return;

	LPGetClassObject * lp = (LPGetClassObject *)::GetProcAddress( hInst, "DllGetClassObject" );
	ATLASSERT( lp );
	if(lp == NULL)
		return;	

	CComPtr<IClassFactory>	cf = NULL;
	(*lp)( clsid, __uuidof(IClassFactory), (LPVOID *)&cf );
	if(cf == NULL)
		return;
	cf->CreateInstance( NULL, iid, ppv );
	cf = NULL;
}

CComPtr<IDispatch> CSchedulerBase::GetPtr(LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid)
{
	CComPtr<IDispatch> Ptr;
	CreateInstance(lpszFilename,clsid,iid,(VOID **)&Ptr);
	return Ptr;
}

CString CSchedulerBase::GetPtrPath(LPCTSTR lpszFilename)
{
	CString strModulePath = GetModulePath();

	strModulePath = strModulePath + lpszFilename;
	return _T("");
}

ATL::CString CSchedulerBase::GetModulePath(HANDLE hModule/* = NULL*/)
{
	TCHAR szPath[MAX_PATH];
	::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	::GetModuleFileName((HMODULE )hModule,szPath,MAX_PATH);
	ATL::CString strPath(szPath);

	int iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind+1);
	return strPath;
}