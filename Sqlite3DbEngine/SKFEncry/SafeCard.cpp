// SafeCard.cpp : CSafeCard 的实现

#include "stdafx.h"
#include "SafeCard.h"
#include "BaseFuncLib.h"

// CSafeCard

void CSafeCard::DelDevice()
{
	if(NULL == m_pSKF)
		return;
	/// 显式释放
	m_pSKF->UnInit();
	delete m_pSKF;
	m_pSKF = NULL;
}

STDMETHODIMP CSafeCard::put_CurDisk(BSTR cDisk)
{
	// TODO: 在此添加实现代码
	if(0 == cDisk)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSKF)
		return E_FAIL;
	CComBSTR bstrDisk;
	bstrDisk.Append(cDisk);
	if(!bstrDisk.Length())
		return E_FAIL;
	bool bRet = false;
	bRet = m_pSKF->PutCurDisk(bstrDisk.m_str[0]);
	/// 尝试3次
	int nIndex = 3;
	DWORD dwErrCode = m_pSKF->GetLastErrCode();
	while(!bRet && ERROR_ACCESS_DENIED == dwErrCode && nIndex > 0)
	{
		::Sleep(200);
		nIndex--;
		bRet = m_pSKF->PutCurDisk(bstrDisk.m_str[0]);
		dwErrCode = m_pSKF->GetLastErrCode();
	}
	bstrDisk.Empty();
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::get_CurDisk(BSTR* pcDisk)
{
	// TODO: 在此添加实现代码
	if(NULL == pcDisk)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_FAIL;
	CComBSTR bstrDisk;
	bstrDisk.Append(m_pSKF->GetCurDisk());
	bstrDisk.CopyTo(pcDisk);
	bstrDisk.Empty();
	return S_OK;
}

STDMETHODIMP CSafeCard::LockDev(VARIANT_BOOL bLockFlag)
{
	// TODO: 在此添加实现代码
	if(NULL == m_pSKF)
		return E_FAIL;
	bool bRet = m_pSKF->LockTFDev(bLockFlag);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::EnumCard(BSTR* pbstrDisks)
{
	// TODO: 在此添加实现代码
	if(NULL == pbstrDisks)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSKF)
		return E_POINTER;
	HRESULT hRet(S_OK);
	CString strDisks(_T(""));
	SHORT nCount = m_pSKF->GetDeviceCount(strDisks);
	if(!nCount)
	{
		LONG nErrCode = m_pSKF->GetLastErrCode();
		if(SAR_OK != nErrCode)
		{
#ifdef APP_LOG_ENABLE
			CString strErrInfo;
			strErrInfo.Format(_T("EnumCard失败:%ld"),nErrCode);
			WRITELOGTOFILE(strErrInfo);
#endif
			hRet = E_FAIL;
		}
	}
	CComBSTR bstrVal(strDisks);
	bstrVal.CopyTo(pbstrDisks);
	bstrVal.Empty();
	strDisks.Empty();
	return hRet;
}

STDMETHODIMP CSafeCard::GetCID(BSTR cDisk,BSTR* bstrCID)
{
	// TODO: 在此添加实现代码
	if(NULL == m_pSKF)
		return E_POINTER;
	CComBSTR bstrDisk;
	if(NULL != cDisk)
		bstrDisk.Append(cDisk);
	if(!bstrDisk.Length())
		return E_FAIL;
	CString strCID = m_pSKF->GetDeviceID(bstrDisk.m_str[0]);
	bstrDisk.Empty();
	if(!strCID.GetLength())
		return E_FAIL;
	CComBSTR bstrID(strCID);
	bstrID.CopyTo(bstrCID);
	bstrID.Empty();
	strCID.Empty();

	return S_OK;
}

STDMETHODIMP CSafeCard::CloseDev(void)
{
	// TODO: 在此添加实现代码
	if(NULL != m_pSKF)
		m_pSKF->CloseCurCard();
	return S_OK;
}

STDMETHODIMP CSafeCard::WaitForDevEvent(SHORT *pnEvent,BSTR* pbstrName)
{
	// TODO: 在此添加实现代码
	if(NULL== pnEvent || NULL == pbstrName)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	CString strDevName(_T(""));
	bool bRet = m_pSKF->WaitForDevEvent(pnEvent,strDevName);
	if(bRet)
	{
		CComBSTR bstrVal(strDevName);
		bstrVal.CopyTo(pbstrName);
		bstrVal.Empty();
		strDevName.Empty();
	}
	return S_OK;
}

STDMETHODIMP CSafeCard::CancelWaitForDevEvent()
{
	// TODO: 在此添加实现代码
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->CancelWaitForDevEvent();
	return S_OK;
}

STDMETHODIMP CSafeCard::GetDeviceInfo(IDispatch** ppDeviceInfo)
{
	// TODO: 在此添加实现代码
	if(NULL == ppDeviceInfo)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->GetDeviceInfo(ppDeviceInfo);
	return S_OK;
}

STDMETHODIMP CSafeCard::GetStatus(SHORT* psStatus)
{
	// TODO: 在此添加实现代码
	if(NULL == psStatus)
		return E_POINTER;
	(*psStatus) = m_pSKF->GetStatus();
	return S_OK;
}

STDMETHODIMP CSafeCard::ChangeDevAuth(unsigned char* szOldPW,unsigned char* szNewPW)
{
	// TODO: 在此添加实现代码
	if(NULL == szOldPW || NULL == szNewPW)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ChangeDevAuth(szOldPW,szNewPW);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DevAuth(unsigned char* szKey)
{
	// TODO: 在此添加实现代码
	if(NULL == szKey)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DevAuth(szKey);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::CreateApplication(BSTR bstrAppName,BSTR bstrAdminPin,\
	SHORT nAdminPinRetryCount,BSTR bstrUserPin,SHORT nUserPinRetryCount,EAappFileSecureType eCreateFileRight,ULONG* phApp)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrAppName || NULL == bstrAdminPin || NULL == bstrUserPin || NULL == phApp)
		return E_POINTER;
	if(!nAdminPinRetryCount || !nUserPinRetryCount)
		return E_INVALIDARG;
	if(NULL == m_pSKF)
		return E_POINTER;
	SHORT sAdminPinRetryCount = nAdminPinRetryCount;
	SHORT sUserPinRetryCount = nUserPinRetryCount;
	if(sAdminPinRetryCount > 0X10)
		sAdminPinRetryCount = 0X10;
	if(sUserPinRetryCount > 0X10)
		sUserPinRetryCount = 0X10;/// 保护代码
	ULONG hApp = (ULONG)m_pSKF->CreateApp(bstrAppName,bstrAdminPin,sAdminPinRetryCount,\
		bstrUserPin,sUserPinRetryCount,eCreateFileRight);
	if(NULL == hApp)
		return E_FAIL;
	(*phApp) = hApp;
	return S_OK;
}

STDMETHODIMP CSafeCard::EnumApplication(BSTR* pbstrApps)
{
	// TODO: 在此添加实现代码
	if(NULL == pbstrApps || NULL == m_pSKF)
		return E_POINTER;
	u32 nLength = 0;
	CString strApps(_T(""));
	bool bRet = m_pSKF->EnumApp(nLength,strApps);
	if(!bRet)
		return E_FAIL;
	if(0 == nLength)
		return S_OK;/// 还没有应用
	bRet = m_pSKF->EnumApp(nLength,strApps);
	if(!bRet)
		return E_FAIL;
	ATLASSERT(strApps.GetLength());
	CComBSTR bstrVal(strApps);
	bstrVal.CopyTo(pbstrApps);
	bstrVal.Empty();
	strApps.Empty();
	return S_OK;
}

STDMETHODIMP CSafeCard::DeleteApplication(BSTR bstrAppName)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrAppName)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DeleteApp(bstrAppName);
	if(!bRet)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CSafeCard::OpenApplication(BSTR bstrAppName,ULONG* phApp)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrAppName || NULL == phApp)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	(*phApp) = (ULONG)m_pSKF->OpenApp(bstrAppName);
	if(NULL == (*phApp))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ClearSecureState(ULONG hApp)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ClearSecureState((HANDLE )hApp);
	if(!bRet)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CSafeCard::CloseApplication(ULONG hApp)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->CloseApp((HANDLE )hApp);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ChangePIN(ULONG hApp,VARIANT_BOOL bAdminFlag,\
	BSTR bstrOldPin,BSTR bstrNewPin,SHORT* pnRetryCount)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrOldPin ||
		NULL == bstrNewPin || NULL == pnRetryCount)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ChangePin((HANDLE )hApp,bAdminFlag,bstrOldPin,bstrNewPin,*pnRetryCount);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::GetPINInfo(ULONG hApp,VARIANT_BOOL bAdminFlag,\
	SHORT* pnMaxRetryCount,SHORT* pnRemainRetryCount,VARIANT_BOOL* pbDefaultPin)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == pbDefaultPin ||
		NULL == pnMaxRetryCount || NULL == pnRemainRetryCount || NULL == pbDefaultPin)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->GetPinInfo((HANDLE )hApp,bAdminFlag,*pnMaxRetryCount,*pnRemainRetryCount,*pbDefaultPin);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::VerifyPIN(ULONG hApp,VARIANT_BOOL bAdminFlag,\
	BSTR bstrPin,SHORT* pnRetryCount)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrPin || NULL == pnRetryCount)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->VerifyPin((HANDLE )hApp,bAdminFlag,bstrPin,*pnRetryCount);
	if(!bRet)
	{
		CString strErrInfo;
		int nErrCode = m_pSKF->GetLastErrCode();
		if(nErrCode)
		{
			/// SAR_PIN_LOCKED用户PIN码被锁定
#ifdef APP_LOG_ENABLE
			strErrInfo.Format(_T("验证PIN码错误:%s，剩余次数:%ld 管理员:%d"),\
				m_pSKF->GetLastErrInfo(nErrCode),(*pnRetryCount),VARIANT_TRUE == bAdminFlag);
			/// 写日志
			WRITELOGTOFILE(strErrInfo);
			strErrInfo.Empty();
#endif
		}
		else
		{
#ifdef APP_LOG_ENABLE
			strErrInfo.Format(_T("验证PIN码未知错误，剩余次数:%ld 管理员:%d"),\
				(*pnRetryCount),VARIANT_TRUE == bAdminFlag);
			/// 写日志
			WRITELOGTOFILE(strErrInfo);
#endif
		}
		strErrInfo.Empty();
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CSafeCard::UnblockPIN(ULONG hApp,BSTR bstrAdminPin,BSTR bstrUserPin,SHORT* pnRetryCount)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrAdminPin 
		|| NULL == bstrUserPin || NULL == pnRetryCount)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->UnblockPin((HANDLE )hApp,bstrAdminPin,bstrUserPin,*pnRetryCount);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::CreateAppFile(ULONG hApp,BSTR bstrFileName,ULONG nFileSize,ULONG nReadRight,ULONG nWriteRight)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrFileName || 0 == nFileSize)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->CreateAppFile((HANDLE )hApp,bstrFileName,nFileSize,nReadRight,nWriteRight);
	if(!bRet)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CSafeCard::DeleteAppFile(ULONG hApp,BSTR bstrFileName)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrFileName)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DeleteAppFile((HANDLE )hApp,bstrFileName);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::EnumAppFile(ULONG hApp,BSTR* bstrFiles)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrFiles)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	CString strFiles;
	ULONG nLength = 0;
	bool bRet = m_pSKF->EnumAppFile((HANDLE )hApp,nLength,strFiles);
	if(!bRet)
		return E_FAIL;
	if(0 == nLength)
		return S_OK;/// 还没有文件
	bRet = m_pSKF->EnumAppFile((HANDLE )hApp,nLength,strFiles);
	if(!bRet)
		return E_FAIL;
	CComBSTR bstrVal(strFiles);
	bstrVal.CopyTo(bstrFiles);
	bstrVal.Empty();
	strFiles.Empty();
	return S_OK;
}

STDMETHODIMP CSafeCard::GetAppFileInfo(ULONG hApp,BSTR bstrFileName,IDispatch** pPara)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrFileName || NULL == pPara)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->GetFileInfo((HANDLE )hApp,bstrFileName,pPara);
	if(!bRet)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CSafeCard::ReadAppFile(ULONG hApp,BSTR bstrFileName,ULONG nOffset,SHORT* pnReadLen,BYTE* pbData)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrFileName || NULL == pbData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	u32 uBufLen = (u32)(*pnReadLen);
	bool bRet = m_pSKF->ReadAppFile((HANDLE )hApp,bstrFileName,nOffset,uBufLen,pbData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::WriteAppFile(ULONG hApp,BSTR bstrFileName,ULONG nOffset,BYTE* pbData,ULONG nSize)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrFileName || NULL == pbData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->WriteAppFile((HANDLE )hApp,bstrFileName,nOffset,pbData,nSize);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::CreateContainer(ULONG hApp,BSTR bstrContainerName,ULONG* phContainer)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrContainerName || NULL == phContainer)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	(*phContainer) = (ULONG)m_pSKF->CreateContainer((HANDLE )hApp,bstrContainerName);
	return S_OK;
}

STDMETHODIMP CSafeCard::DeleteContainer(ULONG hApp,BSTR bstrContainerName)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrContainerName)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DeleteContainer((HANDLE )hApp,bstrContainerName);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::OpenContainer(ULONG hApp,BSTR bstrContainerName,ULONG* phContainer)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == bstrContainerName)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	(*phContainer) = (ULONG)m_pSKF->OpenContainer((HANDLE )hApp,bstrContainerName);
	return S_OK;
}

STDMETHODIMP CSafeCard::CloseContainer(ULONG hContainer)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->CloseContainer((HANDLE )hContainer);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::EnumContainer(ULONG hApp,BSTR* pbstrNameList)
{
	// TODO: 在此添加实现代码
	if(NULL == hApp || NULL == pbstrNameList)
		return E_POINTER;
	u32 nLength = 0;
	CString strNames(_T(""));
	bool bRet = m_pSKF->EnumContainer((HANDLE )hApp,nLength,strNames);
	if(!bRet)
		return E_FAIL;
	if(0 == nLength)
		return S_OK;/// 还没有容器
	bRet = m_pSKF->EnumContainer((HANDLE )hApp,nLength,strNames);
	if(!bRet)
		return E_FAIL;
	CComBSTR bstrVal(strNames);
	bstrVal.CopyTo(pbstrNameList);
	bstrVal.Empty();
	strNames.Empty();
	return S_OK;
}

STDMETHODIMP CSafeCard::GetContainerType(ULONG hContainer,SHORT* psType)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == psType)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->GetContainerType((HANDLE )hContainer,*psType);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ImportCertificate(ULONG hContainer,VARIANT_BOOL bSign,BYTE* pbCert,SHORT nCertLen)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == nCertLen || NULL == pbCert)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ImportCertificate((HANDLE )hContainer,bSign,pbCert,nCertLen);
	if(!bRet)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CSafeCard::ExportCertificate(ULONG hContainer,VARIANT_BOOL bSign,SHORT* pnCertLen,BYTE* pbCert)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == pnCertLen)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	u32 nCertLen = (*pnCertLen);
	bool bRet = m_pSKF->ExportCertificate((HANDLE )hContainer,bSign,pbCert,nCertLen);
	if(!bRet)
		return E_FAIL;
	(*pnCertLen) = (SHORT )nCertLen;
	return S_OK;
}

STDMETHODIMP CSafeCard::GenRandom(SHORT nRandomLen,BSTR *pbstrRandom)
{
	// TODO: 在此添加实现代码
	if(NULL == pbstrRandom)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	CString strRandom(_T(""));
	bool bRet = m_pSKF->GenRandom(strRandom,nRandomLen);
	if(!bRet)
		return E_FAIL;
	CComBSTR bstrVal(strRandom);
	bstrVal.CopyTo(pbstrRandom);
	bstrVal.Empty();
	strRandom.Empty();
	return S_OK;
}

STDMETHODIMP CSafeCard::GenExtRSAKey(ULONG nBitsLen,ULONG* pRsaPrivateKey)
{
	// TODO: 在此添加实现代码
	if(NULL == pRsaPrivateKey || NULL == nBitsLen)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->GenExtRSAKey(nBitsLen,pRsaPrivateKey);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::GenRSAKeyPair(ULONG hContainer,ULONG nBitsLen,ULONG* pRsaPubKey)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == nBitsLen)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->GenRSAKeyPair((HANDLE )hContainer,nBitsLen,pRsaPubKey);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ImportRSAKeyPair(ULONG hContainer,ULONG nSymAlgId,BYTE* pbWrappedKey,\
	ULONG nWrappedKeyLen,BYTE* pbEncryptedData,ULONG nEncryptedDataLen)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == pbWrappedKey || !nEncryptedDataLen
		|| NULL == nWrappedKeyLen || NULL == pbEncryptedData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ImportRSAKeyPair((HANDLE )hContainer,nSymAlgId,pbWrappedKey,\
		nWrappedKeyLen,pbEncryptedData,nEncryptedDataLen);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::RSASignData(ULONG hContainer,SHORT nDataLen,BYTE* pbData,SHORT* pnSignature,BYTE *pbSignature)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || 0 == nDataLen || NULL == pbData 
		|| NULL == pnSignature || NULL == pbSignature)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->RSASignData((HANDLE )hContainer,(ULONG )nDataLen,pbData,\
		(ULONG* )pnSignature,pbSignature);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::RSAVerify(ULONG* pRsaPubKey,SHORT nDataLen,BYTE* pbData,SHORT nSignatureLen,BYTE* pbSignature)
{
	// TODO: 在此添加实现代码
	if(NULL == pRsaPubKey || !nDataLen || !nSignatureLen || NULL == pbSignature)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->RSAVerify(pRsaPubKey,nDataLen,pbData,\
		(ULONG )nSignatureLen,pbSignature);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::RSAExportSessionKey(ULONG hContainer,ULONG nAlgId,ULONG* pRsaPubKey,\
	SHORT* pnDataLen,BYTE* pbData,ULONG *phSessionKey)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == pRsaPubKey 
		|| NULL == pnDataLen || NULL == pbData || NULL == phSessionKey)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->RSAExportSessionKey((HANDLE )hContainer,nAlgId,pRsaPubKey,\
		(ULONG* )pnDataLen,pbData,(HANDLE* )phSessionKey);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ExtRSAPubKeyOperation(ULONG* pRsaPubKey,SHORT nInputLen,BYTE* pbInputData,SHORT* pnOutput,BYTE *pbOutput)
{
	// TODO: 在此添加实现代码
	if(NULL == nInputLen || NULL == pRsaPubKey || 
		NULL == pbInputData || NULL == pnOutput || NULL == pbOutput)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ExtRSAPubKeyOperation(pRsaPubKey,nInputLen,pbInputData,\
		(ULONG* )pnOutput,pbOutput);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ExtRSAPriKeyOperation(ULONG* pRsaPriKey,SHORT nInputLen,BYTE* pbInputData,SHORT* pnOutput,BYTE *pbOutput)
{
	// TODO: 在此添加实现代码
	if(NULL == nInputLen || NULL == pRsaPriKey)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ExtRSAPriKeyOperation(pRsaPriKey,nInputLen,pbInputData,\
		(ULONG* )pnOutput,pbOutput);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ExportPublicKey(ULONG hContainer,VARIANT_BOOL bSign,SHORT* pnBlobLen,ULONG* pbBlob)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || NULL == pnBlobLen)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ExportPublicKey((HANDLE )hContainer,bSign,(ULONG* )pnBlobLen,pbBlob);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::ImportSessionKey(ULONG hContainer,ULONG nAlgId,ULONG nWrapedLen,BYTE* pbWrapedData,ULONG *phSessionKey)
{
	// TODO: 在此添加实现代码
	if(NULL == hContainer || 0 == nWrapedLen)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->ImportSessionKey((HANDLE )hContainer,nAlgId,nWrapedLen,pbWrapedData,(HANDLE* )phSessionKey);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::SetSymmKey(ULONG nAlgId,BYTE* pbKey,ULONG *phSessionKey)
{
	// TODO: 在此添加实现代码
	if(NULL == phSessionKey || 0 == pbKey)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->SetSymmKey(nAlgId,pbKey,(HANDLE *)phSessionKey);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::EncryptInit(ULONG hKey)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->EncryptOrDecryptInit((HANDLE )hKey,TRUE);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::Encrypt(ULONG hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey || NULL == pbData || NULL == pnEncryptedDataLen || NULL == pbEncryptedData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->Encrypt((HANDLE )hKey,nDataLen,pbData,pnEncryptedDataLen,pbEncryptedData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::EncryptUpdate(ULONG hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey || NULL == nDataLen || NULL == pnEncryptedDataLen || NULL == pbEncryptedData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->EncryptUpdate((HANDLE )hKey,nDataLen,pbData,pnEncryptedDataLen,pbEncryptedData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::EncryptFinal(ULONG hKey,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey || NULL == pnEncryptedDataLen || NULL == pbEncryptedData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->EncryptOrDecryptFinal((HANDLE )hKey,pnEncryptedDataLen,pbEncryptedData,TRUE);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DecryptInit(ULONG hKey)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->EncryptOrDecryptInit((HANDLE )hKey,FALSE);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::Decrypt(ULONG hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey || NULL == pbData || NULL == nEncryptedDataLen 
		|| NULL == pbEncryptedData || NULL == pnDataLen || NULL == pbData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->Decrypt((HANDLE )hKey,nEncryptedDataLen,pbEncryptedData,pnDataLen,pbData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DecryptUpdate(ULONG hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey || NULL == nEncryptedDataLen 
		|| NULL == pbEncryptedData || NULL == pnDataLen || NULL == pbData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DecryptUpdate((HANDLE )hKey,nEncryptedDataLen,pbEncryptedData,pnDataLen,pbData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DecryptFinal(ULONG hKey,ULONG* pnDecryptedDataLen,BYTE* pbDecryptedData)
{
	// TODO: 在此添加实现代码
	if(NULL == hKey || NULL == pnDecryptedDataLen || NULL == pbDecryptedData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->EncryptOrDecryptFinal((HANDLE )hKey,pnDecryptedDataLen,pbDecryptedData,FALSE);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DigestInit(ULONG ulAlgID,ULONG* pPubKey,SHORT nIDLen,unsigned char* pucID,ULONG* phHash)
{
	// TODO: 在此添加实现代码
	if(NULL == pPubKey || !nIDLen || NULL == pucID || NULL == phHash)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DigestInit(ulAlgID,pPubKey,nIDLen,pucID,(HANDLE* )phHash);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::Digest(ULONG hHash,SHORT ulDataLen,BYTE* pbData,SHORT* pnHashLen,BYTE* pbHashData)
{
	// TODO: 在此添加实现代码
	if(NULL == hHash || !ulDataLen || NULL == pbData || NULL == pnHashLen || NULL == pbHashData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->Digest((HANDLE )hHash,ulDataLen,pbData,(ULONG* )pnHashLen,pbHashData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DigestUpdate(ULONG hHash,SHORT ulDataLen,BYTE* pbData)
{
	// TODO: 在此添加实现代码
	if(NULL == hHash || !ulDataLen || NULL == pbData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DigestUpdate((HANDLE )hHash,ulDataLen,pbData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::DigestFinal(ULONG hHash,SHORT* pnHashLen,BYTE* pbHashData)
{
	// TODO: 在此添加实现代码
	if(NULL == hHash || !pnHashLen || NULL == pbHashData)
		return E_POINTER;
	if(NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->DigestFinal((HANDLE )hHash,(ULONG* )pnHashLen,pbHashData);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::CloseHandle(ULONG hHandle)
{
	// TODO: 在此添加实现代码
	if(NULL == hHandle || NULL == m_pSKF)
		return E_POINTER;
	bool bRet = m_pSKF->CloseHandle((HANDLE)hHandle);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSafeCard::get_SKFType(SHORT* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal || NULL == m_pSKF)
		return E_POINTER;
	(*pVal) = 1;
	return S_OK;
}

STDMETHODIMP CSafeCard::get_LastErrCode(LONG* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal || NULL == m_pSKF)
		return E_POINTER;
	(*pVal) = m_pSKF->GetLastErrCode();
	return S_OK;
}

STDMETHODIMP CSafeCard::get_LastErrInfo(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
		return E_POINTER;
	CString strLastErr;
	if(NULL == m_pSKF)
		strLastErr = _T("初始化错误");
	else
		strLastErr = CSKFBase::GetLastErrInfo(m_pSKF->GetLastErrCode());
	CComBSTR bstrVal(strLastErr);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	strLastErr.Empty();
	return S_OK;
}