// AccountMgr.h : CAccountMgr 的声明

#pragma once
#include "resource.h"       // 主符号



#include "DispatchModuleCenter_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CAccountMgr

class ATL_NO_VTABLE CAccountMgr :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAccountMgr, &CLSID_AccountMgr>,
	public IDispatchImpl<IAccountMgr, &IID_IAccountMgr, &LIBID_DispatchModuleCenterLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// 当前登录账号
	CString			m_strCurLID;
	/// 当前登录账号用户名
	CString			m_strCurName;
	/// 当前登录账号显示名称
	CString			m_strShowName;
	/// 数据库访问密码
	CString			m_strDataPW;
	/// 当前账号权限
	ULONG			m_nRight;
	/// 授权模块
	ULONG			m_nAuthPacket;
	/// 账号类型
	EAccountType	m_eAccountType;
	/// 只读类型
	VARIANT_BOOL	m_bReadOnly;

	/// 运行模式
	ERunMode		m_eRunMode;

	/// 登录尝试次数
	SHORT			m_nTryLoginCount;

	/// 当前授权公司ID
	ULONG			m_nCorpID;
	/// 授权有效性
	VARIANT_BOOL	m_bAuthValid;

	/// 错误描述
	CString			m_strLastErr;

	/// 创建表
	CString CreateTable(CComPtr <ISafeService>& spiSafeService);

	/// 获得管理员及数据库密码
	CString GetSafePW(CComPtr <ISafeService>& spiSafeService,CString& strDBPW,EAccountType eAccountType = ACCOUNTTYPE_UNKNOWN);

	/// 获得授权信息
	HRESULT GetFileAuthInfo(BSTR bstrFilePath,BYTE* pbKey,IDispatch **ppAuthInfo,BOOL bGetPW = FALSE);

	/// 获得数据库连接
	CComPtr <ISqlite3Connect> GetConnect(const CString& strPW,BOOL bReadFlag = FALSE);

	/// 初始化账号
	BOOL InitAccount(CComPtr <ISafeService>& spiSafeService,CComPtr <ISqlite3Connect>& spiSqlite3Connect,\
		ULONG nAccount,const CString& strSaCCode,const CString& strAdminCCode,const CString& strAuditCCode);
	/// 保存授权文件到卡
	void SaveAuthToCard(CComBSTR bstrDiskName,const CString& strCID,const CString& strAuthInfo,CComPtr <ISafeService>& spiSafeService);

	/// 检查授权码有效性
	void CheckAuthValid(CComPtr <ISafeService>& spiSafeService,\
		CComPtr <IJsonService>& spiJsonService,const CString& strAuthCode);

#ifndef TDHXKJ_VERSION_NOUSB
	/// 获取安装管理员CODE
	BOOL GetInstallCode(CComPtr <ISafeService>& spiSafeService,CString& strSACode);
#endif

public:
	CAccountMgr()
		:m_nRight(0)
		,m_nTryLoginCount(0)
		,m_nCorpID(0)
		,m_nAuthPacket(HXPACKETTYPE_BASE|HXPACKETTYPE_LOGAUDIT|HXPACKETTYPE_FILEWHITE)
		,m_eAccountType(ACCOUNTTYPE_UNKNOWN)
		,m_eRunMode(RUNMODE_PERSONAL)
		,m_bReadOnly(VARIANT_FALSE)
#ifdef _DEBUG
		,m_bAuthValid(VARIANT_TRUE)
#else
		,m_bAuthValid(VARIANT_FALSE)
#endif
		,m_strCurLID(_T(""))
		,m_strCurName(_T(""))
		,m_strShowName(_T(""))
		,m_strDataPW(_T(""))
		,m_strLastErr(_T(""))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ACCOUNTMGR)


BEGIN_COM_MAP(CAccountMgr)
	COM_INTERFACE_ENTRY(IAccountMgr)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



DECLARE_PROTECT_FINAL_CONSTRUCT()

HRESULT FinalConstruct()
{
	return S_OK;
}

void FinalRelease()
{
	m_strCurLID.Empty();
	m_strCurName.Empty();
	m_strShowName.Empty();
	m_strDataPW.Empty();
	m_strLastErr.Empty();
}

public:
	// 
	STDMETHOD(GetAuthInfo)(BSTR bstrFilePath,BSTR bstrDiskName,BYTE* pbKey,IDispatch **ppAuthInfo);
	// 
	STDMETHOD(ImportAuth)(BSTR bstrFilePath,BYTE* pbKey,IDispatch **ppAuthInfo);
	// 
	STDMETHOD(RemoveAuth)(BSTR bstrDeviceID);
	// 
	STDMETHOD(CreateAuth)(BSTR bstrDeviceID,BSTR bstrFilePath);

	// 初始化账号密码
	STDMETHOD(Init)(ULONG nAccount,BSTR bstrPW,VARIANT_BOOL bCheckFile);
	// 设置当前登录账号
	STDMETHOD(SetCurrent)(EAccountType eUserType,BSTR bstrID,BSTR bstrPWHash,EUseAuthType eAuthType,BSTR bstrCurName,SHORT* pVal);

	// 
	STDMETHOD(get_AuthValid)(VARIANT_BOOL* pVal);
	// 
	STDMETHOD(get_AccountType)(EAccountType* pVal);
	// 
	STDMETHOD(get_CurLID)(BSTR* pVal);
	// 
	STDMETHOD(get_Right)(ULONG* pVal);
	// 添加账号
	STDMETHOD(Add)(EAccountType eType, BSTR bstrID, \
		BSTR bstrName,BSTR bstrPWHash, ULONG nRight);
	// 删除账号
	STDMETHOD(Del)(BSTR bstrID);
	// 修改密码
	STDMETHOD(ChangePW)(EAccountType eUserType, BSTR bstrID,BSTR bstrNewPWHash,BSTR bstrAccountName);
	// 修改权限
	STDMETHOD(ChangeRight)(BSTR bstrID, ULONG nRight);
	// 
	STDMETHOD(Login)(IDispatch** ppInfo);
	// 修改名字
	STDMETHOD(ChangeName)(BSTR bstrName);
	// 
	STDMETHOD(ServiceToken)(void);
	// 获得数据库接口
	STDMETHOD(GetDataBase)(BSTR bstrToken,IDispatch** ppVal);
	// 
	STDMETHOD(get_RunMode)(ERunMode* pVal);
	// 
	STDMETHOD(put_RunMode)(ERunMode newVal);
	// 
	STDMETHOD(get_Count)(SHORT* pVal);
	// 
	STDMETHOD(get_ShowName)(BSTR bstrLID, BSTR* pVal);
	// 
	STDMETHOD(get_CanLogin)(BSTR bstrLID, VARIANT_BOOL* pVal);
	// 
	STDMETHOD(get_ReadOnly)(VARIANT_BOOL* pVal);

	// 
	STDMETHOD(ClearStatus)(void);
	// 
	STDMETHOD(UnLock)(BSTR bstrID);
	// 保存数据到卡上
	STDMETHOD(SaveToCard)(BSTR ucDiskName);

	// 保存配置
	STDMETHOD(SaveConfig)(EUseAuthType nAuthType,BSTR bstrNodeName,BSTR bstrNodeValue);
	// 添加白名单U盘
	STDMETHOD(AddUsbWhite)(BSTR bstrUsbID,BSTR bstrDiskName,BSTR bstrMakerName,ULONG nSize,SHORT nType);
	// 删除白名单U盘
	STDMETHOD(DelUsbWhite)(BSTR bstrUsbID,SHORT nType);
	// 刷新授权
	STDMETHOD(RefreshAuth)(VARIANT_BOOL* pVal);
	// 获取功能码
	STDMETHOD(GetAuthCode)(ULONG* pVal);

	// 添加禁用服务
	STDMETHOD(AddStopService)(BSTR bstrServiceName,SHORT nStart,BSTR bstrImagePath);
	// 删除禁用服务
	STDMETHOD(DelStopService)(BSTR bstrServiceName,BSTR bstrImagePath);

	//
	STDMETHOD(CheckAccountExist)(BSTR strAccountName);
	// 
	STDMETHOD(GetRealAccountInfo)(BSTR LID,BSTR* strAccountTypeName, BSTR* strRealPasswd,ULONG* strRight);
	// 
	STDMETHOD(SaveAccountInfo)(BSTR strAccountName, BSTR strPassword,BSTR LID,BSTR strAccountTypeName,BSTR strRealPasswd,ULONG strRight);
	// 
	STDMETHOD(DelelteAccountInfo)(BSTR strAccountName,BSTR LID);
	// 
	STDMETHOD(CheckIsPassswdRight)(BSTR LID,BSTR strAccountName,BSTR bstrMD5,BSTR* strRealPasswd);

	// 
	STDMETHOD(get_CurName)(BSTR* pVal);

	/**
	 * @brief 获得最近一次操作的出错信息
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(AccountMgr), CAccountMgr)
