// SafeCard.h : CSafeCard ������

#pragma once
#include "resource.h"       // ������



#include "SKFEncry_i.h"



using namespace ATL;

template <class T>
class CProxy_ISafeCardEvents : public IConnectionPointImpl<T, &__uuidof( _ISafeCardEvents ), CComDynamicUnkArray>
{
	// ����: �������������������
public:
};



#include "SKFChinaCore.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSafeCard

class ATL_NO_VTABLE CSafeCard :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSafeCard, &CLSID_SafeCard>,
	public IConnectionPointContainerImpl<CSafeCard>,
	public CProxy_ISafeCardEvents<CSafeCard>,
	public IDispatchImpl<ISafeCard, &IID_ISafeCard, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
	/// ��оSKF֧�ַ���
	CSKFChinaCore* m_pSKF;

	/// ɾ���豸
	void DelDevice();

public:
	CSafeCard()
		:m_pSKF(NULL)
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SAFECARD)

	BEGIN_COM_MAP(CSafeCard)
		COM_INTERFACE_ENTRY(ISafeCard)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CSafeCard)
		CONNECTION_POINT_ENTRY(__uuidof(_ISafeCardEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		/// ��ʼ��
		m_pSKF = new CSKFChinaCore();
		if(NULL != m_pSKF)
			m_pSKF->InitSKF();
		return S_OK;
	}

	void FinalRelease()
	{
		DelDevice();
	}

public:

	/// ISafeCard
	STDMETHOD(EnumCard)(BSTR* pbstrDisks);
	STDMETHOD(GetCID)(BSTR cDisk,BSTR* pbstrCID);
	STDMETHOD(put_CurDisk)(BSTR cDisk);
	STDMETHOD(get_CurDisk)(BSTR* pcDisk);
	STDMETHOD(WaitForDevEvent)(SHORT *pnEvent,BSTR* pbstrName);
	STDMETHOD(CancelWaitForDevEvent)();
	STDMETHOD(GetStatus)(SHORT* psStatus);
	STDMETHOD(GetDeviceInfo)(IDispatch** ppDeviceInfo);
	STDMETHOD(CloseDev)(void);

	STDMETHOD(LockDev)(VARIANT_BOOL bLockFlag);
	STDMETHOD(ChangeDevAuth)(unsigned char* szOldPW,unsigned char* szNewPW);
	STDMETHOD(DevAuth)(unsigned char* szKey);
	STDMETHOD(ChangePIN)(ULONG hApp,VARIANT_BOOL bAdminFlag,BSTR bstrOldPin,BSTR bstrNewPin,SHORT* pnRetryCount);
	STDMETHOD(GetPINInfo)(ULONG hApp,VARIANT_BOOL bAdminFlag,SHORT* pnMaxRetryCount,SHORT* pnRemainRetryCount,VARIANT_BOOL* pbDefaultPin);
	STDMETHOD(VerifyPIN)(ULONG hApp,VARIANT_BOOL bAdminFlag,BSTR bstrPin,SHORT* pnRetryCount);
	STDMETHOD(UnblockPIN)(ULONG hApp,BSTR bstrAdminPin,BSTR bstrUserPin,SHORT* pnRetryCount);
	STDMETHOD(ClearSecureState)(ULONG hApp);

	STDMETHOD(CreateApplication)(BSTR bstrAppName,BSTR bstrAdminPin,\
		SHORT nAdminPinRetryCount,BSTR bstrUserPin,SHORT nUserPinRetryCount,EAappFileSecureType eCreateFileRight,ULONG* phApp);
	STDMETHOD(EnumApplication)(BSTR* pbstrApps);
	STDMETHOD(DeleteApplication)(BSTR bstrAppName);
	STDMETHOD(OpenApplication)(BSTR bstrAppName,ULONG* phApp);
	STDMETHOD(CloseApplication)(ULONG hApp);

	STDMETHOD(CreateAppFile)(ULONG hApp,BSTR bstrFileName,ULONG nFileSize,ULONG nReadRight,ULONG nWriteRight);
	STDMETHOD(DeleteAppFile)(ULONG hApp,BSTR bstrFileName);
	STDMETHOD(EnumAppFile)(ULONG hApp,BSTR* bstrFiles);
	STDMETHOD(GetAppFileInfo)(ULONG hApp,BSTR bstrFileName,IDispatch** pPara);
	STDMETHOD(ReadAppFile)(ULONG hApp,BSTR bstrFileName,ULONG nOffset,SHORT* pnReadLen,BYTE* pbData);
	STDMETHOD(WriteAppFile)(ULONG hApp,BSTR bstrFileName,ULONG nOffset,BYTE* pbData,ULONG nSize);

	STDMETHOD(CreateContainer)(ULONG hApp,BSTR bstrContainerName,ULONG* phContainer);
	STDMETHOD(DeleteContainer)(ULONG hApp,BSTR bstrContainerName);
	STDMETHOD(OpenContainer)(ULONG hApp,BSTR bstrContainerName,ULONG* phContainer);
	STDMETHOD(CloseContainer)(ULONG hContainer);
	STDMETHOD(EnumContainer)(ULONG hApp,BSTR* pbstrNameList);
	STDMETHOD(GetContainerType)(ULONG hContainer,SHORT* psType);

	STDMETHOD(ImportCertificate)(ULONG hContainer,VARIANT_BOOL bSign,BYTE* pbCert,SHORT nCertLen);
	STDMETHOD(ExportCertificate)(ULONG hContainer,VARIANT_BOOL bSign,SHORT* pnCertLen,BYTE* pbCert);

	STDMETHOD(GenRandom)(SHORT nRandomLen,BSTR *pbstrRandom);
	STDMETHOD(GenExtRSAKey)(ULONG nBitsLen,ULONG* pRsaPrivateKey);
	STDMETHOD(GenRSAKeyPair)(ULONG hContainer,ULONG nBitsLen,ULONG* pRsaPubKey);
	STDMETHOD(ImportRSAKeyPair)(ULONG hContainer,ULONG nSymAlgId,BYTE* pbWrappedKey,ULONG nWrappedKeyLen,BYTE* pbEncryptedData,ULONG nEncryptedDataLen);
	STDMETHOD(RSASignData)(ULONG hContainer,SHORT nDataLen,BYTE* pbData,SHORT* pnSignature,BYTE *pbSignature);
	STDMETHOD(RSAVerify)(ULONG* pRsaPubKey,SHORT nDataLen,BYTE* pbData,SHORT nSignatureLen,BYTE* pbSignature);
	STDMETHOD(RSAExportSessionKey)(ULONG hContainer,ULONG nAlgId,ULONG* pRsaPubKey,SHORT* pnDataLen,BYTE* pbData,ULONG *phSessionKey);
	STDMETHOD(ExtRSAPubKeyOperation)(ULONG* pRsaPubKey,SHORT nInputLen,BYTE* pbInputData,SHORT* pnOutput,BYTE *pbOutput);
	STDMETHOD(ExtRSAPriKeyOperation)(ULONG* pRsaPubKey,SHORT nInputLen,BYTE* pbInputData,SHORT* pnOutput,BYTE *pbOutput);

	STDMETHOD(ExportPublicKey)(ULONG hContainer,VARIANT_BOOL bSign,SHORT* pnBlobLen,ULONG* pbBlob);
	STDMETHOD(ImportSessionKey)(ULONG hContainer,ULONG nAlgId,ULONG nWrapedLen,BYTE* pbWrapedData,ULONG *phSessionKey);
	STDMETHOD(SetSymmKey)(ULONG nAlgId,BYTE* pbKey,ULONG *phSessionKey);
	STDMETHOD(EncryptInit)(ULONG hKey);
	STDMETHOD(Encrypt)(ULONG hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	STDMETHOD(EncryptUpdate)(ULONG hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	STDMETHOD(EncryptFinal)(ULONG hKey,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	STDMETHOD(DecryptInit)(ULONG hKey);
	STDMETHOD(Decrypt)(ULONG hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData);
	STDMETHOD(DecryptUpdate)(ULONG hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData);
	STDMETHOD(DecryptFinal)(ULONG hKey,ULONG* pnDecryptedDataLen,BYTE* pbDecryptedData);

	STDMETHOD(DigestInit)(ULONG ulAlgID,ULONG* pPubKey,SHORT nIDLen,unsigned char* pucID,ULONG* phHash);
	STDMETHOD(Digest)(ULONG hHash,SHORT ulDataLen,BYTE* pbData,SHORT* pnHashLen,BYTE* pbHashData);
	STDMETHOD(DigestUpdate)(ULONG hHash,SHORT ulDataLen,BYTE* pbData);
	STDMETHOD(DigestFinal)(ULONG hHash,SHORT* pnHashLen,BYTE* pbHashData);

	STDMETHOD(CloseHandle)(ULONG hHandle);

	STDMETHOD(get_SKFType)(SHORT* pVal);
	STDMETHOD(get_LastErrCode)(LONG* pVal);
	STDMETHOD(get_LastErrInfo)(BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(SafeCard), CSafeCard)
