// SafeService.h : CSafeService ������

#pragma once
#include "resource.h"       // ������



#include "SKFEncry_i.h"
#include "_ISafeServiceEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
	/// ��ȫ��֧�ֿ�
	CComPtr <ISafeCard>		m_spiSafeCard;
	/// ˽�������ʽӿ�
	CComPtr <ICryptoStor>	m_spiCryptoStor;

	/// �����Ŀ��̷�
	CString		m_strDiskName;
	/// �û�PIN
	CString		m_strUserPin;
	/// �ͻ�����
	CString		m_strCustomerName;
	/// ȱʡ����Ա����
	CString		m_strDefAdminPW;
	/// ���ݿ��������
	CString		m_strDataPW;

	/// �ͻ�ID
	ULONG		m_nCustomerID;
	/// ��Ч����
	ULONG		m_nValidDay;
	/// ��Ȩ������
	ULONG		m_nAuthPacket;
	/// ��Ȩ�ڵ���
	USHORT		m_nAuthNode;

	/// ��װ�̱��
	VARIANT_BOOL m_bInstallDisk;
	//��װ���̷�
	TCHAR		m_cDiskNum;
	VARIANT_BOOL	m_bDiskNum;

	/// ��ʼ��״̬
	SHORT	m_nInitFlag;

	/// ����ַ����Ĺ�ϣֵ
	CString GetStringSha1(const CString& strString);
	CString GetStringMD5(const CString& strString);

	/// ��������
	void ReSetData();

#ifdef TDHXKJ_VERSION_SAFETOOL

	STDMETHOD(InitApp)(BSTR bstrCID);
	/// ������Ȩ�ļ�
	void CreateAuthFile(const CString& strAppName,const CString& strUserPin);
	/// ��ʼ��Ӧ��
	bool InitCardApp(const CString& strCID);

	void SaveUserInfo(LONG nCustomerID,const CString& strDiskID,LONG nAuthPacket,\
		LONG nValidDay,LONG nAuthNode,const CString& strDataPW,const CString& strDefAdminPW);

#endif
	/// �����Ȩ��Ϣ
	CComPtr <IJsonService> GetAuthInfo();

	/// ��ʼ����ȫ���
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
