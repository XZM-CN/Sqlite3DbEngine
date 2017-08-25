// HXTestClient.cpp : main source file for HXTestClient.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

#include "StaticClass.h"
#include "BaseFuncLib.h"



void ConsoleEcho(BOOL bEcho);
void GetCommandLineToArgv(LPTSTR lpstrCmdLine);
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// 控制台回显
	ConsoleEcho(TRUE);

	GetCommandLineToArgv(lpstrCmdLine);
	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		printf("Unable to parse command line");
		return 0;
	}

	for(int i = 0; i < argCount; i++)
	{
		_bstr_t b(szArgList[i]);
		const char* c = b;
		printf("%s\n",c);
	}

	CBaseFuncLib::m_strProcessName.Empty();
	CBaseFuncLib::m_strProcessName.Format(_T("\\%sLog.txt"),szArgList[1]);
	

	WSADATA wsaData;
	int wsaret=WSAStartup(0x101,&wsaData);

	if(wsaret!=0)
	{
		return 0;
	}

	// 在进入多线程环境之前，初始化临界区
	InitializeCriticalSection(&csHeartBeat);
	InitializeCriticalSection(&csUpLoad);

	HANDLE hThread;
	DWORD dwThreadId;
	hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister, NULL, 0, &dwThreadId);

	LocalFree(szArgList);


	DWORD dw = -10000;
	while (1)
	{
		dw = WaitForSingleObject(hThread, INFINITE);
		switch(dw)
		{
		case WAIT_ABANDONED:
			{
				CString strLogInfo(_T("WaitForSingleObject -- WAIT_ABANDONED"));
				WRITEXZMLOGTOFILE(strLogInfo);
			}
			continue;
		case WAIT_OBJECT_0:
			{
				// The process terminated.
				CString strLogInfo(_T("WaitForSingleObject -- WAIT_OBJECT_0"));
				WRITEXZMLOGTOFILE(strLogInfo);
			}
			break;
		case WAIT_TIMEOUT:
			{
				// The process did not terminate within 5000 milliseconds.
				CString strLogInfo(_T("WaitForSingleObject -- WAIT_TIMEOUT"));
				WRITEXZMLOGTOFILE(strLogInfo);
			}
			continue;
		case WAIT_FAILED:
			{
				// Bad call to function (invalid handle?)
				CString strLogInfo(_T("WaitForSingleObject -- WAIT_FAILED"));
				WRITEXZMLOGTOFILE(strLogInfo);
			}
			break;
		}

		if (dw == WAIT_OBJECT_0)
		{
			CString strLogInfo(_T("已经等待到WAIT_OBJECT_0程序即将退出"));
			WRITEXZMLOGTOFILE(strLogInfo);
			break;
		}
	}

	WRITEXZMLOGTOFILE(_T("程序已退出"));
	return 0;
}

void ConsoleEcho(BOOL bEcho)
{
	if(!bEcho)
		return;

	// 带窗口的应用程序，创建一个临时的控制台，以供cout输出
	AllocConsole();

	HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);

	int hcin = _open_osfhandle((intptr_t)hin,_O_TEXT);
	FILE* fpin = _fdopen(hcin,"r");
	*stdin = *fpin; 

	int hcout = _open_osfhandle((intptr_t)hout,_O_TEXT);
	FILE* fpout = _fdopen(hcout,"wt");
	*stdout = *fpout;

	std::ios_base::sync_with_stdio();

	std::cout << "Test Console Echo" << endl << endl;
}

void GetCommandLineToArgv(LPTSTR lpstrCmdLine){
	LPWSTR *szArgList;
	int argCount;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (szArgList == NULL)
	{
		printf("Unable to parse command line");
		return ;
	}

	for(int i = 0; i < argCount; i++)
	{
		_bstr_t b(szArgList[i]);
		const char* c = b;
		printf("%s\n",c);
	}

	LocalFree(szArgList);
}