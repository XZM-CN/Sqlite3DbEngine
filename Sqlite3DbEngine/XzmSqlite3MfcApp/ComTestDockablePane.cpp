
#include "stdafx.h"
#include "MainFrm.h"
#include "ComTestDockablePane.h"
#include "Resource.h"
#include "XzmSqlite3MfcApp.h"

#include "PublicFunc.h"


#define IDC_COMTESTVIEW 0x02
class CComTestBarMenuButton : public CMFCToolBarMenuButton
{
	friend class CComTestDockablePane;

	DECLARE_SERIAL(CComTestBarMenuButton)

public:
	CComTestBarMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CComTestBarMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

CComTestDockablePane::CComTestDockablePane()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;

	m_spiSqlite3Connect = NULL;
}

CComTestDockablePane::~CComTestDockablePane()
{
}

BEGIN_MESSAGE_MAP(CComTestDockablePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	// tree
	ON_NOTIFY(NM_CLICK, IDC_COMTESTVIEW, &CComTestDockablePane::OnNMClickTree)
	ON_NOTIFY(NM_RCLICK, IDC_COMTESTVIEW, &CComTestDockablePane::OnRclickObjectSelected)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComTestBar 消息处理程序

int CComTestDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD uStyle   =  TVS_HASBUTTONS|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT|TVS_FULLROWSELECT|TVS_EDITLABELS;
	const DWORD uExStyle = TVS_EX_ITEMLINES|TVS_EX_ITEMLINES|TVS_EX_ALTERNATECOLOR|TVS_EX_SUBSELECT|TVS_EX_FULLROWMARK;

	if (!m_cTreeList.CreateEx(WS_EX_CLIENTEDGE,uStyle|WS_CHILD|WS_VISIBLE|WS_TABSTOP,rectDummy,this,IDC_COMTESTVIEW))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}
	// 设置树状控件样式
	m_cTreeList.SetExtendedStyle(uExStyle);
	m_cTreeList.SetUserDataSize(512);
	// 设置Herder列
	m_cTreeList.InsertColumn(0,_T("func"),TVCFMT_CENTER  ,250);
	m_cTreeList.InsertColumn(1,_T("pwd"),TVCFMT_LEFT,200);
	m_cTreeList.InsertColumn(2,_T("path/dbName"),TVCFMT_LEFT  ,200);
	m_cTreeList.InsertColumn(3,_T("特定条件"),TVCFMT_CENTER  ,200);
	// 设置1,2列可以双击编辑
	m_cTreeList.SetColumnAutoEdit(1,TVAE_EDIT  | TVAE_DBLCLICK);
	m_cTreeList.SetColumnAutoEdit(2,TVAE_EDIT  | TVAE_DBLCLICK);
	m_cTreeList.SetColumnAutoEdit(3,TVAE_COMBO | TVAE_DBLCLICK,
		'|',
		_T("密码可以为空|")
		_T("密码不可以为空|")
		_T("路径是绝对地址|")
		_T("数据库名称|")
		_T("数据库名称必须是全路径,密码不为空|")
		);

	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由,而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CComTestBarMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CComTestBarMenuButton* pButton =  DYNAMIC_DOWNCAST(CComTestBarMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// 填入一些静态树视图数据(此处只需填入虚拟代码,而不是复杂的数据)
	FillComTestBar();

	return 0;
}

void CComTestDockablePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CComTestDockablePane::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_cTreeList;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CComTestDockablePane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_cTreeList.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CComTestDockablePane::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CComTestDockablePane::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CComTestBarMenuButton* pButton =  DYNAMIC_DOWNCAST(CComTestBarMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CComTestDockablePane::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CComTestDockablePane::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("添加成员函数..."));
}

void CComTestDockablePane::OnClassAddMemberVariable()
{
	// TODO: 在此处添加命令处理程序代码
}

void CComTestDockablePane::OnClassDefinition()
{
	// TODO: 在此处添加命令处理程序代码
}

void CComTestDockablePane::OnClassProperties()
{
	// TODO: 在此处添加命令处理程序代码
}

void CComTestDockablePane::OnNewFolder()
{
	AfxMessageBox(_T("新建文件夹..."));
}

BOOL CComTestDockablePane::IsYourChild(CString strParentName, CTreeCtrl &Tree,HTREEITEM &hSelItem)
{
	HTREEITEM hParentItem = Tree.GetParentItem(hSelItem);
	CString strParent = Tree.GetItemText(hParentItem);

	if(0 == strParentName.Compare(strParent))
	{
		return TRUE;
	}
	else
	{
		while (NULL != hParentItem)
		{
			hParentItem = Tree.GetParentItem(hParentItem);
			strParent = Tree.GetItemText(hParentItem);
			if(0 == strParentName.Compare(strParent))
				return TRUE;
		}
	}
	return FALSE;
}

void CComTestDockablePane::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_cTreeList.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CComTestDockablePane::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_cTreeList.SetFocus();
}

void CComTestDockablePane::OnChangeVisualStyle()
{
	m_TestBarImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_TestBarImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_TestBarImages.Add(&bmp, RGB(255, 0, 0));

	m_cTreeList.SetImageList(&m_TestBarImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}

void CComTestDockablePane::OnRclickObjectSelected(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CPoint	pt;
	UINT		uFlags;
	CMenu	menu;

	GetCursorPos(&pt);
	m_cTreeList.ScreenToClient(&pt);

	HTREEITEM hItem = m_cTreeList.HitTest(pt, &uFlags);
	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_cTreeList.SetFocus();
		m_cTreeList.Select(hItem,TVGN_CARET);

		VERIFY(menu.LoadMenu(IDR_MENU1));
		CMenu* pPopup = menu.GetSubMenu(0); 

		m_cTreeList.ClientToScreen(&pt);

		if ( pPopup != NULL )
		{
			pPopup->TrackPopupMenu(TPM_RECURSE | TPM_RIGHTBUTTON | TPM_NONOTIFY, pt.x, pt.y, this);
		}
	}
	*pResult = 0;
}

void CComTestDockablePane::FillComTestBar()
{
	HTREEITEM hRoot = m_cTreeList.InsertItem(_T("FakeApp 类"), 0, 0);
	m_cTreeList.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_cTreeList.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_cTreeList.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_cTreeList.Expand(hRoot, TVE_EXPAND);

	hClass = m_cTreeList.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_cTreeList.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_cTreeList.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_cTreeList.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_cTreeList.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_cTreeList.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_cTreeList.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_cTreeList.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_cTreeList.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_cTreeList.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_cTreeList.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_cTreeList.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_cTreeList.Expand(hClass, TVE_EXPAND);

	hClass = m_cTreeList.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_cTreeList.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_cTreeList.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_cTreeList.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_cTreeList.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_cTreeList.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_cTreeList.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_cTreeList.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_cTreeList.Expand(hClass, TVE_EXPAND);





	HTREEITEM hCom  = m_cTreeList.InsertItem(_T("COM"), 2, 2, hRoot);
	HTREEITEM hComItem = NULL,hCom01 = NULL,hCom02 = NULL,hCom03 = NULL,hCom04 = NULL,hCom05 = NULL;

	hCom01=  m_cTreeList.InsertItem(_T("01"), 1, 1, hCom);
	hComItem = m_cTreeList.InsertItem(_T("GetComPtr通过绝对路径"), 3, 3, hCom01);
	m_cTreeList.SetItem(hComItem,3,TVIF_IMAGE|TVIF_TEXT,_T("xzmTrue"),3,3,0,0,0);
	hComItem = m_cTreeList.InsertItem(_T("GetComPtr通过动态路径"), 3, 3, hCom01);
	m_cTreeList.SetItem(hComItem,1,TVIF_IMAGE|TVIF_TEXT,_T("qwer123114"),3,3,0,0,0);
	m_cTreeList.SetItem(hComItem,2,TVIF_IMAGE|TVIF_TEXT,CPublicFunc::GetAppTruncDirByModular(),3,3,0,0,0);
	m_cTreeList.SetItem(hComItem,3,TVIF_IMAGE|TVIF_TEXT,_T("xzmTrue"),3,3,0,0,0);
	m_cTreeList.Expand(hCom01, TVE_EXPAND);

	hCom02=  m_cTreeList.InsertItem(_T("02"), 1, 1, hCom);
	m_cTreeList.InsertItem(_T("Test"), 3, 3, hCom02);
	m_cTreeList.Expand(hCom02, TVE_EXPAND);

	hCom03=  m_cTreeList.InsertItem(_T("03"), 1, 1, hCom);
	m_cTreeList.InsertItem(_T("Test"), 3, 3, hCom03);
	m_cTreeList.Expand(hCom03, TVE_EXPAND);

	hCom04=  m_cTreeList.InsertItem(_T("04"), 1, 1, hCom);
	m_cTreeList.InsertItem(_T("Test"), 3, 3, hCom04);
	m_cTreeList.Expand(hCom04, TVE_EXPAND);

	hCom05=  m_cTreeList.InsertItem(_T("05"), 1, 1, hCom);
	m_cTreeList.InsertItem(_T("Test"), 3, 3, hCom05);
	m_cTreeList.Expand(hCom05, TVE_EXPAND);

	m_cTreeList.Expand(hClass, TVE_EXPAND);
	m_cTreeList.Expand(hCom  , TVE_EXPAND);

	m_hSelItem = NULL;
}

void CComTestDockablePane::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint pt;
	GetCursorPos(&pt);
	CPoint MousePosition = pt;
	m_cTreeList.ScreenToClient ( &MousePosition );
	TVHITTESTINFO HitInfo;
	HitInfo.pt = MousePosition;
	UINT uiMask = TVHT_ONITEMLABEL;//TVHT_ONITEM;// | TVHT_ONITEMRIGHT;
	HTREEITEM hItemHit = m_cTreeList.HitTest(&HitInfo);

	if (hItemHit != NULL &&						// Obvious
		uiMask & HitInfo.flags &&				// Make sure we are on the label
		m_cTreeList.GetParentItem(hItemHit) != NULL)	// Also that we have NO Parent!!
	{
		m_hSelItem = hItemHit;
		// 密码 文件路径
		CString strPwd = _T(""),strPath = _T("");
		
		CString str=m_cTreeList.GetItemText(m_hSelItem);     // 获取当前选中的树项文本

		if(IsYourChild( _T("01"),m_cTreeList,m_hSelItem))
		{
			if(0 == str.Compare( _T("GetComPtr通过绝对路径")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;

				CString str("D:\\newSvnCode\\COM\\project\\XzmSqlite3MfcApp\\bin\\Debug\\SQlite3DbEngine.dll");
				if(!str.IsEmpty())
				{
					CPublicFunc::CreateInstance( str.GetBuffer(),__uuidof(Sqlite3Connect),__uuidof(ISqlite3Connect),(VOID **)&spiSqlite3Connect);
					ATLASSERT(spiSqlite3Connect);
					m_spiSqlite3Connect = spiSqlite3Connect;
				}

				if(m_spiSqlite3Connect != NULL)
				{
					m_spiSqlite3Connect->Open(_T("wqeqwe"),_T("wqeqwe"),TRUE);
				}
			}
			if(0 == str.Compare( _T("GetComPtr通过动态路径")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;

				CString str;
				str = CPublicFunc::GetAppTruncDirByModular();
				str = str + _T("\\SQlite3DbEngine.dll");

				if(!str.IsEmpty())
				{
					CPublicFunc::CreateInstance( str.GetBuffer(),__uuidof(Sqlite3Connect),__uuidof(ISqlite3Connect),(VOID **)&spiSqlite3Connect);
					ATLASSERT(spiSqlite3Connect);
					m_spiSqlite3Connect = spiSqlite3Connect;
				}

				if(m_spiSqlite3Connect != NULL)
				{
					GetDataTree(hItemHit,strPwd,strPath);

					if(strPath.IsEmpty())
					{
						::MessageBox(NULL,_T("数据库文件不存在"),_T(""),MB_OK);
					}
					if(GetDataTree(hItemHit,3))
					{
						::MessageBox(NULL,_T("不允许密码为空"),_T(""),MB_OK);
					}

					strPath = strPath + _T("\\XzmSqlite3MfcApp.sqlite3");
					if(CPublicFunc::IsPathExist(strPath))
					{
						::SetFileAttributes(strPath,FILE_ATTRIBUTE_NORMAL);
						BOOL bDelFlag = ::DeleteFile(strPath);
						if(!bDelFlag)
						{
							::MessageBox(NULL,_T("删除旧数据库文件失败!"),_T(""),MB_OK);
							return;
						}
					}

					m_spiSqlite3Connect->Open(CComBSTR(strPath),CComBSTR(strPwd),TRUE);
				}
			}
			else if(0 == str.Compare( _T("Test01")))
			{
				int x = 1;
			}
		}

		if(IsYourChild( _T("02"),m_cTreeList,m_hSelItem))
		{
			if(0 == str.Compare( _T("Test")))
			{
				int x = 1;
			}
			else if(0 == str.Compare( _T("Test01")))
			{
				int x = 1;
			}
		}
	}
}

void CComTestDockablePane::GetDataTree(HTREEITEM hSelItem,CString& strSelCol01 ,CString& strSelCol02)
{
	strSelCol01.Empty();
	strSelCol02.Empty();

	TCHAR      cText01[256]=_T("");
	TCHAR      cText02[256]=_T("");
	UINT       iSelCol01 = 1; // 第一列
	UINT       iSelCol02 = 2; // 第二列
	TV_ITEM    sItem;

	sItem.mask		 = TVIF_TEXT|TVIF_HANDLE|TVIF_SUBITEM|TVIF_STATE;
	sItem.stateMask	 = TVIS_STATEIMAGEMASK;
	sItem.pszText	 = cText01;
	sItem.cchTextMax = sizeof(cText01);
	sItem.hItem		 = hSelItem;
	sItem.cChildren	 = iSelCol01;
	m_cTreeList.GetItem(&sItem);

	sItem.mask		 = TVIF_TEXT|TVIF_HANDLE|TVIF_SUBITEM|TVIF_STATE;
	sItem.stateMask	 = TVIS_STATEIMAGEMASK;
	sItem.pszText	 = cText02;
	sItem.cchTextMax = sizeof(cText02);
	sItem.hItem		 = hSelItem;
	sItem.cChildren	 = iSelCol02;
	m_cTreeList.GetItem(&sItem);

	strSelCol01.Format(_T("%s"),cText01);
	strSelCol02.Format(_T("%s"),cText02);
}

void CComTestDockablePane::GetDataTree(HTREEITEM hSelItem,UINT iSelCol,CString& strSelCol)
{
	strSelCol.Empty();
	TCHAR      cText[256]=_T("");
	TV_ITEM    sItem;

	sItem.mask		 = TVIF_TEXT|TVIF_HANDLE|TVIF_SUBITEM|TVIF_STATE;
	sItem.stateMask	 = TVIS_STATEIMAGEMASK;
	sItem.pszText	 = cText;
	sItem.cchTextMax = sizeof(cText);
	sItem.hItem		 = hSelItem;
	sItem.cChildren	 = iSelCol;
	m_cTreeList.GetItem(&sItem);

	strSelCol.Format(_T("%s"),cText);
}

BOOL CComTestDockablePane::GetDataTree(HTREEITEM hSelItem,UINT iSelCol)
{
	CString strSelCol;
	TCHAR      cText[256]=_T("");
	TV_ITEM    sItem;

	sItem.mask		 = TVIF_TEXT|TVIF_HANDLE|TVIF_SUBITEM|TVIF_STATE;
	sItem.stateMask	 = TVIS_STATEIMAGEMASK;
	sItem.pszText	 = cText;
	sItem.cchTextMax = sizeof(cText);
	sItem.hItem		 = hSelItem;
	sItem.cChildren	 = iSelCol;
	m_cTreeList.GetItem(&sItem);

	strSelCol.Format(_T("%s"),cText);

	if(0 == strSelCol.Compare( _T("xzmTrue")))
		return TRUE;
	else if(0 == strSelCol.Compare( _T("xzmFalse")))
		return FALSE;
	else
		return FALSE;
}

