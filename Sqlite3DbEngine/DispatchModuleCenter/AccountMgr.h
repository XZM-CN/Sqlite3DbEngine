// AccountMgr.h : CAccountMgr ������

#pragma once
#include "resource.h"       // ������



#include "DispatchModuleCenter_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CAccountMgr

class ATL_NO_VTABLE CAccountMgr :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAccountMgr, &CLSID_AccountMgr>,
	public IDispatchImpl<IAccountMgr, &IID_IAccountMgr, &LIBID_DispatchModuleCenterLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// ��ǰ��¼�˺�
	CString			m_strCurLID;
	/// ��ǰ��¼�˺��û���
	CString			m_strCurName;
	/// ��ǰ��¼�˺���ʾ����
	CString			m_strShowName;
	/// ���ݿ��������
	CString			m_strDataPW;
	/// ��ǰ�˺�Ȩ��
	ULONG			m_nRight;
	/// ��Ȩģ��
	ULONG			m_nAuthPacket;
	/// �˺�����
	EAccountType	m_eAccountType;
	/// ֻ������
	VARIANT_BOOL	m_bReadOnly;

	/// ����ģʽ
	ERunMode		m_eRunMode;

	/// ��¼���Դ���
	SHORT			m_nTryLoginCount;

	/// ��ǰ��Ȩ��˾ID
	ULONG			m_nCorpID;
	/// ��Ȩ��Ч��
	VARIANT_BOOL	m_bAuthValid;

	/// ��������
	CString			m_strLastErr;

	/// ������
	CString CreateTable(CComPtr <ISafeService>& spiSafeService);

	/// ��ù���Ա�����ݿ�����
	CString GetSafePW(CComPtr <ISafeService>& spiSafeService,CString& strDBPW,EAccountType eAccountType = ACCOUNTTYPE_UNKNOWN);

	/// �����Ȩ��Ϣ
	HRESULT GetFileAuthInfo(BSTR bstrFilePath,BYTE* pbKey,IDispatch **ppAuthInfo,BOOL bGetPW = FALSE);

	/// ������ݿ�����
	CComPtr <ISqlite3Connect> GetConnect(const CString& strPW,BOOL bReadFlag = FALSE);

	/// ��ʼ���˺�
	BOOL InitAccount(CComPtr <ISafeService>& spiSafeService,CComPtr <ISqlite3Connect>& spiSqlite3Connect,\
		ULONG nAccount,const CString& strSaCCode,const CString& strAdminCCode,const CString& strAuditCCode);
	/// ������Ȩ�ļ�����
	void SaveAuthToCard(CComBSTR bstrDiskName,const CString& strCID,const CString& strAuthInfo,CComPtr <ISafeService>& spiSafeService);

	/// �����Ȩ����Ч��
	void CheckAuthValid(CComPtr <ISafeService>& spiSafeService,\
		CComPtr <IJsonService>& spiJsonService,const CString& strAuthCode);

#ifndef TDHXKJ_VERSION_NOUSB
	/// ��ȡ��װ����ԱCODE
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

	// ��ʼ���˺�����
	STDMETHOD(Init)(ULONG nAccount,BSTR bstrPW,VARIANT_BOOL bCheckFile);
	// ���õ�ǰ��¼�˺�
	STDMETHOD(SetCurrent)(EAccountType eUserType,BSTR bstrID,BSTR bstrPWHash,EUseAuthType eAuthType,BSTR bstrCurName,SHORT* pVal);

	// 
	STDMETHOD(get_AuthValid)(VARIANT_BOOL* pVal);
	// 
	STDMETHOD(get_AccountType)(EAccountType* pVal);
	// 
	STDMETHOD(get_CurLID)(BSTR* pVal);
	// 
	STDMETHOD(get_Right)(ULONG* pVal);
	// ����˺�
	STDMETHOD(Add)(EAccountType eType, BSTR bstrID, \
		BSTR bstrName,BSTR bstrPWHash, ULONG nRight);
	// ɾ���˺�
	STDMETHOD(Del)(BSTR bstrID);
	// �޸�����
	STDMETHOD(ChangePW)(EAccountType eUserType, BSTR bstrID,BSTR bstrNewPWHash,BSTR bstrAccountName);
	// �޸�Ȩ��
	STDMETHOD(ChangeRight)(BSTR bstrID, ULONG nRight);
	// 
	STDMETHOD(Login)(IDispatch** ppInfo);
	// �޸�����
	STDMETHOD(ChangeName)(BSTR bstrName);
	// 
	STDMETHOD(ServiceToken)(void);
	// ������ݿ�ӿ�
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
	// �������ݵ�����
	STDMETHOD(SaveToCard)(BSTR ucDiskName);

	// ��������
	STDMETHOD(SaveConfig)(EUseAuthType nAuthType,BSTR bstrNodeName,BSTR bstrNodeValue);
	// ��Ӱ�����U��
	STDMETHOD(AddUsbWhite)(BSTR bstrUsbID,BSTR bstrDiskName,BSTR bstrMakerName,ULONG nSize,SHORT nType);
	// ɾ��������U��
	STDMETHOD(DelUsbWhite)(BSTR bstrUsbID,SHORT nType);
	// ˢ����Ȩ
	STDMETHOD(RefreshAuth)(VARIANT_BOOL* pVal);
	// ��ȡ������
	STDMETHOD(GetAuthCode)(ULONG* pVal);

	// ��ӽ��÷���
	STDMETHOD(AddStopService)(BSTR bstrServiceName,SHORT nStart,BSTR bstrImagePath);
	// ɾ�����÷���
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
	 * @brief ������һ�β����ĳ�����Ϣ
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(AccountMgr), CAccountMgr)
