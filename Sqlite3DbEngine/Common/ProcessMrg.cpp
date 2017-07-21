// ProcessMrg.cpp: implementation of the CProcessMrg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <tlhelp32.h>
#include <Psapi.h>
#include "ProcessMrg.h"
#include "BaseFuncLib.h"

#pragma comment(lib, "Kernel32.lib ")
#pragma comment(lib, "Psapi.lib")

BOOL KillApp(const CString strAppName)
{
	BOOL bKillFlag = FALSE;
	SHORT sIndex = 0;
	DWORD dwPID = 0;
	for(;;)
	{
		if(!CProcessMrg::FindProecess(strAppName,dwPID))
		{
			bKillFlag = TRUE;
			break;
		}
		if(!CProcessMrg::TerminateProcess(dwPID))
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("%s 强制杀进程 %s 失败"),_T("KillApp"),strAppName);
			WRITELOGTOFILE(strLogInfo);
#endif
			return bKillFlag;
		}
		bKillFlag = TRUE;
		if(sIndex)
			::Sleep(1000);
		sIndex++;
		if(sIndex > 5)
			break;
	}
	return bKillFlag;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCHAR CProcessMrg::m_str[MAX_PATH];

CProcessMrg::CProcessMrg()
{
	::memset(CProcessMrg::m_str,0,MAX_PATH*sizeof(TCHAR));
}

BOOL CProcessMrg::FindProName(DWORD dwPID,CString& strFilenName)
{
	BOOL bFindFlag = FALSE;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);
	DWORD dwGetProcessID = dwPID;
	if(!dwGetProcessID)
		dwGetProcessID = ::GetCurrentProcessId();
	for(;fok ;fok = Process32Next(hSnapshot , &pe))
	{
		if(pe.th32ProcessID == dwGetProcessID)
		{
			strFilenName = _wcsupr(pe.szExeFile);
			strFilenName.MakeLower();
			bFindFlag = TRUE;
			break;
		}
	}	

	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	return bFindFlag;
}

BOOL CProcessMrg::FindProcPath(DWORD dwPID,CString& strFullPath)
{
	BOOL bGetFlag = FALSE;
	HMODULE hProcess = (HMODULE)OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
	if(NULL == hProcess)
		return bGetFlag;
	TCHAR szPath[MAX_PATH];
	memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	/// WIN 2000不支持
	DWORD dwRet = GetModuleFileNameEx(hProcess, NULL, szPath, MAX_PATH);
	if(dwRet)
		strFullPath = szPath;
	return bGetFlag;
}

BOOL CProcessMrg::FindProecess(const TCHAR *filename, DWORD &dwPID)
{
	BOOL bFindFlag = FALSE;
	HANDLE hSnapshot = NULL;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);
	if(NULL == hSnapshot)
		return bFindFlag;

	TCHAR buf[MAX_PATH];
	wcscpy(buf,filename);
	TCHAR *filenamecmp = _wcsupr(buf);
	TCHAR *exenamecmp = NULL;

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);

	for(;fok ;fok = Process32Next(hSnapshot , &pe))
	{
		exenamecmp = _wcsupr(pe.szExeFile);
		if(wcscmp(filenamecmp,exenamecmp) == 0)
		{
			dwPID = pe.th32ProcessID;
			bFindFlag = TRUE;
			break;
		}
	}	

	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	return bFindFlag;
}

DWORD CProcessMrg::GetParentPID(DWORD dwPID)
{
	DWORD dwParentProcID = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);
	DWORD dwGetProcessID = dwPID;
	if(!dwGetProcessID)
		dwGetProcessID = ::GetCurrentProcessId();
	for(;fok ;fok = Process32Next(hSnapshot , &pe))
	{
		if(pe.th32ProcessID == dwGetProcessID)
		{
			dwParentProcID = pe.th32ParentProcessID;
			break;
		}
	}	

	CloseHandle(hSnapshot);
	hSnapshot = NULL;
	return dwParentProcID;
}

TCHAR * CProcessMrg::GetProcFileName(DWORD dwPID)
{
	wcscpy(m_str,_T(""));
	HANDLE hSnapshot = NULL;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	ATLASSERT(hSnapshot);

	PROCESSENTRY32  pe = {0};
	pe.dwSize = sizeof(pe);
	BOOL fok = Process32First(hSnapshot, &pe);

	DWORD dwGetProcessID = dwPID;
	if(!dwGetProcessID)
		dwGetProcessID = ::GetCurrentProcessId();
	for(;fok ;fok = Process32Next(hSnapshot , &pe))
	{
		if(dwGetProcessID == pe.th32ProcessID)
		{
			wcscpy(m_str,pe.szExeFile);
			break;
		}
	}
	CloseHandle(hSnapshot);	
	hSnapshot = NULL;
	return m_str;
}

BOOL CProcessMrg::TerminateProcess(DWORD dwPID)
{
	HANDLE hProc = NULL;
	hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,dwPID);
	ATLASSERT(hProc);
	if(NULL == hProc)
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("%s 强制杀进程获取句柄 %s 失败，错误码=%ld"),_T("TerminateProcess"),::GetLastError());
		WRITELOGTOFILE(strLogInfo);
#endif
		return FALSE;
	}
	BOOL bRetFlag = ::TerminateProcess(hProc,0);
	if(!bRetFlag)
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("%s 强制杀进程 %s 失败，错误码=%ld"),_T("TerminateProcess"),::GetLastError());
		WRITELOGTOFILE(strLogInfo);
#endif
	}
	::CloseHandle(hProc);
	hProc = NULL;
	return bRetFlag;
}