// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"
#include "XzmTreeViewCtrl.h"
#include "DevListBoxImpl.h"

class CStaticClass;

class CMainDlg :
	public CDialogImpl<CMainDlg>,
	public CUpdateUI<CMainDlg>,
	public CMessageFilter,
	public CIdleHandler,
	// for DDX_CONTROL
	public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}

public:
	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		NOTIFY_HANDLER_EX(IDC_TESTTREE, NM_CLICK, OnTreeXzmClickTree)
		COMMAND_HANDLER(IDC_NODE01, BN_CLICKED, OnBnClickedAppLog)
		COMMAND_HANDLER(IDC_NODE05, BN_CLICKED, OnBnClickedAppLog)
		COMMAND_HANDLER(IDC_NODE10, BN_CLICKED, OnBnClickedAppLog)
		COMMAND_HANDLER(IDC_NODE15, BN_CLICKED, OnBnClickedAppLog)
		COMMAND_HANDLER(IDC_NODE20, BN_CLICKED, OnBnClickedAppLog)
		COMMAND_HANDLER(IDC_BTN_START, BN_CLICKED, OnBtnStart)
		COMMAND_HANDLER(IDC_BTN_PAUSE, BN_CLICKED, OnBtnPause)
		COMMAND_HANDLER(IDC_BTN_STOP, BN_CLICKED, OnBtnStop)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

public:
	BEGIN_DDX_MAP(CMainDlg)
		DDX_CONTROL(IDC_TESTTREE, m_TreeXzm)
		DDX_TEXT(IDC_NODECOUNT, m_strNodeCount)
		DDX_CONTROL_HANDLE(IDC_NODECOUNT, m_NodeCountCtrl)
		DDX_CONTROL_HANDLE(IDC_LOGLIST, m_ListCtrl)
	END_DDX_MAP()

public:
	//	Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedAppLog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBtnStart(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBtnPause(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBtnStop(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	void CloseDialog(int nVal);
	void EnableRadio(BOOL b);
	static UINT CreateSocketThread(LPVOID pParam);
	static UINT CreateSocketThreadA(LPVOID pParam);
public:
	LRESULT OnTreeXzmClickTree(NMHDR* phdr);
	void InitXzmTree();
	void InitDlgTreeSurplus(HTREEITEM  hItem,TV_INSERTSTRUCT& TCItem);// ÃÌº”∂‡”‡ƒ⁄»›
	HTREEITEM InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name);
	BOOL IsYourChild(ATL::CString strParentName, CTreeViewCtrl& Tree,HTREEITEM &hSelItem );
public:
	/// …®√Ë◊¥Ã¨
	CString m_strNodeCount;
	CStatic             m_NodeCountCtrl;
	CListViewCtrl           m_ListCtrl;
	CXzmTreeViewCtrl    m_TreeXzm;
	CDevListBoxImpl m_ListBox;
	CStaticClass    *m_pStaticClass;
};