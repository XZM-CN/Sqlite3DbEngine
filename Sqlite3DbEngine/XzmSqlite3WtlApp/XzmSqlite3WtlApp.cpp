// XzmSqlite3WtlApp.cpp : main source file for XzmSqlite3WtlApp.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "MainAppDlg.h"


CAppModule _Module;


extern HANDLE			g_hInstance;
CComPtr <IAccountMgr>	g_spiAccountMgr = NULL;


//
//
//
//
// 原来是模态对话框，现在改成非模态对话框了，只替换这里的代码即可
//
// 在修改一下主对话框里的退出代码即可
//
//
//
//
//////////////////////////////////////////////////////////////////////////

void InitEnvironment();
void ClearEnvironment();
int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT);

class XzmSqlite3WtlApp_tWinMain
{
public:
	XzmSqlite3WtlApp_tWinMain(){
	}
};

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

	std::cout << "XzmSqlite3WtlApp Application Conlose" << endl << endl;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);  // add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	g_hInstance = hInstance;

	// 控制台回显
	ConsoleEcho(TRUE);

	// 初始化一些数据
	InitEnvironment();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	// 程序退出时，清理一些数据
	ClearEnvironment();

	_Module.Term();
	::CoUninitialize();

	return nRet;
}


void InitEnvironment(){
	g_spiAccountMgr = CBaseHelper::GetAccountMgr();
	ATLASSERT(g_spiAccountMgr);

#ifdef TDHXKJ_VRSION_SINGLE
	g_spiAccountMgr->put_RunMode(RUNMODE_PERSONAL);
#endif
#ifdef TDHXKJ_VRSION_NET
	g_spiAccountMgr->put_RunMode(RUNMODE_NET);
#endif

	/// 初始化账号
	ULONG unAccount = ACCOUNTTYPE_SA|ACCOUNTTYPE_ADMIN|ACCOUNTTYPE_AUDIT;

	g_spiAccountMgr->Init(unAccount,NULL,VARIANT_TRUE);
}

void ClearEnvironment(){
	g_spiAccountMgr = NULL;
}

int Run(LPTSTR lpstrCmdLine/* = NULL*/, int nCmdShow/* = SW_SHOWDEFAULT*/)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainAppDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}