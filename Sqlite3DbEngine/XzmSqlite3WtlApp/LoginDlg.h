// LoginDlg.h : interface of the CLoginDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlddx.h>
#include "resource.h"

class CLoginDlg : public CDialogImpl<CLoginDlg>,
	public CWinDataExchange<CLoginDlg>
{
protected:
	/// �˺�����
	CComboBox 	m_wndAccountType;
	/// �˺���
	CComboBox 	m_wndAccountName;
	/// ��¼����
	CString		m_strLoginPW;
	/// ��¼��֤��Ҫ��Ȩ��
	DWORD		m_dwAuthCode;

	/// ��¼�˺���Ϣ
	CSTRING_VECTOR	m_vectLoginID;
	/// ��¼�˺�����
	EHXAccountType m_eAccountType;
	/// ���ݿ�����
	CComPtr <ISqlite3Connect> m_spiSqlite3Connect;
	ISqlite3Connect
	/// ��ʼ���ɵ�¼�˺��б�
	void InitAccountList();

public:
	CLoginDlg(EHXAccountType eAccountType,DWORD dwAuthCode,CComPtr <ISqlite3Connect>& spiSqlite3Connect)
		:CDialogImpl()
		,m_eAccountType(eAccountType)
		,m_dwAuthCode(dwAuthCode)
		,m_spiSqlite3Connect(spiSqlite3Connect)
	{
	}

	enum { IDD = IDD_DIALOG_LOGIN };

	BEGIN_DDX_MAP(CLoginDlg)  
		DDX_CONTROL_HANDLE(IDC_COMBO_ACCOUNTTYPE, m_wndAccountType)  
		DDX_CONTROL_HANDLE(IDC_COMBO_ACCOUNTNAME, m_wndAccountName)
		DDX_TEXT(IDC_EDIT_PASSWORD, m_strLoginPW)
	END_DDX_MAP()  

	BEGIN_MSG_MAP(CLoginDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
