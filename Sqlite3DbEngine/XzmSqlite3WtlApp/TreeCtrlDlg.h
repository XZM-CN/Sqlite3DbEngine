// TreeCtrlDlg.h : interface of the CTreeCtrlDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"
#include "BuffyTreeCtrl.h"
#include "ODButtonImpl.h"
#include "MyCheckListCtrl.h"
#include "TreeViewCtrlExImpl.h"
#include "XzmTreeViewCtrl.h"

static const int SEASON_MIN = 1, SEASON_MAX = 7;

class CTreeCtrlDlg :
	// ԭʼ�Ի��򶼻���
	public CDialogImpl<CTreeCtrlDlg>,
	// 
	public CUpdateUI<CTreeCtrlDlg>,
	// 
	public CMessageFilter,
	// 
	public CIdleHandler,
	// 
	public CWinDataExchange<CTreeCtrlDlg>,
	// 
	public CCustomDraw<CTreeCtrlDlg>,
	// 
	public CDialogResize<CTreeCtrlDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	CTreeCtrlDlg() : m_nSeason(3)
	{
	}


	// ��Ϣӳ��
	//////////////////////////////////////////////////////////////////////////
	BEGIN_MSG_MAP(CTreeCtrlDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER_EX(IDOK, OnOK)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER_EX(IDC_ALYSON_BTN, OnAlysonODBtn)
		// All Tree 
		//////////////////////////////////////////////////////////////////////////
		//NOTIFY_CODE_HANDLER(TVN_SELCHANGED, OnAllTreeSelChanged)
		// Tree 
		//////////////////////////////////////////////////////////////////////////
		NOTIFY_HANDLER_EX(IDC_TREE, TVN_SELCHANGED, OnTreeSelChanged)
		// CTreeViewCtrlEx:
		//////////////////////////////////////////////////////////////////////////
		NOTIFY_HANDLER_EX(IDC_RELATIONSHIP_TREE, TVN_SELCHANGED, OnTreeRelationshipSelChanged)
		// Tree xzm:
		//////////////////////////////////////////////////////////////////////////
		NOTIFY_HANDLER_EX(IDC_TREEXZM, TVN_SELCHANGED, OnTreeXzmSelChanged)
		NOTIFY_HANDLER_EX(IDC_TREEXZM, NM_CLICK, OnTreeXzmClickTree)
		// List 
		//////////////////////////////////////////////////////////////////////////
		NOTIFY_HANDLER_EX(IDC_CHECK_LIST, LVN_ITEMCHANGED, OnListItemSelChanged)
		//ON_NOTIFY(NM_CLICK, IDC_TREE, OnNMClickTree)
		CHAIN_MSG_MAP(CCustomDraw<CTreeCtrlDlg>)
		CHAIN_MSG_MAP(CDialogResize<CTreeCtrlDlg>)
	END_MSG_MAP()


	// ע��ֻ��ʹ�÷�ģʽ�Ի���WTL��ģʽ�Ի���֧��UpdateUI����
	// http://blog.csdn.net/feimashenhua/article/details/5174150
	//////////////////////////////////////////////////////////////////////////
	BEGIN_UPDATE_UI_MAP(CTreeCtrlDlg)
		UPDATE_ELEMENT(IDC_ALYSON_BMPBTN, UPDUI_CHILDWINDOW)
	END_UPDATE_UI_MAP()


	// DDX��Dialog Data Exchange
	// DDV��Dialog Data Validation 
	// http://blog.csdn.net/laogaoav/article/details/20764231
	//////////////////////////////////////////////////////////////////////////
	BEGIN_DDX_MAP(CTreeCtrlDlg)
		// Always Show Selection:True ָ������ѡ�����ݣ���ʹ�ؼ�û�н���
		// Disable Drag Drop :True ��ֹ����ͼ�ؼ�����TVN_BEGINDRAG֪ͨ��Ϣ
		// Has Buttons :True �ڸ����Ա���ʾ+��-����
		// Has Lines :True ʹ��������ʾ��Ĳ�νṹ
		// Lines At Root : True ʹ������������״��ͼ�ؼ���������
		// Group :True
		// Tabstop:True
		DDX_CONTROL(IDC_TREE, m_wndTree)
		// Always Show Selection:True ָ������ѡ�����ݣ���ʹ�ؼ�û�н���
		// Disable Drag Drop :True ��ֹ����ͼ�ؼ�����TVN_BEGINDRAG֪ͨ��Ϣ
		// Has Buttons :True �ڸ����Ա���ʾ+��-����
		// Has Lines :True ʹ��������ʾ��Ĳ�νṹ
		// Lines At Root : True ʹ������������״��ͼ�ؼ���������
		// Group :True
		// Tabstop:True
		DDX_CONTROL(IDC_RELATIONSHIP_TREE, m_wndRelationshipTree)
		DDX_CONTROL(IDC_TREEXZM, m_wndXzmTree)
		DDX_CONTROL(IDC_ALYSON_BTN, m_wndODBtn)
		DDX_CONTROL(IDC_CP_LINK, m_wndLink)
		// Always Show Selection :True
		// Single Selection :True
		// View :Report
		DDX_CONTROL(IDC_CHECK_LIST, m_wndChkList)
		DDX_INT_RANGE(IDC_FAV_SEASON, m_nSeason, SEASON_MIN, SEASON_MAX)
	END_DDX_MAP()

	
	// �Զ������Ի��������ؿؼ�
	//////////////////////////////////////////////////////////////////////////
	BEGIN_DLGRESIZE_MAP(CTreeCtrlDlg)
		DLGRESIZE_CONTROL(IDOK, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDCANCEL, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_TREE, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_CP_LINK, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_CHECK_LIST, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_SEASON_LABEL, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_FAV_SEASON, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_RELATIONSHIP_TREE, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_TREEXZM, DLSZ_SIZE_X|DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()


	// Inherit WTL::CMessageFilter
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Inherit WTL::CIdleHandler::OnIdle(void)
	virtual BOOL OnIdle();
	// ��д������ override��CCustomDraw::OnPrePaint ( int idCtrl, NMCUSTOMDRAW* pnmcd )
	DWORD OnPrePaint ( int idCtrl, NMCUSTOMDRAW* pnmcd );
	// ��д������ override��CCustomDraw::OnItemPrePaint ( int idCtrl, NMCUSTOMDRAW* pnmcd )
	DWORD OnItemPrePaint ( int idCtrl, NMCUSTOMDRAW* pnmcd );
	// DDX overrides
	void OnDataValidateError(UINT nCtrlID, BOOL bSave, _XData& data);

	// ���ܻ��õ���ģ���ṩ��
	//////////////////////////////////////////////////////////////////////////
	// Handler prototypes (uncomment arguments if needed):
	// LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	// LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	// LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/);
	void OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/);
	void CloseDialog(int nVal);
	void OnAlysonODBtn(UINT uCode, int nID, HWND hwndCtrl);
	void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnAllTreeSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnTreeSelChanged(NMHDR* phdr);
	LRESULT OnTreeRelationshipSelChanged(NMHDR* phdr);
	LRESULT OnTreeXzmSelChanged(NMHDR* phdr);
	LRESULT OnTreeXzmClickTree(NMHDR* phdr);
	LRESULT OnListItemSelChanged(NMHDR* phdr);

	// ctrl 
	void InitODBtn();
	void InitBmpBtn();
	void InitLink();
	void InitChkList();
	void InitTree();
	void InitRelationshipTree();
	void InitXzmTree();
	HTREEITEM InsertXzmTree(HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name);
	BOOL IsYourChild(ATL::CString strParentName, CXzmTreeViewCtrl &Tree,HTREEITEM &hSelItem );
protected:
	// DDX vars
	CBuffyTreeCtrl      m_wndTree;
	CODButtonImpl       m_wndODBtn;
	CHyperLink          m_wndLink;
	CMyCheckListCtrl    m_wndChkList;
	int                 m_nSeason;
	CTreeViewCtrlExImpl m_wndRelationshipTree;
	CXzmTreeViewCtrl    m_wndXzmTree;

	// other controls
	CBitmapButton  m_wndBmpBtn;
};
