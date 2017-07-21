// SafeService.cpp : CSafeService 的实现

#include "stdafx.h"
#include "SafeService.h"
#include "BaseFuncLib.h"
#include "skf_type.h"
#include "SqliteHelper.h"

#include <iostream>  
#include "openssl/sha.h"
#include "openssl/md5.h"
#include "openssl/aes.h"

#define TDHXKJ_READFILE_MAXBUFF		1024*1024

#define	TDHXKJ_SKFAPP_CONNAME		_T("SafeCon")
#ifdef TDHXKJ_VERSION_SAFETOOL
#define	TDHXKJ_DEFAULT_DBPW			_T("HXSafe2016")
#endif

static const unsigned char g_szCCDefaultKey[] = "1234567812345678";/// 国芯卡默认设备认证秘钥

//////////////////////////////    OPENSSL FOR SHA1    /////////////////////////////////////////////////

#ifndef WIN64
#pragma comment(lib, "..\\bin\\Release\\libeay32.lib")  
#pragma comment(lib, "..\\bin\\Release\\ssleay32.lib")
#else
#pragma comment(lib, "..\\bin\\Release\\X64\\libeay32.lib")  
#pragma comment(lib, "..\\bin\\Release\\X64\\ssleay32.lib")
#endif

static const char hex_chars[] = "0123456789ABCDEF";

CString convert_hex(unsigned char *md,int nLen)  
{  
	CString strSha1(_T(""));
	int i = 0;
	unsigned int c = 0;

	for (i = 0; i < nLen; i++)
	{
		c = (md[i] >> 4) & 0x0f;
		strSha1 += hex_chars[c];
		strSha1 += hex_chars[md[i] & 0x0f];
	}
	return strSha1;
} 

CComPtr <ISoftEncry> GetSoftEncry()
{
	CComPtr<IClassFactory>	cf = NULL;
	CComPtr <ISoftEncry> spiSoftEncry = NULL;
	HRESULT hRet = DllGetClassObject( __uuidof(SoftEncry), __uuidof(IClassFactory), (LPVOID *)&cf );
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(ISoftEncry), (VOID **)&spiSoftEncry );
		cf = NULL;
		ATLASSERT(spiSoftEncry);
	}
	return spiSoftEncry;
}

///////////////////////////////////////////////////////////////////////////////

// CSafeService

HRESULT CSafeService::InitSafeCom(SHORT nFlag)
{
	HRESULT hRet = E_FAIL;
	CComPtr<IClassFactory>	cf = NULL;
	if(1 == (1 & nFlag) && NULL == m_spiSafeCard)
	{
		hRet = DllGetClassObject( __uuidof(SafeCard), __uuidof(IClassFactory), (LPVOID *)&cf );
		if(cf != NULL)
		{
			hRet = cf->CreateInstance( NULL, __uuidof(ISafeCard), (VOID **)&m_spiSafeCard );
			cf = NULL;
			ATLASSERT(m_spiSafeCard);
		}
	}
	if(2 == (2 & nFlag) && NULL == m_spiCryptoStor)
	{
		hRet = DllGetClassObject( __uuidof(CryptoStor), __uuidof(IClassFactory), (LPVOID *)&cf );
		if(cf != NULL)
		{
			hRet = cf->CreateInstance( NULL, __uuidof(ICryptoStor), (VOID **)&m_spiCryptoStor );
			cf = NULL;
			ATLASSERT(m_spiCryptoStor);
		}
	}
	return hRet;
}

STDMETHODIMP CSafeService::get_SoftEncry(ISoftEncry** ppSoftEncry)
{
	// TODO: 在此添加实现代码
	CComPtr <ISoftEncry> spiISoftEncry = GetSoftEncry();
	if(NULL == spiISoftEncry)
		return E_FAIL;
	HRESULT hRet = spiISoftEncry->QueryInterface(__uuidof (ISoftEncry),(void **)ppSoftEncry);
	spiISoftEncry = NULL;
	return hRet;
}

STDMETHODIMP CSafeService::get_SafeCard(ISafeCard** ppSafeCard)
{
	// TODO: 在此添加实现代码
	if(NULL == ppSafeCard)
		return E_POINTER;
	if(NULL == m_spiSafeCard)
		InitSafeCom(1);
	ATLASSERT(m_spiSafeCard);
	return m_spiSafeCard->QueryInterface(__uuidof (ISafeCard),(void **)ppSafeCard);
}

STDMETHODIMP CSafeService::get_CryptoStor(ICryptoStor** ppCryptoStor)
{
	// TODO: 在此添加实现代码
	if(NULL == ppCryptoStor)
		return E_POINTER;
	if(NULL == m_spiCryptoStor)
		InitSafeCom(2);
	ATLASSERT(m_spiCryptoStor);
	return m_spiCryptoStor->QueryInterface(__uuidof (ICryptoStor),(void **)ppCryptoStor);
}

STDMETHODIMP CSafeService::get_CurDiskName(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
		return E_POINTER;
	if(NULL == m_spiCryptoStor)
		InitSafeCom(2);
	if(m_strDiskName.IsEmpty())
	{
		m_spiCryptoStor->GetCurDisk(pVal);
	}
	else
	{
		CComBSTR bstrDisk(m_strDiskName);
		bstrDisk.CopyTo(pVal);
		bstrDisk.Empty();
	}
	return S_OK;
}

STDMETHODIMP CSafeService::get_CID(VARIANT_BOOL bInstallFirst,BSTR* pbstrDiskName,BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal || NULL == pbstrDiskName)
		return E_POINTER;
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		InitSafeCom(3);
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		return E_POINTER;
	CString strDisk;
	if(NULL != (*pbstrDiskName))
		strDisk = (*pbstrDiskName);
	HRESULT hRet(E_FAIL);
	CComBSTR bstrVal;
	BOOL bCopyFlag = FALSE;
	if(strDisk.IsEmpty())
	{
		hRet = m_spiSafeCard->EnumCard(&bstrVal);
		if(FAILED(hRet) || !bstrVal.Length())
		{
			if(FAILED(hRet))
			{
#ifdef APP_LOG_ENABLE
				/// 写日志
				CComBSTR bstrErrInfo;
				m_spiSafeCard->get_LastErrInfo(&bstrErrInfo);
				WRITELOGTOFILE(bstrErrInfo.m_str);
				bstrErrInfo.Empty();
#endif
			}
			return hRet;
		}
		bCopyFlag = TRUE;
		strDisk = bstrVal.m_str;
		bstrVal.Empty();
	}
	CString strInstallDisk;
	BOOL bInstallDisk = FALSE;
	CComBSTR bstrCID,bstrInstall,bstrFindDisk;
	int nDiskLen = strDisk.GetLength();

	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	regKey.GetRegStringVal(PRODUCT_COMMAN_INSDISK,strInstallDisk);
	for(int nIndex = 0;nIndex < nDiskLen;nIndex++)
	{
		CComBSTR bstrDisk;
		bstrDisk.Append(strDisk.GetAt(nIndex));
		if(!strDisk.IsEmpty())
		{
			/// 判断释放为安全U卡，直接用SKF接口，可能导致异常
			DWORD dwSize = 0;
			hRet = m_spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);
			if(FAILED(hRet))
				continue;
		}
		hRet = m_spiSafeCard->put_CurDisk(bstrDisk);
		if(FAILED(hRet))
		{
			LONG nErrCode = 0;
			m_spiSafeCard->get_LastErrCode(&nErrCode);
			if(strDisk.IsEmpty() && nErrCode && ERROR_FILE_NOT_FOUND != nErrCode)
			{
#ifdef APP_LOG_ENABLE
				/// 写日志
				m_spiSafeCard->get_LastErrInfo(&bstrVal);
				CString strLog(_T(""));
				strLog.Format(_T("CID设置%s盘失败，原因:%s"),\
					(CString)bstrDisk.m_str,(CString)bstrVal.m_str);
				WRITELOGTOFILE(strLog);
#endif
			}
			bstrDisk.Empty();
			bstrVal.Empty();
			nIndex++;
			continue;
		}
		hRet = m_spiSafeCard->GetCID(bstrDisk,&bstrVal);
		if(SUCCEEDED(hRet))
		{
			if(0 == strInstallDisk.CompareNoCase(bstrVal.m_str))
			{
				if(VARIANT_TRUE == bInstallFirst)
				{
					/// 找到安装盘
					bstrVal.CopyTo(pVal);
					bstrVal.Empty();
					bstrFindDisk = bstrDisk;
					bInstallDisk = TRUE;
					break;
				}
				bstrInstall = bstrVal;
				bstrFindDisk = bstrDisk;
			}
			else
			{
				if(!bstrCID.Length())
				{
					bstrFindDisk.Empty();
					bstrCID = bstrVal;
					bstrFindDisk = bstrDisk;
				}
			}
		}
		bstrDisk.Empty();
		bstrVal.Empty();
		nIndex++;
	}
	if(!bInstallDisk)
	{
		if(VARIANT_TRUE == bInstallFirst)
		{
			if(bstrInstall.Length())
			{
				if(bCopyFlag)
					bstrFindDisk.CopyTo(pbstrDiskName);
				bstrInstall.CopyTo(pVal);
			}
			else
			{
				if(bstrCID.Length())
				{
					if(bCopyFlag)
						bstrFindDisk.CopyTo(pbstrDiskName);
					bstrCID.CopyTo(pVal);
				}
			}
		}
		else
		{
			if(bstrCID.Length())
			{
				if(bCopyFlag)
					bstrFindDisk.CopyTo(pbstrDiskName);
				bstrCID.CopyTo(pVal);
			}
			else
			{
				if(bstrInstall.Length())
				{
					if(bCopyFlag)
						bstrFindDisk.CopyTo(pbstrDiskName);
					bstrInstall.CopyTo(pVal);
				}
			}
		}
	}
	else
	{
		if(bCopyFlag)
			bstrFindDisk.CopyTo(pbstrDiskName);
	}
	bstrCID.Empty();
	bstrInstall.Empty();
	bstrFindDisk.Empty();
	return hRet;
}

#ifdef TDHXKJ_VERSION_SAFETOOL

STDMETHODIMP CSafeService::InitApp(BSTR bstrCID)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrCID)
		return E_FAIL;
	bool bInitFlag = InitCardApp(bstrCID);
	if(!bInitFlag)
		return E_FAIL;
	return S_OK;
}

bool CSafeService::InitCardApp(const CString& strCID)
{
	bool bInitFlag = false;
	m_strUserPin.Empty();
	if(NULL == m_spiSafeCard)
		InitSafeCom(1);
	if(NULL == m_spiSafeCard)
		return bInitFlag;
	HRESULT hRet(E_FAIL);
	CString strSha1 = GetStringSha1(strCID+_T("_HXSafe"));
	CComBSTR bstrAppName(TDHXKJ_SKFAPP_NAME),bstrAdminPin(strSha1.Left(3)),bstrUserPin;
	bstrAdminPin.Append(strSha1.Right(5));
	strSha1.Delete(0,16);
	bstrUserPin.Append(strSha1.Left(8));
	m_strUserPin = bstrUserPin.m_str;

	/// 初始化应用，创建授权文件存储
	ULONG nAppHandle = 0;
	SHORT nMaxTry = 0X0F;
	hRet = m_spiSafeCard->CreateApplication(bstrAppName,bstrAdminPin,nMaxTry,\
		bstrUserPin,nMaxTry,SKFAPP_FILESECURETYPE_ANYONE,&nAppHandle);
	if(FAILED(hRet))
	{
		/// 获取错误信息
		LONG nErrCode = 0;
		CComBSTR bstrErrInfo;
		m_spiSafeCard->get_LastErrCode(&nErrCode);
		m_spiSafeCard->get_LastErrInfo(&bstrErrInfo);
		if(SAR_APPLICATION_EXISTS != nErrCode)
		{
			/// 卡有问题，需要重新格式化
			return bInitFlag;
		}
		hRet = m_spiSafeCard->OpenApplication(bstrAppName,&nAppHandle);
	}
	if(NULL == nAppHandle)
		return bInitFlag;/// 应用句柄无效
	/// 创建容器
	ULONG nContainer = 0;
	hRet = m_spiSafeCard->CreateContainer(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONNAME),&nContainer);
	if(FAILED(hRet))
	{
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = NULL;
		return bInitFlag;
	}
	m_spiSafeCard->CloseContainer(nContainer);
	nContainer = NULL;

	/// 需要先登录验证
	SHORT nTryCount = 0;
	hRet = m_spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,bstrUserPin,&nTryCount);
	if(FAILED(hRet))
	{
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = NULL;
		return bInitFlag;
	}

	SHORT nFileLen = TDHXKJ_SKFAPP_FILELEN;
	CComBSTR bstrFileName(TDHXKJ_SKFAPP_CONFIGFILE);
	/// 创建文件读写数据
	hRet = m_spiSafeCard->CreateAppFile(nAppHandle,bstrFileName,nFileLen,\
		SKFAPP_FILESECURETYPE_USER,SKFAPP_FILESECURETYPE_USER);
	if(FAILED(hRet))
	{
		/// 删除重新创建文件
		hRet = m_spiSafeCard->DeleteAppFile(nAppHandle,bstrFileName);
		hRet = m_spiSafeCard->CreateAppFile(nAppHandle,bstrFileName,\
			nFileLen,SKFAPP_FILESECURETYPE_USER,SKFAPP_FILESECURETYPE_USER);
	}
	m_spiSafeCard->CloseApplication(nAppHandle);
	nAppHandle = NULL;
	if(SUCCEEDED(hRet))
		bInitFlag = true;
	return bInitFlag;
}

void CSafeService::CreateAuthFile(const CString& strAppName,const CString& strUserPin)
{
	if(NULL == m_spiSafeCard)
		InitSafeCom(1);
	CComPtr <ISoftEncry> spiSoftEncry = GetSoftEncry();
	if(NULL == m_spiSafeCard || NULL == spiSoftEncry)
		return;
	ULONG nAppHandle = 0;
	HRESULT hRet = m_spiSafeCard->OpenApplication(CComBSTR(strAppName),&nAppHandle);
	if(FAILED(hRet) || 0 == nAppHandle)
	{
		LONG nLastErr = 0;
		m_spiSafeCard->get_LastErrCode(&nLastErr);
		if(SAR_APPLICATION_NOT_EXISTS != nLastErr)
		{
			spiSoftEncry = NULL;
			return;
		}
		CComBSTR bstrCID;
		m_spiSafeCard->GetCID(NULL,&bstrCID);
		bool bRetFlag = InitCardApp(bstrCID.m_str);
		bstrCID.Empty();
		if(!bRetFlag)
		{
			spiSoftEncry = NULL;
			return;
		}
		hRet = m_spiSafeCard->OpenApplication(CComBSTR(strAppName),&nAppHandle);
		if(FAILED(hRet) || 0 == nAppHandle)
		{
			spiSoftEncry = NULL;
			return;
		}
	}
	SHORT nTryCount = 0;
	hRet = m_spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,CComBSTR(strUserPin),&nTryCount);
	if(FAILED(hRet))
	{
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		spiSoftEncry = NULL;
		return;
	}
	short nReadLen = TDHXKJ_SKFAPP_FILELEN;
	BYTE BufData[TDHXKJ_SKFAPP_FILELEN] = {0};
	hRet = m_spiSafeCard->ReadAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,&nReadLen,BufData);
	if(FAILED(hRet))
	{
#ifdef APP_LOG_ENABLE
		/// 获取错误信息
		CComBSTR bstrErrInfo;
		m_spiSafeCard->get_LastErrInfo(&bstrErrInfo);
		/// 写日志
		WRITELOGTOFILE(bstrErrInfo.m_str);
		bstrErrInfo.Empty();
#endif
		m_spiSafeCard->ClearSecureState(nAppHandle);
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		spiSoftEncry = NULL;
		return;
	}
	m_spiSafeCard->ClearSecureState(nAppHandle);
	m_spiSafeCard->CloseApplication(nAppHandle);
	nAppHandle = 0;

	unsigned char szVI[AES_BLOCK_SIZE] = "HX2016SafeGuard";

	ULONG nOutLen = 0;
	BYTE *pBuf = NULL;
	BYTE szKey[AES_BLOCK_SIZE*2+1] = "01234567899876543210012345678901";
	hRet = spiSoftEncry->AesCbcEnc(BufData,nReadLen,szKey,szVI,&nOutLen,&pBuf);

	CString strAuthFile = CBaseFuncLib::GetAppDataDir()+TDHXKJ_SKFAPP_AUTHFILE;
	BOOL bSafeFlag = CBaseFuncLib::WriteToFile(strAuthFile,pBuf,nOutLen);
	hRet = spiSoftEncry->ReleaseBuf(pBuf);	
	spiSoftEncry = NULL;
}

STDMETHODIMP CSafeService::CheckInit(BSTR bstrDisk,VARIANT_BOOL* pbInitFlag)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrDisk || NULL == pbInitFlag)
		return E_POINTER;
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		InitSafeCom(3);
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		return E_POINTER;
	ReSetData();

	m_nInitFlag = 0;
	(*pbInitFlag) = VARIANT_FALSE;
	m_strDiskName.Empty();
	m_strDiskName = bstrDisk;
	if(m_strDiskName.GetLength() != 1)
		return E_FAIL;
	/// 判断卡的合法性
	HRESULT hRet = m_spiSafeCard->put_CurDisk(bstrDisk);
	DWORD dwSize = 0;
	m_spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);
	if(FAILED(hRet))
	{
		return hRet;
	}
	CComBSTR bstrCID;
	m_spiSafeCard->GetCID(bstrDisk,&bstrCID);
	CString strString(bstrCID.m_str);
	strString += _T("_HXSafe");
	CString strSha1 = GetStringSha1(strString);

	unsigned char szHXKey[17];/// 和兴设备认证秘钥
	memset(szHXKey,0,17*sizeof(unsigned char));
	szHXKey[0] = 'H';
	szHXKey[1] = 'X';
	szHXKey[2] = '_';
	for(int nIndex = 3;nIndex < 16;nIndex++)
	{
		szHXKey[nIndex] = (unsigned char)strSha1.GetAt(nIndex);
	}
	CComBSTR bstrAppName(TDHXKJ_SKFAPP_NAME),bstrAdminPin(strSha1.Left(3)),bstrUserPin;
	bstrAdminPin.Append(strSha1.Right(5));
	strSha1.Delete(0,16);
	bstrUserPin.Append(strSha1.Left(8));
	m_strUserPin = bstrUserPin.m_str;

	m_spiSafeCard->LockDev(VARIANT_TRUE);
	/// 判断是否为和兴的设备授权
	hRet = m_spiSafeCard->DevAuth(szHXKey);
	if(FAILED(hRet))
	{
		/// 不是和兴的设备授权,尝试修改默认设备授权
		hRet = m_spiSafeCard->DevAuth((unsigned char *)g_szCCDefaultKey);
		if(SUCCEEDED(hRet))
			hRet = m_spiSafeCard->ChangeDevAuth((unsigned char *)g_szCCDefaultKey,szHXKey);
		if(FAILED(hRet))
		{
			m_spiSafeCard->LockDev(VARIANT_FALSE);
			bstrUserPin.Empty();
			bstrAdminPin.Empty();
			m_strUserPin.Empty();
			return hRet;
		}
		/// 初始化应用，创建授权文件存储
		bool bRetFlag = InitCardApp(bstrCID.m_str);
		bstrCID.Empty();
		m_spiSafeCard->LockDev(VARIANT_FALSE);
		if(bRetFlag)
		{
			m_nInitFlag = 1;

			/// 生成备份授权文件
			CreateAuthFile(bstrAppName.m_str,m_strUserPin);
		}

		bstrUserPin.Empty();
		bstrAdminPin.Empty();
		return hRet;
	}
	m_spiSafeCard->LockDev(VARIANT_FALSE);
	if(FAILED(hRet))
	{
		/// 设备认证失败
		bstrUserPin.Empty();
		bstrAdminPin.Empty();
		m_strUserPin.Empty();
		CComBSTR bstrErrInfo;
		m_spiSafeCard->get_LastErrInfo(&bstrErrInfo);
		if(bstrErrInfo.Length())
		{
#ifdef APP_LOG_ENABLE
			/// 写日志
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		return hRet;
	}
	m_nInitFlag = 1;
	(*pbInitFlag) = VARIANT_TRUE;
	/// 生成备份授权文件
	CreateAuthFile(bstrAppName.m_str,m_strUserPin);
	bstrUserPin.Empty();
	bstrAdminPin.Empty();
	return S_OK;
}

#endif

STDMETHODIMP CSafeService::put_CustomerName(BSTR newVal)
{
	// TODO: 在此添加实现代码
	if(NULL == newVal)
		return E_POINTER;
	m_strCustomerName.Empty();
	m_strCustomerName = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::put_CustomerID(ULONG newVal)
{
	// TODO: 在此添加实现代码
	m_nCustomerID = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::put_DefaultPW(BSTR newVal)
{
	// TODO: 在此添加实现代码
	if(NULL == newVal)
		return E_POINTER;
	m_strDefAdminPW.Empty();
	m_strDefAdminPW = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::put_DBPW(BSTR newVal)
{
	// TODO: 在此添加实现代码
	if(NULL == newVal)
		return E_POINTER;
	m_strDataPW.Empty();
	m_strDataPW = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::put_AuthNode(USHORT newVal)
{
	// TODO: 在此添加实现代码
	m_nAuthNode = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::put_ValidDay(ULONG newVal)
{
	// TODO: 在此添加实现代码
	m_nValidDay = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::put_AuthPacket(ULONG newVal)
{
	// TODO: 在此添加实现代码
	m_nAuthPacket = newVal;
	return S_OK;
}

void CSafeService::ReSetData()
{
	m_nInitFlag = 0;
	m_nValidDay = 0;
	m_nAuthPacket = 0;
	m_nAuthNode = 0;
	m_nCustomerID = 0;
	m_bInstallDisk = VARIANT_FALSE;

	m_strDataPW.Empty();
	m_strDefAdminPW.Empty();
	m_strCustomerName.Empty();
	m_strDiskName.Empty();
	m_strUserPin.Empty();
	m_strDataPW.Empty();

	if(NULL != m_spiSafeCard)
		m_spiSafeCard->CloseDev();
	if(NULL != m_spiCryptoStor)
		m_spiCryptoStor->CloseDisk();
}

STDMETHODIMP CSafeService::put_InstallDisk(VARIANT_BOOL newVal)
{
	// TODO: 在此添加实现代码
	m_bInstallDisk = newVal;
	return S_OK;
}

STDMETHODIMP CSafeService::get_InstallDisk(VARIANT_BOOL* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_bInstallDisk;
	return S_OK;
}

#ifdef TDHXKJ_VERSION_SAFETOOL
STDMETHODIMP CSafeService::GetCorpID(BSTR bstrCorpName,SHORT nType,ULONG* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrCorpName)
		return E_FAIL;
	CComBSTR bstrVal;

	HRESULT hRet(E_FAIL);
	/// 获得授权相关的信息
	CString strAuthFile(CBaseFuncLib::GetModulePath()+TDHXKJ_CUSTOMER_DB);
	/// 尝试打开
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(TDHXKJ_DEFAULT_DBPW),&spiSqlite3Connect);
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
		return hRet;
	ULONG nUsedCount = 0;
	/// 创建授权表
	CString strSqlCmd(_T(""));
	CString strTableName = SQLITEDB_TABLE_AUTHUSER;
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///记录软件授权客户信息AuthAutoID/Type/AuthCount/AuthCode/AuthDay/CorpID/CorpName/DataPW/AdminPW/InsDisk/DiskID/AddTime
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
							AID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
							Type INTEGER DEFAULT '0' NOT NULL,\
							ACount INTEGER DEFAULT '0' NOT NULL,\
							ACode INTEGER DEFAULT '0' NOT NULL,\
							ADay INTEGER DEFAULT '0' NOT NULL,\
							CorpID INTEGER DEFAULT '0' NOT NULL,\
							InsDisk INTEGER DEFAULT '0' NOT NULL,\
							ATime DOUBLE DEFAULT '' NOT NULL,\
							CorpName VARCHAR DEFAULT '' NOT NULL,\
							DataPW VARCHAR DEFAULT '' NOT NULL,\
							AdminPW VARCHAR DEFAULT '' NOT NULL,\
							DiskID VARCHAR DEFAULT '' NOT NULL);"),\
							strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		if(FAILED(hRet))
		{
			spiSqlite3Connect->get_LastErrorInfo(&bstrVal);
			bstrVal.Empty();
		}
		strSqlCmd.Empty();
	}
	strTableName.Empty();

	CString strCorp(_T(""));
	strSqlCmd.Format(_T("SELECT CorpID FROM [%s] WHERE CorpName='%s' AND InsDisk = 1"),\
		SQLITEDB_TABLE_AUTHUSER,bstrCorpName);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			bstrVal.Empty();
			spiSqlite3Connect->GetValueString(0,&bstrVal);
			(*pVal) = CBaseFuncLib::StrToNum(bstrVal.m_str);
			bstrVal.Empty();
			break;
		}
	}
	strSqlCmd.Empty();

	COleDateTime curTime(COleDateTime::GetCurrentTime());
	if(0 == (*pVal))
	{
		ULONG nCorpID = 1000;
		strSqlCmd.Format(_T("SELECT CorpID FROM [%s] WHERE InsDisk = 1"),SQLITEDB_TABLE_AUTHUSER);
		if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
		{
			while(SUCCEEDED(spiSqlite3Connect->NextRow()))
			{
				bstrVal.Empty();
				spiSqlite3Connect->GetValueString(0,&bstrVal);
				ULONG nTem = CBaseFuncLib::StrToNum(bstrVal.m_str);
				if(nTem > nCorpID)
					nCorpID = nTem;
				bstrVal.Empty();
			}
		}
		/// 递增一个
		(*pVal) = nCorpID+1;
		strSqlCmd.Format(_T("INSERT INTO [%s] (CorpID,CorpName,Type,InsDisk,ATime) \
							VALUES(%ld,\'%s\',%ld,1,%f);"),
							SQLITEDB_TABLE_AUTHUSER,(*pVal),bstrCorpName,nType,curTime.m_dt);
	}
	if(!strSqlCmd.IsEmpty())
	{
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;

	return S_OK;
}

STDMETHODIMP CSafeService::AuthInit(ULONG nCorpID)
{
	// TODO: 在此添加实现代码
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		InitSafeCom(3);
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		return E_FAIL;
	CComPtr <IJsonService> spiJsonService = GetAuthInfo();
	if(NULL == spiJsonService)
		return E_FAIL;
	/// 获得数据库访问密码
	CString strDBPW;
	CComBSTR bstrVal;
	spiJsonService->GetStringValue(CComBSTR(_T("DataPW")),&bstrVal);
	if(bstrVal.Length())
		strDBPW = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService = NULL;

	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	HRESULT hRet = this->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
		return hRet;
	DWORD dwSize = 0;
	CComBSTR bstrDisk;
	m_spiSafeCard->get_CurDisk(&bstrDisk);
	spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);

	CString strAuthFile(CBaseFuncLib::GetTmpPath(FALSE)+TDHX_SQLITEDB_AUTHFILE);
	::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
	::DeleteFile(strAuthFile);

	/// 尝试先读取
	hRet = spiCryptoStor->ReadOnlyFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE),CComBSTR(strAuthFile));
	/// 尝试打开
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
		if(NULL == spiSqlite3Connect)
		{
			spiCryptoStor->DelFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE));
			::DeleteFile(strAuthFile);
			/// 重新创建
			hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
		}
		if(NULL == spiSqlite3Connect)
		{
			CComBSTR bstrErrInfo;
			spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
	{
		spiCryptoStor->CloseDisk();
		spiCryptoStor = NULL;
		return hRet;
	}
	CString strSqlCmd(_T(""));
	if(nCorpID > 0)
	{
		strSqlCmd.Format( _T("DELETE FROM [%s] WHERE CorpID = %ld"), \
			SQLITEDB_TABLE_AUTHLIST,nCorpID);
	}
	else
	{
		strSqlCmd.Format( _T("DELETE FROM [%s]"), \
			SQLITEDB_TABLE_AUTHLIST);
	}

	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	if(SUCCEEDED(hRet))
	{
		hRet = spiCryptoStor->WriteInFile(CComBSTR(strAuthFile),CComBSTR(TDHX_SQLITEDB_AUTHFILE),VARIANT_FALSE);
	}
	spiCryptoStor->CloseDisk();
	spiCryptoStor = NULL;
	strSqlCmd.Empty();
	return hRet;
}

STDMETHODIMP CSafeService::Init(BSTR bstrDisk,VARIANT_BOOL bReInit)
{
	// TODO: 在此添加实现代码
	if(1 != m_nInitFlag)
		return E_FAIL;
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		InitSafeCom(3);
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		return E_FAIL;
	m_strDiskName.Empty();
	m_strDiskName = bstrDisk;
	if(m_strDiskName.GetLength() != 1)
		return E_FAIL;
	ATL::CString strDeviceID(_T(""));
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID);
	if(strDeviceID.IsEmpty())
	{
		CComBSTR bstrVal;
		this->GetUniqueID(&bstrVal);
		strDeviceID = bstrVal.m_str;
		bstrVal.Empty();
	}
	//if(0 != strDeviceID.CompareNoCase(_T("C2927BBBA9FABC620B2764DAFFE832A8")))
	//	return E_FAIL;/// 确保本机才能运行授权
	CComBSTR bstrCID;
	if(VARIANT_TRUE == bReInit)
	{
		/// 删除再重新创建
		HRESULT hRet = m_spiSafeCard->put_CurDisk(bstrDisk);
		if(FAILED(hRet))
		{
			return hRet;
		}
		m_spiSafeCard->DeleteApplication(CComBSTR(TDHXKJ_SKFAPP_NAME));
		m_spiSafeCard->GetCID(bstrDisk,&bstrCID);
		InitCardApp(bstrCID.m_str);
	}
	else
		m_spiSafeCard->GetCID(bstrDisk,&bstrCID);

	ULONG nAppHandle = 0;
	HRESULT hRet = m_spiSafeCard->OpenApplication(CComBSTR(TDHXKJ_SKFAPP_NAME),&nAppHandle);
	if(FAILED(hRet))
	{
		return E_FAIL;
	}
	SHORT nTryCount = 0;
	hRet = m_spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,CComBSTR(m_strUserPin),&nTryCount);
	if(FAILED(hRet))
	{
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		return E_FAIL;
	}

	/// 生成软件授权码并写入应用内的文件
	CString strAuthString(_T(""));
	COleDateTime StartTime(2016,01,01,0,0,0);
	COleDateTime curTime(COleDateTime::GetCurrentTime());
	COleDateTimeSpan spanTime(m_nValidDay,0,0,0);
	COleDateTime ValidDate = StartTime+spanTime;
	CString strAuthCode(_T("")),strValidDay(_T(""));
	strAuthCode.Format(_T("10%04d"),m_nAuthPacket);///1代表主机安全防护软件
	strValidDay.Format(_T("%05d"),m_nValidDay);
	strAuthString = _T("HX_");
	strAuthString += strAuthCode;
	strAuthString = _T("_");
	strAuthString += strValidDay;
	strAuthString = _T("_Safe");
	CString strSha1 = GetStringSha1(strAuthString);
	if(strSha1.IsEmpty())
	{
		m_spiSafeCard->ClearSecureState(nAppHandle);
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
		return E_FAIL;
	}

	CString strDiskID;
	if(VARIANT_TRUE == m_bInstallDisk)
	{
		CComBSTR bstrCID;
		m_spiSafeCard->GetCID(bstrDisk,&bstrCID);
		strDiskID = bstrCID.m_str;
		bstrCID.Empty();
	}

	/// 生成授权码
	CString strCode = strSha1.Left(5);
	strSha1.Delete(0,10);
	strAuthString.Format(_T("%s-%s-%s-%s-%s"),strAuthCode,strValidDay,strCode,strSha1.Left(5),strSha1.Right(5));
	CString strFileString(_T(""));
	/// 生成JSON格式配置
	strFileString.Format(_T("{\"CustomerName\" :\"%s\",\"CustomerID\" :%ld,\"InstallDisk\" :\"%s\",\"AuthPacket\" :%ld,\"ValidDay\" :%ld,\"AuthNode\" :%ld,\"DataPW\" :\"%s\",\"AdminPW\" :\"%s\"}"),\
		m_strCustomerName,m_nCustomerID,strDiskID,m_nAuthPacket,m_nValidDay,m_nAuthNode,m_strDataPW,m_strDefAdminPW);
	/// 通过容器算法进行加密存储?
	ATLASSERT(strFileString.GetLength() < TDHXKJ_SKFAPP_FILELEN);
	char *szBufFile = NULL;
	int nStrLen = CBaseFuncLib::Us2ToChar(strFileString,&szBufFile);
	if(NULL != szBufFile)
	{
		hRet = m_spiSafeCard->WriteAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,(BYTE* )szBufFile,nStrLen-1);
		delete []szBufFile;
		szBufFile = NULL;
		if(FAILED(hRet))
		{
#ifdef APP_LOG_ENABLE
			CComBSTR bstrErrInfo;
			m_spiSafeCard->get_LastErrInfo(&bstrErrInfo);
			/// 写日志
			WRITELOGTOFILE(bstrErrInfo.m_str);
			bstrErrInfo.Empty();
#endif
		}
	}
	m_spiSafeCard->ClearSecureState(nAppHandle);
	m_spiSafeCard->CloseApplication(nAppHandle);
	nAppHandle = 0;
	if(SUCCEEDED(hRet))
	{
		if(strDiskID.IsEmpty())
		{
			CComBSTR bstrCID;
			m_spiSafeCard->GetCID(bstrDisk,&bstrCID);
			strDiskID = bstrCID.m_str;
			bstrCID.Empty();
		}

		/// 记录到用户信息数据库中
		SaveUserInfo(m_nCustomerID,strDiskID,m_nAuthPacket,m_nValidDay,m_nAuthNode,m_strDataPW,m_strDefAdminPW);
	}
	return hRet;
}

void CSafeService::SaveUserInfo(LONG nCustomerID,const CString& strDiskID,LONG nAuthPacket,\
	LONG nValidDay,LONG nAuthNode,const CString& strDataPW,const CString& strDefAdminPW)
{
	HRESULT hRet(E_FAIL);
	/// 获得授权相关的信息
	CString strAuthFile(CBaseFuncLib::GetModulePath()+TDHXKJ_CUSTOMER_DB);
	/// 尝试打开
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(TDHXKJ_DEFAULT_DBPW),&spiSqlite3Connect);
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
		return;
	int nInsDisk = 0;
	CString strSqlCmd;
	CString strTableName(SQLITEDB_TABLE_AUTHUSER);
	COleDateTime curTime(COleDateTime::GetCurrentTime());
	if(VARIANT_TRUE == m_bInstallDisk)
	{
		nInsDisk = 1;
		strSqlCmd.Format(_T("SELECT AID FROM [%s] WHERE CorpID = %ld AND InsDisk=1"),\
			strTableName,nCustomerID);	
		spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		LONG nVal = 0;
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&nVal);
			break;
		}

		if(nVal)
		{
			strSqlCmd.Format(_T("UPDATE [%s] SET ACount=%ld,ACode=%ld,ADay=%ld,InsDisk=%d,CorpName=\'%s\',DiskID=\'%s\',DataPW=\'%s\',AdminPW=\'%s\',ATime=%f WHERE AID=%ld"),\
				strTableName,nAuthNode,nAuthPacket,nValidDay,nInsDisk,m_strCustomerName,strDiskID,strDataPW,strDefAdminPW,curTime.m_dt,nVal);
		}
		else
		{
			strSqlCmd.Format(_T("INSERT INTO [%s] (CorpID,CorpName,DiskID,DataPW,AdminPW,InsDisk,ATime,ACount,ACode,ADay) \
								VALUES(%ld,\'%s\',\'%s\',\'%s\',\'%s\',1,%f,%d,%ld,%d);"),
								strTableName,nCustomerID,m_strCustomerName,strDiskID,strDataPW,strDefAdminPW,curTime.m_dt,nAuthNode,nAuthPacket,nValidDay);
		}
	}
	else
	{
		strSqlCmd.Format(_T("SELECT AID FROM [%s] WHERE CorpID = %ld AND DiskID=\'%s\'"),\
			strTableName,nCustomerID,strDiskID);	
		spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		LONG nVal = 0;
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&nVal);
			break;
		}
		if(nVal)
		{
			strSqlCmd.Format(_T("UPDATE [%s] SET CorpName=\'%s\',ACount=%ld,ACode=%ld,ADay=%ld,InsDisk=0,ATime=%f WHERE AID=%ld"),\
				strTableName,m_strCustomerName,nAuthNode,nAuthPacket,nValidDay,curTime.m_dt,nVal);
		}
		else
		{
			strSqlCmd.Format(_T("INSERT INTO [%s] (CorpID,CorpName,DiskID,InsDisk,ATime,ACount,ACode,ADay) \
								VALUES(%ld,\'%s\',\'%s\',0,%f,%d,%ld,%d);"),
								strTableName,nCustomerID,m_strCustomerName,strDiskID,curTime.m_dt,nAuthNode,nAuthPacket,nValidDay);
		}

	}
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
}

STDMETHODIMP CSafeService::CreateAuth(BSTR bstrFilePath,BYTE* pbKey,VARIANT_BOOL bAddCount)
{
	// TODO: 在此添加实现代码
	/// 非U盘模式，安装盘ID为空
	CString strInstallDisk;
	HRESULT hRet(E_FAIL);
	CString strFileString(_T(""));
	strFileString.Format(_T("{\"CustomerName\" :\"%s\",\"CustomerID\" :%ld,\"InstallDisk\" :\"%s\",\"AuthPacket\" :%ld,\"ValidDay\" :%ld,\"AuthNode\" :%ld,\"DataPW\" :\"%s\",\"AdminPW\" :\"%s\"}"),\
		m_strCustomerName,m_nCustomerID,strInstallDisk,m_nAuthPacket,m_nValidDay,m_nAuthNode,m_strDataPW,m_strDefAdminPW);
	/// 加密保存
	CComPtr<IClassFactory>	cf = NULL;
	CComPtr <ISoftEncry> spiSoftEncry = GetSoftEncry();
	if(NULL == spiSoftEncry)
		return hRet;
	unsigned char szKey[AES_BLOCK_SIZE*2+1] = "01234567899876543210012345678901";
	unsigned char szVI[AES_BLOCK_SIZE]  = "HX2016SafeGuard";

	ULONG nOutLen = 0;
	BYTE *pBuf = NULL,*pIn = NULL;
	int nInLen = CBaseFuncLib::Us2ToChar(strFileString,(char**)&pIn);
	if(NULL == pbKey)
		hRet = spiSoftEncry->AesCbcEnc(pIn,nInLen-1,szKey,szVI,&nOutLen,&pBuf);
	else
		hRet = spiSoftEncry->AesCbcEnc(pIn,nInLen-1,pbKey,szVI,&nOutLen,&pBuf);
	if(NULL != pIn)
	{
		delete []pIn;
		pIn = NULL;
	}
	if(FAILED(hRet) || NULL == pBuf)
	{
		spiSoftEncry = NULL;
		return hRet;
	}

	/// 写入文件保存
	CBaseFuncLib::WriteToFile(bstrFilePath,pBuf,(DWORD)::strlen((char* )pBuf));
	spiSoftEncry->ReleaseBuf(pBuf);
	spiSoftEncry = NULL;
	return S_OK;
}

#endif

CString CSafeService::GetStringSha1(const CString& strString)
{
	SHA_CTX shactx;  
	unsigned char md[SHA_DIGEST_LENGTH];  

	SHA1_Init(&shactx);
	DWORD m = 0;
	char* szBuf = NULL;
	m = CBaseFuncLib::Us2ToChar(strString,&szBuf);
	if(NULL != szBuf)
	{
		SHA1_Update(&shactx,szBuf,m-1);
		delete []szBuf;
		szBuf = NULL;
	}
	SHA1_Final(md, &shactx);
	CString strSha1 = convert_hex(md,SHA_DIGEST_LENGTH);
	return strSha1;
}

STDMETHODIMP CSafeService::StringSha1(BSTR bstrString,BSTR* pbstrSha1)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrString)
		return E_POINTER;
	CString strSha1 = GetStringSha1(bstrString);
	CComBSTR bstrVal(strSha1);
	bstrVal.CopyTo(pbstrSha1);
	bstrVal.Empty();
	strSha1.Empty();
	return S_OK;
}

STDMETHODIMP CSafeService::FileSha1(BSTR bstrFilePath,BSTR* pbstrSha1)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrFilePath)
		return E_POINTER;
	HANDLE hFileOpen = ::CreateFile(bstrFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
		return E_FAIL;/// 文件读取失败
	BYTE *pData = new BYTE[TDHXKJ_READFILE_MAXBUFF];
	if(NULL == pData)
	{
		::CloseHandle(hFileOpen);
		hFileOpen = NULL;
		return E_OUTOFMEMORY;
	}

	SHA_CTX shactx;  
	unsigned char md[SHA_DIGEST_LENGTH];  

	SHA1_Init(&shactx);

	DWORD m = 0;
	memset(pData,0,TDHXKJ_READFILE_MAXBUFF);
	while(TRUE)
	{
		::ReadFile(hFileOpen,pData,TDHXKJ_READFILE_MAXBUFF,&m,NULL);
		if(0 == m)
			break;/// 读取完成
		if(m)
		{
			SHA1_Update(&shactx,pData,m);
			m = 0;
		}
		memset(pData,0,TDHXKJ_READFILE_MAXBUFF);
	}

	SHA1_Final(md, &shactx);
	CString strSha1 = convert_hex(md,SHA_DIGEST_LENGTH);

	delete []pData;
	pData = NULL;

	::CloseHandle(hFileOpen);
	hFileOpen = NULL;

	CComBSTR bstrVal(strSha1);
	bstrVal.CopyTo(pbstrSha1);
	bstrVal.Empty();
	strSha1.Empty();
	return S_OK;
}

CString CSafeService::GetStringMD5(const CString& strString)
{
	MD5_CTX c;
	char* szBuf = NULL;
	CString strMD5(_T(""));
	unsigned char md5[MD5_DIGEST_LENGTH]={0};

	MD5_Init(&c); 
	int nLen = CBaseFuncLib::Us2ToChar(strString,&szBuf);
	if(NULL != szBuf)
	{
		MD5_Update(&c, szBuf, nLen-1);
		delete []szBuf;
		szBuf = NULL;
	}
	MD5_Final(md5,&c);
	strMD5 = convert_hex(md5,MD5_DIGEST_LENGTH);
	return strMD5;
}

STDMETHODIMP CSafeService::StringMD5(BSTR bstrString,BSTR* pbstrMD5)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrString)
		return E_POINTER;
	CString strMD5 = GetStringMD5(bstrString);
	CComBSTR bstrVal(strMD5);
	bstrVal.CopyTo(pbstrMD5);
	bstrVal.Empty();
	strMD5.Empty();
	return S_OK;
}

STDMETHODIMP CSafeService::FileMD5(BSTR bstrFilePath,unsigned char* pbMD5)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrFilePath || NULL == pbMD5)
		return E_POINTER;
	memset(pbMD5,NULL,MD5_DIGEST_LENGTH);
	if(!CBaseFuncLib::IsPathExist(bstrFilePath))
	{
		/// 文件不存在
		return E_FAIL;
	}
	HANDLE hFileOpen = ::CreateFile(bstrFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(NULL == hFileOpen || INVALID_HANDLE_VALUE == hFileOpen)
	{
		return E_FAIL;
	}
	BYTE *pData = new BYTE[TDHXKJ_READFILE_MAXBUFF];
	if(NULL == pData)
	{
		::CloseHandle(hFileOpen);
		hFileOpen = NULL;
		return E_OUTOFMEMORY;
	}
	MD5_CTX c;
	DWORD dwRead = 0;
	BOOL bReadFlag = FALSE;
	int nRet = MD5_Init(&c); 

	while(TRUE)
	{
		dwRead = 0;
		memset(pData,NULL,TDHXKJ_READFILE_MAXBUFF);
		bReadFlag = ::ReadFile(hFileOpen,pData,TDHXKJ_READFILE_MAXBUFF,&dwRead,NULL);
		if(!bReadFlag || 0 == dwRead)
			break;/// 读取完成
		if(0 != dwRead)
			nRet = MD5_Update(&c,pData,dwRead);
	}
	delete []pData;
	pData = NULL;

	::CloseHandle(hFileOpen);
	hFileOpen = NULL;

	nRet = MD5_Final(pbMD5,&c);
	return S_OK;
}

STDMETHODIMP CSafeService::TransToString(BYTE* pbMD5,SHORT nLen,BSTR* pbstrMD5)
{
	// TODO: 在此添加实现代码
	if(NULL == pbMD5)
		return E_POINTER;
	if(NULL == pbstrMD5)
		return E_POINTER;
	/// 调用转换
	CString strMD5 = convert_hex(pbMD5,nLen);

	CComBSTR bstrVal(strMD5);
	bstrVal.CopyTo(pbstrMD5);
	bstrVal.Empty();
	strMD5.Empty();
	return S_OK;
}

STDMETHODIMP CSafeService::FileMD5String(BSTR bstrFilePath,BSTR* pbstrMD5)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrFilePath)
		return E_POINTER;
	if(NULL == pbstrMD5)
		return E_POINTER;
	unsigned char md5[MD5_DIGEST_LENGTH]={0};
	HRESULT hRet = FileMD5(bstrFilePath,md5);
	if(SUCCEEDED(hRet) && NULL != md5)
	{
		CString strMD5 = convert_hex(md5,MD5_DIGEST_LENGTH);

		CComBSTR bstrVal(strMD5);
		bstrVal.CopyTo(pbstrMD5);
		bstrVal.Empty();
		strMD5.Empty();
	}
	return hRet;
}

STDMETHODIMP CSafeService::GetInstallDisk(VARIANT_BOOL bCID,BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
		return E_POINTER;
#ifndef TDHXKJ_VERSION_NOUSB
	CComBSTR bstrVal;
	if(VARIANT_TRUE == bCID)
	{
		CComPtr <IJsonService> spiJsonService = GetAuthInfo();
		if(NULL == spiJsonService)
			return E_FAIL;
		spiJsonService->GetStringValue(CComBSTR(_T("InstallDisk")),&bstrVal);
		if(bstrVal.Length())
		{
			bstrVal.CopyTo(pVal);
			bstrVal.Empty();
		}
		spiJsonService = NULL;
		return S_OK;
	}
	if(NULL == m_spiSafeCard)
		InitSafeCom(1);
	if(NULL == m_spiSafeCard)
		return E_FAIL;
	HRESULT hRet = m_spiSafeCard->EnumCard(&bstrVal);
	if(FAILED(hRet) || !bstrVal.Length())
	{
		if(FAILED(hRet))
		{
#ifdef APP_LOG_ENABLE
			/// 写日志
			m_spiSafeCard->get_LastErrInfo(&bstrVal);
			WRITELOGTOFILE(bstrVal.m_str);
			bstrVal.Empty();
#endif
		}
		return hRet;
	}
	CString strDisk = bstrVal.m_str;
	bstrVal.Empty();
	WCHAR ucDisk = 0,ucInstDisk = 0;
	int nDiskLen = strDisk.GetLength();
	CString strInstallDisk;
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	regKey.GetRegStringVal(PRODUCT_COMMAN_INSDISK,strInstallDisk);
	for(int nIndex = 0;nIndex < nDiskLen;nIndex++)
	{
		CComBSTR bstrDisk;
		ucDisk = strDisk.GetAt(nIndex);
		bstrDisk.Append(ucDisk);
		hRet = m_spiSafeCard->put_CurDisk(bstrDisk);
		if(FAILED(hRet))
		{
			LONG nErrCode = 0;
			m_spiSafeCard->get_LastErrCode(&nErrCode);
			if(nErrCode && ERROR_FILE_NOT_FOUND != nErrCode)
			{
				m_spiSafeCard->get_LastErrInfo(&bstrVal);
#ifdef APP_LOG_ENABLE
				/// 写日志
				CString strLog(_T(""));
				strLog.Format(_T("InsDisk设置%s盘失败，原因:%s"),\
					(CString)bstrDisk.m_str,(CString)bstrVal.m_str);
				WRITELOGTOFILE(strLog);
#endif
				bstrVal.Empty();
			}
			bstrDisk.Empty();
			nIndex++;
			continue;
		}
		hRet = m_spiSafeCard->GetCID(bstrDisk,&bstrVal);
		if(0 == strInstallDisk.CompareNoCase(bstrVal.m_str))
		{
			/// 找到安装盘
			ucInstDisk = ucDisk;
			bstrDisk.Empty();
			bstrVal.Empty();
			break;
		}
		bstrVal.Empty();
		bstrDisk.Empty();
		nIndex++;
	}
	if(0 != ucInstDisk)
	{
		bstrVal.Empty();
		bstrVal.Append(ucInstDisk);
		bstrVal.CopyTo(pVal);
		bstrVal.Empty();
		return S_OK;
	}
#endif
	return E_FAIL;
}

CComPtr <IJsonService> CSafeService::GetAuthInfo()
{
	CComBSTR bstrVal,bstrDisk;
	if(NULL != m_spiSafeCard)
	{
		/// 获得当前使用的盘符
		m_spiSafeCard->get_CurDisk(&bstrDisk);
	}
	else
		bstrDisk.Append(m_strDiskName);

	CComPtr <IJsonService> spiJsonService = NULL;
	HRESULT hRet = get_CID(VARIANT_TRUE,&bstrDisk,&bstrVal);
	if(FAILED(hRet) || !bstrVal.Length())
	{
		return spiJsonService;
	}
	if(NULL == m_spiSafeCard)
		return spiJsonService;
	CString strCID(bstrVal.m_str);
	bstrVal.Empty();
	StringSha1(CComBSTR(strCID+_T("_HXSafe")),&bstrVal);
	strCID.Empty();
	CString strSha1 = bstrVal.m_str;
	bstrVal.Empty();
	strSha1.Delete(0,16);
	CComBSTR bstrAppName(TDHXKJ_SKFAPP_NAME),bstrUserPin(strSha1.Left(8));
	ULONG nAppHandle = 0;
	hRet = m_spiSafeCard->OpenApplication(CComBSTR(TDHXKJ_SKFAPP_NAME),&nAppHandle);
	if(FAILED(hRet))
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("打开应用失败"));
#endif
		return spiJsonService;
	}
	SHORT nTryCount = 0;
	hRet = m_spiSafeCard->VerifyPIN(nAppHandle,VARIANT_FALSE,bstrUserPin,&nTryCount);
	if(FAILED(hRet))
	{
		m_spiSafeCard->CloseApplication(nAppHandle);
		nAppHandle = 0;
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("校验应用失败"));
#endif
		return spiJsonService;
	}
	short nReadLen = TDHXKJ_SKFAPP_FILELEN;
	BYTE BufData[TDHXKJ_SKFAPP_FILELEN] = {0};
	VARIANT_BOOL bRetFlag = VARIANT_FALSE;
	hRet = m_spiSafeCard->ReadAppFile(nAppHandle,CComBSTR(TDHXKJ_SKFAPP_CONFIGFILE),0,&nReadLen,BufData);
	if(FAILED(hRet))
	{
#ifdef APP_LOG_ENABLE
		/// 获取错误信息
		CComBSTR bstrErrInfo;
		m_spiSafeCard->get_LastErrInfo(&bstrErrInfo);
		/// 写日志
		WRITELOGTOFILE(bstrErrInfo.m_str);
		bstrErrInfo.Empty();
#endif
		return spiJsonService;
	}
	CString strData(BufData);
	strData.TrimLeft();
	strData.TrimRight();
	if(!strData.IsEmpty())
	{
		if(NULL == spiJsonService)
			spiJsonService = CDbHelper::GetJsonService();
		if(NULL != spiJsonService)
		{
			spiJsonService->put_CodingType(CODINGTYPE_ANSI);
			hRet = spiJsonService->ParseString(CComBSTR(strData),&bRetFlag);
		}
		if(VARIANT_FALSE == bRetFlag)
			spiJsonService = NULL;
	}
	m_spiSafeCard->ClearSecureState(nAppHandle);
	m_spiSafeCard->CloseApplication(nAppHandle);
	nAppHandle = 0;
	return spiJsonService;
}

STDMETHODIMP CSafeService::CloseSafeCard()
{
	// TODO: 在此添加实现代码
	if(NULL != m_spiSafeCard)
	{
		m_spiSafeCard->CloseDev();
		m_spiSafeCard = NULL;
	}

	if(NULL != m_spiCryptoStor)
	{
		m_spiCryptoStor->CloseDisk();
		m_spiCryptoStor = NULL;
	}
	::Sleep(50);
	/// 释放不再需要的库
	::CoFreeAllLibraries();
	return S_OK;
}

STDMETHODIMP CSafeService::Auth(BSTR bstrDeviceID)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrDeviceID)
		return E_POINTER;
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		InitSafeCom(3);
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		return E_FAIL;
	CComBSTR bstrVal;
	this->GetUniqueID(&bstrVal);
	CString strDeviceID;
	if(bstrVal.Length())
		strDeviceID = bstrVal.m_str;
	bstrVal.Empty();
	if(0 != strDeviceID.CompareNoCase(bstrDeviceID))
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE2(_T("授权对象错误"),_T("Auth"),LOGTYPE_NORMAL);
#endif
		return E_FAIL;
	}
	/// 只支持安全卡的自动授权
	CComPtr <IJsonService> spiJsonService = GetAuthInfo();
	if(NULL == spiJsonService)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE2(_T("读取授权信息失败"),_T("Auth"),LOGTYPE_NORMAL);
#endif
		return E_FAIL;
	}
	CString strDBPW;
	spiJsonService->GetStringValue(CComBSTR(_T("DataPW")),&bstrVal);
	if(bstrVal.Length())
		strDBPW = bstrVal.m_str;
	bstrVal.Empty();
	ULONG nAuthNode = 0;
	spiJsonService->GetStringValue(CComBSTR(_T("AuthNode")),&bstrVal);
	if(bstrVal.Length())
	{
		nAuthNode = CBaseFuncLib::StrToNum(bstrVal.m_str);
	}
	bstrVal.Empty();
	CComVariant varCustomerID;
	spiJsonService->GetVariantValue(CComBSTR(_T("CustomerID")),&varCustomerID);
	varCustomerID.ChangeType(VT_I4);

	/// 获得授权相关的信息
	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	HRESULT hRet = this->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
	{
		spiJsonService = NULL;
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE2(_T("获取私密区对象失败"),_T("Auth"),LOGTYPE_NORMAL);
#endif
		return hRet;
	}
	DWORD dwSize = 0;
	CComBSTR bstrDisk;
	m_spiSafeCard->get_CurDisk(&bstrDisk);
	spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);

	CString strAuthFile(CBaseFuncLib::GetTmpPath(FALSE)+TDHX_SQLITEDB_AUTHFILE);
	::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
	::DeleteFile(strAuthFile);
	/// 尝试先读取
	hRet = spiCryptoStor->ReadOnlyFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE),CComBSTR(strAuthFile));
	/// 尝试打开
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
#ifdef _DEBUG
		if(NULL == spiSqlite3Connect)
		{
			spiCryptoStor->DelFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE));
			::DeleteFile(strAuthFile);
			/// 重新创建
			hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
		}
#endif
		if(NULL == spiSqlite3Connect)
		{
			CComBSTR bstrErrInfo;
			spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
	{
		spiJsonService = NULL;
		spiCryptoStor->CloseDisk();
		spiCryptoStor = NULL;
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE2(_T("读取授权数据库失败"),_T("Auth"),LOGTYPE_NORMAL);
#endif
		return E_FAIL;
	}
	ULONG nUsedCount = 0;
	CString strSqlCmd(_T(""));
	/// 检查创建授权表
	CString strTableName = SQLITEDB_TABLE_AUTHLIST;
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///记录软件授权信息AuthAutoID/DeviceID/AuthCode/CorpID/CorpName/AuthTime
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
							AID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
							DID VARCHAR DEFAULT '' NOT NULL,\
							Code VARCHAR DEFAULT '' NOT NULL,\
							CorpID INTEGER DEFAULT '0' NOT NULL,\
							CorpName VARCHAR DEFAULT '' NOT NULL,\
							ATime DOUBLE DEFAULT '' NOT NULL);"),\
							strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
	}
	else
	{
		strSqlCmd.Format(_T("SELECT Code FROM [%s] WHERE CorpID=%ld"),\
			strTableName,varCustomerID.lVal);
		nUsedCount = CDbHelper::GetRecordCount(spiSqlite3Connect,strSqlCmd);
		strSqlCmd.Empty();
	}
	CString strValidDay,strPacket,strCustomerName;
	spiJsonService->GetStringValue(CComBSTR(_T("ValidDay")),&bstrVal);
	if(bstrVal.Length())
		strValidDay = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService->GetStringValue(CComBSTR(_T("AuthPacket")),&bstrVal);
	if(bstrVal.Length())
		strPacket = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService->GetStringValue(CComBSTR(_T("CustomerName")),&bstrVal);
	if(bstrVal.Length())
		strCustomerName = bstrVal.m_str;
	bstrVal.Empty();
	spiJsonService = NULL;

	/// 计算授权码
	CString strCode,strAuthCode;
	strCode.Format(_T("HXSafe%ld_%s_%s_%s"),varCustomerID.lVal,strDeviceID,strValidDay,strPacket);
	StringSha1(CComBSTR(strCode),&bstrVal);
	strCode.Empty();
	strCode = bstrVal.m_str;
	bstrVal.Empty();
	strAuthCode.Format(_T("HX%04d-%s-%s-%s"),varCustomerID.lVal,strCode.Left(5),strCode.Right(5),strDeviceID.Right(5));
	strAuthCode.MakeUpper();

	CString strOldCode(_T(""));
	strSqlCmd.Format(_T("SELECT Code FROM [%s] WHERE DID='%s' AND CorpID=%ld"),\
		strTableName,strDeviceID,varCustomerID.lVal);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			bstrVal.Empty();
			spiSqlite3Connect->GetValueString(0,&bstrVal);
			strOldCode = bstrVal.m_str;
			bstrVal.Empty();
			break;
		}
	}
	strSqlCmd.Empty();
	COleDateTime curTime(COleDateTime::GetCurrentTime());
	if(strOldCode.GetLength())
	{
		/// 更新
		strSqlCmd.Format(_T("UPDATE [%s] SET Code=\'%s\',ATime=%f WHERE DID='%s' AND CorpID=%ld"),\
			strTableName,strAuthCode,curTime.m_dt,strDeviceID,varCustomerID.lVal);
	}
	else
	{
		if(nAuthNode > nUsedCount)
		{
			/// 没有超过授权节点数，插入
			strSqlCmd.Format(_T("INSERT INTO [%s] (DID,Code,CorpID,CorpName,ATime) \
								VALUES(\'%s\',\'%s\',%ld,\'%s\',%f);"),
								strTableName,strDeviceID,strAuthCode,varCustomerID.lVal,strCustomerName,curTime.m_dt);
		}
		else
		{
			hRet = E_FAIL;
			strSqlCmd.Empty();
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE2(_T("可用授权数已用完"),_T("Auth"),LOGTYPE_NORMAL);
#endif
		}

	}
	if(!strSqlCmd.IsEmpty())
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	if(SUCCEEDED(hRet) && !strSqlCmd.IsEmpty())
	{
		hRet = spiCryptoStor->WriteInFile(CComBSTR(strAuthFile),CComBSTR(TDHX_SQLITEDB_AUTHFILE),VARIANT_FALSE);
#ifndef _DEBUG
		/// 记录授权码到注册表
		CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE);
		regKey.SetRegStringVal(strDeviceID,strAuthCode);
#endif
#ifdef APP_LOG_ENABLE
		if(FAILED(hRet))
			WRITELOGTOFILE2(_T("写入授权信息数据库失败"),_T("Auth"),LOGTYPE_NORMAL);
#endif

	}
	spiCryptoStor->CloseDisk();
	spiCryptoStor = NULL;
	strSqlCmd.Empty();
	return hRet;
}

STDMETHODIMP CSafeService::GetLeftCount(ULONG* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
	{
		return E_POINTER;
	}
	(*pVal) = 0;
#ifndef TDHXKJ_VERSION_NOUSB
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
		InitSafeCom(3);
	if(NULL == m_spiSafeCard || NULL == m_spiCryptoStor)
	{
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("初始化对象失败"));
#endif
		return E_FAIL;
	}
	CComPtr <IJsonService> spiJsonService = GetAuthInfo();
	if(NULL == spiJsonService)
	{
		return E_FAIL;
	}
	CComBSTR bstrVal;
	CString strDBPW;
	spiJsonService->GetStringValue(CComBSTR(_T("DataPW")),&bstrVal);
	if(bstrVal.Length())
		strDBPW = bstrVal.m_str;
	bstrVal.Empty();
	ULONG nAuthNode = 0;
	spiJsonService->GetStringValue(CComBSTR(_T("AuthNode")),&bstrVal);
	if(bstrVal.Length())
	{
		nAuthNode = CBaseFuncLib::StrToNum(bstrVal.m_str);
	}
	bstrVal.Empty();
	CComVariant varCustomerID;
	spiJsonService->GetVariantValue(CComBSTR(_T("CustomerID")),&varCustomerID);
	varCustomerID.ChangeType(VT_I4);

	/// 获得授权相关的信息
	CComPtr<ICryptoStor> spiCryptoStor = NULL;
	HRESULT hRet = this->get_CryptoStor(&spiCryptoStor);
	if(NULL == spiCryptoStor)
	{
		spiJsonService = NULL;
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("初始化对象失败"));
#endif
		return hRet;
	}

	//安装程序时，设置安装盘符（用于解决A、B盘同时插入时获取安装信息失败的问题）
	if (m_bDiskNum)
	{
#ifndef TDHXKJ_VERSION_NOUSB
		//设置当前安装盘为有效U卡


		CComBSTR tempbstrDisk;

		tempbstrDisk.Append(m_cDiskNum);
		m_spiSafeCard->put_CurDisk(tempbstrDisk);

#endif
	}

	DWORD dwSize = 0;
	CComBSTR bstrDisk;
	m_spiSafeCard->get_CurDisk(&bstrDisk);
	spiCryptoStor->PutCurDisk(bstrDisk,&dwSize);

	CString strAuthFile(CBaseFuncLib::GetTmpPath(FALSE)+TDHX_SQLITEDB_AUTHFILE);
	::SetFileAttributes(strAuthFile,FILE_ATTRIBUTE_NORMAL);
	::DeleteFile(strAuthFile);
	/// 尝试先读取
	hRet = spiCryptoStor->ReadOnlyFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE),CComBSTR(strAuthFile));
	/// 尝试打开
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL != spiConnectHelper)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_FALSE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
#ifdef _DEBUG
		if(NULL == spiSqlite3Connect)
		{
			spiCryptoStor->DelFile(CComBSTR(TDHX_SQLITEDB_AUTHFILE));
			::DeleteFile(strAuthFile);
			/// 重新创建
			hRet = spiConnectHelper->OpenDB(CComBSTR(strAuthFile),VARIANT_TRUE,(short)15,CComBSTR(strDBPW),&spiSqlite3Connect);
		}
#endif
		if(NULL == spiSqlite3Connect)
		{
			CComBSTR bstrErrInfo;
			spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
		spiConnectHelper = NULL;
	}
	if(NULL == spiSqlite3Connect)
	{
		spiJsonService = NULL;
		spiCryptoStor->CloseDisk();
		spiCryptoStor = NULL;
		//解决授权后第一次安装时由于没有数据库文件而读取授权数失败的问题
		if (ERROR_FILE_NOT_FOUND == hRet)
		{
			(*pVal) = nAuthNode;
			if (nAuthNode > 0)
			{
				hRet = S_OK;
			}
		}

		return hRet;
	}
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT Code FROM [%s] WHERE CorpID=%ld"),\
		SQLITEDB_TABLE_AUTHLIST,varCustomerID.lVal);
	ULONG nUsedCount = CDbHelper::GetRecordCount(spiSqlite3Connect,strSqlCmd);
	if(nAuthNode >= nUsedCount)
		(*pVal) = nAuthNode-nUsedCount;
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	spiCryptoStor->CloseDisk();
	spiCryptoStor = NULL;
#endif
	return S_OK;
}

STDMETHODIMP CSafeService::GetUniqueID(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
		return E_POINTER;
	CComPtr <IWMIDevice> spiWmiDevice = NULL;
	CComPtr<IClassFactory>	cf = NULL;
	HRESULT hRet = DllGetClassObject( __uuidof(WMIDevice), __uuidof(IClassFactory), (LPVOID *)&cf );
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(IWMIDevice), (VOID **)&spiWmiDevice );
		cf = NULL;
		ATLASSERT(spiWmiDevice);
	}
#ifndef _DEBUG
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE);
#else
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
#endif
	if(NULL == spiWmiDevice)
	{
		CString strDeviceID;
		regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID);
		if(strDeviceID.GetLength())
		{
			CComBSTR bstrVal(strDeviceID);
			bstrVal.CopyTo(pVal);
			bstrVal.Empty();
			return S_OK;
		}
		return hRet;
	}
	SHORT nSize = 0;
	CString strBios(_T(""));
	/// 双因子登录调用会崩溃
	hRet = spiWmiDevice->Query(WMITYPE_BIOSID,CComBSTR(_T("Manufacturer")),&nSize);
	if(nSize)
	{
		CComBSTR bstrID,bstrName,bstrKeyValue;
		spiWmiDevice->GetName(0,&bstrName);
		hRet = spiWmiDevice->GetValue(0,&bstrID);
		hRet = spiWmiDevice->GetKeyValue(0,&bstrKeyValue);
		strBios.Format(_T("%s_%s"),bstrKeyValue.m_str,bstrID.m_str);
	}
	spiWmiDevice = NULL;
	if(strBios.IsEmpty())
	{
		/// 解决安全模式获取ID失败导致授权无效问题
		CString strDeviceID;
		regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID);
		if(strDeviceID.GetLength())
		{
			CComBSTR bstrVal(strDeviceID);
			bstrVal.CopyTo(pVal);
			bstrVal.Empty();
			return S_OK;
		}
#ifdef APP_LOG_ENABLE
		WRITELOGTOFILE(_T("获取设备标识信息失败"));
#endif
		return hRet;
	}

	CComBSTR bstrVal;
	this->StringSha1(CComBSTR(strBios),&bstrVal);
	this->StringMD5(bstrVal,&bstrVal);
	CString strDeviceID;
	regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strDeviceID);
	if(0 != strDeviceID.CompareNoCase(bstrVal.m_str))
		regKey.SetRegStringVal(PRODUCT_COMMAN_DEVICEID,bstrVal.m_str);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	strBios.Empty();
	return S_OK;
}



STDMETHODIMP CSafeService::SetDiskNum(BSTR bstrDiskNum,VARIANT_BOOL bFlag)
{
	if (bstrDiskNum == NULL)
	{
		return E_POINTER;
	}
	//bFlag为true时设置盘符，为false时清除盘符
	if (bFlag)
	{
		CString strTempDisk(bstrDiskNum);
		m_cDiskNum = strTempDisk.GetAt(0);
		m_bDiskNum = bFlag;
	}
	else
	{
		m_cDiskNum = _T('');
		m_bDiskNum = bFlag;
	}

	return S_OK;
}
