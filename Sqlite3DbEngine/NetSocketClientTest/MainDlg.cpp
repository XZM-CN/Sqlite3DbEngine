#include "stdafx.h"
#include "MainDlg.h"
#include "AboutDlg.h"

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

	// Hook up controls & variablesû����һ�У�������ؼ�����ʧ��
	if(!DoDataExchange(false))
		return FALSE;

	InitXzmTree();
	m_pXzmSocket = NULL;

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

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
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

		if(IsYourChild( _T("Socket"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("��ʼ��")))
			{
				if (m_pXzmSocket == NULL)
				{
					m_pXzmSocket = new CXzmSocket;
				}

			}
			else if(0 == str.Compare( _T("ִ��")))
			{
				DWORD dwThreadId;
				HANDLE hThread;

				hThread=CreateThread(
					NULL,//default security attributes
					0,//use default stack size
					(LPTHREAD_START_ROUTINE)CXzmSocket::DoClientTest,//thread function
					&m_pXzmSocket,//argument to thread function
					0,//use default creation flags
					&dwThreadId);//returns the thread identifier
			}
			else if(0 == str.Compare( _T("�ͷ�")))
			{
			}
		}
		else if(IsYourChild( _T("Simulation HxSafeGuard"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("�ϴ�SysLog��־")))
			{
			}
			else if(0 == str.Compare( _T("")))
			{
			}
		}
		else if(IsYourChild( _T("xxxxxx"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("xxxxxx")))
			{
			}
			else if(0 == str.Compare( _T("xxxxxx")))
			{
			}
		}
	}

	return 0;
}

void CMainDlg::InitXzmTree()
{
	HTREEITEM hRoot, hItem;
	TCHAR pc_name[64] = { 0 };
	TV_INSERTSTRUCT TCItem;//�������������ݽṹ
	TCItem.hParent = TVI_ROOT;//���Ӹ���
	TCItem.hInsertAfter = TVI_LAST;//�������֮��
	TCItem.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;//������
	TCItem.item.pszText = _T("Net Socket Communication");
	TCItem.item.lParam = 0;//��� 
	TCItem.item.iImage = 0;//����ͼ�� 
	TCItem.item.iSelectedImage = 1;//ѡ��ʱͼ�� 
	hItem = m_TreeXzm.InsertItem(&TCItem);//���ظ����� 
	m_TreeXzm.Expand(hItem, TVE_EXPAND);//չ����һ����
	hRoot = hItem;

	HTREEITEM hItem1 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("Socket"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("��ʼ��"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("ִ��"));
	InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("�ͷ�"));

	HTREEITEM hItemX = NULL;
	/*HTREEITEM*/ hItemX = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("Simulation HxSafeGuard"));//ģ��HxSafeGuard
	InsertXzmTree( m_TreeXzm, hItemX, TCItem, _T("�ϴ�SysLog��־"));
	InsertXzmTree( m_TreeXzm, hItemX, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemX, TCItem, _T("xxx"));
	/*HTREEITEM*/ hItemX = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�ͻ���ע�ᵽƽ̨"));
	InsertXzmTree( m_TreeXzm, hItemX, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemX, TCItem, _T("xxx"));
	InsertXzmTree( m_TreeXzm, hItemX, TCItem, _T("xxx"));


	m_TreeXzm.Expand(hRoot, TVE_EXPAND);//չ����һ����

}
void CMainDlg::InitDlgTreeSurplus(HTREEITEM hItem,TV_INSERTSTRUCT& TCItem)
{
	HTREEITEM hItemDemo = NULL;
	for(int i=0;i<20;i++){
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));

		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("�Ѷ����֦Ҷ��ȥ"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ʵ֤�����ǵĵ����Ƕ����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���Ƚ��: �Ӹ����ý���������֧�ϵ����н�������㣺��ĳ���Ϊ������������һ��㶼��Ϊ�ý�������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("��ʵ֤�����ǵĵ����Ƕ����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("������Ƕ���ģ�����ȥ"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("h(N)Ϊ���������ĵ�N�h(n)=C(2*n��n)/(n+1)"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("�Ѷ����֦Ҷ��ȥ"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("������Ƕ���ģ�����ȥ"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
		hItemDemo = InsertXzmTree( m_TreeXzm, hItemDemo, TCItem, _T("���ң�����"));
	}
}

HTREEITEM CMainDlg::InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name)
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
