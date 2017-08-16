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
	void InitDlgTreeSurplus(HTREEITEM  hItem,TV_INSERTSTRUCT& TCItem);// 添加多余内容
	HTREEITEM InsertXzmTree(CTreeViewCtrl& Tree,HTREEITEM hItem,TV_INSERTSTRUCT TCItem,TCHAR* pc_name);
	BOOL IsYourChild(ATL::CString strParentName, CTreeViewCtrl& Tree,HTREEITEM &hSelItem );

	static UINT SendSyslogThread(LPVOID pParam);
protected:
	CXzmTreeViewCtrl    m_TreeXzm;
	CXzmTreeViewCtrl    m_TreeDlgs;
	CButtonImpl/*CContainedWindow*/ m_wndOKBtn, m_wndExitBtn;
	CButtonImpl      m_wndAboutBtn;

	CString m_strDbName;    // 数据库的名字
	CString m_DbPwd;        // 数据库的密码
	CString m_strDbPath;    // 数据库的绝对路径
	CString m_DbFolderPath; // 数据库所在文件夹的路径
	CString m_DbModifyPwd;  // 数据库新密码

public:
	CComPtr <ISqlite3Connect> m_spiSqlite3Connect;
	CComPtr <IParaService>    m_spiParaService;
	CComPtr <IJsonService>    m_spiJsonService;
	CComPtr <ITestInterface>    m_spiTestInterface;
	CComPtr <IXMsXmlWrapper>    m_spiXMsXmlWrapper;
	CComPtr <IMySQLLogic>    m_spiMySQLLogic;
	/// 是否开启上传Syslog
	BOOL m_bStartSyslog;
	//创建上传Syslog日志线程
	volatile	HANDLE	m_hSyslogThread;
	/// 日志类型
	int	m_nLogType;
	/// 记录最近一个日志ID
	ULONG	m_nLastLogID;
	/// 最近操作时间
	COleDateTime	m_OptTime;
	/// 描述集合
	CSTRING_MAP		m_mapDesc;
	//上传线程退出标志
	BOOL m_bSysLogFlag;
	static UINT DoClientTest(LPVOID pParam);

	CNetworkInteraction *m_pNetworkInteraction;
};
