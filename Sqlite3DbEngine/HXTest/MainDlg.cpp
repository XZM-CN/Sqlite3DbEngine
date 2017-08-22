#include "stdafx.h"
#include "MainDlg.h"
#include "AboutDlg.h"
#include "BaseFuncLib.h"


#include "StaticClass.h"

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	if(TRUE) {
		m_NodeCountCtrl.Attach(GetDlgItem(IDC_NODECOUNT).m_hWnd);
	}
	else {
		this->GetDlgItem(IDC_NODECOUNT).ShowWindow(FALSE);
	}

	if(FALSE) { // Listctrl 的例子，这里边用的比较繁琐，实际工作用下边的那个
		m_ListCtrl.Attach(GetDlgItem(IDC_LOGLIST).m_hWnd);
		m_ListCtrl.AddColumn(_T("aaa"),0,-1);
		m_ListCtrl.AddColumn(_T("bbb"),1,-1);
		m_ListCtrl.InsertColumn(2,_T("bbb"),LVCFMT_LEFT,100);
		m_ListCtrl.InsertItem(0,_T("这里"));
		m_ListCtrl.SetItemText(0,1,_T("晴天"));
		m_ListCtrl.InsertItem(0,_T("那里"));
		m_ListCtrl.SetItemText(0,1,_T("阴天"));
	}
	else {
		// this->GetDlgItem(IDC_LOGLIST).ShowWindow(FALSE);
	}

	if(TRUE) {
		m_ListCtrl.Attach(GetDlgItem(IDC_LOGLIST).m_hWnd);
		m_ListCtrl.InsertColumn(0,_T("num"),LVCFMT_CENTER,50);
		m_ListCtrl.InsertColumn(1,_T("日志信息"),LVCFMT_LEFT,700);
		for(int i=0;i<0;i++) {
			CString str;
			str.Format(_T("%d"),i);
			m_ListCtrl.InsertItem(i,str);
			m_ListCtrl.SetItemText(i,1,_T("*App*08-03 14:05:12 TID:13460 FileName:g:_programking_sqlite3dbengine_sqlite3dbengine_hxtest_maindlg.cpp LineNum:172 Log:tttttttt"));
		}
	}
	else {
		this->GetDlgItem(IDC_LOGLIST).ShowWindow(FALSE);
	}


	if(FALSE) {// ListBox 的例子
		m_ListBox.SubclassWindow(GetDlgItem(IDC_LOGLISTBOX));
		m_ListBox.AddString(_T("*App*08-03 14:05:12 TID:13460 FileName:g:_programking_sqlite3dbengine_sqlite3dbengine_hxtest_maindlg.cpp LineNum:172 Log:tttttttt"));
		m_ListBox.AddString(_T("1;dddddddd"));
		m_ListBox.AddString(_T("1;dddddddd"));
		m_ListBox.AddString(_T("1;dddddddd"));
		m_ListBox.AddString(_T("1dddddddd"));
		m_ListBox.AddString(_T("1dddddddd"));
		m_ListBox.AddString(_T("1dddddddd"));
	}
	else {
		this->GetDlgItem(IDC_LOGLISTBOX).ShowWindow(FALSE);
	}

	// Hook up controls & variables没有这一行，变量与控件关联失败
	if(!DoDataExchange(false))
		return FALSE;

	// 是否使用树，直接涉及到socket的库的初始化的问题
	if(TRUE){
		this->GetDlgItem(IDC_TESTTREE).ShowWindow(TRUE);
		InitXzmTree();
		m_pStaticClass = new CStaticClass();
	}
	else {
		this->GetDlgItem(IDC_TESTTREE).ShowWindow(FALSE);
	}



	this->GetDlgItem(ID_APP_ABOUT).EnableWindow(FALSE);// 按钮使能
	this->GetDlgItem(ID_APP_ABOUT).ShowWindow(FALSE);// 按钮隐藏
	this->GetDlgItem(IDOK).ShowWindow(FALSE);// 按钮隐藏

	this->GetDlgItem(IDC_BTN_PAUSE).EnableWindow(FALSE);// 按钮使能
	this->GetDlgItem(IDC_BTN_STOP).EnableWindow(FALSE);// 按钮使能

// 	EnableRadio(FALSE);
// 	this->GetDlgItem(IDC_NODE01).EnableWindow(TRUE);

	if(1)
	{
		WSADATA Data;
		int status;

		//初始化windows Socket Dll
		status = WSAStartup(MAKEWORD(0x101,1),&Data);
		if (0!=status)
		{
			OutputDebugString(_T("初始化失败\n"));
		} 
	}
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::EnableRadio(BOOL b){
	this->GetDlgItem(IDC_NODE01).EnableWindow(b);
	this->GetDlgItem(IDC_NODE05).EnableWindow(b);
	this->GetDlgItem(IDC_NODE10).EnableWindow(b);
	this->GetDlgItem(IDC_NODE15).EnableWindow(b);
	this->GetDlgItem(IDC_NODE20).EnableWindow(b);
}

UINT CMainDlg::CreateSocketThread(LPVOID pParam)
{
	PThreadParam pThreadParam = (PThreadParam)pParam;

	// 在进入多线程环境之前，初始化临界区  
	InitializeCriticalSection(&csHeartBeat);  
	InitializeCriticalSection(&csUpLoad);  

	int listCount = pThreadParam->TP_pCMainDlg->m_ListCtrl.GetItemCount();

	pThreadParam->TP_pCMainDlg->m_ListCtrl.InsertItem(listCount,_T("0"));
	pThreadParam->TP_pCMainDlg->m_ListCtrl.SetItemText(listCount++,1,_T("********************************************************************"));


	

	HANDLE *hThread;
	//HANDLE hThread2[30];
	//hThread = hThread2;
	/*
	switch (pThreadParam->TP_RadioID)
	{
	case IDC_NODE01:
		hThread = (HANDLE *)malloc( sizeof(unsigned long) * 1 ); // 以字节为单位分配
		pThreadParam->TP_ThreadCount = 1;
		break;
	case IDC_NODE05:
		hThread = (HANDLE *)malloc( sizeof(unsigned long) * 30 ); // 以字节为单位分配
		pThreadParam->TP_ThreadCount = 30;
		break;
	case IDC_NODE10:
		hThread = (HANDLE *)malloc( sizeof(unsigned long) * 50 ); // 以字节为单位分配
		pThreadParam->TP_ThreadCount = 50;
		break;
	case IDC_NODE15:
		hThread = (HANDLE *)malloc( sizeof(unsigned long) * 100 ); // 以字节为单位分配
		pThreadParam->TP_ThreadCount = 100;
		break;
	case IDC_NODE20:
		hThread = (HANDLE *)malloc( sizeof(unsigned long) * 150 ); // 以字节为单位分配
		pThreadParam->TP_ThreadCount = 150;
		break;
	}
	*/
	switch (pThreadParam->TP_RadioID)
	{
	case IDC_NODE01:
		{
			HANDLE hThread;
			DWORD dwThreadId;
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister, NULL, 0, &dwThreadId);
			for (int i=0;i<1;i++)
			{
				CString str1,str2;
				str1.Format(_T("%d"),listCount);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.InsertItem(listCount,str1);
				str2.Format(_T("模拟客户端%d号启动"),i);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.SetItemText(listCount,1,str2);
			}
		}
		break;
	case IDC_NODE05:
		{
			HANDLE hThread;
			DWORD dwThreadId;
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister2, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister3, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister4, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister5, NULL, 0, &dwThreadId);
			for (int i=0;i<5;i++)
			{
				CString str1,str2;
				str1.Format(_T("%d"),listCount);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.InsertItem(listCount,str1);
				str2.Format(_T("模拟客户端%d号启动"),i);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.SetItemText(listCount,1,str2);
			}
		}
		break;
	case IDC_NODE10:
		{
			HANDLE hThread;
			DWORD dwThreadId;
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister2, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister3, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister4, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister5, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister6, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister7, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister8, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister9, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister10, NULL, 0, &dwThreadId);
			for (int i=0;i<10;i++)
			{
				CString str1,str2;
				str1.Format(_T("%d"),listCount);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.InsertItem(listCount,str1);
				str2.Format(_T("模拟客户端%d号启动"),i);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.SetItemText(listCount,1,str2);
			}
		}
		break;
	case IDC_NODE15:
		{
			HANDLE hThread;
			DWORD dwThreadId;
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister2, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister3, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister4, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister5, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister6, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister7, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister8, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister9, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister10, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister11, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister12, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister13, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister14, NULL, 0, &dwThreadId);
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister15, NULL, 0, &dwThreadId);
			for (int i=0;i<15;i++)
			{
				CString str1,str2;
				str1.Format(_T("%d"),listCount);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.InsertItem(listCount,str1);
				str2.Format(_T("模拟客户端%d号启动"),i);
				pThreadParam->TP_pCMainDlg->m_ListCtrl.SetItemText(listCount,1,str2);
			}
		}
		break;
	case IDC_NODE20:
		{
			HANDLE hThread;
			DWORD dwThreadId;
			hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister, NULL, 0, &dwThreadId);
		}
		break;
	}
	DWORD dwThreadId;


	


	
	for (int i=10000;i<pThreadParam->TP_ThreadCount;i++,listCount++)
	{
		PCStaticClass pCStaticClass = new CStaticClass();
		hThread[i]=::CreateThread(
			NULL,//default security attributes
			0,//use default stack size
			(LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister,//thread function
			pCStaticClass,//argument to thread function
			0,//use default creation flags
			&dwThreadId);//returns the thread identifier

		Sleep(2000);

		hThread[i]=::CreateThread(
			NULL,//default security attributes
			0,//use default stack size
			(LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister2,//thread function
			NULL,//argument to thread function
			0,//use default creation flags
			&dwThreadId);//returns the thread identifier



		CString str1,str2;
		str1.Format(_T("%d"),listCount);
		pThreadParam->TP_pCMainDlg->m_ListCtrl.InsertItem(listCount,str1);
		str2.Format(_T("模拟客户端%d号启动"),i);
		pThreadParam->TP_pCMainDlg->m_ListCtrl.SetItemText(listCount,1,str2);
	}

	// 等待所有ApplyRegister线程结束，向主框架窗口发送消息，所有线程已经结束，按钮可以恢复了
	return 0;
}

LRESULT CMainDlg::OnBnClickedAppLog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch (wID)
	{
	case IDC_NODE01:
		m_NodeCountCtrl.SetWindowText(_T("1"));
		EnableRadio(FALSE);
		break;
	case IDC_NODE05:
		m_NodeCountCtrl.SetWindowText(_T("30"));
		EnableRadio(FALSE);
		break;
	case IDC_NODE10:
		m_NodeCountCtrl.SetWindowText(_T("50"));
		EnableRadio(FALSE);
		break;
	case IDC_NODE15:
		m_NodeCountCtrl.SetWindowText(_T("100"));
		EnableRadio(FALSE);
		break;
	case IDC_NODE20:
		m_NodeCountCtrl.SetWindowText(_T("150"));
		EnableRadio(FALSE);
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CMainDlg::OnBtnStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DWORD dwThreadId;
	HANDLE hThread;
	if(!DoDataExchange(true))
		return FALSE;

	CString strLogInfo(_T("tttttttt"));
	WRITEXZMLOGTOFILE(strLogInfo);

	UINT  IDC_SEL = 0;
	if (0 == m_strNodeCount.Compare( _T("1")))
	{
		IDC_SEL = IDC_NODE01;
	}
	else if (0 == m_strNodeCount.Compare( _T("30")))
	{
		IDC_SEL = IDC_NODE05;
	}
	else if (0 == m_strNodeCount.Compare( _T("50")))
	{
		IDC_SEL = IDC_NODE10;
	}
	else if (0 == m_strNodeCount.Compare( _T("100")))
	{
		IDC_SEL = IDC_NODE15;
	}
	else if (0 == m_strNodeCount.Compare( _T("150")))
	{
		IDC_SEL = IDC_NODE20;
	}
	else
	{
		this->GetDlgItem(IDC_BTN_START).EnableWindow(TRUE);
		this->GetDlgItem(IDC_BTN_PAUSE).EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_STOP).EnableWindow(FALSE);
		return 0;
	}


	//return 0; // 测试按钮显隐时打开


	PThreadParam pThreadParam = new ThreadParam();
	pThreadParam->TP_pCMainDlg = this;
	pThreadParam->TP_RadioID = IDC_SEL;

	hThread=::CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		(LPTHREAD_START_ROUTINE)CMainDlg::CreateSocketThread,//thread function
		pThreadParam,//argument to thread function
		0,//use default creation flags
		&dwThreadId);//returns the thread identifier


	this->GetDlgItem(IDC_BTN_START).EnableWindow(FALSE);
	this->GetDlgItem(IDC_BTN_PAUSE).EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_STOP).EnableWindow(TRUE);
	return 0;
}

LRESULT CMainDlg::OnBtnPause(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// return 0; // 测试按钮显隐时打开

	this->GetDlgItem(IDC_BTN_START).EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_PAUSE).EnableWindow(FALSE);
	this->GetDlgItem(IDC_BTN_STOP).EnableWindow(TRUE);
	return 0;
}

LRESULT CMainDlg::OnBtnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// return 0; // 测试按钮显隐时打开

	this->GetDlgItem(IDC_BTN_START).EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_PAUSE).EnableWindow(FALSE);
	this->GetDlgItem(IDC_BTN_STOP).EnableWindow(FALSE);
	EnableRadio(TRUE);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}



HTREEITEM CMainDlg::InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name)
{
	HTREEITEM hParentItem;
	hParentItem = hItem;
	//MultiByteToWideChar(CP_ACP, 0, troot->Name, strlen(troot->Name) + 1, pc_name,sizeof(pc_name) / sizeof(pc_name[0]));
	TCItem.item.pszText = pc_name;
	TCItem.hParent = hItem;
	hItem = Tree.InsertItem(&TCItem);//返回根项句柄

	Tree.Expand(hParentItem, TVE_EXPAND);//展开上一级树

	return hItem;
}

BOOL CMainDlg::IsYourChild(ATL::CString strParentName, CTreeViewCtrl& Tree,HTREEITEM &hSelItem)
{
	HTREEITEM hParentItem = Tree.GetParentItem(hSelItem);

	BSTR bstr = NULL;
	Tree.GetItemText(hParentItem,bstr);
	if(bstr == NULL)
		return FALSE;
	ATL::CString strParent(bstr);
	if(0 == strParentName.Compare(strParent))
	{
		return TRUE;
	}
	else
	{
		while (NULL != hParentItem)
		{
			bstr = NULL;
			hParentItem = Tree.GetParentItem(hParentItem);
			Tree.GetItemText(hParentItem,bstr);
			strParent.Format(_T("%s"),bstr);
			if(0 == strParentName.Compare(strParent))
				return TRUE;
		}
	}
	return FALSE;
}

void CMainDlg::InitXzmTree()
{
	HTREEITEM hRoot, hItem;
	TCHAR pc_name[64] = { 0 };
	TV_INSERTSTRUCT TCItem;//插入数据项数据结构
	TCItem.hParent = TVI_ROOT;//增加根项
	TCItem.hInsertAfter = TVI_LAST;//在最后项之后
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText = _T("网络测试");
	TCItem.item.lParam = 0;//序号 
	TCItem.item.iImage = 0;//正常图标 
	TCItem.item.iSelectedImage = 1;//选中时图标 
	hItem = m_TreeXzm.InsertItem(&TCItem);//返回根项句柄 
	m_TreeXzm.Expand(hItem, TVE_EXPAND);//展开上一级树
	hRoot = hItem;

	HTREEITEM hItemx = NULL;

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("测试注册线程"));
	if(hItemx != NULL) {
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("初始化windows Socket"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("注册"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("发送SysLog")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("封包"));
	if(hItemx != NULL) {
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包数据头信息"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包注册到平台的数据包"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包注册成功信息"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包卸载返回状态"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包系统资源信息"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包系统版本信息"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包接收扫描磁盘命令的状态信息"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包删除单个进程白名单执行状态信息"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包添加单个进程白名单命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包上传白名单返回状态命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包安装程序命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包上传U盘信息返回状态命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包添加安全U盘命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包开启自身防护命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包设置拦截功能命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包设置安全等级命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包U盘控制设置命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包双因子登录设置命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包授权文件命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包更新授权命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包移除授权命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包开启进程白名单防护命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包获取当前配置信息命令"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打包上传用户信息返回状态命令")); m_TreeXzm.Expand(hItemx, TVE_EXPAND);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("拆包解析"));
	if(hItemx != NULL) {
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("xxxxxxxxx"));
	if(hItemx != NULL) {
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}


	m_TreeXzm.Expand(hRoot, TVE_EXPAND);//展开上一级树

}

LRESULT CMainDlg::OnTreeXzmClickTree(NMHDR* phdr)
{

	POINT pt;
	GetCursorPos(&pt);
	POINT MousePosition = pt;
	m_TreeXzm.ScreenToClient ( &MousePosition );
	TVHITTESTINFO HitInfo;
	HitInfo.pt = MousePosition;
	UINT uiMask = TVHT_ONITEMLABEL;//TVHT_ONITEM;// | TVHT_ONITEMRIGHT;
	HTREEITEM hItemHit = m_TreeXzm.HitTest(&HitInfo);
	if (hItemHit != NULL &&						// Obvious
		uiMask & HitInfo.flags &&				// Make sure we are on the label
		m_TreeXzm.GetParentItem(hItemHit) != NULL)	// Also that we have NO Parent!!
	{
		BSTR bstr = NULL;
		m_TreeXzm.GetItemText(hItemHit,bstr);
		//if(bstr == NULL) return 0;
		ATL::CString str(bstr);

		if(IsYourChild( _T("测试注册线程"),m_TreeXzm,hItemHit))
		{

			if(0 == str.Compare( _T("初始化windows Socket")))
			{
			}
			else if(0 == str.Compare( _T("注册")))
			{
				DWORD dwThreadId;
				HANDLE hThread;

				PCStaticClass pCStaticClass = new CStaticClass();
				hThread=CreateThread(
					NULL,//default security attributes
					0,//use default stack size
					(LPTHREAD_START_ROUTINE)CStaticClass::ApplyRegister,//thread function
					pCStaticClass,//argument to thread function
					0,//use default creation flags
					&dwThreadId);//returns the thread identifier
			}
			else if(0 == str.Compare( _T("发送SysLog")))
			{
				DWORD dwThreadId;
				HANDLE hThread;
				hThread=CreateThread(
					NULL,//default security attributes
					0,//use default stack size
					(LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog,//thread function
					0,//argument to thread function
					0,//use default creation flags
					&dwThreadId);//returns the thread identifier
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
		}
		else if(IsYourChild( _T("封包"),m_TreeXzm,hItemHit))
		{

			if(0 == str.Compare( _T("打包数据头信息"))) {
			}
			else if(0 == str.Compare( _T("打包注册到平台的数据包"))) {
			}
			else if(0 == str.Compare( _T("打包注册成功信息"))) {
			}
			else if(0 == str.Compare( _T("打包卸载返回状态"))) {
			}
			else if(0 == str.Compare( _T("打包系统资源信息"))) {
			}
			else if(0 == str.Compare( _T("打包系统版本信息"))) {
			}
			else if(0 == str.Compare( _T("打包接收扫描磁盘命令的状态信息"))) {
			}
			else if(0 == str.Compare( _T("打包删除单个进程白名单执行状态信息"))) {
			}
			else if(0 == str.Compare( _T("打包添加单个进程白名单命令"))) {
			}
			else if(0 == str.Compare( _T("打包上传白名单返回状态命令"))) {
			}
			else if(0 == str.Compare( _T("打包安装程序命令"))) {
			}
			else if(0 == str.Compare( _T("打包上传U盘信息返回状态命令"))) {
			}
			else if(0 == str.Compare( _T("打包添加安全U盘命令"))) {
			}
			else if(0 == str.Compare( _T("打包开启自身防护命令"))) {
			}
			else if(0 == str.Compare( _T("打包设置拦截功能命令"))) {
			}
			else if(0 == str.Compare( _T("打包设置安全等级命令"))) {
			}
			else if(0 == str.Compare( _T("打包U盘控制设置命令"))) {
			}
			else if(0 == str.Compare( _T("打包双因子登录设置命令"))) {
			}
			else if(0 == str.Compare( _T("打包授权文件命令"))) {
			}
			else if(0 == str.Compare( _T("打包更新授权命令"))) {
			}
			else if(0 == str.Compare( _T("打包移除授权命令"))) {
			}
			else if(0 == str.Compare( _T("打包开启进程白名单防护命令"))) {
			}
			else if(0 == str.Compare( _T("打包获取当前配置信息命令"))) {
			}
			else if(0 == str.Compare( _T("打包上传用户信息返回状态命令"))) {
			}
		}
	}

	return 0;
}
