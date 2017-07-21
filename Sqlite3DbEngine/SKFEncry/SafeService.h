// SafeService.h : CSafeService 的声明

#pragma once
#include "resource.h"       // 主符号



#include "SKFEncry_i.h"
#include "_ISafeServiceEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CSafeService

class ATL_NO_VTABLE CSafeService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSafeService, &CLSID_SafeService>,
	public IConnectionPointContainerImpl<CSafeService>,
	public CProxy_ISafeServiceEvents<CSafeService>,
	public IDispatchImpl<ISafeService, &IID_ISafeService, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
	/// 安全卡支持库
	CComPtr <ISafeCard>		m_spiSafeCard;
	/// 私密区访问接口
	CComPtr <ICryptoStor>	m_spiCryptoStor;

	/// 操作的卡盘符
	CString		m_strDiskName;
	/// 用户PIN
	CString		m_strUserPin;
	/// 客户名称
	CString		m_strCustomerName;
	/// 缺省管理员密码
	CString		m_strDefAdminPW;
	/// 数据库访问密码
	CString		m_strDataPW;

	/// 客户ID
	ULONG		m_nCustomerID;
	/// 有效天数
	ULONG		m_nValidDay;
	/// 授权包掩码
	ULONG		m_nAuthPacket;
	/// 授权节点数
	USHORT		m_nAuthNode;

	/// 安装盘标记
	VARIANT_BOOL m_bInstallDisk;
	//安装盘盘符
	TCHAR		m_cDiskNum;
	VARIANT_BOOL	m_bDiskNum;

	/// 初始化状态
	SHORT	m_nInitFlag;

	/// 获得字符串的哈希值
	CString GetStringSha1(const CString& strString);
	CString GetStringMD5(const CString& strString);

	/// 重置数据
	void ReSetData();

#ifdef TDHXKJ_VERSION_SAFETOOL

	STDMETHOD(InitApp)(BSTR bstrCID);
	/// 创建授权文件
	void CreateAuthFile(const CString& strAppName,const CString& strUserPin);
	/// 初始化应用
	bool InitCardApp(const CString& strCID);

	void SaveUserInfo(LONG nCustomerID,const CString& strDiskID,LONG nAuthPacket,\
		LONG nValidDay,LONG nAuthNode,const CString& strDataPW,const CString& strDefAdminPW);

#endif
	/// 获得授权信息
	CComPtr <IJsonService> GetAuthInfo();

	/// 初始化安全组件
	HRESULT InitSafeCom(SHORT nFlag);

public:
	CSafeService()
		:m_spiSafeCard(NULL)
		,m_spiCryptoStor(NULL)
		,m_nInitFlag(0)
		,m_nValidDay(0)
		,m_nAuthPacket(0)
		,m_nAuthNode(0)
		,m_nCustomerID(0)
		,m_bInstallDisk(VARIANT_FALSE)
		,m_strDataPW(_T(""))
		,m_strUserPin(_T(""))
		,m_strDiskName(_T(""))
		,m_strDefAdminPW(_T(""))
		,m_strCustomerName(_T(""))
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SAFESERVICE)


BEGIN_COM_MAP(CSafeService)
	COM_INTERFACE_ENTRY(ISafeService)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CSafeService)
	CONNECTION_POINT_ENTRY(__uuidof(_ISafeServiceEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		CloseSafeCard();

		m_strDataPW.Empty();
		m_strDefAdminPW.Empty();
		m_strCustomerName.Empty();
		m_strDiskName.Empty();
		m_strUserPin.Empty();
		m_strDataPW.Empty();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(get_SafeCard)(ISafeCard** ppSafeCard);
	STDMETHOD(get_CryptoStor)(ICryptoStor** ppCryptoStor);
	STDMETHOD(get_CurDiskName)(BSTR* pVal);
	STDMETHOD(get_SoftEncry)(ISoftEncry** ppSoftEncry);

	STDMETHOD(put_CustomerName)(BSTR newVal);
	STDMETHOD(put_CustomerID)(ULONG newVal);
	STDMETHOD(put_DefaultPW)(BSTR newVal);
	STDMETHOD(put_AuthNode)(USHORT newVal);
	STDMETHOD(put_ValidDay)(ULONG newVal);
	STDMETHOD(put_AuthPacket)(ULONG newVal);
	STDMETHOD(put_DBPW)(BSTR newVal);
	STDMETHOD(get_CID)(VARIANT_BOOL bInstallFirst,BSTR* pbstrDiskName,BSTR* pVal);
	STDMETHOD(put_InstallDisk)(VARIANT_BOOL newVal);
	STDMETHOD(get_InstallDisk)(VARIANT_BOOL* pVal);
#ifdef TDHXKJ_VERSION_SAFETOOL
	STDMETHOD(GetCorpID)(BSTR bstrCorpName,SHORT nType,ULONG* pVal);
	STDMETHOD(AuthInit)(ULONG nCorpID);
	STDMETHOD(CheckInit)(BSTR bstrDisk,VARIANT_BOOL* pbInitFlag);
	STDMETHOD(Init)(BSTR bstrDisk,VARIANT_BOOL bReInit);
	STDMETHOD(CreateAuth)(BSTR bstrFilePath,BYTE* pbKey,VARIANT_BOOL bAddCount);
#endif
	STDMETHOD(FileSha1)(BSTR bstrFilePath,BSTR* pbstrSha1);
	STDMETHOD(StringSha1)(BSTR bstrString,BSTR* pbstrSha1);
	STDMETHOD(FileMD5)(BSTR bstrFilePath,unsigned char* pbMD5);
	STDMETHOD(StringMD5)(BSTR bstrString,BSTR* pbstrMD5);
	STDMETHOD(FileMD5String)(BSTR bstrFilePath,BSTR* pbstrMD5);
	STDMETHOD(TransToString)(BYTE* pbMD5,SHORT nLen,BSTR* pVal);
	STDMETHOD(GetInstallDisk)(VARIANT_BOOL bCID,BSTR* pVal);

	STDMETHOD(CloseSafeCard)();
	STDMETHOD(Auth)(BSTR bstrDeviceID);
	STDMETHOD(GetLeftCount)(ULONG* pVal);
	STDMETHOD(GetUniqueID)(BSTR* pVal);

	STDMETHOD(SetDiskNum)(BSTR bstrDiskNum,VARIANT_BOOL bFlag);
};

OBJECT_ENTRY_AUTO(__uuidof(SafeService), CSafeService)
