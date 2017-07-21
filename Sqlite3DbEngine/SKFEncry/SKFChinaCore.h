#pragma once
#include "HXBaseDef.h"
#include "SKFBase.h"

class CSKFChinaCore :
	public CSKFBase
{
protected:

	/// 记录函数指针
	PSKF_FUNCLIST m_pFuncList;

	/// 获得设备ID
	CString GetDID(HANDLE hDev);

	/// 加解密结构体
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

	/// 锁定设备
	virtual bool LockTFDev(VARIANT_BOOL bLockFlag);

	/// 获得设备状态
	virtual SHORT GetStatus();

	/// 关闭密码句柄
	bool CloseHandle(HANDLE hHandle);
	/// 设备认证
	bool DevAuth(unsigned char* szKey);
	/// 修改设备认证
	bool ChangeDevAuth(unsigned char* szOldPW,unsigned char* szNewPW);
	/// 获得设备信息
	bool GetDeviceInfo(IDispatch** pPara);

	/// 等待设备插拔
	bool WaitForDevEvent(SHORT *pnEvent,CString& strName);
	/// 取消设备插拔事件通知
	bool CancelWaitForDevEvent();

	/// 创建应用
	HANDLE CreateApp(const CString& strAppName,const CString& strAdminPin,SHORT nAdminPinRetryCount,\
		const CString& strUserPin,SHORT nUserPinRetryCount,ULONG nCreateFileRight);
	/// 枚举应用
	bool EnumApp(u32& sCount,CString& strApps);
	/// 打开应用
	HANDLE OpenApp(const CString& strAppName);
	/// 删除应用
	bool DeleteApp(const CString& strAppName);
	/// 关闭应用
	bool CloseApp(HANDLE hApp);
	/// 清除应用安全状态
	bool ClearSecureState(HANDLE hApp);
	/// 修改应用PIN码
	bool ChangePin(HANDLE hApp,VARIANT_BOOL bAdminFlag,\
		const CString& strOldPin,const CString& strNewPin,SHORT& pnRetryCount);
	/// 获得PIN码信息
	bool GetPinInfo(HANDLE hApp,VARIANT_BOOL bAdminFlag,\
		SHORT& nMaxRetryCount,SHORT& nRemainRetryCount,VARIANT_BOOL& bDefaultPin);
	/// 校验PIN码
	bool VerifyPin(HANDLE hApp,VARIANT_BOOL bAdminFlag,const CString& strPin,SHORT& nRetryCount);
	/// 解锁用户PIN
	bool UnblockPin(HANDLE hApp,const CString& strAdminPin,const CString& strUserPin,SHORT& nRetryCount);

	/// 创建文件
	bool CreateAppFile(HANDLE hApp,const CString& strFileName,ULONG nFileSize,ULONG nReadRight,ULONG nWriteRight);
	/// 写文件
	bool WriteAppFile(HANDLE hApp,const CString& strFileName,ULONG nOffset,BYTE* pbData,ULONG nSize);
	/// 获取文件信息
	bool GetFileInfo(HANDLE hApp,const CString& strFileName,IDispatch** pPara);
	/// 读取文件内容
	bool ReadAppFile(HANDLE hApp,const CString& strFileName,ULONG nOffset,u32& uBufLen,BYTE* pBuf);
	/// 枚举文件
	bool EnumAppFile(HANDLE hApp,u32& uFileLen,CString& strFiles);
	/// 删除文件
	bool DeleteAppFile(HANDLE hApp,const CString& strFileName);

	/// 应用内创建容器
	HANDLE CreateContainer(HANDLE hApp,const CString& strContainerName);
	/// 应用内删除容器
	bool DeleteContainer(HANDLE hApp,const CString& strContainerName);
	/// 应用内打开容器
	HANDLE OpenContainer(HANDLE hApp,const CString& strContainerName);
	/// 枚举容器
	bool EnumContainer(HANDLE hApp,u32& nLength,CString& strNames);
	/// 获得容器类型
	bool GetContainerType(HANDLE hContainer,short& sType);
	/// 关闭容器
	bool CloseContainer(HANDLE hContainer);

	/// 导入证书
	bool ImportCertificate(HANDLE hContainer,VARIANT_BOOL bSign,BYTE* pbCert,u32 nCertLen);
	/// 导出证书
	bool ExportCertificate(HANDLE hContainer,VARIANT_BOOL bSign,BYTE* pbCert,u32& nCertLen);
	/// 获得随机数
	bool GenRandom(CString& strRandom,ULONG nRandomLen);
	/// 生成RSA密匙
	bool GenExtRSAKey(ULONG nBitsLen,ULONG* pRsaPrivateKey);
	/// 生成RSA签名对
	bool GenRSAKeyPair(HANDLE hContainer,ULONG nBitsLen,ULONG* pRsaPubKey);
	/// 导入RSA加密公私钥对
	bool ImportRSAKeyPair(HANDLE hContainer,ULONG nSymAlgId,BYTE* pbWrappedKey,\
		ULONG nWrappedKeyLen,BYTE* pbEncryptedData,ULONG nEncryptedDataLen);
	/// 签名
	bool RSASignData(HANDLE hContainer,ULONG nDataLen,\
		BYTE* pbData,ULONG* pnSignature,BYTE *pbSignature);
	/// 验证签名
	bool RSAVerify(ULONG* pRsaPubKey,ULONG nDataLen,BYTE* pbData,\
		ULONG nSignatureLen,BYTE* pbSignature);
	/// RSA生成并导出会话密钥
	bool RSAExportSessionKey(HANDLE hContainer,ULONG nAlgId,\
		ULONG* pRsaPubKey,ULONG* pnDataLen,BYTE* pbData,HANDLE *phSessionKey);
	/// 外来公钥运算
	bool ExtRSAPubKeyOperation(ULONG* pRsaPubKey,ULONG nInputLen,BYTE* pbInputData,\
		ULONG* pnOutput,BYTE* pbOutput);
	/// 外来私钥运算
	bool ExtRSAPriKeyOperation(ULONG* pRsaPriKey,ULONG nInputLen,BYTE* pbInputData,\
		ULONG* pnOutput,BYTE* pbOutput);

	/// 导出会话公钥
	bool ExportPublicKey(HANDLE hContainer,VARIANT_BOOL bSign,ULONG* pnBlobLen,ULONG* pbBlob);
	/// 导入会话秘钥密文
	bool ImportSessionKey(HANDLE hContainer,ULONG nAlgId,ULONG nWrapedLen,BYTE* pbWrapedData,HANDLE* phSessionKey);

	/// 设置明文对称密钥，返回密钥句柄
	bool SetSymmKey(ULONG nAlgId,BYTE* pbKey,HANDLE* phSessionKey);
	/// 加解密初始化
	bool EncryptOrDecryptInit(HANDLE hKey,bool bEncryFlag);
	/// 单组加密
	bool Encrypt(HANDLE hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	/// 多组加密
	bool EncryptUpdate(HANDLE hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData);
	/// 加密结束
	bool EncryptOrDecryptFinal(HANDLE hKey,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData,BOOL bEncryptFlag);
	/// 单组解密
	bool Decrypt(HANDLE hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData);
	/// 多组解密
	bool DecryptUpdate(HANDLE hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData);
	/// 杂项初始化
	bool DigestInit(ULONG ulAlgID,ULONG* pPubKey,SHORT nIDLen,unsigned char* pucID,HANDLE* phHash);
	/// 单组数据密码杂凑
	bool Digest(HANDLE hHash,ULONG ulDataLen,BYTE* pbData,ULONG* pnHashLen,BYTE* pbHashData);
	/// 多组数据密码杂凑
	bool DigestUpdate(HANDLE hHash,ULONG ulDataLen,BYTE* pbData);
	/// 结束密码杂凑
	bool DigestFinal(HANDLE hHash,ULONG* pnHashLen,BYTE* pbHashData);
};