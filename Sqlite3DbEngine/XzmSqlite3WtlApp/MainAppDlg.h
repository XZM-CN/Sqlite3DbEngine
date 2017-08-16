// MainAppDlg.h : interface of the CMainAppDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"
#include "XzmTreeViewCtrl.h"
#include "ButtonImpl.h"

#include <atlcomtime.h>

class CNetworkInteraction ;

class CMainAppDlg :
	// 
	public CDialogImpl<CMainAppDlg>,
	// 
	public CUpdateUI<CMainAppDlg>,
	// 
	public CMessageFilter,
	// 
	public CIdleHandler,
	// for DDX_CONTROL
	public CWinDataExchange<CMainAppDlg>
{
public:
	enum { IDD = IDD_MAINAPPDLG };

	CMainAppDlg();
	~CMainAppDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateChildWindows();
		return FALSE;
	}

	// DDX override
	void OnDataExchangeError(UINT nCtrlID, BOOL bSave);

	BEGIN_UPDATE_UI_MAP(CMainAppDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainAppDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDC_BTN_OPENDBPATH, OpenDbPath)
		COMMAND_ID_HANDLER(IDC_BTN_OPENDBFOLDERPATH, OpenDbFolderPath)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		NOTIFY_HANDLER_EX(IDC_TREEXZM, NM_CLICK, OnTreeXzmClickTree)
		ALT_MSG_MAP(1)
		MSG_WM_SETCURSOR(OnSetCursor_OK)
		ALT_MSG_MAP(2)
		MSG_WM_SETCURSOR(OnSetCursor_Exit)
		ALT_MSG_MAP(3)
		REFLECT_NOTIFICATIONS() // bianjikuang 
	END_MSG_MAP()

	BEGIN_DDX_MAP(CMainAppDlg)
		DDX_CONTROL(IDC_TREEXZM, m_TreeXzm)
		DDX_CONTROL(IDC_TREEDLGS, m_TreeDlgs)
		DDX_TEXT(IDC_EDIT_DBNAME, m_strDbName)
		DDX_TEXT(IDC_EDIT_DBPWD, m_DbPwd)
		DDX_TEXT(IDC_EDIT_DBPATH, m_strDbPath)
		DDX_TEXT(IDC_EDIT_DBFOLDERPATH, m_DbFolderPath)
		DDX_TEXT(IDC_EDIT_DBMODIFYPWD, m_DbModifyPwd)
	END_DDX_MAP()
	// Handler prototypes (uncomment arguments if needed):
	// LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	// LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	// LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OpenDbPath(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OpenDbFolderPath(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);


	LRESULT OnTreeXzmClickTree(NMHDR* phdr);
	LRESULT OnSetCursor_OK(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);
	LRESULT OnSetCursor_Exit(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);


	void InitXzmTree();
	void InitDlgTree();
	void InitDlgTreeSurplus(HTREEITEM  hItem,TV_INSERTSTRUCT& TCItem);// ��Ӷ�������
	HTREEITEM InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name);
	BOOL IsYourChild(ATL::CString strParentName, CTreeViewCtrl& Tree,HTREEITEM &hSelItem );

	static UINT SendSyslogThread(LPVOID pParam);
protected:
	CXzmTreeViewCtrl    m_TreeXzm;
	CXzmTreeViewCtrl    m_TreeDlgs;
	CButtonImpl/*CContainedWindow*/ m_wndOKBtn, m_wndExitBtn;
	CButtonImpl      m_wndAboutBtn;

	CString m_strDbName;    // ���ݿ������
	CString m_DbPwd;        // ���ݿ������
	CString m_strDbPath;    // ���ݿ�ľ���·��
	CString m_DbFolderPath; // ���ݿ������ļ��е�·��
	CString m_DbModifyPwd;  // ���ݿ�������

public:
	CComPtr <ISqlite3Connect> m_spiSqlite3Connect;
	CComPtr <IParaService>    m_spiParaService;
	CComPtr <IJsonService>    m_spiJsonService;
	CComPtr <ITestInterface>    m_spiTestInterface;
	CComPtr <IXMsXmlWrapper>    m_spiXMsXmlWrapper;
	CComPtr <IMySQLLogic>    m_spiMySQLLogic;
	/// �Ƿ����ϴ�Syslog
	BOOL m_bStartSyslog;
	//�����ϴ�Syslog��־�߳�
	volatile	HANDLE	m_hSyslogThread;
	/// ��־����
	int	m_nLogType;
	/// ��¼���һ����־ID
	ULONG	m_nLastLogID;
	/// �������ʱ��
	COleDateTime	m_OptTime;
	/// ��������
	CSTRING_MAP		m_mapDesc;
	//�ϴ��߳��˳���־
	BOOL m_bSysLogFlag;
	static UINT DoClientTest(LPVOID pParam);

	CNetworkInteraction *m_pNetworkInteraction;
};
