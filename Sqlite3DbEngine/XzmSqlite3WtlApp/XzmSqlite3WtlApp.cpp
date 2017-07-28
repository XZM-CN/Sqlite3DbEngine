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
// ԭ����ģ̬�Ի������ڸĳɷ�ģ̬�Ի����ˣ�ֻ�滻����Ĵ��뼴��
//
// ���޸�һ�����Ի�������˳����뼴��
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

	// �����ڵ�Ӧ�ó��򣬴���һ����ʱ�Ŀ���̨���Թ�cout���
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

	// ����̨����
	ConsoleEcho(TRUE);

	// ��ʼ��һЩ����
	InitEnvironment();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	// �����˳�ʱ������һЩ����
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

	/// ��ʼ���˺�
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