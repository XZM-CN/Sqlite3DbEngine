#include "StdAfx.h"
#include "MainAppDlg.h"
#include "AboutDlg.h"
#include "LoginDlg.h"
// common
#include "BaseFuncLib.h"
#include "NetworkInteraction.h"
#include "Syslog.h"
#include "SqliteHelper.h"

extern HANDLE  g_hInstance;
extern int LoginDlg(HWND hWnd,EHXAccountType eAccountType,DWORD dwAuthCode);


CComPtr <ISqlite3Connect> CheckDataConnectEx(HWND hWnd)
{
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CString strModulePath = CBaseFuncLib::GetModulePath();

	strModulePath = strModulePath + _T("SQlite3DbEngine.dll");

	if(!strModulePath.IsEmpty())
	{
		CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(Sqlite3Connect),__uuidof(ISqlite3Connect),(VOID **)&spiSqlite3Connect);
		ATLASSERT(spiSqlite3Connect);
		return spiSqlite3Connect;
	}


	/// 返回数据库连接
	return NULL;
}



CMainAppDlg::CMainAppDlg(void)
	: m_wndOKBtn(0x01), m_wndExitBtn(0x02),m_wndAboutBtn(0x03),
	m_strDbName(_T("")),m_DbOpenPwd(_T("")),m_strDbPath(_T(""))
{
	m_spiSqlite3Connect   = NULL;
	m_spiParaService      = NULL;
	m_spiJsonService      = NULL;
	m_pNetworkInteraction = NULL;
	m_spiTestInterface    = NULL;
	m_spiXMsXmlWrapper    = NULL;
	m_spiMySQLLogic       = NULL;
	m_spiMd5Logic         = NULL;
}


CMainAppDlg::~CMainAppDlg(void)
{
}

LRESULT CMainAppDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screenO
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



	// CButtonImpl: subclass the About button
	m_wndOKBtn.SubclassWindow ( GetDlgItem(IDOK) );
	m_wndExitBtn.SubclassWindow ( GetDlgItem(IDCANCEL) );
	m_wndAboutBtn.SubclassWindow ( GetDlgItem(ID_APP_ABOUT) );

	// Hook up controls & variables没有这一行,变量与控件关联失败
	if(!DoDataExchange(false))
		return FALSE;

	InitXzmTree();
	InitDlgTree();

	return TRUE;
}

LRESULT CMainAppDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainAppDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainAppDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainAppDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

// 打开文件路径
LRESULT CMainAppDlg::OpenDbPath(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 数据从控件到变量
	if(!DoDataExchange(true)){
		return FALSE;
		printf("更新控件失败\n");
	}

	TCHAR  strFileFilters[1024]=_T("Png Files(*.png)\0*.png\0\0");
	CFileDialog   selImageDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFileFilters,this->m_hWnd);


	TCHAR  temp[1024];  
	ZeroMemory(temp,sizeof(TCHAR)*1024);


	selImageDialog.m_ofn.lpstrFile=temp;
	selImageDialog.m_ofn.nMaxFile=1024;

	if (IDOK==selImageDialog.DoModal())
	{
		wstring   wstrImagePath=temp;
		m_strDbPath.Format(_T("%s"),temp);
	}

	// 数据从变量到控件
	if(!DoDataExchange(false)){
		return FALSE;
	}

	return 0;
}

// 打开文件夹路径
LRESULT CMainAppDlg::OpenDbFolderPath(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

void CMainAppDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainAppDlg::OnSetCursor_OK(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
	static HCURSOR hcur = LoadCursor ( NULL, IDC_HAND );

	if ( NULL != hcur )
	{
		SetCursor ( hcur );
		return TRUE;
	}
	else
	{
		SetMsgHandled(false);
		return FALSE;
	}
}

LRESULT CMainAppDlg::OnSetCursor_Exit(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
	static HCURSOR hcur = LoadCursor ( NULL, IDC_NO );

	if ( NULL != hcur )
	{
		SetCursor ( hcur );
		return TRUE;
	}
	else
	{
		SetMsgHandled(false);
		return FALSE;
	}
}

void CMainAppDlg::InitXzmTree()
{
	HTREEITEM hRoot, hItem;
	TCHAR pc_name[64] = { 0 };
	TV_INSERTSTRUCT TCItem;//插入数据项数据结构
	TCItem.hParent = TVI_ROOT;//增加根项
	TCItem.hInsertAfter = TVI_LAST;//在最后项之后
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText = _T("COM");
	TCItem.item.lParam = 0;//序号 
	TCItem.item.iImage = 0;//正常图标 
	TCItem.item.iSelectedImage = 1;//选中时图标 
	hItem = m_TreeXzm.InsertItem(&TCItem);//返回根项句柄 
	m_TreeXzm.Expand(hItem, TVE_EXPAND);//展开上一级树
	hRoot = hItem;

	HTREEITEM hItemx = NULL;


	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("测试接口指针,以后可能废除"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("GetComPtr通过绝对路径"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("GetComPtr通过动态路径"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("x3")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("解密数据库文件(重置密码)"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("取得接口指针"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打开带密码的数据库"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清空密码"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("重置密码"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("释放接口指针"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("Sqlite3-xzm"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打开数据库"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("加密数据库"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清空数据库密码"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("解密数据库"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("创建表T_Sys"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("向表T_Sys插入数据"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("创建表T_Event"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("向表T_Event插入数据"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("创建表T_Warn"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("向表T_Warn插入数据"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("创建表T_Para"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("向表T_Para插入数据"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("查询表T_Sys"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("查询表T_Event"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("查询表T_Warn"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("查询表T_Para"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_EXPAND);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("MySQL-xzmdb"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打开数据库"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL05"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL06"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL07"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL08"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL09"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MySQL10"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("测试接口")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}



	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("Json Operator"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("取得接口指针"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json05"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json06"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json07"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json08"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json09"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json10"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Json11"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("IJsonService组装Json数据"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("TraversalJson"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打开Json文件,文件不存在则创建"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("创建Json文件,如果文件存在则删除重建"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("MSXMLBase"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml05"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml06"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml07"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml08"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml09"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml10"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml11"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SystemCfg"));// 正常(没有经过编码的)系统性配置文件（针对本工程）

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("MSXMLBaseEx"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml05"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml06"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml07"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml08"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml09"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml10"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_MsXml11"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("打印Log测试"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELOGTOFILE"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELOGTOFILE2"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELASTLOGTOFILE"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELASTLOGTOFILE2"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITETOFILE2"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("CREATE_MYTHREAD"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("md5测试,使用独立包"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("对字符串进行MD5并Hash"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("SecurityCollection"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC05"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("xxx"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("清理垃圾文件")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}


	m_TreeXzm.Expand(hRoot, TVE_EXPAND);//展开上一级树

}

void CMainAppDlg::InitDlgTree()
{
	HTREEITEM hRoot, hItem;
	TCHAR pc_name[64] = { 0 };
	TV_INSERTSTRUCT TCItem;//插入数据项数据结构
	TCItem.hParent = TVI_ROOT;//增加根项
	TCItem.hInsertAfter = TVI_LAST;//在最后项之后
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//设屏蔽
	TCItem.item.pszText = _T("COM");
	TCItem.item.lParam = 0;//序号 
	TCItem.item.iImage = 0;//正常图标 
	TCItem.item.iSelectedImage = 1;//选中时图标 
	hItem = m_TreeDlgs.InsertItem(&TCItem);//返回根项句柄 
	m_TreeDlgs.Expand(hItem, TVE_EXPAND);//展开上一级树
	hRoot = hItem;

	HTREEITEM hItem1 = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("01"));
	InsertXzmTree( m_TreeDlgs, hItem1, TCItem, _T("GetComPtr通过绝对路径"));
	InsertXzmTree( m_TreeDlgs, hItem1, TCItem, _T("GetComPtr通过动态路径"));
	InsertXzmTree( m_TreeDlgs, hItem1, TCItem, _T("x3"));
	HTREEITEM hItem2 = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("02"));
	InsertXzmTree( m_TreeDlgs, hItem2, TCItem, _T("x1"));
	InsertXzmTree( m_TreeDlgs, hItem2, TCItem, _T("x2"));
	InsertXzmTree( m_TreeDlgs, hItem2, TCItem, _T("x3"));
	HTREEITEM hItem3 = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("03"));

	InitDlgTreeSurplus(hItem,TCItem);


	m_TreeDlgs.Expand(hRoot, TVE_EXPAND);//展开上一级树

}

void CMainAppDlg::InitDlgTreeSurplus(HTREEITEM  hItem,TV_INSERTSTRUCT& TCItem)
{
	HTREEITEM hItemDemo = NULL;
	for(int i=0;i<20;i++){
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));

		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("把多余的枝叶剪去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("事实证明我们的担心是多余的"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("祖先结点: 从根到该结点的所经分支上的所有结点子孙结点：以某结点为根的子树中任一结点都称为该结点的子孙"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("事实证明我们的担心是多余的"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("这个字是多余的,可以去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("h(N)为卡特兰数的第N项。h(n)=C(2*n,n)/(n+1)"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("把多余的枝叶剪去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("这个字是多余的,可以去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左(右)子树"));
	}
}

HTREEITEM CMainAppDlg::InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name)
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

BOOL CMainAppDlg::IsYourChild(ATL::CString strParentName, CTreeViewCtrl &Tree,HTREEITEM &hSelItem)
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

UINT CMainAppDlg::SendSyslogThread(LPVOID pParam)
{
	CMainAppDlg* pThis = (CMainAppDlg *)pParam;
	if(NULL == pThis)
		return 0;

	//创建上传日志对象
	CSysLog syslog;
	//创建通信机制
	syslog.CreateUDPCommunication();

	while(pThis->m_bSysLogFlag)
	{
		//处理上传事务
		OutputDebugString(_T("开始处理上传syslog日志"));
		syslog.GetLogDataFromDB();

		Sleep(2000);
	}

	//在主界面退出时,退出当前线程

	return 0;
}

void CMainAppDlg::OnDataExchangeError(UINT nCtrlID, BOOL bSave)
{
	ATL::CString str;

	str.Format ( _T("DDX error during exchange with control: %u"), nCtrlID );
	MessageBox ( str, _T("CMainAppDlg"), MB_ICONWARNING );

	::SetFocus ( GetDlgItem(nCtrlID) );
}

LRESULT CMainAppDlg::OnTreeXzmClickTree(NMHDR* phdr)
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


		if(IsYourChild( _T("01"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("GetComPtr通过绝对路径")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;

				ATL::CString str("D:\\newSvnCode\\COM\\project\\XzmSqlite3MfcApp\\bin\\Debug\\SQlite3DbEngine.dll");
				if(!str.IsEmpty())
				{
					CBaseFuncLib::CreateInstance( str.GetBuffer(),__uuidof(Sqlite3Connect),__uuidof(ISqlite3Connect),(VOID **)&spiSqlite3Connect);
					ATLASSERT(spiSqlite3Connect);
					m_spiSqlite3Connect = spiSqlite3Connect;
				}

				if(m_spiSqlite3Connect != NULL)
				{
					m_spiSqlite3Connect->Open(_T("wqeqwe"),_T("wqeqwe"),TRUE);
				}
			}
			else if(0 == str.Compare( _T("GetComPtr通过动态路径")))
			{
			}
			else if(0 == str.Compare( _T("x3")))
			{
				OutputDebugString(_T("01\n"));
			}
		}
		else if(IsYourChild( _T("Sqlite3-xzm"),m_TreeXzm,hItemHit))
		{
			if(m_spiSqlite3Connect == NULL)
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = CheckDataConnectEx(this->m_hWnd);
				if(m_spiSqlite3Connect == NULL)
					m_spiSqlite3Connect = spiSqlite3Connect;
				else
					::MessageBox( 0, _T("接口指针获取失败"), _T("warning"), MB_OK);
			}
			else if(0 == str.Compare( _T("打开数据库")))
			{
				CString strPath = _T("");
				strPath = CBaseFuncLib::GetModulePath(g_hInstance);
				strPath = strPath + _T("HXLog.hdb");

				if ( !DoDataExchange(true) ) // == 更新控件关联的变量UpdateData
					return 0;

				HRESULT hr = E_FAIL;
				if (0 == m_DbOpenPwd.Compare( _T(""))) {
					printf("m_DbOpenPwd的值为空\n");
					hr = m_spiSqlite3Connect->Open(CComBSTR(strPath),_T(""),TRUE);
					if(FAILED(hr)){
						printf("打开数据库失败\n");
					}
				}
				else {
					printf("m_DbOpenPwd的值不为空\n");
					hr = m_spiSqlite3Connect->Open(CComBSTR(strPath),CComBSTR(m_DbOpenPwd),TRUE);
					if(FAILED(hr)){
						printf("打开数据库失败\n");
					}
				}
				
			}
			else if(0 == str.Compare( _T("加密数据库")))
			{
				if ( !DoDataExchange(true) ) // == 更新控件关联的变量UpdateData
					return 0;

				if (0 == m_DbModifyPwd.Compare( _T(""))) {
					int iMsg = ::MessageBoxW(NULL,_T("密码为空请设置一个密码"),_T("waring"),MB_OKCANCEL);
					if(iMsg == 1/*确定*/){
						if ( !DoDataExchange(true) ) // == 更新控件关联的变量UpdateData
							return 0;

						// 进行数据库加密代码逻辑
					}
					else if (iMsg == 2/*取消*/)
					{
						return 0;
					}
				}
				else{
					int iMsg = ::MessageBoxW(NULL,_T("请记住加密的密码"),_T("waring"),MB_OK);
					if(iMsg == 1/*确定*/){
						// 解密数据库，使用DbPwd控件对应的变量 -- m_DbPwd
					}
					else if (iMsg == 2/*取消*/)
					{
						return 0;
					}
				}
			}
			else if(0 == str.Compare( _T("清空数据库密码")))
			{
				int iMsg = ::MessageBoxW(NULL,_T("确定要清空数据库的密码吗?"),_T("waring"),MB_OKCANCEL);
				if(iMsg == 1/*确定*/){
					// 清空密码
				}
				else if (iMsg == 2/*取消*/)
				{
					return 0;
				}
			}
			else if(0 == str.Compare( _T("解密数据库")))
			{

			}
			else if(0 == str.Compare( _T("创建表T_Sys")))
			{
			}
			else if(0 == str.Compare( _T("向表T_Sys插入数据")))
			{
			}
			else if(0 == str.Compare( _T("创建表T_Event")))
			{
			}
			else if(0 == str.Compare( _T("向表T_Event插入数据")))
			{
			}
			else if(0 == str.Compare( _T("创建表T_Warn")))
			{
			}
			else if(0 == str.Compare( _T("向表T_Warn插入数据")))
			{
			}
			else if(0 == str.Compare( _T("创建表T_Para")))
			{
			}
			else if(0 == str.Compare( _T("向表T_Para插入数据")))
			{
			}
			else if(0 == str.Compare( _T("查询表T_Sys")))
			{
			}
			else if(0 == str.Compare( _T("查询表T_Event")))
			{
			}
			else if(0 == str.Compare( _T("查询表T_Warn")))
			{
			}
			else if(0 == str.Compare( _T("查询表T_Para")))
			{
			}
			else if(0 == str.Compare( _T("更新表的T_Sys字段")))
			{
			}
			else if(0 == str.Compare( _T("更新表的T_Event字段")))
			{
			}
			else if(0 == str.Compare( _T("更新表的T_Warn字段")))
			{
			}
			else if(0 == str.Compare( _T("更新表的T_Para字段")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxx")))
			{
			}
		}
		else if(IsYourChild( _T("MySQL-xzmdb"),m_TreeXzm,hItemHit)){
			if(m_spiMySQLLogic == NULL)
			{
				CComPtr <IMySQLLogic> spiMySQLLogic = NULL;
				CString strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("MySqlDbEngine.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(MySQLLogic),__uuidof(IMySQLLogic),(VOID **)&spiMySQLLogic);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spiMySQLLogic);
				}
				if(m_spiMySQLLogic == NULL)
					m_spiMySQLLogic = spiMySQLLogic;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}

			if(0 == str.Compare( _T("打开数据库"))){
				m_spiMySQLLogic->OpenDB(_T("localhost"), _T("root"), _T("qwer123124&S"), _T("xzmdb"), 3306);
			}
			else if(0 == str.Compare( _T("e_MySQL00"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL00);
			}
			else if(0 == str.Compare( _T("e_MySQL01"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL01);
			}
			else if(0 == str.Compare( _T("e_MySQL02"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL02);
			}
			else if(0 == str.Compare( _T("e_MySQL03"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL03);
			}
			else if(0 == str.Compare( _T("e_MySQL04"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL04);
			}
			else if(0 == str.Compare( _T("e_MySQL05"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL05);
			}
			else if(0 == str.Compare( _T("e_MySQL06"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL06);
			}
			else if(0 == str.Compare( _T("e_MySQL07"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL07);
			}
			else if(0 == str.Compare( _T("e_MySQL08"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL08);
			}
			else if(0 == str.Compare( _T("e_MySQL09"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL09);
			}
			else if(0 == str.Compare( _T("e_MySQL10"))){
				m_spiMySQLLogic->TestAllDesignFormat(e_MySQL10);
			}
			else if(0 == str.Compare( _T("测试接口"))){
			}
		}
		else if(IsYourChild( _T("Json Operator"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("取得接口指针")))
			{
				CComPtr <IParaService> spInterface = NULL;
				CString strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("JsonEngine.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(ParaService),__uuidof(IParaService),(VOID **)&spInterface);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spInterface);
				}
				if(m_spiParaService == NULL)
					m_spiParaService = spInterface;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);

				CComPtr <IJsonService> spIJsonService = NULL;
				strModulePath.Empty();
				strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("JsonEngine.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(JsonService),__uuidof(IJsonService),(VOID **)&spIJsonService);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spIJsonService);
				}
				if(m_spiJsonService == NULL)
					m_spiJsonService = spIJsonService;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}
			else if(0 == str.Compare( _T("e_Json00")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}
				m_spiJsonService->TestAllJsonDesignFormat(e_Json00);
			}
			else if(0 == str.Compare( _T("e_Json01")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json01);
			}
			else if(0 == str.Compare( _T("e_Json02")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json02);
			}
			else if(0 == str.Compare( _T("e_Json03")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json03);
			}
			else if(0 == str.Compare( _T("e_Json04")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json04);
			}
			else if(0 == str.Compare( _T("e_Json05")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json05);
			}
			else if(0 == str.Compare( _T("e_Json06")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json06);
			}
			else if(0 == str.Compare( _T("e_Json07")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json07);
			}
			else if(0 == str.Compare( _T("e_Json08")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Json08);
			}
			// IJsonService组装Json数据
			else if(0 == str.Compare( _T("IJsonService组装Json数据")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				/// 写入配置
				CComBSTR bstrVal,bstrVal1,bstrVal2;
				CComPtr <IJsonService> spiJsonService2 = NULL;


				m_spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
				m_spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),2);
				m_spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),enumHeartbeat);
				m_spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
				spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
				spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_REGISTAR),1);
				spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));

				bstrVal1.Empty();
				m_spiJsonService->get_ObjectString(&bstrVal1);
				bstrVal2.Empty();
				spiJsonService2->get_ObjectString(&bstrVal2);

				m_spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
				m_spiJsonService->get_ObjectString(&bstrVal);

				CString strTemp1 = bstrVal.m_str;
				bstrVal.Empty();
				spiJsonService2->get_ObjectString(&bstrVal);
				CString strTemp2 = bstrVal.m_str;
				bstrVal.Empty();

				//spiJsonService->PutChild(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
				m_spiJsonService->get_ObjectString(&bstrVal);

				CString m_strHeartBeat = bstrVal.m_str;
			}
			// 遍历整个Json
			else if(0 == str.Compare( _T("TraversalJson")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}

				CString strPath = CBaseFuncLib::GetAppConfigPath();
				strPath = strPath + _T("Traversal.json");
				m_spiJsonService->TraversalJson(CComBSTR(strPath));
			}
			else if(0 == str.Compare( _T("打开Json文件,文件不存在则创建")))
			{
				if(m_spiParaService == NULL || m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiParaService | m_spiJsonService为NULL"), _T("XzmSqlite3WtlApp"), MB_OK);
					return 0;
				}
				CString strPath = CBaseFuncLib::GetAppConfigPath();
				strPath = strPath + _T("JsonTest.json");

				VARIANT_BOOL bRet = FALSE;
				m_spiJsonService->ParseFile(CComBSTR(strPath),&bRet);
			}
			else if(0 == str.Compare( _T("创建Json文件,如果文件存在则删除重建")))
			{
				if(m_spiParaService == NULL || m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiParaService | m_spiJsonService为NULL"), _T("XzmSqlite3WtlApp"), MB_OK);
					return 0;
				}
				CString strModulePath = CBaseFuncLib::GetModulePath();
				int iFind = -1;
				iFind = strModulePath.ReverseFind(_T('\\'));
				strModulePath = strModulePath.Left(iFind);
				iFind = strModulePath.ReverseFind(_T('\\'));
				strModulePath = strModulePath.Left(iFind);
				iFind = strModulePath.ReverseFind(_T('\\'));
				strModulePath = strModulePath.Left(iFind+1);
				strModulePath = strModulePath + _T("Config\\JsonCreateTest.json");

				m_spiJsonService->TestCreateJsonToFile(CComBSTR(strModulePath));
			}
		}
		else if(IsYourChild( _T("MSXMLBase"),m_TreeXzm,hItemHit))
		{
			if(m_spiXMsXmlWrapper == NULL)
			{
				CComPtr <IXMsXmlWrapper> spiXMsXmlWrapper = NULL;
				CString strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("MsXmlEngine.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(XMsXmlWrapper),__uuidof(IXMsXmlWrapper),(VOID **)&spiXMsXmlWrapper);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spiXMsXmlWrapper);
				}
				if(m_spiXMsXmlWrapper == NULL)
					m_spiXMsXmlWrapper = spiXMsXmlWrapper;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}

			if(0 == str.Compare( _T("e_MsXml00")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml00);
			}
			else if(0 == str.Compare( _T("e_MsXml01")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml01);
			}
			else if(0 == str.Compare( _T("e_MsXml02")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml02);
			}
			else if(0 == str.Compare( _T("e_MsXml03")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml03);
			}
			else if(0 == str.Compare( _T("e_MsXml04")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml04);
			}
			else if(0 == str.Compare( _T("e_MsXml05")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml05);
			}
			else if(0 == str.Compare( _T("e_MsXml06")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml06);
			}
			else if(0 == str.Compare( _T("e_MsXml07")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml07);
			}
			else if(0 == str.Compare( _T("e_MsXml08")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml08);
			}
			else if(0 == str.Compare( _T("e_MsXml09")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml09);
			}
			else if(0 == str.Compare( _T("e_MsXml10")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml10);
			}
			else if(0 == str.Compare( _T("e_MsXml11")))
			{
				m_spiXMsXmlWrapper->TestAllMsxmlDesignFormat(e_MsXml11);
			}
			else if(0 == str.Compare( _T("SystemCfg")))
			{
				m_spiXMsXmlWrapper->SystemCfg();
			}
			else if(0 == str.Compare( _T("清理垃圾文件")))
			{
				// 程序生成的文件
				CBaseFuncLib::ClearConfigFolderFiles(_T("createXml.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("createXmlEx.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo01.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo02.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo03.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo04.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo05.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo06.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo07.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo08.xml"));
				CBaseFuncLib::ClearConfigFolderFiles(_T("CreateDemo09.xml"));

				// 程序需要读取的文件
				// CBaseFuncLib::ClearConfigFolderFiles(_T("OpenXml.xml"));
			}
		}
		else if(IsYourChild( _T("MSXMLBaseEx"),m_TreeXzm,hItemHit))
		{
			if(m_spiXMsXmlWrapper == NULL)
			{
				CComPtr <IXMsXmlWrapper> spiXMsXmlWrapper = NULL;
				CString strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("MsXmlEngine.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(XMsXmlWrapper),__uuidof(IXMsXmlWrapper),(VOID **)&spiXMsXmlWrapper);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spiXMsXmlWrapper);
				}
				if(m_spiXMsXmlWrapper == NULL)
					m_spiXMsXmlWrapper = spiXMsXmlWrapper;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}

			if(0 == str.Compare( _T("e_MsXml00")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml01")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml02")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml03")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml04")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml05")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml06")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml07")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml08")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml09")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml10")))
			{
			}
			else if(0 == str.Compare( _T("e_MsXml11")))
			{
			}
		}
		else if(IsYourChild( _T("打印Log测试"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("WRITELOGTOFILE")))
			{
				LONG lFoldID = 1000;
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("SHGetSpecialFolderPath调用 %d 失败，错误码：%ld"),lFoldID,::GetLastError());
				WRITELOGTOFILE(strLogInfo);
			}
			else if(0 == str.Compare( _T("WRITELOGTOFILE2")))
			{
				WRITELOGTOFILE2(_T("授权对象错误"),_T("Auth"),LOGTYPE_NORMAL);
			}
			else if(0 == str.Compare( _T("WRITELASTLOGTOFILE")))
			{
			}
			else if(0 == str.Compare( _T("WRITELASTLOGTOFILE2")))
			{
			}
			else if(0 == str.Compare( _T("WRITETOFILE2")))
			{
			}
			else if(0 == str.Compare( _T("CREATE_MYTHREAD")))
			{
			}
			else if(0 == str.Compare( _T("清理垃圾文件")))
			{
				CBaseFuncLib::ClearDebugFolderFiles(_T("Log.txt"));
			}
		}
		else if(IsYourChild( _T("md5测试,使用独立包"),m_TreeXzm,hItemHit))
		{
			if(m_spiMd5Logic ==  NULL) {
				CComPtr <IMd5Logic> spIMd5Logic = NULL;
				CString strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("EncryEngine.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(Md5Logic),__uuidof(IMd5Logic),(VOID **)&spIMd5Logic);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spIMd5Logic);
				}
				if(m_spiMd5Logic == NULL)
					m_spiMd5Logic = spIMd5Logic;
				else
					::MessageBox( 0, _T("出现不可能的差错了"), _T("warning"), MB_OK);
			}
			if(0 == str.Compare( _T("对字符串进行MD5并Hash")))
			{
				if(!DoDataExchange(true))
					return FALSE;
				if(m_DbOpenPwd.GetLength() < 5)
				{
					this->MessageBox(_T("密码输入不合法！"),_T("Warning"),MB_OK);
					return 0;
				}

				CComBSTR bstrMD5,bstrSha1;

				m_spiMd5Logic->StringMD5(CComBSTR(m_DbOpenPwd),&bstrMD5);
				OutputDebugString(_T("m_DbOpenPwd   "));
				OutputDebugString(m_DbOpenPwd);
				OutputDebugString(_T("\n"));
				OutputDebugString(_T("bstrMD5   "));
				OutputDebugString(bstrMD5);
				OutputDebugString(_T("\n"));

				m_spiMd5Logic->StringSha1(bstrMD5,&bstrSha1);
				OutputDebugString(_T("bstrMD5   "));
				OutputDebugString(bstrMD5);
				OutputDebugString(_T("\n"));
				OutputDebugString(_T("bstrSha1   "));
				OutputDebugString(bstrSha1);
				OutputDebugString(_T("\n"));
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
		}
		else if(IsYourChild( _T("SecurityCollection"),m_TreeXzm,hItemHit))
		{
			if(m_spiTestInterface == NULL)
			{
				CComPtr <ITestInterface> spInterface = NULL;
				CString strModulePath = CBaseFuncLib::GetModulePath();
				strModulePath = strModulePath + _T("SecurityCollection.dll");
				if(!strModulePath.IsEmpty())
				{
					HINSTANCE hInst = NULL;
					hInst = CBaseFuncLib::CreateInstance( strModulePath.GetBuffer(),__uuidof(TestInterface),__uuidof(ITestInterface),(VOID **)&spInterface);
					if(hInst == NULL)
						return 0;
					ATLASSERT(spInterface);
				}
				if(m_spiJsonService == NULL)
					m_spiTestInterface = spInterface;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}

			if(0 == str.Compare( _T("SC00")))
			{
				m_spiTestInterface->Test(e_SC00);
			}
			else if(0 == str.Compare( _T("SC01")))
			{
				m_spiTestInterface->Test(e_SC01);
			}
			else if(0 == str.Compare( _T("SC02")))
			{
			}
			else if(0 == str.Compare( _T("SC03")))
			{
			}
		}
		else if(IsYourChild( _T("xxx"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
		}
		else if(IsYourChild( _T("xxx"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
		}
		else if(IsYourChild( _T("xxx"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
		}
		else if(IsYourChild( _T("xxx"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
			else if(0 == str.Compare( _T("xxx")))
			{
			}
		}
	}

	return 0;
}

UINT CMainAppDlg::DoClientTest(LPVOID pParam)
{
	CString strSendData;
	if(TRUE){
		DWORD dwCrc32 = 0;
		CComBSTR bstrVal;

		CComPtr <IJsonService> spiJsonService = NULL;
		CComPtr <IJsonService> spiJsonService2 = NULL;
		if(NULL == spiJsonService)
			spiJsonService = CDbHelper::GetJsonService();
		if(NULL != spiJsonService)
		{
			/// 写入配置
			spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
			spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),dwCrc32);
			spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),enumHeartbeat);
			spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
			spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
			spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_REGISTAR),1);
			spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
			spiJsonService->get_ObjectString(&bstrVal);

			CString strTemp1 = bstrVal.m_str;
			bstrVal.Empty();
			spiJsonService2->get_ObjectString(&bstrVal);
			CString strTemp2 = bstrVal.m_str;
			bstrVal.Empty();

			//spiJsonService->PutChild(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
			spiJsonService->get_ObjectString(&bstrVal);

			CString m_strHeartBeat = bstrVal.m_str;
			strSendData = m_strHeartBeat;
		}

	}


	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(8088);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		return 0;
	}


	char *pText = NULL;
	int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
	::send(xxx, pText, strlen(pText), 0);


	// 接收数据
	char buff[256];
	int nRecv = ::recv(xxx, buff, 256, 0);
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" 接收到数据：%s", buff);
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

