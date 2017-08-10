#include "stdafx.h"
#include "SqliteHelper.h"
#include "BaseFuncLib.h"

/// 全局句柄
extern HANDLE g_hInstance;

ATL::CString CDbHelper::m_strModelName = TDHXKJ_SQLITE3DBENGINE;

void CDbHelper::FreeDB(void)
{
	FreeAllHandle();
}

CComPtr <ISqlite3Connect> CDbHelper::GetSqlite3Connect()
{
	/// 底层SQLITE数据库访问接口
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(Sqlite3Connect),__uuidof(ISqlite3Connect),(VOID **)&spiSqlite3Connect);
		ATLASSERT(spiSqlite3Connect);
		return spiSqlite3Connect;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),m_strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),m_strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),m_strModelName);
	}
	if(GetSourceProtected())
	{
		/// 防范破解
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*800)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(Sqlite3Connect),__uuidof(ISqlite3Connect),(VOID **)&spiSqlite3Connect);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiSqlite3Connect);
	return spiSqlite3Connect;
}

CComPtr <IConnectHelper> CDbHelper::GetDBHelper(ATL::CString strModelName /*= TDHXKJ_SQLITE3DBENGINE*/)
{
	/// 底层SQLITE数据库访问辅助接口
	CComPtr <IConnectHelper> spiConnectHelper = NULL;
	HINSTANCE hHandle = GetComHandle(strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(ConnectHelper),__uuidof(IConnectHelper),(VOID **)&spiConnectHelper);
		ATLASSERT(spiConnectHelper);
		return spiConnectHelper;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),strModelName);
	}
	if(GetSourceProtected())
	{
		/// 防范破解
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(ConnectHelper),__uuidof(IConnectHelper),(VOID **)&spiConnectHelper);
	if(NULL != hHandle)
	{
		m_mapInstance[strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiConnectHelper);
	return spiConnectHelper;
}

long CDbHelper::GetSingleLongValue(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const CString &strSqlString)
{
	long lValue = 0;
	if(NULL == spiSqlite3Connect)
		return lValue;
	/// 执行查询
	CComPtr <ISqlite3Statement> spiSqlite3Statement = NULL;
	spiSqlite3Connect->ExecQueryEx(CComBSTR(strSqlString),&spiSqlite3Statement);
	if(NULL != spiSqlite3Statement)
	{
		while(SUCCEEDED(spiSqlite3Statement->NextRow()))
		{
			spiSqlite3Statement->GetValueInt(0,&lValue);
			break;
		}
		spiSqlite3Statement = NULL;
	}
	return lValue;
}

ATL::CString CDbHelper::GetSingleStringValue(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString)
{
	ATL::CString strValue(_T(""));
	if(NULL == spiSqlite3Connect)
		return strValue;
	CComPtr <ISqlite3Statement> spiSqlite3Statement = NULL;
	spiSqlite3Connect->ExecQueryEx(CComBSTR(strSqlString),&spiSqlite3Statement);
	if(NULL != spiSqlite3Statement)
	{
		while(SUCCEEDED(spiSqlite3Statement->NextRow()))
		{
			CComBSTR bstrValue;
			bstrValue.Empty();
			spiSqlite3Statement->GetValueString(0,&bstrValue);
			if(bstrValue.Length())
			{
				strValue = bstrValue.m_str;
				bstrValue.Empty();
			}
			break;
		}
		spiSqlite3Statement = NULL;
	}
	return strValue;
}

ATL::CString CDbHelper::GetSingleStringValueEx(const ATL::CString &strDBPath,const ATL::CString &strSqlString,\
	const ATL::CString& strPW,int nOverTime)
{
	ATL::CString strValue(_T(""));
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL == spiConnectHelper)
		return strValue;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	spiConnectHelper->OpenDB(CComBSTR(strDBPath),VARIANT_FALSE,(short)nOverTime,CComBSTR(strPW),&spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
	{
		CComBSTR bstrErrInfo;
		spiConnectHelper->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
		spiConnectHelper = NULL;
		if(bstrErrInfo.Length())
		{
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::GetSingleStringValueEx 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
		}
		return strValue;
	}
	strValue = CDbHelper::GetSingleStringValue(spiSqlite3Connect,strSqlString);
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	spiConnectHelper = NULL;

	return strValue;
}

long CDbHelper::GetSingleLongValueEx(const ATL::CString &strDBPath,const ATL::CString &strSqlString,const ATL::CString& strPW)
{
	long lValue = 0;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL == spiConnectHelper)
		return lValue;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	spiConnectHelper->OpenDB(CComBSTR(strDBPath),VARIANT_FALSE,SQLITE_OPEN_DEFAULTOVERTIME,CComBSTR(strPW),&spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
	{
		CComBSTR bstrErrInfo;
		spiConnectHelper->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
		spiConnectHelper = NULL;
		if(bstrErrInfo.Length())
		{
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::GetSingleLongValueEx 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
		}
		return lValue;
	}
	lValue = CDbHelper::GetSingleLongValue(spiSqlite3Connect,strSqlString);
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	spiConnectHelper = NULL;
	return lValue;
}

ULONG CDbHelper::GetRecordCount(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString)
{
	ULONG unCount = 0;
	ATLASSERT(spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
		return unCount;
	CComPtr <ISqlite3Statement> spiSqlite3Statement = NULL;
	spiSqlite3Connect->ExecQueryEx(CComBSTR(strSqlString),&spiSqlite3Statement);
	if(NULL != spiSqlite3Statement)
	{
		spiSqlite3Statement->get_RecordCount(&unCount);
		spiSqlite3Statement = NULL;
	}
	return unCount;
}

BOOL CDbHelper::RecordHaveExist(const ATL::CString &strDBPath,const ATL::CString &strSqlString,const ATL::CString& strPW)
{
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL == spiConnectHelper)
		return FALSE;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	spiConnectHelper->OpenDB(CComBSTR(strDBPath),VARIANT_FALSE,SQLITE_OPEN_DEFAULTOVERTIME,\
		CComBSTR(strPW),&spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
	{
		CComBSTR bstrErrInfo;
		spiConnectHelper->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
		spiConnectHelper = NULL;
		if(bstrErrInfo.Length())
		{
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::RecordHaveExist 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
		}
		return FALSE;
	}
	ULONG nCount = GetRecordCount(spiSqlite3Connect,strSqlString);
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	if(nCount)
		return TRUE;
	return FALSE;
}

HRESULT CDbHelper::ExecDBCommond(CComPtr <ISqlite3Connect> &spiSqlite3Connect,CSTRING_VECTOR &StringList)
{
	HRESULT hRet(E_FAIL);
	if(NULL == spiSqlite3Connect)
		return hRet;
	size_t nCount = StringList.size();
	if(nCount > 1)
	{
		/// 多个SQL命令，开始事务模式执行
		hRet = spiSqlite3Connect->BeginTransaction();
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::ExecDBCommond BeginTransaction 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
			return hRet;
		}
	}
	size_t nIndex = 0;
	for(nIndex = 0;nIndex<nCount;nIndex++)
	{
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(StringList[nIndex]));
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::ExecDBCommond 命令 %s 执行错误：%s"),StringList[nIndex],ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
		}
	}
	if(nCount > 1)
	{
		hRet = spiSqlite3Connect->CommitTransaction();
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::ExecDBCommond CommitTransaction 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
			return hRet;
		}
	}
	StringList.clear();
	return hRet;
}

HRESULT CDbHelper::OperateDB(const ATL::CString &strDBPath,CSTRING_VECTOR &StringList,\
	const ATL::CString& strPW,BOOL bCompressDB)
{
	HRESULT hRet(E_FAIL);
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL == spiConnectHelper)
		return hRet;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	hRet = spiConnectHelper->OpenDB(CComBSTR(strDBPath),VARIANT_FALSE,60,CComBSTR(strPW),&spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
	{
		CComBSTR bstrErrInfo;
		spiConnectHelper->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
		spiConnectHelper = NULL;
		if(bstrErrInfo.Length())
		{
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::OperateDB 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
		}
		return hRet;
	}
	hRet = ExecDBCommond(spiSqlite3Connect,StringList);
	if(bCompressDB)
	{
		/// 压缩数据库，必须在事务之外执行
		hRet = spiSqlite3Connect->CompressDB();
		if(FAILED(hRet))
		{
			CComBSTR bstrErrInfo;
			bstrErrInfo.Empty();
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo); /// 获得出错信息
			hRet = S_FALSE;
#ifdef APP_LOG_ENABLE
			ATL::CString strLogInfo(_T(""));
			strLogInfo.Format(_T("DbHelper::OperateDB CompressDB 执行错误：%s"),ATL::CString(bstrErrInfo.m_str));
			WRITELOGTOFILE(strLogInfo);
#endif
			bstrErrInfo.Empty();
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	spiConnectHelper = NULL;
	return hRet;
}

CComPtr <IJsonService> CDbHelper::GetJsonService(ATL::CString strModelName /*= TDHXKJ_JSONENGINE*/)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	HINSTANCE hHandle = GetComHandle(strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(JsonService),__uuidof(IJsonService),(VOID **)&spiJsonService);
		ATLASSERT(spiJsonService);
		return spiJsonService;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),strModelName);
	}
	if(GetSourceProtected())
	{
		/// 防范破解
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(JsonService),__uuidof(IJsonService),(VOID **)&spiJsonService);
	if(NULL != hHandle)
	{
		m_mapInstance[strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiJsonService);
	return spiJsonService;
}

CComPtr <IParaService> CDbHelper::GetParaService()
{
	CComPtr <IParaService> spiParaService = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(ParaService),__uuidof(IParaService),(VOID **)&spiParaService);
		ATLASSERT(spiParaService);
		return spiParaService;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),m_strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),m_strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),m_strModelName);
	}
	if(GetSourceProtected())
	{
		/// 防范破解
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(ParaService),__uuidof(IParaService),(VOID **)&spiParaService);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiParaService);
	return spiParaService;
}

CComPtr <ILogService> CDbHelper::GetLogService()
{
	CComPtr <ILogService> spiLogService = NULL;
	HINSTANCE hHandle = GetComHandle(m_strModelName);
	if(NULL != hHandle)
	{
		CBaseFuncLib::CreateInterface( hHandle,__uuidof(LogService),__uuidof(ILogService),(VOID **)&spiLogService);
		ATLASSERT(spiLogService);
		return spiLogService;
	}
	ATL::CString strDllPath(_T(""));
	strDllPath.Format(_T("%s%s"),CBaseFuncLib::GetModulePath(g_hInstance),m_strModelName);
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetComTempPath(),m_strModelName);
	}
	if(!CBaseFuncLib::IsPathExist(strDllPath))
	{
		strDllPath.Empty();
		strDllPath.Format(_T("%s%s"),GetAppInsPath(),m_strModelName);
	}
	if(GetSourceProtected())
	{
		/// 防范破解
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(strDllPath);
		if(dwFileSize>1024*350)
			return NULL;
	}
	hHandle = CBaseFuncLib::CreateInstance( strDllPath,__uuidof(LogService),__uuidof(ILogService),(VOID **)&spiLogService);
	if(NULL != hHandle)
	{
		m_mapInstance[m_strModelName] = hHandle;
	}
	strDllPath.Empty();
	ATLASSERT(spiLogService);
	return spiLogService;
}

ULONG CDbHelper::WriteSysLog(short nUserType,const CString& strLID,const CString& strName,const CString& strLog)
{
	ULONG nLogID = 0;
	CComPtr <ILogService> spiLogService = GetLogService();
	if(NULL == spiLogService)
		return nLogID;
	spiLogService->WriteSysLog(nUserType,CComBSTR(strLID),CComBSTR(strName),0,CComBSTR(strLog),&nLogID);
	spiLogService = NULL;
	return nLogID;
}

ULONG CDbHelper::WriteWarnLog(EHXSafeLogType eLogType,DATE dtTime,short nFlag,const CString& strID,const CString& strLog)
{
	ULONG nLogID = 0;
	CComPtr <ILogService> spiLogService = GetLogService();
	if(NULL == spiLogService)
		return nLogID;
	HRESULT hRet(E_FAIL);
	if(SAFELOGTYPE_USBACCESS == eLogType)
		hRet = spiLogService->WriteUsbLog(nFlag,CComBSTR(strID),dtTime,CComBSTR(strLog),&nLogID);
	else
		hRet = spiLogService->WriteAppLog(eLogType,nFlag,dtTime,CComBSTR(strID),CComBSTR(strLog),&nLogID);
	spiLogService = NULL;
	return nLogID;
}