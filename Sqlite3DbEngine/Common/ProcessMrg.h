// ProcessMrg.h: interface for the CProcessMrg class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>

class CProcessMrg  
{
	static TCHAR m_str[MAX_PATH];
public:
	CProcessMrg();
	virtual ~CProcessMrg()
	{
	}

	static DWORD GetParentPID(DWORD dwPID = 0);

	static BOOL TerminateProcess(DWORD dwPID);

	static TCHAR * GetProcFileName(DWORD dwPID = 0);

	static BOOL FindProecess(const TCHAR *filename,DWORD &dwPID);

	static BOOL FindProName(DWORD dwPID,CString& strFilenName);

	static BOOL FindProcPath(DWORD dwPID,CString& strFullPath);
};
