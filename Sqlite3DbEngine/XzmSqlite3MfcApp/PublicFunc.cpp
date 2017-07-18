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

BOOL CPublicFunc::IsPathExist(const CString& strPath) throw()
{
	BOOL bRet = FALSE;
	if(strPath.IsEmpty())
		return bRet;
	WIN32_FIND_DATA	data;
	BOOL bFindDir = FALSE;

	CString strFind(strPath);
	if (0 == strPath.Right(1).CompareNoCase( _T("\\")))
	{
		/// 查找目录
		bFindDir = TRUE;
		strFind+=_T("*.*");
	}
	
	/**
	 * @brief             FindFirstFile（）
	 *
	 * @Function          
	 *
	 * @param[  _In_   ]  
	 *
	 * @param[  _Out_  ]  
	 *
	 * @param[_Out_opt_]  
	 *
	 * @return            如果函数成功，则得到一个句柄，否则失败将得到值INVALID_HANDLE_VALUE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	HANDLE hFindFile = ::FindFirstFile(strFind,&data);
	if(INVALID_HANDLE_VALUE != hFindFile)
	{
		if(bFindDir)
		{
			while(INVALID_HANDLE_VALUE != hFindFile)
			{
				if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY )
				{
					
					bRet = TRUE;
					break;
				}
				/**
				 * @brief             FindNextFile
				 *
				 * @Function          
				 *
				 * @param[  _In_   ]  
				 *
				 * @param[  _Out_  ]  
				 *
				 * @param[_Out_opt_]  
				 *
				 * @return            如果函数成功，则返回值不为零，lpFindFileData 参数包含有关找到的下一个文件或目录的信息。
				 *                    如果函数失败，则返回值为零，并且lpFindFileData的内容 是不确定的。要获取扩展错误信息，请调用 GetLastError函数。
				 *
				 * @Date xzm_@_2017/06/30  13:40:11
				*/
				if (!FindNextFile(hFindFile,&data))
					break;
			}
		}
		else
			bRet = TRUE;
		::FindClose(hFindFile);
		hFindFile = NULL;
	}
	else
		bRet=FALSE;
	return bRet;
}
