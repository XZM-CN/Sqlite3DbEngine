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


	/// �������ݿ�����
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

	// Hook up controls & variablesû����һ�У�������ؼ�����ʧ��
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
	TV_INSERTSTRUCT TCItem;//�������������ݽṹ
	TCItem.hParent = TVI_ROOT;//���Ӹ���
	TCItem.hInsertAfter = TVI_LAST;//�������֮��
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText = _T("COM");
	TCItem.item.lParam = 0;//��� 
	TCItem.item.iImage = 0;//����ͼ�� 
	TCItem.item.iSelectedImage = 1;//ѡ��ʱͼ�� 
	hItem = m_TreeXzm.InsertItem(&TCItem);//���ظ����� 
	m_TreeXzm.Expand(hItem, TVE_EXPAND);//չ����һ����
	hRoot = hItem;

	HTREEITEM hItem1 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("���Խӿ�ָ�룬�Ժ���ܷϳ�"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("GetComPtrͨ������·��"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("GetComPtrͨ����̬·��"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("x3"));
	HTREEITEM hItem2 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�������ݿ��ļ����������룩"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("ȡ�ýӿ�ָ��"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("�򿪴���������ݿ�"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("�������"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("��������"));
	InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("�ͷŽӿ�ָ��"));
	HTREEITEM hItem3 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�����ݿ���������ھ��˳�"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("ȡ�ýӿ�ָ��"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("�򿪴���������ݿ�"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("�򿪲�����������ݿ�"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
	HTREEITEM hItem4 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�������ݿ⣬������ݿ���ھͲ����ؽ�"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("ȡ�ýӿ�ָ��"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("�����ݿ�"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("�������ݿ�"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("�������ݿ�"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("������T_Sys"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("���T_Sys��������"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("������T_Event"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("���T_Event��������"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("������T_Warn"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("���T_Warn��������"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("������T_Para"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("���T_Para��������"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("��ѯ��T_Sys"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("��ѯ��T_Event"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("��ѯ��T_Warn"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("��ѯ��T_Para"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx"));

	HTREEITEM hItemx = NULL;
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��Json���ݣ�����ļ����������˳�"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("ȡ�ýӿ�ָ��"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("��Json�ļ����ļ����������˳�"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("��ȡJson��"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("����Json���ݣ�����ļ��������򴴽�"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("ȡ�ýӿ�ָ��"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("��Json�ļ����ļ��������򴴽�"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("����Json�ļ�"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("md5���ԣ�ʹ�ö�����"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("���紫�����"));
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


	m_TreeXzm.Expand(hRoot, TVE_EXPAND);//չ����һ����

}

void CMainAppDlg::InitDlgTree()
{
	HTREEITEM hRoot, hItem;
	TCHAR pc_name[64] = { 0 };
	TV_INSERTSTRUCT TCItem;//�������������ݽṹ
	TCItem.hParent = TVI_ROOT;//���Ӹ���
	TCItem.hInsertAfter = TVI_LAST;//�������֮��
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText = _T("COM");
	TCItem.item.lParam = 0;//��� 
	TCItem.item.iImage = 0;//����ͼ�� 
	TCItem.item.iSelectedImage = 1;//ѡ��ʱͼ�� 
	hItem = m_TreeDlgs.InsertItem(&TCItem);//���ظ����� 
	m_TreeDlgs.Expand(hItem, TVE_EXPAND);//չ����һ����
	hRoot = hItem;

	HTREEITEM hItem1 = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("01"));
	InsertXzmTree( m_TreeDlgs, hItem1, TCItem, _T("GetComPtrͨ������·��"));
	InsertXzmTree( m_TreeDlgs, hItem1, TCItem, _T("GetComPtrͨ����̬·��"));
	InsertXzmTree( m_TreeDlgs, hItem1, TCItem, _T("x3"));
	HTREEITEM hItem2 = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("02"));
	InsertXzmTree( m_TreeDlgs, hItem2, TCItem, _T("x1"));
	InsertXzmTree( m_TreeDlgs, hItem2, TCItem, _T("x2"));
	InsertXzmTree( m_TreeDlgs, hItem2, TCItem, _T("x3"));
	HTREEITEM hItem3 = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("03"));

	InitDlgTreeSurplus(hItem,TCItem);


	m_TreeDlgs.Expand(hRoot, TVE_EXPAND);//չ����һ����

}

void CMainAppDlg::InitDlgTreeSurplus(HTREEITEM  hItem,TV_INSERTSTRUCT& TCItem)
{
	HTREEITEM hItemDemo = NULL;
	for(int i=0;i<20;i++){
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));

		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("�Ѷ����֦Ҷ��ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ʵ֤�����ǵĵ����Ƕ����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���Ƚ��: �Ӹ����ý���������֧�ϵ����н�������㣺��ĳ���Ϊ������������һ��㶼��Ϊ�ý�������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��ʵ֤�����ǵĵ����Ƕ����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("������Ƕ���ģ�����ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("h(N)Ϊ���������ĵ�N�h(n)=C(2*n��n)/(n+1)"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("�Ѷ����֦Ҷ��ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("������Ƕ���ģ�����ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���ң�����"));
	}
}

HTREEITEM CMainAppDlg::InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name)
{
	HTREEITEM hParentItem;
	hParentItem = hItem;
	//MultiByteToWideChar(CP_ACP, 0, troot->Name, strlen(troot->Name) + 1, pc_name,sizeof(pc_name) / sizeof(pc_name[0]));
	TCItem.item.pszText = pc_name;
	TCItem.hParent = hItem;
	hItem = Tree.InsertItem(&TCItem);//���ظ�����

	Tree.Expand(hParentItem, TVE_EXPAND);//չ����һ����

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
			if(0 == str.Compare( _T("GetComPtrͨ������·��")))
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
			else if(0 == str.Compare( _T("GetComPtrͨ����̬·��")))
			{
			}
			else if(0 == str.Compare( _T("x3")))
			{
				OutputDebugString(_T("01\n"));
			}
		}
		else if(IsYourChild( _T("�������ݿ��ļ����������룩"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("ȡ�ýӿ�ָ��")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = CheckDataConnectEx(this->m_hWnd);
			}
			else if(0 == str.Compare( _T("�򿪴���������ݿ�")))
			{
			}
			else if(0 == str.Compare( _T("�������")))
			{
			}
			else if(0 == str.Compare( _T("��������")))
			{
			}
			else if(0 == str.Compare( _T("�ͷŽӿ�ָ��")))
			{
			}
		}
		else if(IsYourChild( _T("�������ݿ⣬������ݿ���ھͲ����ؽ�"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("ȡ�ýӿ�ָ��")))
			{
				CComPtr <ISqlite3Connect> spiSqlite3Connect = CheckDataConnectEx(this->m_hWnd);
				if(m_spiSqlite3Connect == NULL)
					m_spiSqlite3Connect = spiSqlite3Connect;
				else
					::MessageBox( 0, _T(""), _T(""), MB_OK);
			}
			else if(0 == str.Compare( _T("�����ݿ�")))
			{
				CString strPath = _T("");
				strPath = CBaseFuncLib::GetModulePath(g_hInstance);
				strPath = strPath + _T("HXLog.hdb");

				if ( !DoDataExchange(true) ) // == ���¿ؼ������ı���UpdateData
					return 0;

				if(m_spiSqlite3Connect == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}
				m_spiSqlite3Connect->Open(CComBSTR(strPath),_T(""),TRUE);
			}
			else if(0 == str.Compare( _T("�������ݿ�")))
			{
			}
			else if(0 == str.Compare( _T("�������ݿ�")))
			{
			}
			else if(0 == str.Compare( _T("������T_Sys")))
			{
			}
			else if(0 == str.Compare( _T("���T_Sys��������")))
			{
			}
			else if(0 == str.Compare( _T("������T_Event")))
			{
			}
			else if(0 == str.Compare( _T("���T_Event��������")))
			{
			}
			else if(0 == str.Compare( _T("������T_Warn")))
			{
			}
			else if(0 == str.Compare( _T("���T_Warn��������")))
			{
			}
			else if(0 == str.Compare( _T("������T_Para")))
			{
			}
			else if(0 == str.Compare( _T("���T_Para��������")))
			{
			}
			else if(0 == str.Compare( _T("��ѯ��T_Sys")))
			{
			}
			else if(0 == str.Compare( _T("��ѯ��T_Event")))
			{
			}
			else if(0 == str.Compare( _T("��ѯ��T_Warn")))
			{
			}
			else if(0 == str.Compare( _T("��ѯ��T_Para")))
			{
			}
			else if(0 == str.Compare( _T("���±��T_Sys�ֶ�")))
			{
			}
			else if(0 == str.Compare( _T("���±��T_Event�ֶ�")))
			{
			}
			else if(0 == str.Compare( _T("���±��T_Warn�ֶ�")))
			{
			}
			else if(0 == str.Compare( _T("���±��T_Para�ֶ�")))
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
		else if(IsYourChild( _T("����Json���ݣ�����ļ��������򴴽�"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("ȡ�ýӿ�ָ��")))
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
			else if(0 == str.Compare( _T("��Json�ļ����ļ��������򴴽�")))
			{
				if(m_spiParaService == NULL || m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiParaService | m_spiJsonServiceΪNULL"), _T("XzmSqlite3WtlApp"), MB_OK);
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
			else if(0 == str.Compare( _T("����Json�ļ�")))
			{
				if(m_spiParaService == NULL || m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiParaService | m_spiJsonServiceΪNULL"), _T("XzmSqlite3WtlApp"), MB_OK);
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