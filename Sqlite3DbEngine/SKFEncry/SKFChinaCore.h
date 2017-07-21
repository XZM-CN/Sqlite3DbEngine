#pragma once
#include "HXBaseDef.h"
#include "SKFBase.h"

class CSKFChinaCore :
	public CSKFBase
{
protected:

	/// ��¼����ָ��
	PSKF_FUNCLIST m_pFuncList;

	/// ����豸ID
	CString GetDID(HANDLE hDev);

	/// �ӽ��ܽṹ��
	BLOCKCIPHERPARAM* m_pBlockCipherParam;

public:
	CSKFChinaCore(void);

	virtual ~CSKFChinaCore(void);

	virtual bool InitSKF();

	virtual void UnInit();

	virtual bool CloseCurCard();

	virtual SHORT GetDeviceCount(CString& strCIDS);

	virtual HANDLE ConnectCard(WCHAR cDisk);

	virtual bool PutCurDisk(WCHAR cDisk);

	virtual CString GetDeviceID(WCHAR cDisk = 0);

	/// �����豸
	virtual bool LockTFDev(VARIANT_BOOL bLockFlag);

	/// ����豸״̬
	virtual SHORT GetStatus();

	/// �ر�������
	bool CloseHandle(HANDLE hHandle);
	/// �豸��֤
	bool DevAuth(unsigned char* szKey);
	/// �޸��豸��֤
	bool ChangeDevAuth(unsigned char* szOldPW,unsigned char* szNewPW);
	/// ����豸��Ϣ
	bool GetDeviceInfo(IDispatch** pPara);

	/// �ȴ��豸���
	bool WaitForDevEvent(SHORT *pnEvent,CString& strName);
	/// ȡ���豸����¼�֪ͨ
	bool CancelWaitForDevEvent();

	/// ����Ӧ��
	HANDLE CreateApp(const CString& strAppName,const CString& strAdminPin,SHORT nAdminPinRetryCount,\
		const CString& strUserPin,SHORT nUserPinRetryCount,ULONG nCreateFileRight);
	/// ö��Ӧ��
	bool EnumApp(u32& sCount,CString& strApps);
	/// ��Ӧ��
	HANDLE OpenApp(const CString& strAppName);
	/// ɾ��Ӧ��
	bool DeleteApp(const CString& strAppName);
	/// �ر�Ӧ��
	bool CloseApp(HANDLE hApp);
	/// ���Ӧ�ð�ȫ״̬
	bool ClearSecureState(HANDLE hApp);
	/// �޸�Ӧ��PIN��
	bool ChangePin(HANDLE hApp,VARIANT_BOOL bAdminFlag,\
		const CString& strOldPin,const CString& strNewPin,SHORT& pnRetryCount);
	/// ���PIN����Ϣ
	bool GetPinInfo(HANDLE hApp,VARIANT_BOOL bAdminFlag,\
		SHORT& nMaxRetryCount,SHORT& nRemainRetryCount,VARIANT_BOOL& bDefaultPin);
	/// У��PIN��
	bool VerifyPin(HANDLE hApp,VARIANT_BOOL bAdminFlag,const CString& strPin,SHORT& nRetryCount);
	/// �����û�PIN
	bool UnblockPin(HANDLE hApp,const CString& strAdminPin,const CString& strUserPin,SHORT& nRetryCount);

	/// �����ļ�
	bool CreateAppFile(HANDLE hApp,const CString& strFileName,ULONG nFileSize,ULONG nReadRight,ULONG nWriteRight);
	/// д�ļ�
	bool WriteAppFile(HANDLE hApp,const CString& strFileName,ULONG nOffset,BYTE* pbData,ULONG nSize);
	/// ��ȡ�ļ���Ϣ
	bool GetFileInfo(HANDLE hApp,const CString& strFileName,IDispatch** pPara);
	/// ��ȡ�ļ�����
	bool ReadAppFile(HANDLE hApp,const CString& strFileName,ULONG nOffset,u32& uBufLen,BYTE* pBuf);
	/// ö���ļ�
	bool EnumAppFile(HANDLE hApp,u32& uFileLen,CString& strFiles);
	/// ɾ���ļ�
	bool DeleteAppFile(HANDLE hApp,const CString& strFileName);

	/// Ӧ���ڴ�������
	HANDLE CreateContainer(HANDLE hApp,const CString& strContainerName);
	/// Ӧ����ɾ������
	bool DeleteContainer(HANDLE hApp,const CString& strContainerName);
	/// Ӧ���ڴ�����
	HANDLE OpenContainer(HANDLE hApp,const CString& strContainerName);
	/// ö������
	bool EnumContainer(HANDLE hApp,u32& nLength,CString& strNames);
	/// �����������
	bool GetContainerType(HANDLE hContainer,short& sType);
	/// �ر�����
	bool CloseContainer(HANDLE hContainer);

	/// ����֤��
	bool ImportCertificate(HANDLE hContainer,VARIANT_BOOL bSign,BYTE* pbCert,u32 nCertLen);
	/// ����֤��
	bool ExportCertificate(HANDLE hContainer,VARIANT_BOOL bSign,BYTE* pbCert,u32& nCertLen);
	/// ��������
	bool GenRandom(CString& strRandom,ULONG nRandomLen);
	/// ����RSA�ܳ�
	bool GenExtRSAKey(ULONG nBitsLen,ULONG* pRsaPrivateKey);
	/// ����RSAǩ����
	bool GenRSAKeyPair(HANDLE hContainer,ULONG nBitsLen,ULONG* pRsaPubKey);
	/// ����RSA���ܹ�˽Կ��
	bool ImportRSAKeyPair(HANDLE hContainer,ULONG nSymAlgId,BYTE* pbWrappedKey,\
		ULONG nWrappedKeyLen,BYTE* pbEncryptedData,ULONG nEncryptedDataLen);
	/// ǩ��
	bool RSASignData(HANDLE hContainer,ULONG nDataLen,\
		BYTE* pbData,ULONG* pnSignature,BYTE *pbSignature);
	/// ��֤ǩ��
	bool RSAVerify(ULONG* pRsaPubKey,ULONG nDataLen,BYTE* pbData,\
		ULONG nSignatureLen,BYTE* pbSignature);
	/// RSA���ɲ������Ự��Կ
	bool RSAExportSessionKey(HANDLE hContainer,ULONG nAlgId,\
		ULONG* pRsaPubKey,ULONG* pnDataLen,BYTE* pbData,HANDLE *phSessionKey);
	/// ������Կ����
	bool ExtRSAPubKeyOperation(ULONG* pRsaPubKey,ULONG nInputLen,BYTE* pbInputData,\
		ULONG* pnOutput,BYTE* pbOutput);
	/// ����˽Կ����
	bool ExtRSAPriKeyOperation(ULONG* pRsaPriKey,ULONG nInputLen,BYTE* pbInputData,\
		ULONG* pnOutput,BYTE* pbOutput);

	/// �����Ự��Կ
	bool ExportPublicKey(HANDLE hContainer,VARIANT_BOOL bSign,ULONG* pnBlobLen,ULONG* pbBlob);
	/// ����Ự��Կ����
	bool ImportSessionKey(HANDLE hContainer,ULONG nAlgId,ULONG nWrapedLen,BYTE* pbWrapedData,HANDLE* phSessionKey);

	/// �������ĶԳ���Կ��������Կ���
	bool SetSymmKey(ULONG nAlgId,BYTE* pbKey,HANDLE* phSessionKey);
	/// �ӽ��ܳ�ʼ��
	bool EncryptOrDecryptInit(HANDLE hKey,bool bEncryFlag);
	/// �������
	bool Encrypt(HANDLE hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	/// �������
	bool EncryptUpdate(HANDLE hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	/// ���ܽ���
	bool EncryptOrDecryptFinal(HANDLE hKey,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData,BOOL bEncryptFlag);
	/// �������
	bool Decrypt(HANDLE hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData);
	/// �������
	bool DecryptUpdate(HANDLE hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData);
	/// �����ʼ��
	bool DigestInit(ULONG ulAlgID,ULONG* pPubKey,SHORT nIDLen,unsigned char* pucID,HANDLE* phHash);
	/// �������������Ӵ�
	bool Digest(HANDLE hHash,ULONG ulDataLen,BYTE* pbData,ULONG* pnHashLen,BYTE* pbHashData);
	/// �������������Ӵ�
	bool DigestUpdate(HANDLE hHash,ULONG ulDataLen,BYTE* pbData);
	/// ���������Ӵ�
	bool DigestFinal(HANDLE hHash,ULONG* pnHashLen,BYTE* pbHashData);
};