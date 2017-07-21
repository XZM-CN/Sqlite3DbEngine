// ConnectHelper.cpp : CConnectHelper 的实现

#include "stdafx.h"
#include "ConnectHelper.h"
#include "BaseFuncLib.h"

// CConnectHelper

HRESULT CConnectHelper::CreateOptionTable(CComPtr<ISqlite3Connect>& spiSqlite3Connect,DWORD dwVersion)
{
	HRESULT hRet(E_FAIL);
	ATLASSERT(spiSqlite3Connect);
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	CString strSqlCmd(_T("")),strTemp(_T(""));
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(SQLITEDB_TABLE_SYSPARA),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		strSqlCmd.Format(_T("CREATE TABLE [%s] (%s VARCHAR DEFAULT \'\' NOT NULL PRIMARY KEY UNIQUE,%s VARCHAR DEFAULT \'\' NOT NULL);"),\
			SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_OPTIONRECORD_FIELDNAME2);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
		if(FAILED(hRet))
		{
#ifdef _DEBUG
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
			::Sleep(1);
#endif
			return hRet;///无法创建数据库表
		}
	}
	/// 插入记录,保存当前数据库表定义版本
	strTemp.Format(_T("%ld"),dwVersion);
	strSqlCmd.Format(_T("insert into [%s] values (\'%s\',\'%s\');"),\
		SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONNAME_DBVERSION,strTemp);
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	return hRet;
}

HRESULT CConnectHelper::CreateDefaultDB(CComBSTR bstrDBPath,CComBSTR bstrPW,ISqlite3Connect** ppVal)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	if(!bstrDBPath || !ppVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	HRESULT hRet(E_FAIL);
	CComPtr<ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr<IClassFactory>	cf = NULL;
	hRet = DllGetClassObject( __uuidof(Sqlite3Connect), __uuidof(IClassFactory), (LPVOID *)&cf );
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(ISqlite3Connect), (VOID **)&spiSqlite3Connect );
		cf = NULL;
	}
	if(FAILED(hRet) || !spiSqlite3Connect)
	{
		/// 创建数据库访问接口失败
		m_strLastErrInfo = _T("创建数据库访问接口失败。");
		return E_FAIL;
	}
	try
	{
		hRet = spiSqlite3Connect->Open(bstrDBPath,bstrPW,VARIANT_TRUE);
	}
	catch ( ... )
	{
		/// 异常
		hRet = E_FAIL;
	}
	if(FAILED(hRet))
	{
		/// 打开数据文件失败，获得出错信息
		CComBSTR bstrErrInfo;
		hRet = spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
		if(SUCCEEDED(hRet))
		{
			if(bstrErrInfo.Length())
				m_strLastErrInfo = bstrErrInfo.m_str;
		}
		if(m_strLastErrInfo.IsEmpty())
			m_strLastErrInfo = _T("未知原因打开数据库文件失败。");
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	hRet = CreateOptionTable(spiSqlite3Connect);
	if(FAILED(hRet))
	{
		//创建缺省数据库失败
		spiSqlite3Connect->Close();
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	/// 查询返回接口指针
	hRet = spiSqlite3Connect->QueryInterface(IID_ISqlite3Connect,(LPVOID *)ppVal);
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CConnectHelper::OpenDB(BSTR bstrDbPath,VARIANT_BOOL bCreateFlag,\
	short sOverTimeSecond,BSTR bstrPW,ISqlite3Connect** ppVal)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	if(!bstrDbPath || !ppVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	ULONGLONG dwFileSize = 0;
	BOOL bExistFlag = CBaseFuncLib::IsPathExist(bstrDbPath);
	if(bExistFlag)
	{
		/// 可能因为共享问题导致打开失败
		dwFileSize = CBaseFuncLib::GetFileSize(bstrDbPath);
		int nIndex = 100;
		while(0 == dwFileSize && nIndex > 0)
		{
			::Sleep(100);
			nIndex--;
			dwFileSize = CBaseFuncLib::GetFileSize(bstrDbPath);
		}
	}
	if(!bExistFlag || !dwFileSize)
	{
		if(bExistFlag && !dwFileSize)
		{
			/// 异常数据库
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("ConnectHelper::OpenDB %s 异常数据库"),\
				CString(bstrDbPath));
			WRITELOGTOFILE(strLogInfo);
#endif
		}

		if(VARIANT_FALSE == bCreateFlag)
		{

			m_strLastErrInfo.Format(_T("数据库文件 %s 不存在或异常"),CString(bstrDbPath));
			//解决授权后第一次安装时由于没有数据库文件而读取授权数失败的问题
			if (!bExistFlag)
			{
				return ERROR_FILE_NOT_FOUND;
			}

			return E_FAIL;
		}
		return CreateDefaultDB(bstrDbPath,bstrPW,ppVal);
	}
	HRESULT hRet(E_FAIL);
	CComPtr<ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr<IClassFactory>	cf = NULL;
	hRet = DllGetClassObject(__uuidof(Sqlite3Connect),__uuidof(IClassFactory),(LPVOID *)&cf);
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(ISqlite3Connect), (VOID **)&spiSqlite3Connect );
		cf = NULL;
	}
	if(FAILED(hRet) || !spiSqlite3Connect)
	{
		/// 创建数据库访问接口失败
		m_strLastErrInfo.Format(_T("%s打开失败，错误码：%ld"),CString(bstrDbPath),hRet);
		return E_FAIL;
	}
	try
	{
		if(sOverTimeSecond)
			spiSqlite3Connect->put_TimeoutSecond(sOverTimeSecond);

		hRet = spiSqlite3Connect->Open(bstrDbPath,bstrPW,bCreateFlag);
	}
	catch ( ... )
	{
		/// 异常
		hRet = E_FAIL;
		m_strLastErrInfo.Format(_T("%s 数据库打开异常"),CString(bstrDbPath));
	}
	if(FAILED(hRet))
	{
		/// 打开数据文件失败，获得出错信息
		CComBSTR bstrErrInfo;
		if(m_strLastErrInfo.IsEmpty())
			hRet = spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
		if(SUCCEEDED(hRet) && bstrErrInfo.Length())
		{
			m_strLastErrInfo = bstrErrInfo.m_str;
			bstrErrInfo.Empty();
		}
		if(m_strLastErrInfo.IsEmpty())
			m_strLastErrInfo.Format(_T("%s打开失败，未知错误原因"),CString(bstrDbPath));
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	/// 如果数据库表定义升级，需要取版本号来判断，修改版本号SQLITEDB_OPTIONVALUE_DEFAULTVERSION，通过程序升级数据库
	long lDBVersion = 0;
	CString strSqlString(_T(""));
	strSqlString.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
		SQLITEDB_OPTIONRECORD_FIELDNAME2,SQLITEDB_TABLE_SYSPARA,\
		SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_OPTIONNAME_DBVERSION);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlString))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&lDBVersion);
			break;
		}
	}
	if(!lDBVersion && VARIANT_FALSE == bCreateFlag)
	{
		spiSqlite3Connect = NULL;
		m_strLastErrInfo.Format(_T("打开 %s 时未发现版本信息"),(CString)bstrDbPath);
		return E_FAIL;
	}
	/// 如果数据库表定义升级，需要取版本号来判断，通过程序升级数据库
	if(!lDBVersion && VARIANT_TRUE == bCreateFlag && NULL == bstrPW)
	{
		/// 数据库创建非法
		spiSqlite3Connect = NULL;
#ifdef _DEBUG
		///判断是哪个应用调用，如果是服务，不弹出
		TCHAR szModemName[MAX_PATH];
		::memset(szModemName,0,MAX_PATH*sizeof(TCHAR));
		::GetModuleFileName(NULL,szModemName,MAX_PATH*sizeof(TCHAR));
		CString strAppName(szModemName);
		if(-1 == strAppName.Find(TDHXKJ_HOSTSERVICE_NAME))
		{
			/// 解决服务模式弹出提示阻塞运行问题
			CString strMessage(_T(""));
			strMessage.Format(_T("数据库文件 %s 访问异常！\r\n可能格式被破坏，是否删除重建？"),(CString )bstrDbPath);
			if(::MessageBox(::GetForegroundWindow(),strMessage,_T("提示"),
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				::DeleteFile(bstrDbPath);
				return CreateDefaultDB(bstrDbPath,bstrPW,ppVal);
			}
		}
#endif
		m_strLastErrInfo.Format(_T("打开数据库时执行 %s 错误"),strSqlString);
		return E_FAIL;
	}
	if(NULL != spiSqlite3Connect)
	{
		/// 查询返回接口指针
		hRet = spiSqlite3Connect->QueryInterface(IID_ISqlite3Connect,(LPVOID *)ppVal);
		spiSqlite3Connect = NULL;
	}
	return hRet;
}

STDMETHODIMP CConnectHelper::get_LastErrorInfo(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(!m_strLastErrInfo.IsEmpty())
	{
		CComBSTR bstrErr(m_strLastErrInfo);
		bstrErr.CopyTo(pVal);

		return S_OK;
	}
	/// 无出错信息
	return E_FAIL;
}

STDMETHODIMP CConnectHelper::InitDBEnv(BSTR bstrDbPath)
{
	// TODO: 在此添加实现代码
	if(!bstrDbPath)
		return E_POINTER;
	CBaseFuncLib::GetAppDir(TDHXKJ_HOSTSERVICE_NAME);
	return S_OK;
}