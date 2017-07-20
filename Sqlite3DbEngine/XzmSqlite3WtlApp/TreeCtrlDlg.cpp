#include "stdafx.h"
#include "TreeCtrlDlg.h"

LRESULT CTreeCtrlDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Init dialog resizing
	DlgResize_Init();

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

	// Hook up controls & variables
	DoDataExchange(false);

	// 
	InitODBtn();

	// NOTE: CBitmapButton owns the image list now, no need to free it here.
	InitBmpBtn();

	// 
	InitLink();

	// 
	InitChkList();

	// 
	InitTree();

	// Fill in the tree
	InitRelationshipTree();

	// Fill in the tree
	InitXzmTree();
	return TRUE;
}

LRESULT CTreeCtrlDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

void CTreeCtrlDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	if ( !DoDataExchange(true) )
		return;

	CloseDialog(wID);
}

void CTreeCtrlDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/)
{
	CloseDialog(wID);
}

void CTreeCtrlDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	PostQuitMessage(nVal);
}

BOOL CTreeCtrlDlg::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

BOOL CTreeCtrlDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

DWORD CTreeCtrlDlg::OnPrePaint ( int idCtrl, NMCUSTOMDRAW* pnmcd )
{
	if ( pnmcd->hdr.idFrom != IDC_CHECK_LIST )
	{
		SetMsgHandled(false);
		return 0;
	}
	else
	{
		return CDRF_NOTIFYITEMDRAW;
	}
}

DWORD CTreeCtrlDlg::OnItemPrePaint ( int idCtrl, NMCUSTOMDRAW* pnmcd )
{
	NMLVCUSTOMDRAW* pnmlv = (NMLVCUSTOMDRAW*) pnmcd;

	if ( pnmcd->hdr.idFrom != IDC_CHECK_LIST )
	{
		SetMsgHandled(false);
		return 0;
	}

	switch ( pnmlv->nmcd.dwItemSpec )
	{
	case 0: pnmlv->clrText = RGB(255,0,0); break;
	case 1: pnmlv->clrText = RGB(0,255,0); break;
	case 2: pnmlv->clrText = RGB(0,0,255); break;
	}

	return CDRF_DODEFAULT;
}

void CTreeCtrlDlg::OnDataValidateError(UINT nCtrlID, BOOL bSave, _XData& data)
{
	ATL::CString sMsg;

	sMsg.Format ( _T("Enter a number between %d and %d"),
		data.intData.nMin, data.intData.nMax );

	MessageBox ( sMsg, _T("ControlMania2"), MB_ICONEXCLAMATION );

	GotoDlgCtrl ( GetDlgItem(nCtrlID) );
}

void CTreeCtrlDlg::OnAlysonODBtn(UINT uCode, int nID, HWND hwndCtrl)
{
	UIEnable ( IDC_ALYSON_BMPBTN, !m_wndBmpBtn.IsWindowEnabled() );
}

void CTreeCtrlDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{

}

LRESULT CTreeCtrlDlg::OnAllTreeSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	//OutputDebugString(_T("OnAllTreeSelChanged\n"));
	return 0;
}

LRESULT CTreeCtrlDlg::OnTreeSelChanged(NMHDR* phdr)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr;
	int nSelItem = m_wndChkList.GetSelectedIndex();
	ATL::CString sMsg;

	// If no item is selected, show "none". Otherwise, show its index.
	if ( -1 == nSelItem )
		sMsg = _T("(none)");
	else
		sMsg.Format ( _T("OnListItemSelChanged %3d\n"), nSelItem );

	OutputDebugString(sMsg);
	return 0;   // retval ignored
}

LRESULT CTreeCtrlDlg::OnTreeRelationshipSelChanged(NMHDR* phdr)
{
	// 
	// http://blog.csdn.net/s_h_12/article/details/8654324
	//////////////////////////////////////////////////////////////////////////
	ATL::CString sMsg;
	NMTREEVIEW *pvNmTreeView =  (NMTREEVIEW*) phdr;


	sMsg.Format ( _T("action:0x%08X  POINT.x:%3d  POINT.y:%3d\n"),
		pvNmTreeView->hdr,
		pvNmTreeView->ptDrag.x,
		pvNmTreeView->ptDrag.y);
	OutputDebugString(sMsg);
	sMsg.Empty();

	NMHDR hdr = pvNmTreeView->hdr;
	sMsg.Format ( _T("hwndFrom:0x%08X  idFrom:0x%08X  code:0x%08X\n"),
		hdr.hwndFrom,
		hdr.idFrom,
		hdr.code);
	OutputDebugString(sMsg);
	sMsg.Empty();


	TVITEM itemOld = pvNmTreeView->itemOld;
	sMsg.Format ( _T("itemOld\n  mask:%04d  hItem:0x%08X  state:%04d  stateMask:%04d  pszText:%s  cchTextMax:%04d  iImage:%04d  iSelectedImage:%04d  cChildren:%04d  lParam:0x%08X \n"),
		itemOld.mask,
		itemOld.hItem,
		itemOld.state,
		itemOld.stateMask,
		itemOld.pszText,
		itemOld.cchTextMax,
		itemOld.iImage,
		itemOld.iSelectedImage,
		itemOld.cChildren,
		itemOld.lParam);
	OutputDebugString(sMsg);
	sMsg.Empty();


	TVITEM itemNew = pvNmTreeView->itemNew;
	sMsg.Format ( _T("itemNew\n  mask:%04d  hItem:0x%08X  state:%04d  stateMask:%04d  pszText:%s  cchTextMax:%04d  iImage:%04d  iSelectedImage:%04d  cChildren:%04d  lParam:0x%08X \n"),
		itemOld.mask,
		itemOld.hItem,
		itemOld.state,
		itemOld.stateMask,
		itemOld.pszText,
		itemOld.cchTextMax,
		itemOld.iImage,
		itemOld.iSelectedImage,
		itemOld.cChildren,
		itemOld.lParam);
	OutputDebugString(sMsg);
	sMsg.Empty();


	OutputDebugString(_T("\n"));

	return 0;
}

LRESULT CTreeCtrlDlg::OnListItemSelChanged(NMHDR* phdr)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr;
	int nSelItem = m_wndChkList.GetSelectedIndex();
	ATL::CString sMsg;

	// If no item is selected, show "none". Otherwise, show its index.
	if ( -1 == nSelItem )
		sMsg = _T("(none)");
	else
		sMsg.Format ( _T("OnListItemSelChanged %3d\n"), nSelItem );

	OutputDebugString(sMsg);
	return 0;   // retval ignored
}

void CTreeCtrlDlg::InitODBtn()
{
	// Set up the bitmap button
	CImageList iml;

	iml.CreateFromImage ( IDB_ALYSON_IMGLIST, 81, 1, CLR_NONE, IMAGE_BITMAP, 
		LR_CREATEDIBSECTION );

	m_wndBmpBtn.SubclassWindow ( GetDlgItem(IDC_ALYSON_BMPBTN) );
	m_wndBmpBtn.SetToolTipText ( _T("Alyson") );
	m_wndBmpBtn.SetImageList ( iml );
	m_wndBmpBtn.SetImages ( 0, 1, 2, 3 );

}

void CTreeCtrlDlg::InitBmpBtn()
{

}

void CTreeCtrlDlg::InitLink()
{
	// Set up the hyperlink
	m_wndLink.SetHyperLink ( _T("http://www.codeproject.com/") );
}

void CTreeCtrlDlg::InitChkList()
{
	// Set up the check list ctrl.
	m_wndChkList.InsertColumn ( 0, _T("Scoobies"), LVCFMT_LEFT, 100, 0 );

	m_wndChkList.InsertItem ( 0, _T("Willow") );
	m_wndChkList.InsertItem ( 1, _T("Buffy") );
	m_wndChkList.InsertItem ( 2, _T("Giles") );

	m_wndChkList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
}

void CTreeCtrlDlg::InitTree()
{
	HTREEITEM htiSummers, htiHank, htiDawn;

	htiSummers = m_wndTree.InsertItem ( _T("Summers"), TVI_ROOT, TVI_LAST );
	m_wndTree.InsertItem ( _T("Joyce"), htiSummers, TVI_LAST );
	htiHank = m_wndTree.InsertItem ( _T("Hank"), htiSummers, TVI_LAST );
	m_wndTree.InsertItem ( _T("Buffy"), htiHank, TVI_LAST );
	htiDawn = m_wndTree.InsertItem ( _T("Dawn"), htiHank, TVI_LAST );
	m_wndTree.SetItemData ( htiDawn, 1 );

	HTREEITEM htiVamps = m_wndTree.InsertItem ( _T("Vamps"), TVI_ROOT, htiSummers );
	HTREEITEM htiMaster, htiDarla, htiAngel, htiDru, htiSpike;

	htiMaster = m_wndTree.InsertItem ( _T("The Master"), htiVamps, TVI_LAST );
	htiDarla = m_wndTree.InsertItem ( _T("Darla"), htiMaster, TVI_LAST );
	htiAngel = m_wndTree.InsertItem ( _T("Angel"), htiDarla, TVI_LAST );
	htiDru = m_wndTree.InsertItem ( _T("Drusilla"), htiAngel, TVI_LAST );
	htiSpike = m_wndTree.InsertItem ( _T("Spike"), htiDru, TVI_LAST );

	m_wndTree.SetItemData ( htiAngel, 3 );
	m_wndTree.SetItemData ( htiSpike, 3 );
	m_wndTree.SetItemData ( htiMaster, 4 );
	m_wndTree.SetItemData ( htiDarla, 4 );
	m_wndTree.SetItemData ( htiDru, 2 );

	m_wndTree.Expand ( htiSummers );
	m_wndTree.Expand ( htiHank );
}

void CTreeCtrlDlg::InitRelationshipTree()
{
	// Here's how you use CTreeItems with a CTreeViewCtrlEx
	CTreeItem tiBuffy, tiWillow, tiTara;

	// Insert a "Buffy" item, then add children to it. AddTail() adds an item
	// at the end of the existing group of child nodes. 0 is the image index,
	// which is ignored since this tree has no image list.
	tiBuffy = m_wndRelationshipTree.InsertItem ( _T("Buffy"), TVI_ROOT, TVI_LAST );
	tiBuffy.AddTail ( _T("Angel"), 0 );
	tiBuffy.AddTail ( _T("Parker"), 0 );
	tiBuffy.AddTail ( _T("Riley"), 0 );
	tiBuffy.AddTail ( _T("Spike"), 0 );

	// Now insert a "Willow" node, but insert children with AddHead(). This 
	// adds to the beginning of the current group of child nodes.
	tiWillow = m_wndRelationshipTree.InsertItem ( _T("Willow"), TVI_ROOT, tiBuffy );
	tiWillow.AddHead ( _T("Kennedy"), 0 );
	tiTara = tiWillow.AddHead ( _T("Tara"), 0 );
	tiWillow.AddHead ( _T("Oz"), 0 );

	tiWillow.Expand();

	// Just to demonstrate another CTreeItem method, this sets the item data
	// for the Tara node.
	tiTara.SetData ( 42 );
}

HTREEITEM CTreeCtrlDlg::InsertXzmTree(HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name)
{
	HTREEITEM hParentItem;
	hParentItem = hItem;
	//MultiByteToWideChar(CP_ACP, 0, troot->Name, strlen(troot->Name) + 1, pc_name,sizeof(pc_name) / sizeof(pc_name[0]));
	TCItem.item.pszText = pc_name;
	TCItem.hParent = hItem;
	hItem = m_wndXzmTree.InsertItem(&TCItem);//返回根项句柄

	m_wndXzmTree.Expand(hParentItem, TVE_EXPAND);//展开上一级树

	return hItem;
}

BOOL CTreeCtrlDlg::IsYourChild(ATL::CString strParentName, CXzmTreeViewCtrl &Tree,HTREEITEM &hSelItem)
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

void CTreeCtrlDlg::InitXzmTree()
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
	hItem = m_wndXzmTree.InsertItem(&TCItem);//返回根项句柄 
	m_wndXzmTree.Expand(hItem, TVE_EXPAND);//展开上一级树
	hRoot = hItem;

	HTREEITEM hItem1 = InsertXzmTree(hItem,TCItem,_T("01"));
	InsertXzmTree(hItem1,TCItem,_T("GetComPtr通过绝对路径"));
	InsertXzmTree(hItem1,TCItem,_T("GetComPtr通过动态路径"));
	InsertXzmTree(hItem1,TCItem,_T("x3"));
	HTREEITEM hItem2 = InsertXzmTree(hItem,TCItem,_T("02"));
	InsertXzmTree(hItem2,TCItem,_T("x1"));
	InsertXzmTree(hItem2,TCItem,_T("x2"));
	InsertXzmTree(hItem2,TCItem,_T("x3"));
	HTREEITEM hItem3 = InsertXzmTree(hItem,TCItem,_T("03"));


	m_wndXzmTree.Expand(hRoot, TVE_EXPAND);//展开上一级树

}

LRESULT CTreeCtrlDlg::OnTreeXzmSelChanged(NMHDR* phdr)
{
	return 0;   // retval ignored
}

LRESULT CTreeCtrlDlg::OnTreeXzmClickTree(NMHDR* phdr)
{
	POINT pt;
	GetCursorPos(&pt);
	POINT MousePosition = pt;
	m_wndXzmTree.ScreenToClient ( &MousePosition );
	TVHITTESTINFO HitInfo;
	HitInfo.pt = MousePosition;
	UINT uiMask = TVHT_ONITEMLABEL;//TVHT_ONITEM;// | TVHT_ONITEMRIGHT;
	HTREEITEM hItemHit = m_wndXzmTree.HitTest(&HitInfo);
	if (hItemHit != NULL &&						// Obvious
		uiMask & HitInfo.flags &&				// Make sure we are on the label
		m_wndXzmTree.GetParentItem(hItemHit) != NULL)	// Also that we have NO Parent!!
	{
		BSTR bstr = NULL;
		m_wndXzmTree.GetItemText(hItemHit,bstr);
		//if(bstr == NULL) return 0;
		ATL::CString str(bstr);

		if(IsYourChild( _T("01"),m_wndXzmTree,hItemHit))
		{
			if(0 == str.Compare( _T("GetComPtr通过绝对路径")))
			{
			}
			else if(0 == str.Compare( _T("GetComPtr通过动态路径")))
			{
			}
			else if(0 == str.Compare( _T("x3")))
			{
				OutputDebugString(_T("01\n"));
			}
		}
		else if(IsYourChild( _T("02"),m_wndXzmTree,hItemHit))
		{
			if(0 == str.Compare( _T("x3")))
			{
				OutputDebugString(_T("02\n"));
			}
		}
	}

	return 0;
}