
/**
 * @file BaseFuncLib.cpp
 *
 * Copyright (c) 2016-?,天地和兴科技
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 基础函数库实现
 *
 *
 *
 * @version 
 *
 * @date 2016-01-18
 *
 * @author WZH
 *
 * @history 
 *
 */
// BaseFuncLib.cpp : 基础函数库实现

#include "stdafx.h"
#include <shlobj.h>
#include <shellapi.h>
#include <TlHelp32.h>

#include <Sddl.h>


#include "BaseFuncLib.h"
#include "common.h"

#pragma warning( disable : 4819 )
#pragma comment(lib,"version.lib")
#pragma comment(lib, "advapi32.lib")

struct TRANSLATION 
{  
	WORD langID; // language ID  
	WORD charset;// character set (code page)  
}Translation; 

/// 全局模块句柄
HANDLE		g_hInstance = NULL;

/// 查找窗口名称
ATL::CString	CBaseFuncLib::m_strWndName(_T(""));
/// 是否可见
BOOL			CBaseFuncLib::m_bVisibleFlag = TRUE;
/// 查找的窗口句柄
HWND			CBaseFuncLib::m_hWnd = NULL;

/// 日志文件
ATL::CString	CBaseFuncLib::m_strLogFile = _T("");


// 不同进程使用的log日志名称
ATL::CString	CBaseFuncLib::m_strAppPath = _T("");

ATL::CString	CBaseFuncLib::m_strProcessName = _T("\\Log.txt");

BOOL CBaseFuncLib::IsW2K()
{
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	return ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 0));
}

BOOL CBaseFuncLib::IsVistaOrLater()
{
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	return ((osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 0));
}


BOOL CBaseFuncLib::IsWin64(HMODULE& hKernel)
{
	BOOL b64Flag = FALSE;
	if(NULL == hKernel)
		hKernel	= ::GetModuleHandle(TDHXKJ_SYSCOREDLL);
	if(NULL == hKernel)
		return b64Flag;
	lpFN_GNSI lpGetNativeSystemInfo = NULL;
	lpGetNativeSystemInfo = (lpFN_GNSI)GetProcAddress(\
		hKernel,"GetNativeSystemInfo"); 
	SYSTEM_INFO si;
	memset(&si,0,sizeof(SYSTEM_INFO));
	if(NULL != lpGetNativeSystemInfo)
	{
		lpGetNativeSystemInfo(&si);
		lpGetNativeSystemInfo = NULL;
	}
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
		b64Flag = TRUE;
	return b64Flag;
}

int CBaseFuncLib::CharToUtf8(const char* pSrc, char** pDst)
{
	char* cUTF8=NULL;
	WCHAR* wchar=NULL;
	int iLength=0;

	iLength=MultiByteToWideChar(CP_ACP,0,pSrc,-1,NULL,0);
	if(!iLength)
		return iLength;
	iLength++;
	wchar=new WCHAR[iLength];
	if(wchar==NULL)
		return 0;
	::memset(wchar,0,iLength*sizeof(WCHAR));
	::MultiByteToWideChar(CP_ACP,0,pSrc,-1,wchar,iLength-1);
	iLength=::WideCharToMultiByte(CP_UTF8,0,wchar,-1,NULL,0,NULL,NULL);
	if(!iLength)
	{
		delete []wchar;
		wchar=NULL;
		return iLength;
	}
	iLength++;
	cUTF8=new char[iLength];
	if(cUTF8!=NULL)
	{
		::memset(cUTF8,0,iLength*sizeof(char));
		iLength=::WideCharToMultiByte(CP_UTF8,0,wchar,-1,cUTF8,iLength-1,NULL,NULL);
		*pDst = cUTF8;
	}
	else
		iLength = 0;
	delete []wchar;
	wchar=NULL;
	return iLength;
}

int CBaseFuncLib::US2ToUtf8(const ATL::CString& strSrc,char** pDst)
{
	char* cUTF8=NULL;
	int iLength=0;

	iLength=::WideCharToMultiByte(CP_UTF8,0,strSrc,-1,NULL,0,NULL,NULL);
	if(!iLength)
		return iLength;
	iLength++;
	ATLTRY(cUTF8=new char[iLength]);
	if(cUTF8!=NULL)
	{
		::memset(cUTF8,0,iLength*sizeof(char));
		iLength=::WideCharToMultiByte(CP_UTF8,0,strSrc,-1,cUTF8,iLength-1,NULL,NULL);
		*pDst = cUTF8;
	}
	else
		iLength = 0;
	return iLength;
}

int CBaseFuncLib::Utf8ToChar(LPCSTR pSrc, char** pDst)
{
	WCHAR*  strW=NULL;
	int iLength=0;
    iLength=::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,NULL,0);
	if(!iLength)
		return iLength;
	iLength++;
    strW = new WCHAR[iLength];
	if(strW==NULL)
		return 0;
 	::memset(strW,0,iLength*sizeof(WCHAR));
   ::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,strW,iLength-1);
    iLength=::WideCharToMultiByte(CP_ACP,0,strW,-1,NULL,0,NULL,NULL);
 	if(!iLength)
	{
		delete []strW;
		strW=NULL;
		return 0;
	}
	iLength++;
    char *strA=NULL;
	strA=new char[iLength];
	if(strA!=NULL)
	{
	 	::memset(strA,0,iLength*sizeof(char));
		iLength = ::WideCharToMultiByte(CP_ACP,0,strW,-1,strA,iLength-1,NULL,NULL);
		*pDst=strA;
    }
	else
		iLength = 0;
	delete []strW;
	strW=NULL;
	return iLength;
}

int CBaseFuncLib::Us2ToChar(const ATL::CString& strSrc, char** pDst,int nCodePage)
{
	int iLength=::WideCharToMultiByte(nCodePage,0,strSrc,-1,NULL,0,NULL,NULL);
 	if(!iLength)
		return iLength;
	iLength++;
    char *strA=NULL;
	strA= new char[iLength];
	if(strA==NULL)
		return 0;
	::memset(strA,0,iLength*sizeof(char));
	int iLength1 = 0;
	if(nCodePage)
		iLength1 = ::WideCharToMultiByte(nCodePage,WC_COMPOSITECHECK,strSrc,-1,strA,iLength-1,NULL,NULL);
	else
		iLength1 = ::WideCharToMultiByte(nCodePage,0,strSrc,-1,strA,iLength-1,NULL,NULL);
	*pDst=strA;
	return iLength1;
}

int CBaseFuncLib::CharToUsCString(const char* strSrc, ATL::CString pDst,int nCodePage /*= CP_ACP*/)
{
	WCHAR* strW=NULL;
	CharToUS2(strSrc,&strW);
	ATL::CString str(strW);
	pDst = str;
	return 0;
}

int CBaseFuncLib::CharToUS2(const char* szSrc, WCHAR** pDst,int nCodePage)
{
	WCHAR* strW=NULL;
	int iLength=0;
	
	iLength=MultiByteToWideChar(nCodePage,0,szSrc,-1,NULL,0);
	if(!iLength)
		return iLength;
	iLength++;
	strW=new WCHAR[iLength];
	if(strW==NULL)
		return 0;
	::memset(strW,0,iLength*sizeof(WCHAR));
	int iLength1 = 0;
	if(nCodePage)
		iLength1 = ::MultiByteToWideChar(nCodePage,MB_COMPOSITE,szSrc,-1,strW,iLength-1);
	else
		iLength1 = ::MultiByteToWideChar(nCodePage,0,szSrc,-1,strW,iLength-1);
	*pDst=strW;
	return iLength1;
}

int CBaseFuncLib::Utf8ToUS2(LPCSTR pSrc, WCHAR** pDst)
{
	WCHAR *strW = NULL;
	int iLength = ::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,NULL,0);
 	if(!iLength)
		return 0;
	iLength++;
    ATLTRY(strW = new WCHAR[iLength]);
	if(NULL == strW)
		return 0;
	::memset(strW,0,iLength*sizeof(WCHAR));
    int iLength1=::MultiByteToWideChar(CP_UTF8,0,pSrc,-1,strW,iLength-1);
	*pDst=strW;
	return iLength1;
}

ATL::CString CBaseFuncLib::GBKToGB2312(const ATL::CString &strSrc)
{
	DWORD dwErrCode = 0;
	ATL::CString strDest(_T(""));
	DWORD wLanguageID = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_BIG5);
	int nLen = ::LCMapString(wLanguageID,LCMAP_SIMPLIFIED_CHINESE,strSrc,-1,NULL,0);
	WCHAR *szDest = NULL;
	if(nLen)
	{
		nLen++;
		szDest = new WCHAR[nLen];
	}
	else
		dwErrCode = ::GetLastError();
	if(NULL == szDest)
		return strDest;
	memset(szDest,0,sizeof(WCHAR)*nLen);
	nLen = ::LCMapString(wLanguageID,LCMAP_SIMPLIFIED_CHINESE,strSrc,-1,szDest,nLen); 
	if(NULL != szDest)
	{
		strDest = szDest;
		delete []szDest;
		szDest = NULL;
	}
	return strDest;
}

ATL::CString CBaseFuncLib::GB2312ToGBK(const ATL::CString &strSrc)
{
	DWORD dwErrCode = 0;
	ATL::CString strDest(_T(""));
	DWORD wLanguageID = MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRC);
	int nLen = ::LCMapString(wLanguageID,LCMAP_TRADITIONAL_CHINESE,strSrc,-1,NULL,0);
	WCHAR *szDest = NULL;
	if(nLen)
	{
		nLen++;
		szDest = new WCHAR[nLen];
	}
	else
		dwErrCode = ::GetLastError();
	if(NULL == szDest)
		return strDest;
	memset(szDest,0,sizeof(WCHAR)*nLen);
	nLen = ::LCMapString(wLanguageID,LCMAP_TRADITIONAL_CHINESE,strSrc,-1,szDest,nLen); 
	if(NULL != szDest)
	{
		strDest = szDest;
		delete []szDest;
		szDest = NULL;
	}
	return strDest;
}

BYTE CBaseFuncLib::toHex(const BYTE &x)
{
   return x > 9 ? x + 55: x + 48;
}

ATL::CString CBaseFuncLib::NumToStr(LONGLONG i)
{
	ATL::CString strTemp(_T(""));	
	if (i == 0)
	{
		strTemp = _T("0");
		return strTemp;
	}
	LONGLONG dwNumber = i;
	while (dwNumber)
	{
		TCHAR Temp = (TCHAR)(dwNumber % 10 + '0');
		strTemp.Insert(0,Temp);
		dwNumber = dwNumber / 10;
	}
	return strTemp;
}

DWORD CBaseFuncLib::StrToNum(const ATL::CString& strTemp)
{
	DWORD dwValue = 0;
	if(strTemp.IsEmpty())
		return dwValue;

	for (int j = 0; j < strTemp.GetLength(); j++)
	{
		TCHAR c = strTemp[j];
		if (c <= '9' && c >= '0')
		{
			dwValue *= 10;
			dwValue = (dwValue + (c - '0')) ; 
		}
	}
	return dwValue;
}

void CBaseFuncLib::StrToNum2(const ATL::CString& strTemp,DWORD &dwNum1,DWORD &dwNum2)
{
	dwNum1 =0;
	dwNum2 =0;
	if(strTemp.IsEmpty())
		return;

	DWORD dwValue = 0;
	int	j=0,nLen = strTemp.GetLength();
	for (j = 0; j < nLen; j++)
	{
		TCHAR c = strTemp[j];
		if (c <= '9' && c >= '0')
		{
			dwValue *= 10;
			dwValue = (dwValue + (c - '0')) ; 
		}
		if(nLen-8 == j)
		{
			dwNum1 = dwValue;
			dwValue = 0;
		}
	}
	if(dwValue)
		dwNum2 = dwValue;
}

void CBaseFuncLib::ChangeToChar(byte *szSrc,int nLen,char *szDest)
{
	if(NULL == szSrc || NULL == szDest)
		return;
	char tmp[4];
	for(int nIndex = 0;nIndex<nLen;nIndex++)
	{
		memset(tmp,0,4);
		_itoa_s(szSrc[nIndex],tmp,4,16);
		if(strlen(tmp)<2)
			strcat_s(szDest,MAX_PATH,"0");
		strcat_s(szDest,MAX_PATH,tmp);
	}
}

ULONG CBaseFuncLib::GetFileSize(const ATL::CString &strFilePath)
{
	WIN32_FIND_DATA	data;
	ULONG dwFileSize = 0;
		
	HANDLE hFindFile = ::FindFirstFile(strFilePath, &data);
	if(INVALID_HANDLE_VALUE != hFindFile)
	{
		dwFileSize = data.nFileSizeLow+data.nFileSizeHigh*MAXDWORD+data.nFileSizeHigh;
		::FindClose(hFindFile);
		hFindFile = NULL;
	}
	return dwFileSize;
}

COleDateTime CBaseFuncLib::GetMsgTime(ULONG ulTotalSecond)
{
	TIME_ZONE_INFORMATION   tzi;  
    GetTimeZoneInformation(&tzi);  
	int zone = - tzi.Bias/60; /// 时区，如果是中国标准时间则得到8

	COleDateTime MsgTime(1970,1,1,zone,0,0);

	ULONG nTotalSecond = ulTotalSecond;
	LONG nDays = 0,nHours = 0,nMinutes = 0;
	nDays = nTotalSecond/(60*60*24);
	nTotalSecond -= nDays*60*60*24;
	nHours = nTotalSecond/(3600);
	nTotalSecond -= nHours*3600;
	nMinutes = nTotalSecond/60;
	nTotalSecond -= nMinutes*60;
	COleDateTimeSpan spanTime(nDays,nHours,nMinutes,nTotalSecond);
	return (MsgTime + spanTime);
}

//转换时间格式 DATE -> time_t
DWORD CBaseFuncLib::LotusToTime( double dt )
{
	TIME_ZONE_INFORMATION   tzi;  
	GetTimeZoneInformation(&tzi);  
	int zone = - tzi.Bias/60; /// 时区，如果是中国标准时间则得到8
	int nZoneSecond = zone*60*60;
	return (DWORD)( (dt-25569)*(24*60*60)-nZoneSecond + 0.5 );
}

COleDateTime CBaseFuncLib::GetFileWriteTime(const ATL::CString& strFilePath)
{
	COleDateTime FileWriteTime = COleDateTime::GetCurrentTime();
	HANDLE hFile = ::CreateFile(strFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(INVALID_HANDLE_VALUE == hFile || NULL == hFile)
		return FileWriteTime;
	FILETIME last_write_time;/// 修改时间
	memset(&last_write_time,0,sizeof(FILETIME));
	if(GetFileTime(hFile,NULL,NULL,&last_write_time))
	{
		SYSTEMTIME sysTime;
		::FileTimeToSystemTime(&last_write_time,&sysTime);
		FileWriteTime = sysTime;
	}
	::CloseHandle(hFile);
	hFile = NULL;
	return FileWriteTime;
}

ATL::CString CBaseFuncLib::GetShowSize(DWORD dwFileSize,int iPoint)
{
	ATL::CString strShowSize;
	if(dwFileSize<1024)
	{
		strShowSize.Format(_T("%dB"),dwFileSize);
		return strShowSize;
	}
	int iKSize=(int )dwFileSize/1024;
	if(iKSize<1024)
	{
		int iTem = dwFileSize%1024;
		if(3 == iPoint)
		{
			iTem = (int)(1000*iTem/1024);
			strShowSize.Format(_T("%d.%03d"),iKSize,iTem);
		}
		else if(2 == iPoint)
		{
			iTem = (int)(100*iTem/1024);
			strShowSize.Format(_T("%d.%02d"),iKSize,iTem);
		}
		else if(1 == iPoint)
		{
			iTem = (int)(10*iTem/1024);
			strShowSize.Format(_T("%d.%d"),iKSize,iTem);
		}
		else
			strShowSize.Format(_T("%d"),iKSize);
		strShowSize += _T("K");
		return strShowSize;
	}
	int iMSize=(int )dwFileSize/(1024*1024);
	int iTem = dwFileSize%(1024*1024);
	if(3 == iPoint)
	{
		iTem = (int)(1000*iTem/(1024*1024));
		strShowSize.Format(_T("%d.%03d"),iMSize,iTem);
	}
	else if(2 == iPoint)
	{
		iTem = (int)(100*iTem/(1024*1024));
		strShowSize.Format(_T("%d.%02d"),iMSize,iTem);
	}
	else if(1 == iPoint)
	{
		iTem = (int)(10*iTem/(1024*1024));
		strShowSize.Format(_T("%d.%d"),iMSize,iTem);
	}
	else
		strShowSize.Format(_T("%d"),iMSize);
	strShowSize += _T("M");
	return strShowSize;
}

DWORD CBaseFuncLib::RunExe(LPCTSTR szAppPath,LPCTSTR szCommandLine,BOOL bWaitFlag,BOOL bShowFlag)
{
	STARTUPINFO stStartUpInfo;
	memset(&stStartUpInfo, 0, sizeof(STARTUPINFO));
	stStartUpInfo.cb = sizeof(STARTUPINFO);
	if(bShowFlag)
		stStartUpInfo.wShowWindow = SW_SHOWNORMAL;
	else
		stStartUpInfo.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION stProcessInfo;
	::memset(&stProcessInfo,0,sizeof(PROCESS_INFORMATION));
	
	try
	{
		if(CreateProcess((LPWSTR)szAppPath,(LPWSTR)szCommandLine, NULL, NULL, FALSE,
			NORMAL_PRIORITY_CLASS, NULL,NULL, &stStartUpInfo, &stProcessInfo))
		{
			if(bWaitFlag)
			{
				//等待结束
				WaitForSingleObject(stProcessInfo.hProcess,INFINITE);
				::CloseHandle(stProcessInfo.hProcess);
				stProcessInfo.hThread=NULL;
				stProcessInfo.hProcess=NULL;
				stProcessInfo.dwProcessId = 0;
				stProcessInfo.dwThreadId = 0;
			}
		}
		else
		{
			DWORD dwErrCode = ::GetLastError();
			if(dwErrCode)
			{
				WRITELASTLOGTOFILE2(dwErrCode,_T("RunExe"));
			}
		}
	}
	catch( ... )
	{
	}

	return stProcessInfo.dwProcessId;
}

BOOL CBaseFuncLib::IsPathExist(const ATL::CString& strPath)
{
	BOOL bRet = FALSE;
	if(strPath.IsEmpty())
		return bRet;
	WIN32_FIND_DATA	data;
	BOOL bFindDir = FALSE;

	ATL::CString strFind(strPath);
	if (0 == strPath.Right(1).CompareNoCase( _T("\\")))
	{
		/// 查找目录
		bFindDir = TRUE;
		strFind+=_T("*.*");
	}
		
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

ATL::CString CBaseFuncLib::GetSpecialFolderPath(long lFoldID)
{
	TCHAR szSpecialPath[MAX_PATH];
	::memset(szSpecialPath,0,MAX_PATH*sizeof(TCHAR));
	BOOL bGetFlag = ::SHGetSpecialFolderPath(NULL,szSpecialPath,lFoldID,TRUE);
	if(!bGetFlag)
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("SHGetSpecialFolderPath调用 %d 失败，错误码：%ld"),lFoldID,::GetLastError());
		WRITELOGTOFILE(strLogInfo);
#endif
		return _T("");
	}

	ATL::CString strSpecialPath(szSpecialPath);
	///路径后面都有"\"
	if(!strSpecialPath.IsEmpty() && 0 != strSpecialPath.Right(1).CompareNoCase(_T("\\")))
		strSpecialPath+=_T("\\");
	return strSpecialPath;
}

BOOL CBaseFuncLib::CreatePath(const ATL::CString& strLocalDir)
{
	BOOL bCreateFlag = FALSE;
	ATL::CString strWorkDir,strPath=strLocalDir;
	int index = -2;
	index = strPath.Find(_T('\\'));
	if(index == -1)
		return FALSE;
	strWorkDir = strPath.Left(index);
	strPath.Delete(0,index+1);
	while(strPath.GetLength())
	{
		index=strPath.Find(_T('\\'));
		if(index == -1)
		{
			bCreateFlag = ::CreateDirectory(strLocalDir,NULL);
			strWorkDir.Empty();
			strPath.Empty();
			break;
		}
		strWorkDir+=_T("\\");
		strWorkDir+=strPath.Left(index);
		strPath.Delete(0,index+1);
		if(!CBaseFuncLib::IsPathExist(strWorkDir+_T("\\")))
			bCreateFlag = ::CreateDirectory(strWorkDir,NULL);
	}
	return bCreateFlag;
}

void CBaseFuncLib::DelDirFile(const ATL::CString &strPath,BOOL bDirFlag)
{
	if(strPath.GetLength()<3)
		return;/// 避免删除错误
	WIN32_FIND_DATA fd; 
	ATL::CString strFindPath(strPath);
	if(0 != strFindPath.Right(1).CompareNoCase(_T("\\")))
		strFindPath+=_T("\\");
	HANDLE hFind = FindFirstFile(strFindPath+_T("*.*"), &fd); 
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (FILE_ATTRIBUTE_DIRECTORY != (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			ATL::CString strFile(_T(""));
			strFile.Format(_T("%s%s"),strFindPath,fd.cFileName);
			::SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);
			::DeleteFile(strFile);
			strFile.Empty();
		}
		else
		{
			ATL::CString strFileName(fd.cFileName);
			if(0 != strFileName.CompareNoCase(_T("."))
				&& 0 != strFileName.CompareNoCase(_T("..")))
				DelDirFile(strFindPath+fd.cFileName,bDirFlag);
		}
		if (!FindNextFile (hFind, &fd))
			break;
	}
	if(hFind != INVALID_HANDLE_VALUE)
		FindClose (hFind);	
	if(bDirFlag)
		::RemoveDirectory(strPath);
}

HANDLE CBaseFuncLib::WriteToFile2(const ATL::CString& strDataFile,LPCVOID lpBuffer,DWORD nlen,BOOL bEndFlag, const char* strFileName, const DWORD dwLineNumber)
{
	if(NULL == lpBuffer || !nlen)
		return NULL;
	HANDLE hFileOpen = CreateFile(strDataFile,GENERIC_WRITE,FILE_SHARE_WRITE,\
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	DWORD dwWriteLen = 0;
	if(!bEndFlag)
	{
		SetFilePointer(hFileOpen,NULL,NULL,FILE_BEGIN);   
		SetEndOfFile(hFileOpen);
	}
	else
		SetFilePointer(hFileOpen,NULL,NULL,FILE_END);   
	WriteFile(hFileOpen,lpBuffer,nlen,&dwWriteLen,NULL);
	ATLASSERT(dwWriteLen == nlen);
	return hFileOpen;
}

BOOL CBaseFuncLib::WriteLastLogToFile(DWORD nError, const char* strFileName, const DWORD dwLineNumber, const ATL::CString &strModuleName,ELogType eLogType)
{
	if(!nError)
		return FALSE;
	LPVOID lpMsgBuf = NULL;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,nError,0, // Default language
		(LPTSTR)&lpMsgBuf,0,NULL );
	BOOL bWriteFlag = WriteLogToFile((LPTSTR)lpMsgBuf,strFileName,dwLineNumber,strModuleName,eLogType);
	LocalFree(lpMsgBuf);
	lpMsgBuf = NULL;
	return bWriteFlag;
}

BOOL CBaseFuncLib::WriteLogToFile(const ATL::CString& strLogInfo, const char* strFileName, const DWORD dwLineNumber,
		const ATL::CString &strModuleName,ELogType eLogType)
{
	DWORD dwLogType = 0;
	if(eLogType == LOGTYPE_INVALID)
	{
		CHXRegKey regKey;
		BOOL bGetFlag = regKey.GetRegDwordVal(_T("LogType"),dwLogType);
		if(bGetFlag && dwLogType <1)
		{
			return FALSE;
		}
		if(!bGetFlag)
		{
#ifdef APP_LOG_ENABLE
			regKey.SetRegDwordVal(_T("LogType"),APP_LOG_ENABLE);
#endif
		}
	}
	else
	{
#ifdef APP_LOG_ENABLE
		if(eLogType < APP_LOG_ENABLE)
		{
			return FALSE;/// 不需要写日志
		}
#endif
	}
	if(m_strLogFile.IsEmpty())
	{
		TCHAR szPath[MAX_PATH];
		::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
		::GetModuleFileName(NULL,szPath,MAX_PATH);
		ATL::CString strPath(szPath);
		int iFind = strPath.ReverseFind(_T('\\'));
		strPath = strPath.Right(strPath.GetLength()-iFind-1);
		if(-1 != strPath.Find(TDHXKJ_MAIN_SAFEAPP) 
			|| -1 != strPath.Find(TDHXKJ_HOSTSERVICE)
			|| -1 != strPath.Find(TDHXKJ_SAFEMGR))
		{
			ATL::CString strLogPath(CBaseFuncLib::GetTmpPath());
			m_strLogFile.Format(_T("%s%s.txt"),strLogPath,strPath.Left(strPath.GetLength()-4));
		}
		else
		{
			::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
			::GetTempPath(MAX_PATH,szPath);
			m_strLogFile = szPath;
			if(0 != m_strLogFile.Right(1).CompareNoCase(_T("\\")))
				m_strLogFile += _T("\\");
			iFind = strPath.ReverseFind(_T('.'));
			m_strLogFile += strPath.Left(iFind);
			m_strLogFile += _T(".txt");
		}
		strPath.Empty();
	}
	DWORD dwFileSize = 5*1024*1024;/// 5MB
	if(CBaseFuncLib::GetFileSize(m_strLogFile) > dwFileSize)
	{
		int nIndex = 1;
		ATL::CString strMoreLog(_T(""));
		strMoreLog.Format(_T("%s%d.bak"),m_strLogFile,nIndex);
		while(CBaseFuncLib::IsPathExist(strMoreLog))
		{
			strMoreLog.Empty();
			nIndex++;
			strMoreLog.Format(_T("%s%d.bak"),m_strLogFile,nIndex);
		}
		::MoveFile(m_strLogFile,strMoreLog);/// 备份日志输出
	}
	ATL::CString strModule(strModuleName);
	if(strModule.IsEmpty())
		strModule = GetModuleName((HMODULE)g_hInstance);

	//
	ATL::CString FileName(strFileName);

	ATL::CString strLog(_T(""));
	DWORD dwThreadID = ::GetCurrentThreadId();
	COleDateTime curTime = COleDateTime::GetCurrentTime();
	strLog.Format(_T("*%s*%02d-%02d %02d:%02d:%02d TID:%ld FileName:%s LineNum:%d Log:%s\r\n"),strModule,\
		curTime.GetMonth(),curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),\
		curTime.GetSecond(),dwThreadID,FileName,dwLineNumber, strLogInfo);
	strModule.Empty();

	char *szLog = NULL;
	int nLen = CBaseFuncLib::Us2ToChar(strLog,&szLog);
	strLog.Empty();
	if(NULL == szLog)
	{
		return FALSE;
	}
	BOOL bWriteFlag = CBaseFuncLib::WriteToFile(m_strLogFile,(BYTE *)szLog,nLen-1,TRUE);
	delete []szLog;
	szLog = NULL;
#ifdef _DEBUG
	if(!bWriteFlag)
	{
		ATL::CString strMsgInfo(_T(""));
		DWORD dwErrCode = ::GetLastError();
		strMsgInfo.Format(_T("写数据失败！错误码：%ld，日志文件：%s\r\n请确保还有存储空间。"),\
			dwErrCode,m_strLogFile);
		::MessageBox(::GetForegroundWindow(),strMsgInfo,TDHXKJ_APPLICATION_NAME,MB_OK);
	}
#endif
	return bWriteFlag;
}

BOOL CBaseFuncLib::WriteXzmLogToFile(const ATL::CString& strLogInfo, const char* strFileName, const DWORD dwLineNumber)
{
	CString App = _T("App");
	ATL::CString FileName(strFileName);
	ATL::CString strLog(_T(""));
	DWORD dwThreadID = ::GetCurrentThreadId();
	COleDateTime curTime = COleDateTime::GetCurrentTime();
	strLog.Format(_T("*%s*%02d-%02d %02d:%02d:%02d TID:%ld FileName:%s LineNum:%d Log:%s\r\n"),App,\
		curTime.GetMonth(),curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),\
		curTime.GetSecond(),dwThreadID,FileName,dwLineNumber, strLogInfo);
	App.Empty();

	char *szLog = NULL;
	int nLen = CBaseFuncLib::Us2ToChar(strLog,&szLog);
	strLog.Empty();
	if(NULL == szLog)
	{
		return FALSE;
	}

	int nPos;
	CString strTempPath;
	::GetModuleFileName(NULL,strTempPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	strTempPath.ReleaseBuffer ();
	nPos=strTempPath.ReverseFind ('\\');
	strTempPath=strTempPath.Left (nPos);
	strTempPath = strTempPath + CBaseFuncLib::m_strProcessName;

	BOOL bWriteFlag = CBaseFuncLib::WriteToFile(strTempPath,(BYTE *)szLog,nLen-1,TRUE);

	return bWriteFlag;
}

BOOL CBaseFuncLib::WriteToFile(const ATL::CString& strDataFile,BYTE *pData,DWORD nlen,BOOL bEndFlag)
{
	HANDLE hFileOpen = NULL;
	hFileOpen = ::CreateFile(strDataFile,GENERIC_WRITE,0,\
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
	{
		::DWORD dwLastErr = ::GetLastError();
		int nIndex = 100;
		/// 不是拒绝访问
		while(ERROR_ACCESS_DENIED != dwLastErr && ERROR_INVALID_ACCESS != dwLastErr
			&& nIndex > 0 && hFileOpen == INVALID_HANDLE_VALUE)
		{
			nIndex--;
			::Sleep(100);
			/// 重新尝试打开
			hFileOpen = ::CreateFile(strDataFile,GENERIC_WRITE,FILE_SHARE_WRITE,\
				NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		}
		if(hFileOpen == INVALID_HANDLE_VALUE)
			return FALSE;
	}
	if(!bEndFlag)
	{
		::SetFilePointer(hFileOpen,NULL,NULL,FILE_BEGIN);   
		::SetEndOfFile(hFileOpen);
	}
	else
		::SetFilePointer(hFileOpen,NULL,NULL,FILE_END);   
	DWORD dwWriteLen = 0;
	::WriteFile(hFileOpen,pData,nlen,&dwWriteLen,NULL);
	::CloseHandle(hFileOpen);
	if(dwWriteLen)
		return TRUE;
	return FALSE;
}

DWORD CBaseFuncLib::ReadAllData(const ATL::CString& FileName,BYTE **ppData)
{
	DWORD m = 0;
	ATLASSERT(ppData);
	HANDLE hFileOpen = ::CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
		return m;
	int len = ::GetFileSize(hFileOpen,NULL);
	*ppData = new BYTE[len+1];
	ATLASSERT(*ppData);
	if(NULL == *ppData)
		return m;
	memset(*ppData,0,len+1);
	::ReadFile(hFileOpen,*ppData,len,&m,NULL);
	::CloseHandle(hFileOpen);
	hFileOpen = NULL;
	ATLASSERT(m);
	return m;
}

char* CBaseFuncLib::UrlEncodeToChar(const ATL::CString& strPara)
{
	ATL::CString strParaTem=strPara;
	strParaTem.Replace(_T("%"),_T("%25"));
//	strParaTem.Replace(_T("#"),_T("%23"));
	strParaTem.Replace(_T("\""),_T("%22"));
	strParaTem.Replace(_T("@"),_T("%40"));
	strParaTem.Replace(_T("&"),_T("%26"));
	strParaTem.Replace(_T("+"),_T("%2B"));
	strParaTem.Replace(_T("-"),_T("%2D"));
	strParaTem.Replace(_T(";"),_T("%3B"));
	strParaTem.Replace(_T(" "),_T("%20"));
	strParaTem.Replace(_T("'"),_T("%27"));
	strParaTem.Replace(_T(":"),_T("%3A"));
	strParaTem.Replace(_T("?"),_T("%3F"));
	strParaTem.Replace(_T("/"),_T("%2F"));
	strParaTem.Replace(_T("="),_T("%3D"));

	char *inbuffer = NULL;
	US2ToUtf8(strParaTem,&inbuffer);
	return inbuffer;
}

ATL::CString CBaseFuncLib::PercentStringDecode(const ATL::CString& strSrc,BOOL bUtf8)
{
	char *szBuf = NULL;
	char cTem[8192] = {};
	int nLen = strSrc.GetLength(),nC = 0;
	for(int nIndex = 0;nIndex < nLen;)
	{
		ATL::CString strVal(_T(""));
		WCHAR wc = strSrc.GetAt(nIndex);
		if(wc != _T('%'))
		{
			cTem[nC] = (char)wc;
			nC++;
			nIndex++;
			continue;
		}
		nIndex++;
		if(nIndex < nLen)
		{
			strVal += strSrc.GetAt(nIndex);
			nIndex++;
		}
		if(nIndex < nLen)
		{
			strVal += strSrc.GetAt(nIndex);
			nIndex++;
		}
		CBaseFuncLib::Us2ToChar(strVal,&szBuf);
		cTem[nC] = (char )strtoul(szBuf, NULL, 16);
		nC++;
		delete []szBuf;
		szBuf = NULL;
		if(nC >= 8192)
			break;
	}
	if(bUtf8)
	{
		CBaseFuncLib::Utf8ToChar(cTem,&szBuf);
		ATL::CString strReturn(szBuf);
		delete []szBuf;
		szBuf = NULL;
		return strReturn;
	}
	else
	{
		ATL::CString strReturn(cTem);
		return strReturn;
	}
}

ATL::CString CBaseFuncLib::UrlEncode(const ATL::CString& strPara,BOOL bFlag)
{
	if(!bFlag)
	{
		ATL::CString strParaTem = strPara;
		strParaTem.Replace(_T("%25"),_T("%"));
//		strParaTem.Replace(_T("%23"),_T("#"));
		strParaTem.Replace(_T("%22"),_T("\""));
		strParaTem.Replace(_T("%40"),_T("@"));
		strParaTem.Replace(_T("%26"),_T("&"));
		strParaTem.Replace(_T("%2B"),_T("+"));
		strParaTem.Replace(_T("%2D"),_T("-"));
		strParaTem.Replace(_T("%3B"),_T(";"));
		strParaTem.Replace(_T("%20"),_T(" "));
		strParaTem.Replace(_T("%27"),_T("'"));
		strParaTem.Replace(_T("%3A"),_T(":"));
		strParaTem.Replace(_T("%3F"),_T("?"));
		strParaTem.Replace(_T("%2F"),_T("/"));
		strParaTem.Replace(_T("%3D"),_T("="));
		return strParaTem;
	}
	char *inbuffer = UrlEncodeToChar(strPara);
	if(NULL == inbuffer)
		return strPara;
	ATL::CString out(_T(""));
	int nLen = (int)strlen(inbuffer);
	for(int nIndex = 0;nIndex < nLen;nIndex++)
	{
		BYTE ch = inbuffer[nIndex];
		if(ch >= 0x80)
		{
			ATL::CString strTem;
			strTem.Format(_T("%%%02X"),ch);
			out+=strTem;
			strTem.Empty();
		}
		else
			out += ch;
	}

	delete []inbuffer;
	inbuffer = NULL;

	return out;
}

void CBaseFuncLib::UrlEncodeToFile(const ATL::CString& strPara,HANDLE hFileHandle,BOOL bTryFlag)
{
	ATL::CString strParaTem=strPara;
	strParaTem.Replace(_T("%"),_T("%25"));
//	strParaTem.Replace(_T("#"),_T("%23"));
	strParaTem.Replace(_T("\""),_T("%22"));
	strParaTem.Replace(_T("@"),_T("%40"));
	strParaTem.Replace(_T("&"),_T("%26"));
	strParaTem.Replace(_T(" "),_T("%20"));
	if(bTryFlag)
		strParaTem.Replace(_T("+"),_T("%252B"));
	else
		strParaTem.Replace(_T("+"),_T("%2B"));
	strParaTem.Replace(_T("-"),_T("%2D"));
	strParaTem.Replace(_T(";"),_T("%3B"));
	strParaTem.Replace(_T("'"),_T("%27"));
	strParaTem.Replace(_T(":"),_T("%3A"));
	strParaTem.Replace(_T("."),_T("%2E"));
	strParaTem.Replace(_T("/"),_T("%2F"));
	strParaTem.Replace(_T("="),_T("%3D"));

	char *inbuffer=NULL;
	int nLen = US2ToUtf8(strParaTem,&inbuffer);
	if(NULL == inbuffer)
		return;
	DWORD dwWriteLen = 0;
	for(int nIndex = 0;nIndex<nLen;nIndex++)
	{
		BYTE bc = inbuffer[nIndex];
		if(bc >= 0x80)
		{
			ATL::CString strTem;
			strTem.Format(_T("%%%02X"),bc);
			BYTE *szBuff = NULL;
			int nWriteLen = CBaseFuncLib::Us2ToChar(strTem,(char **)&szBuff);
			ATLASSERT(szBuff);
			if(NULL != szBuff)
			{
				::WriteFile(hFileHandle,szBuff,nWriteLen-1,&dwWriteLen,NULL);
				delete []szBuff;
				szBuff = NULL;
			}
			strTem.Empty();
		}
		else if(bc)
		{
			::WriteFile(hFileHandle,&bc,1,&dwWriteLen,NULL);
		}
	}

	delete []inbuffer;
	inbuffer = NULL;
}

#define KeyNameInformation_ 3
#define STATUS_BUFFER_TOO_SMALL ((LONG)0xC0000023L)

CString CBaseFuncLib::QueryRegistryLink(const ATL::CString& strRegPath)
{
	/// 必须是以HKEY_LOCAL_MACHINE或HKEY_CURRENT_USER开始的全路径
	CString strQueryPath(_T(""));
	HMODULE hKernel	= ::GetModuleHandle(TDHXKJ_SYNTDLL);
	if(NULL == hKernel)
		return strQueryPath;
	lpNtQueryKey NtQueryKey = (lpNtQueryKey)GetProcAddress(hKernel,"ZwQueryKey"); 
	hKernel = NULL;
	if(NULL == NtQueryKey)
	{
#ifdef APP_LOG_ENABLE
		WRITELASTLOGTOFILE2(::GetLastError(),_T("QueryRegistryLink"));
#endif
		return strQueryPath;
	}
	CString strReg(strRegPath);
	int nFind = strReg.Find(_T("\\"));
	HKEY hRoot = HKEY_CURRENT_USER;
	if( strRegPath.Find(L"HKEY_LOCAL_MACHINE\\") != -1 )
		hRoot = HKEY_LOCAL_MACHINE;
	if(-1 != nFind)
		strReg.Delete(0,nFind+1);
	HKEY hKey = NULL;
	LSTATUS ret = RegOpenKeyEx(hRoot, strReg, 0, KEY_READ, &hKey);
	strReg.Empty();
	if( ret != ERROR_SUCCESS )
		return strQueryPath;
	DWORD len=0;
	ret = NtQueryKey(hKey, KeyNameInformation_, NULL, 0, &len);
	if( ret!=STATUS_BUFFER_TOO_SMALL )
	{
		RegCloseKey(hKey);
		return strQueryPath;
	}
	PVOID KeyInfo = new BYTE[len+sizeof(WCHAR)];
	if(NULL != KeyInfo)
	{
		memset(KeyInfo,0,len+sizeof(WCHAR));
		ret = NtQueryKey(hKey, KeyNameInformation_, KeyInfo, len, &len);
		if(!ret)
			strQueryPath = (PCWSTR)((BYTE*)KeyInfo+sizeof(ULONG));
		delete []KeyInfo;
		KeyInfo = NULL;
	}
	RegCloseKey(hKey);
	NtQueryKey = NULL;

	return strQueryPath;
}

/**
 * @brief 将COLORREF颜色转换为字符串表示的颜色
 *
 *
 * @param rgbcolor 颜色
 * @return 
 */
ATL::CString CBaseFuncLib::GetStringFromRGB(COLORREF& rgbcolor)
{
	ATL::CString strColor(_T(""));
	BYTE Red	= GetRValue(rgbcolor);	///得到红颜色 
	BYTE Green	= GetGValue(rgbcolor);	///得到绿颜色 
	BYTE Blue	= GetBValue(rgbcolor);	///得到兰颜色
	strColor.Format(_T("#%02X%02X%02X"),Red,Green,Blue);
	return strColor;
}

/**
 * @brief 将字符串颜色值转换为COLORREF颜色
 *
 *
 * @param strSource 颜色字符串
 * @param rgbcolor  返回颜色
 * @return 
 */
BOOL CBaseFuncLib::GetRGBFromString( ATL::CString strSource, COLORREF& rgbcolor )
{
    rgbcolor = 0;
	int iLength = strSource.GetLength();
	if ( iLength<7 )
        return FALSE;
    BYTE Red = 0,Green = 0,Blue = 0;
	ATL::CString strColor(strSource),strHex(_T(""));

	if(0 == strColor.Left(1).CompareNoCase(_T("\"")))
		strColor.Delete(0);
	if(_T('#') != strColor.GetAt(0))
		return FALSE;
	strColor.Delete(0);
	strHex = strColor.Left(2);
	strColor.Delete(0,2);
    Red = (BYTE)_tcstoul(strHex, NULL, 16);
	strHex = strColor.Left(2);
	strColor.Delete(0,2);
    Green = (BYTE)_tcstoul(strHex, NULL, 16);
	strHex = strColor.Left(2);
    Blue = (BYTE)_tcstoul(strHex, NULL, 16);

    rgbcolor = RGB( Red, Green, Blue );
    return TRUE;
}

BOOL CBaseFuncLib::GetAppStringValue(const ATL::CString& strkeyName,ATL::CString& strValue)
{
	CHXRegKey regKey;
	return regKey.GetRegStringVal(strkeyName,strValue);
}

void CBaseFuncLib::SetAppStringValue(const ATL::CString& strkeyName,const ATL::CString& strValue)
{
	CHXRegKey regKey;
	regKey.SetRegStringVal(strkeyName,strValue);
}

BOOL CBaseFuncLib::GetAppDWordValue(const ATL::CString& strkeyName,DWORD& dwValue)
{
	CHXRegKey regKey;
	return regKey.GetRegDwordVal(strkeyName,dwValue);
}

void CBaseFuncLib::SetAppDWordValue(const ATL::CString& strkeyName,DWORD dwValue)
{
	CHXRegKey regKey;
	regKey.SetRegDwordVal(strkeyName,dwValue);
}

BOOL CBaseFuncLib::GetAppBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes)
{
	ATLASSERT(ppValue && pnBytes);
	CHXRegKey regKey;
	return regKey.GetRegBinaryValue(strKeyName,ppValue,pnBytes);
}

void CBaseFuncLib::SetAppBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes)
{
	CHXRegKey regKey;
	regKey.SetRegBinaryValue(strKeyName,Value,nBytes);
}

ATL::CString CBaseFuncLib::GetAppDir(const ATL::CString& strRegPath, HKEY hKey)
{
#ifndef _DEBUG
	if(!strRegPath.IsEmpty())
	{
		/// 指定路径
		CString strRegPath(_T(""));
		CHXRegKey regKey(strRegPath,PRODUCT_REGNODENAME,hKey);
		regKey.GetRegStringVal(PRODUCT_COMMAN_RUNPATH,strRegPath);
		if(strRegPath.GetLength() > 3)
			return strRegPath;
	}
	if(!m_strAppPath.IsEmpty())
		return m_strAppPath;
	if(!GetAppStringValue(PRODUCT_COMMAN_RUNPATH,m_strAppPath))
#endif
	{
		m_strAppPath = GetModulePath(g_hInstance);
	}
	return m_strAppPath;
}

ATL::CString CBaseFuncLib::GetAppDataDir(void)
{
	ATL::CString strDataPath = CBaseFuncLib::GetAppDir();
	strDataPath += _T("Data");
	::DeleteFile(strDataPath);
	CBaseFuncLib::CreatePath(strDataPath);
	strDataPath += _T("\\");

	return strDataPath;
}

BOOL CBaseFuncLib::GetAppInsPath(ATL::CString &strInsPath)
{
	BOOL bGetFlag = FALSE;
	ATL::CString strRegPath;
	strRegPath.Format(_T("%s %s"),COMPANY_NAME,TDHXKJ_APPLICATION_NAME);
	CHXRegKey regKey(_T("SOFTWARE\\Apps"),strRegPath,HKEY_LOCAL_MACHINE);

	bGetFlag= regKey.GetRegStringVal(_T("InstallDir"),strInsPath);
	if(strInsPath.IsEmpty())
		bGetFlag= regKey.GetRegStringVal(_T("InstlDir"),strInsPath);
	strRegPath.Empty();
	if(bGetFlag)
	{
		if(strInsPath.IsEmpty())
			bGetFlag = FALSE;
	}

	return bGetFlag;
}

ATL::CString CBaseFuncLib::GetModulePath(HANDLE hModule)
{
	TCHAR szPath[MAX_PATH];
	::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	::GetModuleFileName((HMODULE )hModule,szPath,MAX_PATH);
	ATL::CString strPath(szPath);

	int iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind+1);
	return strPath;
}

ATL::CString CBaseFuncLib::GetAppConfigPath(HANDLE hModule /*= NULL*/)
{
	ATL::CString strPath = CBaseFuncLib::GetModulePath(hModule);

	int iFind = -1;
	iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind);
	iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind);
	iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind+1);

	strPath = strPath + _T("Config\\");
	return strPath;
}

ATL::CString CBaseFuncLib::GetAppDocumentPath(HANDLE hModule /*= NULL*/)
{
	ATL::CString strPath = CBaseFuncLib::GetModulePath(hModule);

	int iFind = -1;
	iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind);
	iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind);
	iFind = strPath.ReverseFind(_T('\\'));
	strPath = strPath.Left(iFind+1);

	strPath = strPath + _T("doc\\");
	return strPath;
}

ATL::CString CBaseFuncLib::GetModuleName(HMODULE hModule)
{
	TCHAR szPath[MAX_PATH];
	::memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	::GetModuleFileName(hModule,szPath,MAX_PATH);
	ATL::CString strPath(szPath);
	int iFind = strPath.ReverseFind(_T('\\'));
	strPath.Delete(0,iFind+1);
	return strPath;
}

ATL::CString CBaseFuncLib::GetTmpPath(BOOL bAppFlag)
{
	ATL::CString strTempPath(_T(""));
	if(-1 == GetModuleName(NULL).Find(_T("HXSafeSetup")))
	{
#ifndef _DEBUG
		if(bAppFlag)
		{
			strTempPath = GetAppDir();
			if(strTempPath.GetLength() < 3)
				strTempPath = GetModulePath(NULL);
		}
		else
#endif
			strTempPath = GetModulePath(NULL);
		strTempPath += _T("Temp");
		CreatePath(strTempPath);
		strTempPath += _T("\\");
	}
	else
	{
		TCHAR szPath[MAX_PATH];
		memset(szPath,0,MAX_PATH*sizeof(TCHAR));
		::GetTempPath(MAX_PATH,szPath);
		strTempPath = szPath;
		if(0 != strTempPath.Right(1).CollateNoCase(_T("\\")))
			strTempPath += _T("\\");
		strTempPath += PRODUCT_REGNODENAME;
		CBaseFuncLib::CreatePath(strTempPath);
		strTempPath += _T("\\");
	}
	return strTempPath;
}

BOOL CBaseFuncLib::EnumAppProc(HWND hWnd, LPARAM lp)
{
	if(!hWnd)
		return TRUE;
	lp = 0;
	BOOL bFindWnd = TRUE;
	if(m_bVisibleFlag)
	{
		///只查找显示的窗口
		if(WS_VISIBLE != (::GetWindowLong(hWnd,GWL_STYLE) & WS_VISIBLE))
			bFindWnd = FALSE;
	}
	else
	{
		///只查找隐藏的窗口
		if(WS_VISIBLE == (::GetWindowLong(hWnd,GWL_STYLE) & WS_VISIBLE))
			bFindWnd = FALSE;
	}
	if (!bFindWnd)
		return TRUE;

	TCHAR tcClassName[MAX_PATH];
	::memset(tcClassName,0,MAX_PATH*sizeof(TCHAR));
	GetClassName(hWnd,tcClassName,MAX_PATH);
	ATL::CString strClassName(tcClassName);
	if (0==strClassName.CompareNoCase(m_strWndName))
	{
		m_hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HWND CBaseFuncLib::GetAppWnd(const ATL::CString& strWndName,BOOL bVisibleFlag)
{
	m_strWndName.Empty();
	m_strWndName = strWndName;
	m_bVisibleFlag = bVisibleFlag;
	m_hWnd = NULL;
	::EnumWindows(EnumAppProc,(LPARAM)NULL);
	return m_hWnd;
}

void CBaseFuncLib::QuitApp(const ATL::CString& strWndName,BOOL bVisibleFlag)
{
	GetAppWnd(strWndName,bVisibleFlag);
	if(NULL == m_hWnd || !::IsWindow(m_hWnd))
		return;
	//退出
	::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
	if(NULL == m_hWnd || !::IsWindow(m_hWnd))
		return;
	::Sleep(1000);
	GetAppWnd(strWndName,bVisibleFlag);
	if(NULL == m_hWnd || !::IsWindow(m_hWnd))
		return;
	DWORD dwProcID = 0;
	::GetWindowThreadProcessId(m_hWnd,&dwProcID);
	if(!dwProcID)
		return;
	HANDLE hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,dwProcID);
	ATLASSERT(hProc);
	if(NULL != hProc)
	{
		::TerminateProcess(hProc,0);
		DWORD dwFlag = 0;
		BOOL bGetFlag = GetHandleInformation(hProc,&dwFlag);
		if(bGetFlag)
		{
			dwFlag = dwFlag & HANDLE_FLAG_PROTECT_FROM_CLOSE;
			if(!dwFlag)
				::CloseHandle(hProc);
		}
	}
}

void CBaseFuncLib::OpenUrl(const ATL::CString& strUrl)
{
	ShellExecute(NULL,_T("open"),strUrl,NULL,NULL,SW_SHOW);
}

BOOL CBaseFuncLib::CopyStringToClipboard(const ATL::CString& strCopy)
{
	HANDLE hGlobalMemory = GlobalAlloc(GHND | GMEM_SHARE, (strCopy.GetLength() + 1)*sizeof(TCHAR)); 
	ATLASSERT(hGlobalMemory);
	if(NULL == hGlobalMemory)
		return FALSE;
	TCHAR *pBuf = (TCHAR*)GlobalLock(hGlobalMemory); 
	ATLASSERT(pBuf);
	if(NULL == pBuf)
	{
		::GlobalFree(hGlobalMemory);
		return FALSE;
	}
	lstrcpy(pBuf, strCopy); 
	GlobalUnlock(hGlobalMemory); // 解锁内存块
	BOOL bOpenFlag = ::OpenClipboard(NULL);
	if(!bOpenFlag)
		return bOpenFlag;
	::EmptyClipboard();
	::SetClipboardData(CF_UNICODETEXT, hGlobalMemory); // 将内存中的数据放置到剪贴板
	::CloseClipboard();
	return TRUE;
}

HINSTANCE CBaseFuncLib::CreateInstance( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv )
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

void CBaseFuncLib::CreateInterface( HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv )
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

BOOL CBaseFuncLib::IsProcessExist( LPCTSTR lpszProcessName, BOOL bFlag)
{
	HANDLE			hProcessSnap	= NULL; 
	BOOL			bRet		= FALSE; 
	PROCESSENTRY32	pe32		= {0}; 
	DWORD			dwCount		= 0;
	//  Take a snapshot of all processes in the system. 

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (hProcessSnap == INVALID_HANDLE_VALUE) 
		return (FALSE); 

	//  Fill in the size of the structure before using it. 

	pe32.dwSize = sizeof(PROCESSENTRY32); 

	//  Walk the snapshot of the processes, and for each process, 
	//  display information. 

	if (Process32First(hProcessSnap, &pe32)) 
	{ 
		CString strTempName(lpszProcessName);

		do 
		{ 
			
			if( 0 == strTempName.CompareNoCase(pe32.szExeFile))
			{
				
				dwCount++;
			}
		} 
		while (Process32Next(hProcessSnap, &pe32)); 
		
		if (bFlag)
		{
			if (dwCount > 2)
			{
				bRet = TRUE; 
			}
		}
		else
		{

			if (dwCount > 1)
			{
				bRet = TRUE; 
			}
		}
	} 
	else 
		bRet = FALSE;    // could not walk the list of processes 

	// Do not forget to clean up the snapshot object. 

	CloseHandle (hProcessSnap); 
	return (bRet); 

}


int CBaseFuncLib::GetCategory(int EventType,CString& EventCategory)
{
	switch(EventType)
	{
	case EVENTLOG_SUCCESS:
		EventCategory = _T("成功");
		break;
	case EVENTLOG_ERROR_TYPE:
		EventCategory = _T("错误");
		break;
	case EVENTLOG_WARNING_TYPE:
		EventCategory = _T("警告");
		break;
	case EVENTLOG_INFORMATION_TYPE:
		EventCategory = _T("信息");
		break;
	case EVENTLOG_AUDIT_SUCCESS:
		EventCategory = _T("审核成功");
		break;
	case EVENTLOG_AUDIT_FAILURE:
		EventCategory = _T("审核失败");
		break;
	default:
		break;
	}

	return EventType;
}


int CBaseFuncLib::GetInfoClass(int EventType,CString& EventCategory)
{
	switch(EventType)
	{
	case enumSecurityClass:
		EventCategory = SEC_EVENT_LOG_NAME;
		break;
	case enumSystemClass:
		EventCategory = SYS_EVENT_LOG_NAME;
		break;
	case enumApplicationClass:
		EventCategory = APP_EVENT_LOG_NAME;
		break;
	case enumSetupClass:
		EventCategory = SETUP_EVENT_LOG_NAME;
		break;
	default:
		break;
	}

	return EventType;
}

//获取文件属性

bool CBaseFuncLib::QueryValue(const char* ValueName, const char* szModuleName, char* RetStr, int RetLen)
{
	bool bSuccess = FALSE;
	BYTE*  m_lpVersionData = NULL;
	DWORD   m_dwLangCharset = 0;
	CHAR *tmpstr = NULL;

	do
	{  

		if (ValueName == NULL || szModuleName == NULL)
		{
			break;
		}

		DWORD dwHandle;

		

		DWORD dwDataSize = ::GetFileVersionInfoSizeA(szModuleName, &dwHandle);  

		if (dwDataSize == 0)
		{
			break;
		}

		m_lpVersionData = new BYTE[dwDataSize];// 分配缓冲区  
		if ( NULL == m_lpVersionData)
		{
			break;
		}

		if (!::GetFileVersionInfoA((LPCSTR)szModuleName, dwHandle, dwDataSize,(void*)m_lpVersionData))
		{
			break;
		}

		UINT nQuerySize;

		DWORD* pTransTable;

		// 设置语言  

		//https://msdn.microsoft.com/zh-cn/vstudio/aa909243

		if (!::VerQueryValueA(m_lpVersionData, "\\VarFileInfo\\Translation", (void **)&pTransTable, &nQuerySize))  

			break;  

		//MAKELONG 将两个16位的数联合成一个无符号的32位数

		m_dwLangCharset = MAKELONG(HIWORD(pTransTable[0]), LOWORD(pTransTable[0]));  

		if (m_lpVersionData == NULL)
		{
			break;
		}

		tmpstr = new CHAR[128];// 分配缓冲区  

		if (NULL == tmpstr)
		{
			break;
		}

		sprintf_s(tmpstr, 128, "\\StringFileInfo\\%08lx\\%s", m_dwLangCharset, ValueName);

		LPVOID lpData;

		// 调用此函数查询前需要先依次调用函数GetFileVersionInfoSize和GetFileVersionInfo  

		if (::VerQueryValueA((void *)m_lpVersionData, tmpstr, &lpData, &nQuerySize))
		{
			if ( RetLen > nQuerySize)
			{
				strcpy_s(RetStr, nQuerySize, (char*)lpData);
			}
			else
			{
				strcpy_s(RetStr, RetLen, (char*)lpData);
			}
			
		}

		bSuccess = TRUE;

	} while (FALSE);

	// 销毁缓冲区  

	if (m_lpVersionData)
	{
		delete[] m_lpVersionData;
		m_lpVersionData = NULL;

	}

	if (tmpstr)
	{
		delete[] tmpstr;
		tmpstr = NULL;

	}  

	return bSuccess;  

}  


//获取文件说明  
bool   CBaseFuncLib::GetFileDescription(const char* szModuleName, char* RetStr, int RetLen)
{ return QueryValue("FileDescription", szModuleName, RetStr, RetLen); };   

//获取文件版本  
bool   CBaseFuncLib::GetFileVersion(const char* szModuleName, char* RetStr, int RetLen)
{ return QueryValue("FileVersion", szModuleName, RetStr, RetLen); };        

//获取内部名称  
bool   CBaseFuncLib::GetInternalName(const char* szModuleName, char* RetStr, int RetLen)
{ return QueryValue("InternalName", szModuleName, RetStr, RetLen); };      

//获取公司名称  
bool   CBaseFuncLib::GetCompanyName(const char* szModuleName, char* RetStr, int RetLen)
{ return QueryValue("CompanyName", szModuleName, RetStr, RetLen); };       

 //获取版权  
bool   CBaseFuncLib::GetLegalCopyright(const char* szModuleName, char* RetStr, int RetLen)
{ return QueryValue("LegalCopyright", szModuleName, RetStr, RetLen); };   

//获取原始文件名  
bool   CBaseFuncLib::GetOriginalFilename(const char* szModuleName, char* RetStr, int RetLen)
{ return QueryValue("OriginalFilename", szModuleName, RetStr, RetLen); };  

//获取产品名称  
bool   CBaseFuncLib::GetProductName(const char* szModuleName, char* RetStr, int RetLen)       
{ return QueryValue("ProductName", szModuleName, RetStr, RetLen); };       

//获取产品版本  
bool   CBaseFuncLib::GetProductVersion(const char* szModuleName, char* RetStr, int RetLen)       
{ return QueryValue("ProductVersion", szModuleName, RetStr, RetLen); };    

//获取文件大小
DWORD CBaseFuncLib::GetSpecialFileSize(const CString szModuleName)
{

	HANDLE hFile = CreateFile(szModuleName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	DWORD dwSize = ::GetFileSize(hFile,NULL);

	return dwSize;
}


BOOL CBaseFuncLib::GetExtFile(DWORD dwExt,const CString& strVal)
{
	BOOL bCheckFlag = FALSE;
	if( 0 == strVal.CompareNoCase(_T("exe")) || 0 == strVal.CompareNoCase(_T("bat")) || 
		0 == strVal.CompareNoCase(_T("com")) || 0 == strVal.CompareNoCase(_T("dll")) || 
		0 == strVal.CompareNoCase(_T("cpl")) || 0 == strVal.CompareNoCase(_T("acm")) || 
		0 == strVal.CompareNoCase(_T("ime")) || 0 == strVal.CompareNoCase(_T("tsp")) || 
		0 == strVal.CompareNoCase(_T("ax")) || 0 == strVal.CompareNoCase(_T("aux")) || 
		0 == strVal.CompareNoCase(_T("ocx")) || 0 == strVal.CompareNoCase(_T("js")) || 
		0 == strVal.CompareNoCase(_T("sys")) || 0 == strVal.CompareNoCase(_T("drv")) || 
		0 == strVal.CompareNoCase(_T("cab")) ||
		//		0 == strVal.CompareNoCase(_T("inf")) ||
		0 == strVal.CompareNoCase(_T("jar")) || 0 == strVal.CompareNoCase(_T("msi")))
		bCheckFlag = TRUE;
	if(!bCheckFlag)
	{
		/// 不在扫描格式范围之内
		return bCheckFlag;
	}
	if(0 == strVal.CompareNoCase(_T("dll")) || 0 == strVal.CompareNoCase(_T("ocx")) || 0 == strVal.CompareNoCase(_T("ax"))
		|| 0 == strVal.CompareNoCase(_T("cpl")) || 0 == strVal.CompareNoCase(_T("acm")) || 0 == strVal.CompareNoCase(_T("aux"))
		|| 0 == strVal.CompareNoCase(_T("ime")) || 0 == strVal.CompareNoCase(_T("tsp")))
	{
		if(FL_INTERCEPT_DLL_OCX != (dwExt & FL_INTERCEPT_DLL_OCX))
			bCheckFlag = FALSE;
	}
	if(0 == strVal.CompareNoCase(_T("inf")) || 0 == strVal.CompareNoCase(_T("cab")))
	{
		if(FL_INTERCEPT_CAB_INF != (dwExt & FL_INTERCEPT_CAB_INF))
			bCheckFlag = FALSE;
	}
	else if(0 == strVal.CompareNoCase(_T("exe")))
	{
		if(FL_INTERCEPT_EXE != (dwExt & FL_INTERCEPT_EXE))
			bCheckFlag = FALSE;
	}
	else if(0 == strVal.CompareNoCase(_T("bat")))
	{
		if(FL_INTERCEPT_BAT != (dwExt & FL_INTERCEPT_BAT))
			bCheckFlag = FALSE;
	}
	else if(0 == strVal.CompareNoCase(_T("sys")) || 0 == strVal.CompareNoCase(_T("drv")))
	{
		if(FL_INTERCEPT_SYS != (dwExt & FL_INTERCEPT_SYS))
			bCheckFlag = FALSE;
	}
	else if(0 == strVal.CompareNoCase(_T("js")))
	{
		if(FL_INTERCEPT_JS != (dwExt & FL_INTERCEPT_JS))
			bCheckFlag = FALSE;
	}
	else if(0 == strVal.CompareNoCase(_T("msi")))
	{
		if(FL_INTERCEPT_MSI != (dwExt & FL_INTERCEPT_MSI))
			bCheckFlag = FALSE;
	}
	else if(0 == strVal.CompareNoCase(_T("jar")))
	{
		if(FL_INTERCEPT_JAR != (dwExt & FL_INTERCEPT_JAR))
			bCheckFlag = FALSE;
	}
	return bCheckFlag;
}

BOOL CBaseFuncLib::GetFileInfo(const CString& strFilePath,CString& strCorpName,\
	CString& strVersion,CString& strSize,CString& strDesc)
{
	strSize = CBaseFuncLib::NumToStr(CBaseFuncLib::GetFileSize(strFilePath));

	BOOL bGetFlag = FALSE;
	DWORD dwHandle = 0,dwLen = 0;
	dwLen = ::GetFileVersionInfoSize(strFilePath,&dwHandle);
	if(!dwLen)
	{
		/// 无版本信息
		return bGetFlag;
	}
	dwLen++;
	BYTE *szBuf = new BYTE[dwLen];
	if(NULL == szBuf)
		return bGetFlag;///内存不足
	memset(szBuf,0,dwLen*sizeof(BYTE));
	bGetFlag = GetFileVersionInfo(strFilePath,dwHandle,dwLen-1,szBuf);
	if(!bGetFlag)
	{
		delete []szBuf;
		szBuf = NULL;
		return bGetFlag;
	}
	UINT BufLen = 0;
	VOID  *pVI = NULL;
	VS_FIXEDFILEINFO FileInfo;
	bGetFlag = VerQueryValue(szBuf,_T("\\"),&pVI,(PUINT)&BufLen);
	if(bGetFlag)
	{
		memcpy(&FileInfo,pVI,sizeof(VS_FIXEDFILEINFO));
		strVersion.Format(_T("%d.%d.%d"),HIWORD(FileInfo.dwFileVersionMS),LOWORD(FileInfo.dwFileVersionMS),HIWORD(FileInfo.dwFileVersionLS));
	}
	Translation.langID = 0x0409;
	Translation.charset = 1252;// default = ANSI code page
	bGetFlag = VerQueryValue(szBuf,_T("\\VarFileInfo\\Translation"),&pVI,(PUINT)&BufLen);
	if(bGetFlag && BufLen >= 4)
		memcpy(&Translation,pVI,sizeof(TRANSLATION));
	CString strVal;
	strVal.Format(_T("\\StringFileInfo\\%04X%04X\\CompanyName"),Translation.langID, Translation.charset);
	bGetFlag = VerQueryValue(szBuf,strVal,&pVI,(PUINT)&BufLen);
	if(bGetFlag)
	{
		strCorpName.Format(_T("%s"),(char*)pVI);
	}
	strVal.Empty();
	strVal.Format(_T("\\StringFileInfo\\%04X%04X\\FileDescription"),Translation.langID, Translation.charset);
	bGetFlag = VerQueryValue(szBuf,strVal,&pVI,(PUINT)&BufLen);
	if(bGetFlag)
	{
		strDesc.Format(_T("%s"),(char*)pVI);
	}
	strVal.Empty();
	delete []szBuf;
	szBuf = NULL;
	return bGetFlag;
}


BOOL CBaseFuncLib::GetUserNameForSID (TCHAR* StringSid, TCHAR* strUserName, long nUserNameLen)
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

BOOL CBaseFuncLib::m_bTable = FALSE;
UINT CBaseFuncLib::m_uTable[256] = {0};

void CBaseFuncLib::InitTable()
{
	int i, j;
	m_bTable = FALSE;
	for (i = 0 ; i < 256 ; i++)
	{
		for (j = 0, m_uTable[i] = i ; j < 8 ; j++)
		{
			m_uTable[i] = (m_uTable[i]>>1)^((m_uTable[i]&1)?POLYNOMIAL:0);
		}
	}
}


UINT CBaseFuncLib::Crc32(UINT crc, char *buff, int len)
{
	UINT uCrc = crc;
	if (!m_bTable) InitTable() ;
	uCrc = ~uCrc;
	if (buff != NULL)
	{
		
		for (int i = 0; i < len; i++)
		{
			uCrc = (uCrc >> 8) ^ m_uTable[(uCrc ^ buff[i]) & 0xff];
		}
	}
	return ~uCrc;
}

BOOL CBaseFuncLib::ClearFiles(CString strFile,CString strConfigFolderPath)
{
	CString str = _T("");
	str = strConfigFolderPath + strFile;

	if(CBaseFuncLib::IsPathExist(str)) {
		BOOL bFlag = FALSE;
		bFlag = ::SetFileAttributes(str,FILE_ATTRIBUTE_NORMAL);
		if (bFlag) {
			//printf("设置文件%S属性成功%s%d",strFile, __FILE__, __LINE__);
		}
		else{
			printf("设置文件%S属性失败%s%d",strFile, __FILE__, __LINE__);
		}

		bFlag = FALSE;
		bFlag = ::DeleteFile(str);
		if (bFlag) {
			//printf("删除%S文件成功%s%d",strFile, __FILE__, __LINE__);
			return TRUE;
		}
		else{
			printf("删除文件%S失败%s%d",strFile, __FILE__, __LINE__);
			return FALSE;
		}
	}else{
		printf("文件%S根本就不存在%s：%d\n",strFile, __FILE__, __LINE__);
		return FALSE;
	}

	return FALSE;
}

BOOL CBaseFuncLib::ClearConfigFolderFiles(CString strFile)
{
	CString strConfigFolderPath;
	strConfigFolderPath = CBaseFuncLib::GetAppConfigPath();

	return ClearFiles(strFile,strConfigFolderPath);
}

BOOL CBaseFuncLib::ClearDebugFolderFiles(CString strFile)
{
	CString strConfigFolderPath;
	strConfigFolderPath = CBaseFuncLib::GetModulePath();

	return ClearFiles(strFile,strConfigFolderPath);
}

// 查找pSrc字符串中是否含有pDst子串，如果有则返回pDst第一次出现的位置
int CBaseFuncLib::StringFind(const char *pSrc, const char *pDst)
{
	int i, j;
	for (i=0; pSrc[i]!='\0'; i++)
	{
		if(pSrc[i]!=pDst[0])
			continue;
		j = 0;
		while(pDst[j]!='\0' && pSrc[i+j]!='\0')
		{
			j++;
			if(pDst[j]!=pSrc[i+j])
				break;
		}
		if(pDst[j]=='\0')
			return i;
	}
	return -1;
}

