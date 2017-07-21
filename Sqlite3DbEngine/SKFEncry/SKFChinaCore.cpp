#include "StdAfx.h"
#include "SKFChinaCore.h"
#include "BaseFuncLib.h"
#include "SqliteHelper.h"

/// 全局句柄
extern HANDLE g_hInstance;

/// 函数指针定义
typedef  ULONG(DEVAPI *PSKF_GetFuncList)(OUT PSKF_FUNCLIST* pFuncList);

CSKFChinaCore::CSKFChinaCore(void)
	:m_pFuncList(NULL)
	,m_pBlockCipherParam(NULL)
{
}

CSKFChinaCore::~CSKFChinaCore(void)
{
}

bool CSKFChinaCore::InitSKF()
{
	BOOL bLoadFlag = FALSE;
	if(NULL != m_hLib)
		bLoadFlag = TRUE;
	if(bLoadFlag)
	{
		/// 已经载入模块
		if(NULL == m_pFuncList)
		{
			/// 获取函数指针
			PSKF_GetFuncList GetFuncList = NULL;
			GetFuncList = (PSKF_GetFuncList)GetProcAddress((HMODULE )m_hLib,"SKF_GetFuncList");
			if(NULL != GetFuncList)
			{
				ULONG ret = GetFuncList(&m_pFuncList);
				GetFuncList = NULL;
			}
		}
		if(NULL != m_pFuncList)
		{
			m_sGlobalCriticalSection.Lock();
			m_nLoadRef++;
			m_sGlobalCriticalSection.Unlock();
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(_T("添加SKF引用次数"));
#endif
			return true;
		}
		/// 载入支持库失败
		m_strLastErrInfo = _T("获取函数地址失败");
		return false;
	}
	m_sGlobalCriticalSection.Lock();
	/// 载入安全卡支持库
	TCHAR szPath[MAX_PATH];
	memset(szPath,0,MAX_PATH*sizeof(TCHAR));
	::GetModuleFileName((HMODULE )g_hInstance,szPath,MAX_PATH);
	CString strModulePath(szPath);
	int nFind = strModulePath.ReverseFind(_T('\\'));
	CString	strDllPath = strModulePath.Left(nFind+1);
	/// 判断执行路径是否存在支持库
	if(!CBaseFuncLib::IsPathExist(strDllPath+TDHXKJ_SKFSUPPORTDLL))
	{
		/// 采用程序安全路径
		strDllPath.Empty();
		strDllPath = CComHelper::GetAppInsPath();
	}
	HMODULE handle = ::LoadLibrary(strDllPath+TDHXKJ_SKFSUPPORTDLL);
	if(NULL == handle)
	{
		/// 断言语句，输出运行错误日志
		m_nLastErrCode = ::GetLastError();
		m_sGlobalCriticalSection.Unlock();
		m_strLastErrInfo = _T("没有找到SKF支持库");
		return false;
	}
	/// 记录动态库句柄
	m_hLib = (HANDLE )handle;
	PSKF_GetFuncList GetFuncList = NULL;
	GetFuncList = (PSKF_GetFuncList)GetProcAddress(handle,"SKF_GetFuncList");
	if(NULL != GetFuncList)
	{
		ULONG ret = GetFuncList(&m_pFuncList);
		if(NULL != m_pFuncList)
		{
			GetFuncList = NULL;
			/// 添加应用计数，避免被非法卸载
			m_nLoadRef++;
			m_sGlobalCriticalSection.Unlock();
			return true;
		}
		GetFuncList = NULL;
	}
	/// 错误DLL，输出运行错误日志
	FreeLibrary((HMODULE )m_hLib);
	m_hLib = NULL;
	m_sGlobalCriticalSection.Unlock();
	m_strLastErrInfo = _T("获取SKF函数列表失败");
	return false;
}

SHORT CSKFChinaCore::GetStatus()
{
	ResetErrInfo();
	if(NULL == m_pFuncList)
		return m_nDeviceState;
	u32 nStatus = 0;
	m_nLastErrCode = m_pFuncList->SKF_GetDevState((LPSTR)&m_cCurDisk,&nStatus);
	if(SAR_OK == m_nLastErrCode)
		m_nDeviceState = nStatus;
	return (SHORT)m_nDeviceState;
}

bool CSKFChinaCore::CloseCurCard()
{
	ResetErrInfo();
	if(NULL == m_pFuncList || NULL == m_hDev)
		return true;
	if(true == m_bLockDev)
	{
		/// 先解除锁定
		m_pFuncList->SKF_UnlockDev(m_hDev);
		m_bLockDev = false;
	}
	bool bRet = false;
	m_nLastErrCode = m_pFuncList->SKF_DisConnectDev(m_hDev);
	if (SAR_OK == m_nLastErrCode)
		bRet = true;
	m_hDev = NULL;
	m_cCurDisk = 0;
	if(NULL != m_pBlockCipherParam)
	{
		delete m_pBlockCipherParam;
		m_pBlockCipherParam = NULL;
	}
	return bRet;
}

void CSKFChinaCore::UnInit()
{
	/// 资源释放
	CloseCurCard();

	///释放函数指针
	m_pFuncList = NULL;
	__super::UnInit();
}

SHORT CSKFChinaCore::GetDeviceCount(CString& strCIDS)
{
	ResetErrInfo();
	if(m_mapCard.size())
	{
		CSTRING_MAP::iterator it = m_mapCard.begin();
		while(it != m_mapCard.end())
		{
			if(strCIDS.GetLength())
				strCIDS += _T(";");
			strCIDS += it->first;
			it++;
		}
		return (SHORT)m_mapCard.size();
	}

	SHORT sDeviceCount = 0;
	if (NULL == m_pFuncList)
	{
		/// 还没有完成初始化
		if (!InitSKF())
		{
#ifdef APP_LOG_ENABLE
			if(m_strLastErrInfo.GetLength())
				WRITELOGTOFILE(m_strLastErrInfo);
#endif
			return sDeviceCount;
		}
		/// 初始化失败
		if (NULL == m_pFuncList)
			return sDeviceCount;
	}
	/// 先扫描可能的安全卡
	u32 nLen = 0;
#ifdef APP_LOG_ENABLE
	/// 写日志
	//	WRITELOGTOFILE(_T("SKF_EnumDev Len Begin"));
#endif
	m_nLastErrCode = m_pFuncList->SKF_EnumDev(TRUE, NULL, &nLen);
#ifdef APP_LOG_ENABLE
	/// 写日志
	//	WRITELOGTOFILE(_T("SKF_EnumDev Len OK"));
#endif
	if (0 == nLen)
	{
#ifdef APP_LOG_ENABLE
		/// 写日志
		if(SAR_OK != m_nLastErrCode)
		{
			CString strErrInfo = CSKFBase::GetLastErrInfo(m_nLastErrCode);
			WRITELOGTOFILE(strErrInfo);
			strErrInfo.Empty();
		}
#endif
		return sDeviceCount;
	}
	nLen++;
	char *szListName = (char *)malloc(nLen);
	if (NULL == szListName)
	{
		m_nLastErrCode = SAR_MEMORYERR;
		return sDeviceCount;
	}
	memset(szListName,0,nLen*sizeof(char));
	m_nLastErrCode = m_pFuncList->SKF_EnumDev(TRUE,szListName,&nLen);
	if (SAR_OK == m_nLastErrCode)
	{
		u32 nIndex = 0;
		for(nIndex = 0;nIndex<nLen;nIndex++)
		{
			if(szListName[nIndex] == 0 || szListName[nIndex] == NULL)
				continue;
			if(strCIDS.GetLength())
				strCIDS += _T(";");
			strCIDS += szListName[nIndex];
			sDeviceCount++;
		}
	}
	::free(szListName);
	szListName = NULL;
	return sDeviceCount;
}

bool CSKFChinaCore::PutCurDisk(WCHAR cDisk)
{
	ResetErrInfo();
	bool bRet = FALSE;
	if(cDisk == m_cCurDisk)
	{
		if(NULL != m_hDev)
			return true;/// 已经连接
	}
	if(NULL != m_hDev && cDisk != m_cCurDisk)
		this->CloseCurCard();
	if(NULL == m_pFuncList)
	{
		m_nLastErrCode = ERROR_FILE_NOT_FOUND;
		return bRet;
	}
	m_hDev = NULL;
	/// 连接设备
	m_hDev = ConnectCard(cDisk);
	if(NULL != m_hDev)
	{
		bRet = true;
		m_cCurDisk = cDisk;
	}

	return bRet;
}

bool CSKFChinaCore::WaitForDevEvent(SHORT *pnEvent,CString& strName)
{
	ResetErrInfo();
	if(NULL == pnEvent)
		return false;
	if(NULL == m_pFuncList)
		return false;
	char szDevName[4] = {0};
	u32 nNameLen = 4,nEvent = 0;
	m_nLastErrCode = m_pFuncList->SKF_WaitForDevEvent((LPSTR)(&szDevName),&nNameLen,&nEvent);
	if(SAR_OK == m_nLastErrCode)
	{
		(*pnEvent) = (SHORT )nEvent;
		strName = szDevName;
		return true;
	}
	return false;
}

bool CSKFChinaCore::CancelWaitForDevEvent()
{
	ResetErrInfo();
	if(NULL == m_pFuncList)
		return false;
	char szDevName[2] = {0};
	m_nLastErrCode = m_pFuncList->SKF_CancelWaitForDevEvent();
	if(SAR_OK == m_nLastErrCode)
		return true;
	return false;
}

bool CSKFChinaCore::LockTFDev(VARIANT_BOOL bLockFlag)
{
	ResetErrInfo();
	if(NULL == m_hDev || NULL == m_pFuncList)
		return false;
	if(true == m_bLockDev && VARIANT_TRUE == bLockFlag)
		return true;/// 已经锁定
	if(VARIANT_TRUE == bLockFlag)
	{
		m_nLastErrCode = m_pFuncList->SKF_LockDev(m_hDev,-1);
		if(SAR_OK == m_nLastErrCode)
			m_bLockDev = true;
	}
	else
	{
		m_nLastErrCode = m_pFuncList->SKF_UnlockDev(m_hDev);
		if(SAR_OK == m_nLastErrCode)
			m_bLockDev = false;
	}

	return true;
}

HANDLE CSKFChinaCore::ConnectCard(WCHAR cDisk)
{
	ResetErrInfo();
	/// 连接设备
	HANDLE hDev = NULL;
	if (NULL == m_pFuncList)
	{
		/// 断言 输出错误日志
		return hDev;
	}
	if (0 == cDisk)
	{
		/// 盘符错误
		return hDev;
	}
	try
	{
		m_nLastErrCode = m_pFuncList->SKF_ConnectDev((LPSTR)&cDisk, &hDev);
	}
	catch( ... )
	{
	}
	return hDev;
}

CString CSKFChinaCore::GetDID(HANDLE hDev)
{
	ResetErrInfo();
	CString strDeviceID(_T(""));
	if(NULL == m_pFuncList || NULL == hDev)
		return strDeviceID;
	DEVINFO info;
	memset(&info,0,sizeof(DEVINFO));
	m_nLastErrCode = m_pFuncList->SKF_GetDevInfo(hDev,&info);
	if (SAR_OK != m_nLastErrCode)
	{
		/// 获取失败信息
		return strDeviceID;
	}
	CString strManufacturer(info.Manufacturer);
	if(-1 != strManufacturer.Find(_T("C*CORE")))
		strDeviceID = info.SerialNumber;///确认是国芯的TF卡
	return strDeviceID;
}

CString CSKFChinaCore::GetDeviceID(WCHAR cDisk)
{
	ResetErrInfo();
	CString strDeviceID(_T(""));
	WCHAR cFindID = cDisk;
	if(0 == cFindID)
		cFindID = m_cCurDisk;/// 获得当前虚拟的ID
	CString strDisk(_T(""));
	strDisk.AppendChar(cFindID);
	CSTRING_MAP::iterator it = m_mapCard.find(strDisk);
	if(it != m_mapCard.end())
		strDeviceID = it->second;
	if(strDeviceID.GetLength())
		return strDeviceID;
	HANDLE hDev = NULL;
	if(cDisk != m_cCurDisk || NULL == m_hDev)
	{
		/// 尝试连接设备获取
		hDev = ConnectCard(cFindID);
		if(NULL == hDev)
			return strDeviceID;
		if(0 == m_cCurDisk || m_cCurDisk == cFindID)
		{
			m_cCurDisk = cFindID;
			m_hDev = hDev;/// 记录为当前设备ID
		}
	}
	else
		hDev = m_hDev;
	strDeviceID = GetDID(hDev);
	if(m_hDev != hDev)
	{
		/// 连接其他设备，马上关闭端口
		int nRet = m_pFuncList->SKF_DisConnectDev(hDev);
		if (SAR_OK != nRet)
		{
			/// 关闭设备失败，写日志
		}
	}
	if(strDeviceID.GetLength())
		m_mapCard[strDisk] = strDeviceID;/// 缓存盘符和设备ID的映射
	return strDeviceID;
}

bool CSKFChinaCore::GetDeviceInfo(IDispatch** pPara)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == pPara)
		return bRet;
	if(NULL == m_hDev)
		return bRet;
	u8 rand[TDHXKJ_SKF_AUTHKEYLEN] = {0}, authdata[TDHXKJ_SKF_AUTHKEYLEN*2] = {0};
	u32 nKey = TDHXKJ_SKF_AUTHKEYLEN;
	char ss[12] = {0};
	char* szAlgo = NULL;
	DEVINFO info;
	memset(&info,0,sizeof(DEVINFO));
	m_nLastErrCode = m_pFuncList->SKF_GetDevInfo(m_hDev,&info);
	if(SAR_OK != m_nLastErrCode)
		return bRet;
	/// 创建参数对象
	bRet = true;
	CComPtr <IParaService> spiParaService = CDbHelper::GetParaService();
	if(NULL != spiParaService)
	{
		spiParaService->AddNumber(_T("AuthAlgId"),info.DevAuthAlgId);
		spiParaService->AddNumber(_T("TotalSpace"),info.TotalSpace);
		spiParaService->AddNumber(_T("FreeSpace"),info.FreeSpace);
		spiParaService->AddNumber(_T("AlgSymCap"),info.AlgSymCap);
		spiParaService->AddNumber(_T("AlgAsymCap"),info.AlgAsymCap);
		spiParaService->AddNumber(_T("AlgHashCap"),info.AlgHashCap);
		spiParaService->AddString(_T("Manufacturer"),CComBSTR(info.Manufacturer));
		spiParaService->AddString(_T("Label"),CComBSTR(info.Label));
		spiParaService->AddString(_T("SerialNumber"),CComBSTR(info.SerialNumber));
		spiParaService->AddString(_T("Issuer"),CComBSTR(info.Issuer));
		spiParaService->QueryInterface(IID_IDispatch, (void**)pPara);
		spiParaService = NULL;
	}
	return bRet;
}

bool CSKFChinaCore::DevAuth(unsigned char* szKey)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == szKey)
		return bRet;
	if(NULL == m_hDev)
		return bRet;
	u8 rand[TDHXKJ_SKF_AUTHKEYLEN] = {0}, authdata[TDHXKJ_SKF_AUTHKEYLEN*2] = {0};
	u32 nKey = TDHXKJ_SKF_AUTHKEYLEN;
	char ss[12] = {0};
	char* szAlgo = NULL;
	DEVINFO info;
	memset(&info,0,sizeof(DEVINFO));
	m_nLastErrCode = m_pFuncList->SKF_GetDevInfo(m_hDev,&info);
	if(SAR_OK != m_nLastErrCode)
	{
		return bRet;
	}
	switch(info.DevAuthAlgId)
	{
	case SGD_SM1_ECB:
		szAlgo = "SGD_SM1_ECB";
		break;
	case SGD_SMS4_ECB:
		szAlgo = "SGD_SMS4_ECB";
		break;
	default:
		szAlgo = ss;
	}
	if(info.DevAuthAlgId != SGD_SM1_ECB && info.DevAuthAlgId != SGD_SMS4_ECB)
	{
		/// 设备错误
		m_nLastErrCode = SAR_INVALIDPARAMERR;
		return bRet;
	}
	HANDLE hKey = NULL;
	BLOCKCIPHERPARAM bp;

	bp.IVLen = 0;
	bp.PaddingType = NO_PADDING;
	bp.FeedBitLen = 0;

	m_nLastErrCode = m_pFuncList->SKF_SetSymmKey(m_hDev, (u8*)szKey, SGD_SM1_ECB, &hKey);
	if(SAR_OK != m_nLastErrCode)
	{
		m_pFuncList->SKF_CloseHandle(hKey);
		return bRet;
	}
	try
	{
		/// 卡和支持库不匹配的话，可能死机
		m_nLastErrCode = m_pFuncList->SKF_EncryptInit(hKey, bp);
	}
	catch ( ... )
	{
	}
	if(SAR_OK != m_nLastErrCode)
	{
		m_pFuncList->SKF_CloseHandle(hKey);
		return bRet;
	}

	m_nLastErrCode = m_pFuncList->SKF_GenRandom(m_hDev, rand, 8);
	if(SAR_OK != m_nLastErrCode)
	{
		m_pFuncList->SKF_CloseHandle(hKey);
		return bRet;
	}

	m_nLastErrCode = m_pFuncList->SKF_Encrypt(hKey, rand, TDHXKJ_SKF_AUTHKEYLEN, authdata, &nKey);
	m_pFuncList->SKF_CloseHandle(hKey);
	if(SAR_OK != m_nLastErrCode) 
	{
		return bRet;
	}
	/// 调用设备认证
	m_nLastErrCode = m_pFuncList->SKF_DevAuth(m_hDev,authdata,nKey);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ChangeDevAuth(unsigned char* szOldPW,unsigned char* szNewPW)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	m_pFuncList->SKF_LockDev(m_hDev,-1);
	bRet = DevAuth(szOldPW);
	if(!bRet) 
	{
		m_pFuncList->SKF_UnlockDev(m_hDev);
		return bRet;
	}
	bRet = false;
	m_nLastErrCode = m_pFuncList->SKF_ChangeDevAuthKey(m_hDev,(u8*)szNewPW,(u32)strlen((const char *)szNewPW));
	m_pFuncList->SKF_UnlockDev(m_hDev);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

HANDLE CSKFChinaCore::CreateApp(const CString& strAppName,const CString& strAdminPin,SHORT nAdminPinRetryCount,\
	const CString& strUserPin,SHORT nUserPinRetryCount,ULONG nCreateFileRight)
{
	ResetErrInfo();
	HANDLE hApp = NULL;
	if(NULL == m_pFuncList)
		return hApp;
	char *szAppName = NULL,*szAdminPin = NULL,*szUserPin = NULL;
	CBaseFuncLib::Us2ToChar(strAppName,&szAppName);
	CBaseFuncLib::Us2ToChar(strAdminPin,&szAdminPin);
	CBaseFuncLib::Us2ToChar(strUserPin,&szUserPin);
	HAPPLICATION hApplication = NULL;
	if(NULL != szAppName && NULL != szAdminPin && NULL != szUserPin)
	{
		m_nLastErrCode = m_pFuncList->SKF_CreateApplication(m_hDev,szAppName,szAdminPin,\
			nAdminPinRetryCount,szUserPin,nUserPinRetryCount,nCreateFileRight,&hApplication);
		delete []szAppName;
		szAppName = NULL;
		delete []szAdminPin;
		szAdminPin = NULL;
		delete []szUserPin;
		szUserPin = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		/// 获得句柄
		hApp = hApplication;
	}

	return hApp;
}

bool CSKFChinaCore::EnumApp(u32& sCount,CString& strApps)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	LPSTR szBuf = NULL;
	if(0 != sCount)
		szBuf = new CHAR[sCount];
	m_nLastErrCode = m_pFuncList->SKF_EnumApplication(m_hDev,szBuf,&sCount);
	if(SAR_OK == m_nLastErrCode)
	{
		if(NULL != szBuf)
		{
			BOOL bNull = FALSE;
			for(u32 nIndex = 0;nIndex < sCount;nIndex++)
			{
				if(NULL == szBuf[nIndex])
				{
					bNull = TRUE;
					continue;
				}
				if(bNull && strApps.GetLength())
					strApps += _T(";");
				strApps += szBuf[nIndex];
				bNull = NULL;
			}
		}
		bRet = true;
	}
	if(NULL != szBuf)
	{
		delete []szBuf;
		szBuf = NULL;
	}
	return bRet;
}

HANDLE CSKFChinaCore::OpenApp(const CString& strAppName)
{
	ResetErrInfo();
	HANDLE hApp = NULL;
	char *szAppName = NULL;
	HAPPLICATION hApplication = NULL;

	CBaseFuncLib::Us2ToChar(strAppName,&szAppName);
	if(NULL != szAppName)
	{
		m_nLastErrCode = m_pFuncList->SKF_OpenApplication(m_hDev,szAppName,&hApplication);
		delete []szAppName;
		szAppName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
		hApp = hApplication;

	return hApp;
}

bool CSKFChinaCore::DeleteApp(const CString& strAppName)
{
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	char *szAppName = NULL;
	CBaseFuncLib::Us2ToChar(strAppName,&szAppName);
	if(NULL != szAppName)
	{
		m_nLastErrCode = m_pFuncList->SKF_DeleteApplication(m_hDev,szAppName);
		delete []szAppName;
		szAppName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ClearSecureState(HANDLE hApp)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	/// 清除用户验证通过状态
	m_nLastErrCode = m_pFuncList->SKF_ClearSecureState(hApp);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::CloseApp(HANDLE hApp)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_CloseApplication(hApp);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ChangePin(HANDLE hApp,VARIANT_BOOL bAdminFlag,\
	const CString& strOldPin,const CString& strNewPin,SHORT& pnRetryCount)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	char *szAppName = NULL,*szOldPin = NULL,*szNewPin = NULL;
	CBaseFuncLib::Us2ToChar(strOldPin,&szOldPin);
	CBaseFuncLib::Us2ToChar(strNewPin,&szNewPin);
	u32 nTryCount = 0;
	if(NULL != szOldPin && NULL != szNewPin)
	{
		u32 ulPinType = ADMIN_TYPE;
		if(VARIANT_FALSE == bAdminFlag)
			ulPinType = USER_TYPE;
		m_nLastErrCode = m_pFuncList->SKF_ChangePIN(hApp,ulPinType,szOldPin,szNewPin,&nTryCount);
		delete []szOldPin;
		szOldPin = NULL;
		delete []szNewPin;
		szNewPin = NULL;
	}
	pnRetryCount = (SHORT)nTryCount;
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::GetPinInfo(HANDLE hApp,VARIANT_BOOL bAdminFlag,\
	SHORT& nMaxRetryCount,SHORT& nRemainRetryCount,VARIANT_BOOL& bDefaultPin)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	u32 nPinType = ADMIN_TYPE,nMaxPinCount = 0,nLeftCount = 0;
	BOOL bDefault = FALSE;
	bDefaultPin = VARIANT_FALSE;
	if(VARIANT_FALSE == bAdminFlag)
		nPinType = USER_TYPE;
	m_nLastErrCode = m_pFuncList->SKF_GetPINInfo(hApp,nPinType,&nMaxPinCount,&nLeftCount,&bDefault);
	if(SAR_OK == m_nLastErrCode)
	{
		if(bDefault)
			bDefaultPin = VARIANT_TRUE;
		nMaxRetryCount = (SHORT )nMaxPinCount;
		nRemainRetryCount = (SHORT )nLeftCount;
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::VerifyPin(HANDLE hApp,VARIANT_BOOL bAdminFlag,const CString& strPin,SHORT& nRetryCount)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	u32 nPinType = ADMIN_TYPE,nTry = 0;
	BOOL bDefault = FALSE;
	if(VARIANT_FALSE == bAdminFlag)
		nPinType = USER_TYPE;
	char *szPin = NULL;
	CBaseFuncLib::Us2ToChar(strPin,&szPin);
	if(NULL != szPin)
	{
		m_nLastErrCode = m_pFuncList->SKF_VerifyPIN(hApp,nPinType,szPin,&nTry);
		delete []szPin;
		szPin = NULL;
	}
	nRetryCount = (SHORT )nTry;
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::UnblockPin(HANDLE hApp,const CString& strAdminPin,const CString& strUserPin,SHORT& nRetryCount)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	u32 nTry = 0;
	char *szAdminPin = NULL,*szUserPin = NULL;
	CBaseFuncLib::Us2ToChar(strAdminPin,&szAdminPin);
	CBaseFuncLib::Us2ToChar(strUserPin,&szUserPin);
	if(NULL != szAdminPin && NULL != szUserPin)
	{
		m_nLastErrCode = m_pFuncList->SKF_UnblockPIN(hApp,szAdminPin,szUserPin,&nTry);
		delete []szAdminPin;
		szAdminPin = NULL;
		delete []szUserPin;
		szUserPin = NULL;
	}
	nRetryCount = (SHORT )nTry;
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::CreateAppFile(HANDLE hApp,const CString& strFileName,\
	ULONG nFileSize,ULONG nReadRight,ULONG nWriteRight)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	char *szFileName = NULL;
	CBaseFuncLib::Us2ToChar(strFileName,&szFileName);
	if(NULL != szFileName)
	{
		m_nLastErrCode = m_pFuncList->SKF_CreateFile(hApp,szFileName,nFileSize,nReadRight,nWriteRight);
		delete []szFileName;
		szFileName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::WriteAppFile(HANDLE hApp,const CString& strFileName,ULONG nOffset,BYTE* pbData,ULONG nSize)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	char *szFileName = NULL;
	CBaseFuncLib::Us2ToChar(strFileName,&szFileName);
	if(NULL != szFileName)
	{
		m_nLastErrCode = m_pFuncList->SKF_WriteFile(hApp,szFileName,nOffset,pbData,nSize);
		delete []szFileName;
		szFileName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::ReadAppFile(HANDLE hApp,const CString& strFileName,ULONG nOffset,u32& uBufLen,BYTE* pBuf)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == pBuf)
		return bRet;
	u32 nOutLen = uBufLen;
	char *szFileName = NULL;
	CBaseFuncLib::Us2ToChar(strFileName,&szFileName);
	if(NULL != szFileName)
	{
		m_nLastErrCode = m_pFuncList->SKF_ReadFile(hApp,szFileName,nOffset,uBufLen,pBuf,&nOutLen);
		delete []szFileName;
		szFileName = NULL;
	}
	if(SAR_OK == m_nLastErrCode || nOutLen > 0)
	{
		uBufLen = nOutLen;
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::DeleteAppFile(HANDLE hApp,const CString& strFileName)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	u32 nTry = 0;
	char *szFileName = NULL;
	CBaseFuncLib::Us2ToChar(strFileName,&szFileName);
	if(NULL != szFileName)
	{
		m_nLastErrCode = m_pFuncList->SKF_DeleteFile(hApp,szFileName);
		delete []szFileName;
		szFileName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::GetFileInfo(HANDLE hApp,const CString& strFileName,IDispatch** pPara)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	u32 nTry = 0;
	char *szFileName = NULL;
	FILEATTRIBUTE FileAttr;
	memset(&FileAttr,0,sizeof(FILEATTRIBUTE));
	CBaseFuncLib::Us2ToChar(strFileName,&szFileName);
	if(NULL != szFileName)
	{
		m_nLastErrCode = m_pFuncList->SKF_GetFileInfo(hApp,szFileName,&FileAttr);
		delete []szFileName;
		szFileName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
		/// 创建参数对象
		CComPtr <IParaService> spiParaService = CDbHelper::GetParaService();
		if(NULL != spiParaService)
		{
			spiParaService->AddString(_T("FileName"),CComBSTR(FileAttr.FileName));
			spiParaService->AddNumber(_T("FileSize"),FileAttr.FileSize);
			spiParaService->AddNumber(_T("ReadRight"),FileAttr.ReadRights);
			spiParaService->AddNumber(_T("WriteRight"),FileAttr.WriteRights);
			spiParaService->QueryInterface(IID_IDispatch, (void**)pPara);
			spiParaService = NULL;
		}
	}
	return bRet;
}

bool CSKFChinaCore::EnumAppFile(HANDLE hApp,u32& uFileLen,CString& strFiles)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	LPSTR szBuf = NULL;
	if(0 != uFileLen)
		szBuf = new CHAR[uFileLen];
	m_nLastErrCode = m_pFuncList->SKF_EnumFiles(hApp,szBuf,&uFileLen);
	if(SAR_OK == m_nLastErrCode)
	{
		if(NULL != szBuf)
		{
			BOOL bNull = FALSE;
			for(u32 nIndex = 0;nIndex < uFileLen;nIndex++)
			{
				if(NULL == szBuf[nIndex])
				{
					bNull = TRUE;
					continue;
				}
				if(bNull && strFiles.GetLength())
					strFiles += _T(";");
				strFiles += szBuf[nIndex];
				bNull = NULL;
			}
		}
		bRet = true;
	}
	if(NULL != szBuf)
	{
		delete []szBuf;
		szBuf = NULL;
	}
	return bRet;
}

HANDLE CSKFChinaCore::CreateContainer(HANDLE hApp,const CString& strContainerName)
{
	ResetErrInfo();
	HANDLE bHandle = NULL;
	if(NULL == m_pFuncList)
		return bHandle;
	HCONTAINER hContainer = NULL;
	char *szContainerName = NULL;
	CBaseFuncLib::Us2ToChar(strContainerName,&szContainerName);
	if(NULL != szContainerName)
	{
		m_nLastErrCode = m_pFuncList->SKF_CreateContainer(hApp,szContainerName,&hContainer);
		delete []szContainerName;
		szContainerName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bHandle = (HANDLE )hContainer;
	}

	return bHandle;
}

bool CSKFChinaCore::DeleteContainer(HANDLE hApp,const CString& strContainerName)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	char *szContainerName = NULL;
	CBaseFuncLib::Us2ToChar(strContainerName,&szContainerName);
	if(NULL != szContainerName)
	{
		m_nLastErrCode = m_pFuncList->SKF_DeleteContainer(hApp,szContainerName);
		delete []szContainerName;
		szContainerName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

HANDLE CSKFChinaCore::OpenContainer(HANDLE hApp,const CString& strContainerName)
{
	ResetErrInfo();
	HANDLE bHandle = NULL;
	if(NULL == m_pFuncList)
		return bHandle;
	HCONTAINER hContainer = NULL;
	char *szContainerName = NULL;
	CBaseFuncLib::Us2ToChar(strContainerName,&szContainerName);
	if(NULL != szContainerName)
	{
		m_nLastErrCode = m_pFuncList->SKF_OpenContainer(hApp,szContainerName,&hContainer);
		delete []szContainerName;
		szContainerName = NULL;
	}
	if(SAR_OK == m_nLastErrCode)
	{
		bHandle = (HANDLE )hContainer;
	}

	return bHandle;
}

bool CSKFChinaCore::EnumContainer(HANDLE hApp,u32& nLength,CString& strNames)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	LPSTR szBuf = NULL;
	if(0 != nLength)
		szBuf = new CHAR[nLength];
	m_nLastErrCode = m_pFuncList->SKF_EnumContainer(hApp,szBuf,&nLength);
	if(SAR_OK == m_nLastErrCode)
	{
		if(NULL != szBuf)
		{
			BOOL bNull = FALSE;
			for(u32 nIndex = 0;nIndex < nLength;nIndex++)
			{
				if(NULL == szBuf[nIndex])
				{
					bNull = TRUE;
					continue;
				}
				if(bNull && strNames.GetLength())
					strNames += _T(";");
				strNames += szBuf[nIndex];
				bNull = NULL;
			}
		}
		bRet = true;
	}
	if(NULL != szBuf)
	{
		delete []szBuf;
		szBuf = NULL;
	}
	return bRet;
}

bool CSKFChinaCore::GetContainerType(HANDLE hContainer,short& sType)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	ULONG nType = 0;
	m_nLastErrCode = m_pFuncList->SKF_GetContainerType(hContainer,&nType);
	if(SAR_OK == m_nLastErrCode)
	{
		sType = (short)nType;
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::ImportCertificate(HANDLE hContainer,VARIANT_BOOL bSign,BYTE* pbCert,u32 nCertLen)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	BOOL bSignFlag = FALSE;
	if(VARIANT_TRUE == bSign)
		bSignFlag = TRUE;
	m_nLastErrCode = m_pFuncList->SKF_ImportCertificate(hContainer,bSignFlag,pbCert,nCertLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ExportCertificate(HANDLE hContainer,VARIANT_BOOL bSign,BYTE* pbCert,u32& nCertLen)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	BOOL bSignFlag = FALSE;
	if(VARIANT_TRUE == bSign)
		bSignFlag = TRUE;
	m_nLastErrCode = m_pFuncList->SKF_ExportCertificate(hContainer,bSignFlag,pbCert,&nCertLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::CloseContainer(HANDLE hContainer)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_CloseContainer(hContainer);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::GenRandom(CString& strRandom,ULONG nRandomLen)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || nRandomLen < 6)
		return bRet;
	BYTE* pBuf = NULL;
	pBuf = new BYTE[nRandomLen];
	m_nLastErrCode = m_pFuncList->SKF_GenRandom(m_hDev,pBuf,nRandomLen);
	if(SAR_OK == m_nLastErrCode)
	{
		strRandom = pBuf;
		bRet = true;
	}
	if(NULL != pBuf)
	{
		delete []pBuf;
		pBuf = NULL;
	}
	return bRet;
}

bool CSKFChinaCore::GenExtRSAKey(ULONG nBitsLen,ULONG* pRsaPrivateKey)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	if(NULL == pRsaPrivateKey || nBitsLen < 1)
		return bRet;
	RSAPRIVATEKEYBLOB* pRsaPriKey = (RSAPRIVATEKEYBLOB* )pRsaPrivateKey;
	ATLASSERT(pRsaPriKey);
	m_nLastErrCode = m_pFuncList->SKF_GenExtRSAKey(m_hDev,nBitsLen,pRsaPriKey);
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::ImportRSAKeyPair(HANDLE hContainer,ULONG nSymAlgId,BYTE* pbWrappedKey,\
	ULONG nWrappedKeyLen,BYTE* pbEncryptedData,ULONG nEncryptedDataLen)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	if(NULL == hContainer || nWrappedKeyLen < 1 || NULL == pbWrappedKey
		|| NULL == pbEncryptedData || nEncryptedDataLen < 1)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_ImportRSAKeyPair(hContainer,nSymAlgId,pbWrappedKey,\
		nWrappedKeyLen,pbEncryptedData,nEncryptedDataLen);
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::RSASignData(HANDLE hContainer,ULONG nDataLen,\
	BYTE* pbData,ULONG* pnSignature,BYTE *pbSignature)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	if(NULL == hContainer || nDataLen < 1 || NULL == pnSignature
		|| NULL == pbData || NULL == pbSignature)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_RSASignData(hContainer,pbData,nDataLen,\
		pbSignature,pnSignature);
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::RSAVerify(ULONG* pRsaPubKey,ULONG nDataLen,BYTE* pbData,\
	ULONG nSignatureLen,BYTE* pbSignature)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	if(NULL == pRsaPubKey || nDataLen < 1 || nSignatureLen < 1
		|| NULL == pbData || NULL == pbSignature)
		return bRet;
	RSAPUBLICKEYBLOB* pRsaPublicKey = (RSAPUBLICKEYBLOB* )pRsaPubKey;
	ATLASSERT(pRsaPublicKey);
	m_nLastErrCode = m_pFuncList->SKF_RSAVerify(m_hDev,pRsaPublicKey,pbData,nDataLen,\
		pbSignature,nSignatureLen);
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::GenRSAKeyPair(HANDLE hContainer,ULONG nBitsLen,ULONG* pRsaPublicKey)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	RSAPUBLICKEYBLOB* RsaPubKey = (RSAPUBLICKEYBLOB* )pRsaPublicKey;
	ATLASSERT(RsaPubKey);
	m_nLastErrCode = m_pFuncList->SKF_GenRSAKeyPair(hContainer,nBitsLen,RsaPubKey);
	if(SAR_OK == m_nLastErrCode)
	{
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::ExportPublicKey(HANDLE hContainer,VARIANT_BOOL bSign,ULONG* pnBlobLen,ULONG* pbBlob)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == pnBlobLen)
		return bRet;
	BOOL bSignFlag = FALSE;
	if(VARIANT_TRUE == bSign)
		bSignFlag = TRUE;
	m_nLastErrCode = m_pFuncList->SKF_ExportPublicKey(hContainer,bSignFlag,(u8* )pbBlob,pnBlobLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ImportSessionKey(HANDLE hContainer,ULONG nAlgId,\
	ULONG nWrapedLen,BYTE* pbWrapedData,HANDLE* phSessionKey)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == phSessionKey)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_ImportSessionKey(hContainer,nAlgId,pbWrapedData,nWrapedLen,phSessionKey);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::RSAExportSessionKey(HANDLE hContainer,ULONG nAlgId,\
	ULONG* pRsaPubKey,ULONG* pnDataLen,BYTE* pbData,HANDLE *phSessionKey)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == phSessionKey)
		return bRet;
	RSAPUBLICKEYBLOB* pRsaPublicKey= (RSAPUBLICKEYBLOB* )pRsaPubKey;
	ATLASSERT(pRsaPublicKey);
	m_nLastErrCode = m_pFuncList->SKF_RSAExportSessionKey(hContainer,nAlgId,pRsaPublicKey,pbData,pnDataLen,phSessionKey);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ExtRSAPubKeyOperation(ULONG* pRsaPubKey,ULONG nInputLen,BYTE* pbInputData,\
	ULONG* pnOutput,BYTE* pbOutput)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == pRsaPubKey || nInputLen < 1 
		|| NULL == pbInputData || NULL == pnOutput || NULL == pbOutput)
		return bRet;
	RSAPUBLICKEYBLOB* pRsaPublicKey= (RSAPUBLICKEYBLOB* )pRsaPubKey;
	ATLASSERT(pRsaPublicKey);
	m_nLastErrCode = m_pFuncList->SKF_ExtRSAPubKeyOperation(m_hDev,pRsaPublicKey,\
		pbInputData,nInputLen,pbOutput,pnOutput);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::ExtRSAPriKeyOperation(ULONG* pRsaPriKey,ULONG nInputLen,BYTE* pbInputData,\
	ULONG* pnOutput,BYTE* pbOutput)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == pRsaPriKey || nInputLen < 1 
		|| NULL == pbInputData || NULL == pnOutput || NULL == pbOutput)
		return bRet;
	RSAPRIVATEKEYBLOB* pRsaPrivateKey= (RSAPRIVATEKEYBLOB* )pRsaPriKey;
	ATLASSERT(pRsaPrivateKey);
	m_nLastErrCode = m_pFuncList->SKF_ExtRSAPriKeyOperation(m_hDev,pRsaPrivateKey,\
		pbInputData,nInputLen,pbOutput,pnOutput);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}


bool CSKFChinaCore::SetSymmKey(ULONG nAlgId,BYTE* pbKey,HANDLE* phSessionKey)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == phSessionKey)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_SetSymmKey(m_hDev,pbKey,nAlgId,phSessionKey);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::EncryptOrDecryptInit(HANDLE hKey,bool bEncryFlag)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hKey)
		return bRet;
	if(NULL == m_pBlockCipherParam)
	{
		m_pBlockCipherParam = new BLOCKCIPHERPARAM();
		memset(m_pBlockCipherParam,0,sizeof(BLOCKCIPHERPARAM));
	}
	if(NULL != m_pBlockCipherParam)
	{
		if(bEncryFlag)
			m_pBlockCipherParam->PaddingType = NO_PADDING;
		else
			m_pBlockCipherParam->PaddingType = PKCS5_PADDING;
	}
	if(bEncryFlag)
		m_nLastErrCode = m_pFuncList->SKF_EncryptInit(hKey,*m_pBlockCipherParam);
	else
		m_nLastErrCode = m_pFuncList->SKF_DecryptInit(hKey,*m_pBlockCipherParam);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::Encrypt(HANDLE hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hKey)
		return bRet;
	if(NULL == pnEncryptedDataLen || NULL == pbEncryptedData)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_Encrypt(hKey,pbData,nDataLen,pbEncryptedData,pnEncryptedDataLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::EncryptUpdate(HANDLE hKey,ULONG nDataLen,BYTE* pbData,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hKey)
		return bRet;
	if(NULL == pnEncryptedDataLen || NULL == pbEncryptedData)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_EncryptUpdate(hKey,pbData,nDataLen,pbEncryptedData,pnEncryptedDataLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::EncryptOrDecryptFinal(HANDLE hKey,ULONG* pnEncryptedDataLen,BYTE* pbEncryptedData,BOOL bEncryptFlag)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hKey)
		return bRet;
	if(NULL == pnEncryptedDataLen || NULL == pbEncryptedData)
		return bRet;
	if(bEncryptFlag)
		m_nLastErrCode = m_pFuncList->SKF_EncryptFinal(hKey,pbEncryptedData,pnEncryptedDataLen);
	else
		m_nLastErrCode = m_pFuncList->SKF_DecryptFinal(hKey,pbEncryptedData,pnEncryptedDataLen);
	if(SAR_OK == m_nLastErrCode)
	{
		if(NULL != m_pBlockCipherParam)
		{
			delete m_pBlockCipherParam;
			m_pBlockCipherParam = NULL;
		}
		bRet = true;
	}
	return bRet;
}

bool CSKFChinaCore::Decrypt(HANDLE hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hKey)
		return bRet;
	if(NULL == nEncryptedDataLen || NULL == pbEncryptedData || NULL == pbData || NULL == pnDataLen)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_Decrypt(hKey,pbEncryptedData,nEncryptedDataLen,pbData,pnDataLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::DecryptUpdate(HANDLE hKey,ULONG nEncryptedDataLen,BYTE* pbEncryptedData,ULONG* pnDataLen,BYTE* pbData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hKey)
		return bRet;
	if(NULL == nEncryptedDataLen || NULL == pbEncryptedData || NULL == pnDataLen || NULL == pbData)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_DecryptUpdate(hKey,pbEncryptedData,nEncryptedDataLen,pbData,pnDataLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::DigestInit(ULONG ulAlgID,ULONG* pPubKey,SHORT nIDLen,unsigned char* pucID,HANDLE* phHash)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == pPubKey)
		return bRet;
	if(NULL == nIDLen || NULL == pucID || NULL == phHash)
		return bRet;
	/// 类型转换
	ECCPUBLICKEYBLOB* pPublicKey = (ECCPUBLICKEYBLOB* )pPubKey;
	m_nLastErrCode = m_pFuncList->SKF_DigestInit(m_hDev,ulAlgID,pPublicKey,pucID,nIDLen,phHash);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::Digest(HANDLE hHash,ULONG ulDataLen,BYTE* pbData,ULONG* pnHashLen,BYTE* pbHashData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hHash)
		return bRet;
	if(NULL == ulDataLen || NULL == pbData || NULL == pnHashLen || NULL == pbHashData)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_Digest(hHash,pbData,ulDataLen,pbHashData,pnHashLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::DigestUpdate(HANDLE hHash,ULONG ulDataLen,BYTE* pbData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hHash)
		return bRet;
	if(NULL == ulDataLen || NULL == pbData || NULL == ulDataLen)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_DigestUpdate(hHash,pbData,ulDataLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::DigestFinal(HANDLE hHash,ULONG* pnHashLen,BYTE* pbHashData)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList || NULL == hHash)
		return bRet;
	if(NULL == pnHashLen || NULL == pbHashData)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_DigestFinal(hHash,pbHashData,pnHashLen);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}

bool CSKFChinaCore::CloseHandle(HANDLE hHandle)
{
	ResetErrInfo();
	bool bRet = false;
	if(NULL == m_pFuncList)
		return bRet;
	m_nLastErrCode = m_pFuncList->SKF_CloseHandle(hHandle);
	if(SAR_OK == m_nLastErrCode)
		bRet = true;
	return bRet;
}