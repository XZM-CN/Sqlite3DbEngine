#include "StdAfx.h"
#include "PublicFunc.h"


CPublicFunc::CPublicFunc(void)
{
}


CPublicFunc::~CPublicFunc(void)
{
}

void CPublicFunc::CreateInterface(HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv)
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

HINSTANCE CPublicFunc::CreateInstance(LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv)
{
	HINSTANCE hInst = ::LoadLibrary( lpszFilename ); 
	if(hInst == NULL)
	{
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			ATLASSERT( hInst );
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

CString CPublicFunc::GetAppDirByRegistry(const CString& strRegPath /*= _T("")*/, HKEY hKey /*= HKEY_CURRENT_USER*/)
{
	return 0;
}

CString CPublicFunc::GetAppDirByModular()
{
	CString strTempPath;

	GetModuleFileName(NULL,strTempPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	return strTempPath;
}

CString CPublicFunc::GetAppTruncDirByModular()
{
	int nPos;
	CString strTempPath;
	strTempPath = CPublicFunc::GetAppDirByModular();
	strTempPath.ReleaseBuffer ();

	nPos=strTempPath.ReverseFind ('\\');
	strTempPath=strTempPath.Left (nPos);

	return strTempPath;
}
