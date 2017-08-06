#include "StdAfx.h"
#include "MainAppDlg.h"
#include "AboutDlg.h"
#include "LoginDlg.h"
// common
#include "BaseFuncLib.h"
#include "NetworkInteraction.h"
#include "XzmSyslog.h"
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


	/// �������ݿ�����
	return NULL;
}



CMainAppDlg::CMainAppDlg(void)
	: m_wndOKBtn(0x01), m_wndExitBtn(0x02),m_wndAboutBtn(0x03)
{
	m_spiSqlite3Connect = NULL;
	m_spiParaService    = NULL;
	m_spiJsonService    = NULL;
	m_pNetworkInteraction = NULL;
	m_spiTestInterface = NULL;
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

	// Hook up controls & variablesû����һ��,������ؼ�����ʧ��
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

	HTREEITEM hItemx = NULL;

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("����̨����"));


	HTREEITEM hItem1 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("���Խӿ�ָ��,�Ժ���ܷϳ�"));
	if (hItem1 != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("GetComPtrͨ������·��"));
		InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("GetComPtrͨ����̬·��"));
		InsertXzmTree( m_TreeXzm, hItem1, TCItem, _T("x3")); m_TreeXzm.Expand(hItem1, TVE_COLLAPSE);
	}

	HTREEITEM hItem2 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�������ݿ��ļ�(��������)"));
	if (hItem2 != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("ȡ�ýӿ�ָ��"));
		InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("�򿪴���������ݿ�"));
		InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("�������"));
		InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("��������"));
		InsertXzmTree( m_TreeXzm, hItem2, TCItem, _T("�ͷŽӿ�ָ��")); m_TreeXzm.Expand(hItem2, TVE_COLLAPSE);
	}

	HTREEITEM hItem3 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�����ݿ���������ھ��˳�"));
	if (hItem3 != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("ȡ�ýӿ�ָ��"));
		InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("�򿪴���������ݿ�"));
		InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("�򿪲�����������ݿ�"));
		InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItem3, TCItem, _T("xxx")); m_TreeXzm.Expand(hItem3, TVE_COLLAPSE);
	}

	HTREEITEM hItem4 = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("�������ݿ�,������ݿ���ھͲ����ؽ�"));
	if (hItem4 != NULL)
	{
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
		InsertXzmTree( m_TreeXzm, hItem4, TCItem, _T("xxx")); m_TreeXzm.Expand(hItem4, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("Json Operator"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("ȡ�ýӿ�ָ��"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test05"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test06"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test07"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test08"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test09"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test10"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test11"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test12"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test13"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test14"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test15"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test16"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test17"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test18"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test19"));
		// 	InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("e_Test20"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("IJsonService��װJson����"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("TraversalJson"));

		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("��Json�ļ�,�ļ��������򴴽�"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("����Json�ļ�,����ļ�������ɾ���ؽ�"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("��ӡLog����"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELOGTOFILE"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELOGTOFILE2"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELASTLOGTOFILE"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITELASTLOGTOFILE2"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("WRITETOFILE2"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("CREATE_MYTHREAD"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_EXPAND);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("md5����,ʹ�ö�����"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("���紫�����-ģ��ͻ���"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("����ͨ�Žӿ�"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("ע�ᵽƽ̨"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("���紫�����-�Զ���"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("��ʼ��windows Socket"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("��ʼ������"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("SecurityCollection"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC00"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC01"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC02"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC03"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC04"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("SC05")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}

	/*HTREEITEM*/ hItemx = InsertXzmTree( m_TreeXzm, hItem, TCItem, _T("xxx"));
	if (hItemx != NULL)
	{
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx"));
		InsertXzmTree( m_TreeXzm, hItemx, TCItem, _T("xxx")); m_TreeXzm.Expand(hItemx, TVE_COLLAPSE);
	}


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
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));

		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("�Ѷ����֦Ҷ��ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ʵ֤�����ǵĵ����Ƕ����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("���Ƚ��: �Ӹ����ý���������֧�ϵ����н�������㣺��ĳ���Ϊ������������һ��㶼��Ϊ�ý�������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��ʵ֤�����ǵĵ����Ƕ����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("������Ƕ����,����ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("��ȫ�����������"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItem, TCItem, _T("h(N)Ϊ���������ĵ�N�h(n)=C(2*n,n)/(n+1)"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("�Ѷ����֦Ҷ��ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("������Ƕ����,����ȥ"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
		hItemDemo = InsertXzmTree( m_TreeDlgs, hItemDemo, TCItem, _T("��(��)����"));
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

UINT CMainAppDlg::SendSyslogThread(LPVOID pParam)
{
	CMainAppDlg* pThis = (CMainAppDlg *)pParam;
	if(NULL == pThis)
		return 0;

	//�����ϴ���־����
	CSysLog syslog;
	//����ͨ�Ż���
	syslog.CreateUDPCommunication();

	while(pThis->m_bSysLogFlag)
	{
		//�����ϴ�����
		OutputDebugString(_T("��ʼ�����ϴ�syslog��־"));
		syslog.GetLogDataFromDB();

		Sleep(2000);
	}

	//���������˳�ʱ,�˳���ǰ�߳�

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
		else if(IsYourChild( _T("�������ݿ��ļ�(��������)"),m_TreeXzm,hItemHit))
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
		else if(IsYourChild( _T("�������ݿ�,������ݿ���ھͲ����ؽ�"),m_TreeXzm,hItemHit))
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
		else if(IsYourChild( _T("Json Operator"),m_TreeXzm,hItemHit))
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
			else if(0 == str.Compare( _T("e_Test00")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}
				m_spiJsonService->TestAllJsonDesignFormat(e_Test00);
			}
			else if(0 == str.Compare( _T("e_Test01")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test01);
			}
			else if(0 == str.Compare( _T("e_Test02")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test02);
			}
			else if(0 == str.Compare( _T("e_Test03")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test03);
			}
			else if(0 == str.Compare( _T("e_Test04")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test04);
			}
			else if(0 == str.Compare( _T("e_Test05")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test05);
			}
			else if(0 == str.Compare( _T("e_Test06")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test06);
			}
			else if(0 == str.Compare( _T("e_Test07")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test07);
			}
			else if(0 == str.Compare( _T("e_Test08")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				m_spiJsonService->TestAllJsonDesignFormat(e_Test08);
			}
			// IJsonService��װJson����
			else if(0 == str.Compare( _T("IJsonService��װJson����")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				/// д������
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
			// ��������Json
			else if(0 == str.Compare( _T("TraversalJson")))
			{
				if(m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiSqlite3Connectδ��ʼ��"), _T(""), MB_OK);
					return 0;
				}

				CString strPath = CBaseFuncLib::GetAppConfigPath();
				strPath = strPath + _T("Traversal.json");
				m_spiJsonService->TraversalJson(CComBSTR(strPath));
			}
			else if(0 == str.Compare( _T("��Json�ļ�,�ļ��������򴴽�")))
			{
				if(m_spiParaService == NULL || m_spiJsonService == NULL) {
					::MessageBox( 0, _T("m_spiParaService | m_spiJsonServiceΪNULL"), _T("XzmSqlite3WtlApp"), MB_OK);
					return 0;
				}
				CString strPath = CBaseFuncLib::GetAppConfigPath();
				strPath = strPath + _T("JsonTest.json");

				VARIANT_BOOL bRet = FALSE;
				m_spiJsonService->ParseFile(CComBSTR(strPath),&bRet);
			}
			else if(0 == str.Compare( _T("����Json�ļ�,����ļ�������ɾ���ؽ�")))
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

				m_spiJsonService->TestCreateJsonToFile(CComBSTR(strModulePath));
			}
		}
		else if(IsYourChild( _T("��ӡLog����"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("WRITELOGTOFILE")))
			{
				LONG lFoldID = 1000;
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("SHGetSpecialFolderPath���� %d ʧ�ܣ������룺%ld"),lFoldID,::GetLastError());
				WRITELOGTOFILE(strLogInfo);
			}
			else if(0 == str.Compare( _T("WRITELOGTOFILE2")))
			{
				WRITELOGTOFILE2(_T("��Ȩ�������"),_T("Auth"),LOGTYPE_NORMAL);
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
		}
		else if(IsYourChild( _T("md5����,ʹ�ö�����"),m_TreeXzm,hItemHit))
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
		else if(IsYourChild( _T("���紫�����-ģ��ͻ���"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("����ͨ�Žӿ�")))
			{
				if(m_pNetworkInteraction == NULL)
				{
					m_pNetworkInteraction = new CNetworkInteraction;

					m_pNetworkInteraction->CreateConnect();
				}
			}
			else if(0 == str.Compare( _T("ע�ᵽƽ̨")))
			{
				if(m_pNetworkInteraction == NULL)
				{
					::MessageBox(NULL,_T("m_pParsejsonΪNULLδ����ͨ�Žӿ�"),_T(""),MB_OK);
					return 0;
				}
				m_pNetworkInteraction->PacketJsonHeartBeat();
				//����ע�����ݰ�


				//�ж�ƽ̨�Ƿ񷵻سɹ�


				//ע��ɹ�����Ҫ���Ĵ���

				//�����δ����,������syslog��־
				if (!m_bStartSyslog)
				{
					//δ����Syslog��־�ϴ�,ע����ɺ�����
					m_bStartSyslog = TRUE;
					//m_ctlSendSyslog.SetWindowTextW(_T("ֹͣ�ϴ�Syslog"));

					//�����ϴ��߳�
					DWORD dwSyslogID = 0;
					m_hSyslogThread = CREATE_MYTHREAD(SendSyslogThread,this,dwSyslogID);
					if(NULL != m_hSyslogThread)
					{
						m_bSysLogFlag = TRUE;
						::SetThreadPriority(m_hSyslogThread,THREAD_PRIORITY_BELOW_NORMAL);

						CloseHandle(m_hSyslogThread);
					}
				}

				//������ش���


				//���������û���������

				//����ȷ��ע��ɹ�����
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
		else if(IsYourChild( _T("���紫�����-�Զ���"),m_TreeXzm,hItemHit))
		{
			if(0 == str.Compare( _T("��ʼ��windows Socket")))
			{
				WSADATA Data;
				int status;

				//��ʼ��windows Socket Dll
				status = WSAStartup(MAKEWORD(1,1),&Data);
				if (0!=status)
				{
					OutputDebugString(_T("��ʼ��ʧ��\n"));
				} 
			}
			else if(0 == str.Compare( _T("��ʼ������")))
			{

				DWORD dwThreadId;
				HANDLE hThread;

				hThread=CreateThread(
					NULL,//default security attributes
					0,//use default stack size
					(LPTHREAD_START_ROUTINE)DoClientTest,//thread function
					NULL,//argument to thread function
					0,//use default creation flags
					&dwThreadId);//returns the thread identifier
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
			/// д������
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


	// �����׽���
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// Ҳ�������������bind������һ�����ص�ַ;����ϵͳ�����Զ�����

	// ��дԶ�̵�ַ��Ϣ
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(8088);

	// ע��,����Ҫ��д����������(TCPServer����)���ڻ�����IP��ַ
	// �����ļ����û������,ֱ��ʹ��127.0.0.1����
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		return 0;
	}

	
	char *pText = NULL;
	int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
	::send(xxx, pText, strlen(pText), 0);


	// ��������
	char buff[256];
	int nRecv = ::recv(xxx, buff, 256, 0);
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" ���յ����ݣ�%s", buff);
	}

	// �ر��׽���
	::closesocket(xxx);
	return 0;
}

