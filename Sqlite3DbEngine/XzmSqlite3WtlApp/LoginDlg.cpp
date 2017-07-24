// LoginDlg.cpp : implementation of the CLoginDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BaseFuncLib.h"
#include "SqliteHelper.h"
#include "BaseHelper.h"
#include "SKFHelper.h"
#include "LoginDlg.h"

extern CComPtr <IAccountMgr>	g_spiAccountMgr;

SHORT	g_nHostStatus	= 0;
SHORT	g_nDriverStatus = 0;

BOOL CheckServiceStatus()
{
	/// 服务控制接口
	BOOL bRunFlag = FALSE;
	CComPtr <IServiceControl>	spiServiceControl = CBaseHelper::GetServiceControl();
	if(NULL == spiServiceControl)
		return bRunFlag;
	VARIANT_BOOL bExitFlag = VARIANT_FALSE;
	spiServiceControl->PutServiceName(VARIANT_FALSE,CComBSTR(TDHXKJ_HOSTSERVICE_NAME));
	spiServiceControl->IsExist(&bExitFlag);
	if(VARIANT_FALSE == bExitFlag)
		return bRunFlag;/// 不存在
	spiServiceControl->GetServiceStatus(&g_nHostStatus);
	spiServiceControl->PutServiceName(VARIANT_TRUE,CComBSTR(TDHXKJ_SAFEDRIVER_NAME));
	spiServiceControl->IsExist(&bExitFlag);
	if(VARIANT_FALSE == bExitFlag)
		return bRunFlag;/// 不存在
	spiServiceControl->GetServiceStatus(&g_nDriverStatus);
	if((SERVICE_RUNNING == g_nHostStatus || SERVICE_START_PENDING == g_nHostStatus) 
		&& (SERVICE_RUNNING == g_nDriverStatus || SERVICE_START_PENDING == g_nDriverStatus))
		return TRUE;///同时启动才认为开始了安全防护
	return bRunFlag;
}

CComPtr <ISqlite3Connect> CheckDataConnect(HWND hWnd)
{
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	if(NULL == g_spiAccountMgr)
		g_spiAccountMgr = CBaseHelper::GetAccountMgr();
	if(NULL == g_spiAccountMgr)
		return spiSqlite3Connect;
	/// 正常获取只读数据库来操作，如果登录后获取正常数据库来使用
	HRESULT hRet = g_spiAccountMgr->GetDataBase(NULL,(IDispatch** )&spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
	{
		CComBSTR bstrVal;
		g_spiAccountMgr->get_LastErrorInfo(&bstrVal);
		if(bstrVal.Length())
			::MessageBox(hWnd,bstrVal.m_str,TDHXKJ_APPLICATION_NAME,MB_OK|MB_ICONQUESTION);
		else
			::MessageBox(hWnd,_T("未知错误导致访问数据库失败！"),TDHXKJ_APPLICATION_NAME,MB_OK|MB_ICONQUESTION);
		bstrVal.Empty();
	}
	/// 返回数据库连接
	return spiSqlite3Connect;
}

int LoginDlg(HWND hWnd,EHXAccountType eAccountType,DWORD dwAuthCode)
{
	int nRet = IDCANCEL;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = CheckDataConnect(hWnd);
	if(NULL == spiSqlite3Connect)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("获得数据库连接失败！"));
#endif
		return nRet;
	}
	CLoginDlg LoginDlg(eAccountType,dwAuthCode,spiSqlite3Connect);
	nRet = (int)LoginDlg.DoModal();
	spiSqlite3Connect = NULL;

	return nRet;
}

void CLoginDlg::InitAccountList()
{
	if(NULL == m_spiSqlite3Connect)
		return;
	int nCurSel = -1;
	CComBSTR bstrVal;
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT LID,SName,UType,URight FROM [%s] ORDER BY UType ASC"),SQLITEDB_TABLE_ACCOUNT);
	HRESULT hRet = m_spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(m_spiSqlite3Connect->NextRow()))
	{
		LONG nuthCode = 0;
		m_spiSqlite3Connect->GetValueInt(3,&nuthCode);
		if((m_dwAuthCode & nuthCode) != m_dwAuthCode)
			continue;/// 只显示可操作账号
		m_spiSqlite3Connect->GetValueString(0,&bstrVal);
		m_vectLoginID.push_back(bstrVal.m_str);
		bstrVal.Empty();
		m_spiSqlite3Connect->GetValueString(1,&bstrVal);
		int nCount = m_wndAccountType.AddString(bstrVal.m_str);
		bstrVal.Empty();
		LONG nType = 0;
		m_spiSqlite3Connect->GetValueInt(2,&nType);
		m_wndAccountType.SetItemData(nCount,nType);
		if(-1 == nCurSel && m_eAccountType == nType)
			nCurSel = nCount;
	}

	//导入用户名
	LONG nTempType = 0;
	strSqlCmd.Empty();
	strSqlCmd.Format(_T("SELECT LID,STempName,UType,URight FROM [%s] ORDER BY UType ASC"),SQLITEDB_TABLE_TEMP_ACCOUNT);
	hRet = m_spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(m_spiSqlite3Connect->NextRow()))
	{
		LONG nuthCode = 0;
		m_spiSqlite3Connect->GetValueInt(3,&nuthCode);
		if((m_dwAuthCode & nuthCode) != m_dwAuthCode)
			continue;/// 只显示可操作账号
		m_spiSqlite3Connect->GetValueString(0,&bstrVal);
		//m_vectLoginID.push_back(bstrVal.m_str);
		bstrVal.Empty();
		m_spiSqlite3Connect->GetValueString(1,&bstrVal);
		int nCount = m_wndAccountName.AddString(bstrVal.m_str);
		OutputDebugString(bstrVal);
		bstrVal.Empty();
		nTempType++;
		m_wndAccountName.SetItemData(nCount,nTempType);
		
	}

	m_spiSqlite3Connect->Close();
	m_spiSqlite3Connect = NULL;
	if(m_wndAccountType.GetCount())
	{
		if(-1 == nCurSel)
			nCurSel = 0;
		m_wndAccountType.SetCurSel(nCurSel);
	}
	if (m_wndAccountName.GetCount())
	{
		m_wndAccountName.SetCurSel(0);
	}
}

LRESULT CLoginDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();
	if(NULL == g_spiAccountMgr)
		g_spiAccountMgr = CBaseHelper::GetAccountMgr();
	if(NULL == g_spiAccountMgr)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("创建账号管理对象失败！"));
#endif
		return FALSE;
	}
	m_wndAccountType.Attach(GetDlgItem(IDC_COMBO_ACCOUNTTYPE).m_hWnd);
	m_wndAccountName.Attach(GetDlgItem(IDC_COMBO_ACCOUNTNAME).m_hWnd);
	/// 读取数据库账号
	InitAccountList();
	if(!DoDataExchange(false))
		return FALSE;
	return TRUE;
}

LRESULT CLoginDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	this->GetDlgItem(IDC_STATIC_ERROR).SetWindowText(_T(""));
	if(!DoDataExchange(true))
		return FALSE;
	if(m_strLoginPW.GetLength() < 5)
	{
		this->MessageBox(_T("密码输入不合法！"),TDHXKJ_APPLICATION_NAME,MB_OK);
		return 0;
	}

	/// 校验登录
	int nCurSel = m_wndAccountType.GetCurSel();
	if(nCurSel < 0)
		return 0;
	DWORD dwUserType = (DWORD )m_wndAccountType.GetItemData(nCurSel);
	CComBSTR bstrMD5,bstrSha1;
	CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
	if(NULL != spiSafeService)
	{
		spiSafeService->StringMD5(CComBSTR(m_strLoginPW),&bstrMD5);
		OutputDebugString(_T("m_strLoginPW   "));
		OutputDebugString(m_strLoginPW);
		OutputDebugString(_T("\n"));
		OutputDebugString(_T("bstrMD5   "));
		OutputDebugString(bstrMD5);
		OutputDebugString(_T("\n"));
		spiSafeService->StringSha1(bstrMD5,&bstrSha1);
		OutputDebugString(_T("bstrMD5   "));
		OutputDebugString(bstrMD5);
		OutputDebugString(_T("\n"));
		OutputDebugString(_T("bstrSha1   "));
		OutputDebugString(bstrSha1);
		OutputDebugString(_T("\n"));
	}
	spiSafeService = NULL;

	//判断输入密码是否正确
	CComBSTR strRealPasswd;
	int nCurNameSel = m_wndAccountName.GetCurSel();
	CString strAccountName;
	CComBSTR nstrVal;
	int nTempLen = m_wndAccountName.GetLBTextLen(nCurNameSel);
	m_wndAccountName.GetLBText(nCurNameSel,strAccountName.GetBuffer(nTempLen));
	//strAccountName = nstrVal.m_str;
	///
	OutputDebugString(strAccountName);
	OutputDebugString(_T("\n"));

	HRESULT hRet = g_spiAccountMgr->CheckIsPassswdRight(CComBSTR(m_vectLoginID[nCurSel]),(CComBSTR)strAccountName,(CComBSTR)bstrSha1,&strRealPasswd);
	if(FAILED(hRet))
	{
		/*CComBSTR bstrVal;
		g_spiAccountMgr->get_LastErrorInfo(&bstrVal);
		
		this->GetDlgItem(IDC_STATIC_ERROR).SetWindowText(bstrVal.m_str);
		return 0;*/
		strRealPasswd = bstrMD5;

	}


	SHORT nTryCount = 0;
	hRet = g_spiAccountMgr->SetCurrent((EHXAccountType )dwUserType,CComBSTR(m_vectLoginID[nCurSel]),(CComBSTR)strRealPasswd,\
		(EHXUseAuthType )m_dwAuthCode,(CComBSTR)strAccountName,&nTryCount);
	bstrMD5.Empty();
	if(FAILED(hRet))
	{
		CComBSTR bstrVal;
		g_spiAccountMgr->get_LastErrorInfo(&bstrVal);
		if(!nTryCount)
		{
			EndDialog(IDCANCEL);
			this->MessageBox(bstrVal.m_str,TDHXKJ_APPLICATION_NAME,MB_OK);
		}
		else
			this->GetDlgItem(IDC_STATIC_ERROR).SetWindowText(bstrVal.m_str);
		return 0;
	}
	/// 校验使用权限
	ULONG uAuthCode = 0;
	g_spiAccountMgr->get_Right(&uAuthCode);
	if(m_dwAuthCode != (uAuthCode & m_dwAuthCode))
	{
		g_spiAccountMgr->ClearStatus();
		this->GetDlgItem(IDC_STATIC_ERROR).SetWindowText(_T("您当前登录的账号没有操作此功能的权限！\r\n请重新选择登录账号或联系超级管理员赋予权限，谢谢！"));
		return 0;
	}
	int nCount = m_wndAccountType.GetCount();
	for(int nIndex = 0;nIndex < nCount;nIndex++)
		m_wndAccountType.DeleteString(0);

	nCount = m_wndAccountName.GetCount();
	for(int nIndex = 0;nIndex < nCount;nIndex++)
		m_wndAccountName.DeleteString(nIndex);

	EndDialog(wID);
	return 0;
}

LRESULT CLoginDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nCount = m_wndAccountType.GetCount();
	for(int nIndex = 0;nIndex < nCount;nIndex++)
		m_wndAccountType.DeleteString(0);
	EndDialog(wID);
	return 0;
}
