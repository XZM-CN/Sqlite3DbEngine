#include "StdAfx.h"
#include "MainAppDlg.h"
#include "AboutDlg.h"

// common
#include "BaseFuncLib.h"

extern HANDLE			g_hInstance;

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
	: m_wndOKBtn(0x01), m_wndExitBtn(0x02),m_wndAboutBtn(0x03)
{
	m_spiSqlite3Connect = NULL;
	m_spiParaService    = NULL;
	m_spiJsonService    = NULL;
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

	// Hook up controls & variables没有这一行，变量与控件关联失败
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

	HTREEITEM hItem1 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("测试接口指针，以后可能废除"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("GetComPtr通过绝对路径"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("GetComPtr通过动态路径"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("x3"));
	HTREEITEM hItem2 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("解密数据库文件（重置密码）"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("取得接口指针"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("打开带密码的数据库"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("清空密码"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("重置密码"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("释放接口指针"));
	HTREEITEM hItem3 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("打开数据库如果不存在就退出"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("取得接口指针"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("打开带密码的数据库"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("打开不带密码的数据库"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
	HTREEITEM hItem4 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("创建数据库，如果数据库存在就擦除重建"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("取得接口指针"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("打开数据库"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("加密数据库"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("解密数据库"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("创建表T_Sys"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("向表T_Sys插入数据"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("创建表T_Event"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("向表T_Event插入数据"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("创建表T_Warn"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("向表T_Warn插入数据"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("创建表T_Para"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("向表T_Para插入数据"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("查询表T_Sys"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("查询表T_Event"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("查询表T_Warn"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("查询表T_Para"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));

	HTREEITEM hItemx = NULL;
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("打开Json数据，如果文件不存在则退出"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("取得接口指针"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打开Json文件，文件不存在则退出"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("读取Json链"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("创建Json数据，如果文件不存在则创建"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("取得接口指针"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("打开Json文件，文件不存在则创建"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("创建Json文件"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("md5测试，使用独立包"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("网络传输测试"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));


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
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));

		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("把多余的枝叶剪去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("事实证明我们的担心是多余的"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("祖先结点: 从根到该结点的所经分支上的所有结点子孙结点：以某结点为根的子树中任一结点都称为该结点的子孙"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("事实证明我们的担心是多余的"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("这个字是多余的，可以去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("完全二叉树的深度"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("h(N)为卡特兰数的第N项。h(n)=C(2*n，n)/(n+1)"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("把多余的枝叶剪去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("这个字是多余的，可以去"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("左（右）子树"));
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
		else if(IsYourChild( _T("解密数据库文件（重置密码）"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("取得接口指针")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = CheckDataConnectEx(this->m_hWnd);
			}
			else if(0 == str.Compare( _T("打开带密码的数据库")))
			{
			}
			else if(0 == str.Compare( _T("清空密码")))
			{
			}
			else if(0 == str.Compare( _T("重置密码")))
			{
			}
			else if(0 == str.Compare( _T("释放接口指针")))
			{
			}
		}
		else if(IsYourChild( _T("创建数据库，如果数据库存在就擦除重建"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("取得接口指针")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = CheckDataConnectEx(this->m_hWnd);
				if(m_spiSqlite3Connect == NULL)
					m_spiSqlite3Connect = spiSqlite3Connect;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}
			else if(0 == str.Compare( _T("打开数据库")))
			{
				CString strPath = _T("");
				strPath = CBaseFuncLib::GetModulePath(g_hInstance);
				strPath = strPath + _T("HXLog.hdb");

				if ( !DoDataExchange(true) ) // == 更新控件关联的变量UpdateData
					return 0;

				if(m_spiSqlite3Connect == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connect未初始化"), _T(""), MB_OK);
					return 0;
				}
				m_spiSqlite3Connect->Open(CComBSTR(strPath),_T(""),TRUE);
			}
			else if(0 == str.Compare( _T("加密数据库")))
			{
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
		else if(IsYourChild( _T("创建Json数据，如果文件不存在则创建"),m_TreeXzm,hItemHit))
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
			else if(0 == str.Compare( _T("打开Json文件，文件不存在则创建")))
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
				strModulePath = strModulePath + _T("Config\\JsonTest.json");

				VARIANT_BOOL bRet = FALSE;
				m_spiJsonService->ParseFile(CComBSTR(strModulePath),&bRet);
			}
			else if(0 == str.Compare( _T("创建Json文件")))
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

				m_spiJsonService->TestCreateJsonFile(CComBSTR(strModulePath));
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
			else if(0 == str.Compare( _T("xxx")))
			{
			}
		}
	}

	return 0;
}