// Sqlite3Connect.cpp : CSqlite3Connect 的实现

#include "stdafx.h"
#include "Sqlite3Connect.h"

#include <shlobj.h>  // for CSIDL_COMMON_APPDATA

#include "BaseFuncLib.h"
#include "MD5.h"
#include "Sqlite3ConnectStatement.h"
#include "DBItemDef.h"

CString GetFileMD5(const CString& strFilePath)
{
	CString strFileCode( _T(""));

	FILE* file = NULL;
	char *szFilePath = NULL;
	CBaseFuncLib::Us2ToChar(strFilePath,&szFilePath);
	ATLASSERT(szFilePath);
	if(NULL == szFilePath)
		return strFileCode;
	errno_t err = fopen_s(&file,szFilePath, "rb");
	delete []szFilePath;
	szFilePath = NULL;
	if(NULL == file)
	{
		/// 打开文件失败
		return strFileCode;
	}
	unsigned char digest[16];
	memset(digest,0,16);
	MD5_File md5(file);
	md5.GetDigest(digest);
	fclose(file);

	CString tmp(_T(""));
	for(int i = 0; i < 16; i ++)
	{
		tmp.Format(_T("%02X"),digest[i]);
		strFileCode += tmp;
		tmp.Empty();
	}
	return strFileCode;
}


// CSqlite3Connect
STDMETHODIMP CSqlite3Connect::ReadOpen(BSTR bstrDbFile,BSTR bstrPW)
{
	// TODO: 在此添加实现代码
	if(!bstrDbFile)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	/// 判断数据文件合法性
	if(!CBaseFuncLib::IsPathExist(bstrDbFile))
	{
		m_strLastErrInfo = bstrDbFile;
		m_strLastErrInfo += _T(" 数据库文件不存在。");
		return E_FAIL;
	}
	if(m_pSqliteWraper)
	{
		if(0 == m_dwCurrentThreadID)
			m_dwCurrentThreadID = ::GetCurrentThreadId();
		else
		{
			ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
		}
		Close();
	}
	else
		m_dwCurrentThreadID = ::GetCurrentThreadId();
	ATLASSERT(!m_pSqliteWraper);
	m_pSqliteWraper = new CSqlite3Wrapper();
	ATLASSERT(m_pSqliteWraper);
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("内存不足。");
		return E_POINTER;
	}
	CString strReadDB(bstrDbFile);
	strReadDB += _T(".bak");
	if(CBaseFuncLib::IsPathExist(strReadDB))
	{
		CString strMD51 = GetFileMD5(bstrDbFile);
		CString strMD52 = GetFileMD5(strReadDB);
		if(0 != strMD51.CompareNoCase(strMD52))
		{
			/// 已经有变化
			::SetFileAttributes(strReadDB,FILE_ATTRIBUTE_NORMAL);
			int nIndex = 5;
			BOOL bDelFlag = ::DeleteFile(strReadDB);
			while(!bDelFlag && nIndex--)
			{
				DWORD dwErrCode = ::GetLastError();
				::Sleep(50);
				bDelFlag = ::DeleteFile(strReadDB);
			}
			if(bDelFlag)
				::CopyFile(bstrDbFile,strReadDB,FALSE);
			else
				strReadDB = bstrDbFile;
		}
	}
	else
	{
		/// 打开备份
		::CopyFile(bstrDbFile,strReadDB,FALSE);
	}

	BOOL bOpenFlag = m_pSqliteWraper->Open(strReadDB,bstrPW);
	if(!bOpenFlag)
	{
		m_strLastErrInfo = strReadDB + _T(" 打开失败。");
		Close();
		return E_FAIL;
	}
	m_bReadOnly = TRUE;
	m_strDbFile.Empty();
	m_strDbFile = bstrDbFile;
	if(NULL != bstrPW)
		m_strPassword = bstrPW;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::Open(BSTR bstrDbFile,BSTR bstrPW,VARIANT_BOOL bCreateFlag)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	if(NULL == bstrDbFile)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	/// 判断数据文件合法性
	if(VARIANT_FALSE == bCreateFlag && !CBaseFuncLib::IsPathExist(bstrDbFile))
	{
		m_strLastErrInfo = bstrDbFile;
		m_strLastErrInfo += _T(" 数据库文件不存在。");
		return E_FAIL;
	}
	if(VARIANT_TRUE == bCreateFlag && CBaseFuncLib::IsPathExist(bstrDbFile))
	{
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(bstrDbFile);
		if(!dwFileSize)
		{
			/// 可能正在创建，不能删除
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("Sqlite::Open 无大小数据库文件 %s"),\
				CString(bstrDbFile));
			WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
			//			::SetFileAttributes(bstrDbFile,FILE_ATTRIBUTE_NORMAL);
			//			::DeleteFile(bstrDbFile);
			/// 等待其他程序创建
			int nIndex = 50;
			dwFileSize = CBaseFuncLib::GetFileSize(bstrDbFile);
			while(!dwFileSize && nIndex > 0)
			{
				::Sleep(100);
				nIndex--;
				dwFileSize = CBaseFuncLib::GetFileSize(bstrDbFile);
			}
		}
	}

	if(NULL != m_pSqliteWraper)
	{
		if(0 == m_dwCurrentThreadID)
			m_dwCurrentThreadID = ::GetCurrentThreadId();
		else
		{
			ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
		}
		Close();
	}
	else
		m_dwCurrentThreadID = ::GetCurrentThreadId();
	/// 全局事件，保证服务程序和桌面程序都可以访问使用
	CString strEventPath(_T("Global\\")),strDBName(bstrDbFile);
	/// 去掉不合法的字符串
	strDBName.Replace(_T("\\"),_T(""));
	strDBName.Replace(_T(":"),_T(""));
	strDBName.Replace(_T(" "),_T(""));
	if(strDBName.IsEmpty())
		return E_FAIL;
	strEventPath += strDBName;

	/// 设置事件的安全级别，确保系统服务和桌面进程都可以操作
	SECURITY_DESCRIPTOR secutityDese;
	::InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
	/// 把参数三设置为NULL，其实相当于将object的安全级别降到了最低，所有的访问请求都将成功
	::SetSecurityDescriptorDacl(&secutityDese,TRUE,NULL,FALSE);
	SECURITY_ATTRIBUTES securityAttr;
	securityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttr.bInheritHandle = FALSE;
	securityAttr.lpSecurityDescriptor = &secutityDese;
	m_hSyncEvent = ::CreateEvent(&securityAttr,FALSE,TRUE,strEventPath);
	if(NULL != m_hSyncEvent && INVALID_HANDLE_VALUE != m_hSyncEvent)
	{
		DWORD dwLastErr = ::GetLastError();
		if(ERROR_ALREADY_EXISTS == dwLastErr)
		{
			COleDateTime curTime = COleDateTime::GetCurrentTime();
			/// 已经被打开，需要等待
			DWORD dwRet = WaitForSingleObject(m_hSyncEvent,1000*m_lOverTimeSecond);///超时等待
			if(WAIT_OBJECT_0 != dwRet)
			{
				/// 释放事件句柄
				::CloseHandle(m_hSyncEvent);
				m_hSyncEvent = NULL;
				/// 打开超时
				m_strLastErrInfo.Format(_T("数据库%s超时 %d 秒未打开"),\
					(CString )bstrDbFile,m_lOverTimeSecond);

#ifdef APP_LOG_ENABLE
				WRITELOGTOFILE(m_strLastErrInfo);
#endif
				return E_FAIL;
			}
			DOUBLE dtWaitSecond = (COleDateTime::GetCurrentTime()-curTime).GetTotalSeconds();
			if(dtWaitSecond > 3.0)
			{
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("打开数据库 %s 等待 %d 秒"),\
					(CString )bstrDbFile,(int)dtWaitSecond);
				WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
			}
		}
	}
	ATLASSERT( NULL != m_hSyncEvent );
	ATLASSERT(NULL == m_pSqliteWraper);
	m_pSqliteWraper = new CSqlite3Wrapper();
	ATLASSERT(m_pSqliteWraper);
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("内存不足。");
		return E_POINTER;
	}
	BOOL bOpenFlag = m_pSqliteWraper->Open(bstrDbFile,bstrPW);
	if(!bOpenFlag)
	{
		m_strLastErrInfo = strEventPath + _T(" 打开失败。原因：");
		m_strLastErrInfo += m_pSqliteWraper->LastError();
		Close();
		return E_FAIL;
	}
	m_strDbFile.Empty();
	m_strDbFile = bstrDbFile;
	m_bReadOnly = FALSE;
	if(NULL != bstrPW)
		m_strPassword = bstrPW;
	/// 设置数据库超时等待时间
	m_pSqliteWraper->SetTimeOut(3000);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::BeginTransaction()
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(m_bBeginTrans)
	{
		m_strLastErrInfo = _T("已经开始事务，不能嵌套事务操作。");
		return E_FAIL;//已经开始事务
	}
	if(!m_pSqliteWraper->Begin())
		return E_FAIL;
	m_bBeginTrans = TRUE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::CommitTransaction()
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(!m_bBeginTrans)
	{
		m_strLastErrInfo = _T("没有开启事务功能。");
		return E_FAIL;
	}
	FinalizeStatement();///提交事务前先释放原来的查询语句
	short sIndex = 0;
	bool bCommitFlag = m_pSqliteWraper->Commit();
	while(!bCommitFlag || sIndex>5)
	{
		::Sleep(200);
		sIndex++;
		bCommitFlag = m_pSqliteWraper->Commit();
	}
	if(!bCommitFlag)
		return E_FAIL;
	m_bBeginTrans = FALSE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::RollbackTransaction(void)
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(!m_bBeginTrans)
	{
		m_strLastErrInfo = _T("没有开启事务功能。");
		return E_FAIL;
	}
	if(!m_pSqliteWraper->Rollback())
		return E_FAIL;
	m_bBeginTrans = FALSE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::CompressDB(void)
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(m_bBeginTrans)
	{
		m_strLastErrInfo = _T("在事务中不能执行此命令。");
		return E_FAIL;
	}
	if(!m_pSqliteWraper->DirectStatement(CComBSTR(_T("VACUUM;"))))
	{
		TCHAR *szErrInfo=NULL;
		CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqliteWraper->LastError(),&szErrInfo);
		if(NULL != szErrInfo)
		{
			m_strLastErrInfo = szErrInfo;
			delete []szErrInfo;
			szErrInfo=NULL;
		}
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::ExecCommond(BSTR bstrSqlString)
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == bstrSqlString)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()>7 && 0==strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("不能执行SELECT 查询语句。");
		return E_FAIL;//命令调用错误
	}
	if(!m_pSqliteWraper->DirectStatement(CComBSTR(bstrSqlString)))
	{
		TCHAR *szErrInfo=NULL;
		CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqliteWraper->LastError(),&szErrInfo);
		if(NULL != szErrInfo)
		{
			m_strLastErrInfo = szErrInfo;
			delete []szErrInfo;
			szErrInfo=NULL;
		}
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::PrepareCommond(BSTR bstrSqlString,ISqlite3Statement** pVal)
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(!bstrSqlString)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()>7 && 0==strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("不能执行SELECT 查询语句。");
		return E_FAIL;//命令调用错误
	}
	CComObject< CSqlite3ConnectStatement > *Sqlite3ConnectStatement=NULL;
#ifdef NEED_FINAL_CONSTRUCT
	CComObject<CSqlite3ConnectStatement>::CreateInstance(&(Sqlite3ConnectStatement));
#else
	Sqlite3ConnectStatement = new CComObject< CSqlite3ConnectStatement >;
#endif
	ATLASSERT(Sqlite3ConnectStatement);
	CSqlite3Statement *pSqlite3Statement = m_pSqliteWraper->Statement(bstrSqlString);
	if(!pSqlite3Statement)
		return E_FAIL;
	if(!pSqlite3Statement->IsOK())
	{
		ATLASSERT(0);
		return E_FAIL;
	}
	HRESULT hRet = Sqlite3ConnectStatement->SetObj(pSqlite3Statement);
	if(SUCCEEDED(hRet))
		hRet = Sqlite3ConnectStatement->QueryInterface(IID_ISqlite3Statement,(LPVOID *)pVal);
	return hRet;
}

STDMETHODIMP CSqlite3Connect::get_TransactionStatus(VARIANT_BOOL* pVal)
{
	// TODO: 在此添加实现代码
	if(m_bReadOnly)
		return E_FAIL;
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	if(m_bBeginTrans)
		*pVal = VARIANT_TRUE;
	else
		*pVal = VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::SysBackup(BSTR bstrFileName)
{
	// TODO: 在此添加实现代码
	if(m_strDbFile.IsEmpty() || m_bReadOnly)
		return ERROR_INVALID_PARAMETER;
	ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(m_strDbFile);
	if(!dwFileSize)
		return E_FAIL;/// 文件异常
	CString strSysDB(CBaseFuncLib::GetSpecialFolderPath(CSIDL_COMMON_APPDATA)+bstrFileName);
	::SetFileAttributes(strSysDB,FILE_ATTRIBUTE_NORMAL);
	::CopyFile(m_strDbFile,strSysDB,FALSE);
	::SetFileAttributes(strSysDB,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::Close()
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	FinalizeStatement();
	try
	{
		if(m_pSqliteWraper && m_bBeginTrans)
			RollbackTransaction();//缺省放弃事务
	}
	catch ( ... )
	{
		m_strLastErrInfo = _T("回滚事务异常。");
		return E_FAIL;
	}
	if(NULL != m_pSqliteWraper)
	{
		delete m_pSqliteWraper;
		m_pSqliteWraper = NULL;

		if(NULL != m_hSyncEvent && INVALID_HANDLE_VALUE != m_hSyncEvent)
		{
			/// 关闭了，设置信号，告诉等待访问的连接可以继续进行
			::SetEvent(m_hSyncEvent);
			::Sleep(100);
			DWORD dwFlag = 0;
			BOOL bGetFlag = GetHandleInformation(m_hSyncEvent,&dwFlag);
			if(bGetFlag)
			{
				dwFlag = dwFlag & HANDLE_FLAG_PROTECT_FROM_CLOSE;
				if(dwFlag)
					::CloseHandle(m_hSyncEvent);
			}
			m_hSyncEvent = NULL;
		}
#ifdef _DEBUG
#ifdef APP_LOG_ENABLE
		//CString strLogInfo(_T(""));
		//strLogInfo.Format(_T("Sqlite3Connect::Close 成功关闭：%s"),m_strDbFile);
		//WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
#endif
	}
	m_bReadOnly = FALSE;
	m_strPassword.Empty();
	m_strDbFile.Empty();
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::ExecQuery(BSTR bstrSqlString)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(!bstrSqlString)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()<7 || 0 != strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("不能执行非SELECT 命令语句。");
		return E_FAIL;//命令调用错误，非查询语句
	}
	FinalizeStatement();
	m_pSqlite3Statement = m_pSqliteWraper->Statement(bstrSqlString);
	if(!m_pSqlite3Statement)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::ExecQueryEx(BSTR bstrSqlString,ISqlite3Statement** pVal)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		CString strDBFile = m_strDbFile;
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	if(!bstrSqlString || !pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()<7 || 0 != strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("不能执行非SELECT 命令语句。");
		return E_FAIL;//命令调用错误，非查询语句
	}

	CComObject< CSqlite3ConnectStatement > *Sqlite3ConnectStatement=NULL;
#ifdef NEED_FINAL_CONSTRUCT
	CComObject<CSqlite3ConnectStatement>::CreateInstance(&(Sqlite3ConnectStatement));
#else
	Sqlite3ConnectStatement = new CComObject< CSqlite3ConnectStatement >;
#endif
	ATLASSERT(Sqlite3ConnectStatement);
	CSqlite3Statement *pSqlite3Statement = m_pSqliteWraper->Statement(bstrSqlString);
	if(!pSqlite3Statement || !pSqlite3Statement->IsOK())
		return E_FAIL;
	HRESULT hRet = Sqlite3ConnectStatement->SetObj(pSqlite3Statement);
	if(SUCCEEDED(hRet))
		hRet = Sqlite3ConnectStatement->QueryInterface(IID_ISqlite3Statement,(LPVOID *)pVal);
	return S_OK;
}

HRESULT CSqlite3Connect::FinalizeStatement()
{
	// TODO: 在此添加实现代码
	if(NULL == m_pSqlite3Statement)
		return S_OK;
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	try
	{
		delete m_pSqlite3Statement;
		m_pSqlite3Statement = NULL;
	}
	catch ( ... )
	{
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_TimeoutSecond(LONG* pVal)
{
	if(!pVal)
		return E_POINTER;
	*pVal = m_lOverTimeSecond;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::put_TimeoutSecond(long newVal)
{
	if(!newVal)
		return E_INVALIDARG;
	m_lOverTimeSecond = newVal;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_ParaValue(BSTR bstrKeyName,BSTR* pVal)
{
	if(NULL == bstrKeyName || NULL == pVal)
		return E_POINTER;
	if(NULL == m_pSqliteWraper)
		return E_POINTER;
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s=\'%s\'"),\
		SQLITEDB_OPTIONRECORD_FIELDNAME2,SQLITEDB_TABLE_SYSPARA,\
		SQLITEDB_OPTIONRECORD_FIELDNAME1,(CString )bstrKeyName);
	CComPtr <ISqlite3Statement> spiSqlite3Statement = NULL;
	HRESULT hRet = this->ExecQueryEx(CComBSTR(strSqlCmd),&spiSqlite3Statement);
	strSqlCmd.Empty();
	BOOL bExistFlag = FALSE;
	while(NULL != spiSqlite3Statement && SUCCEEDED(spiSqlite3Statement->NextRow()))
	{
		bExistFlag = TRUE;
		CComBSTR bstrVal;
		hRet = spiSqlite3Statement->GetValueString(0,&bstrVal);
		bstrVal.CopyTo(pVal);
		bstrVal.Empty();
		break;
	}
	spiSqlite3Statement = NULL;
	if(bExistFlag)
		return S_OK;
	return E_FAIL;
}

STDMETHODIMP CSqlite3Connect::put_ParaValue(BSTR bstrKeyName,BSTR newVal)
{
	if(m_bReadOnly)
		return E_FAIL;
	if(NULL == bstrKeyName)
		return E_INVALIDARG;
	if(NULL == m_pSqliteWraper)
		return E_POINTER;
	CString strNewVal(_T(""));
	if(NULL != newVal)
		strNewVal = newVal;
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("SELECT %s FROM [%s] WHERE %s=\'%s\'"),\
		SQLITEDB_OPTIONRECORD_FIELDNAME2,SQLITEDB_TABLE_SYSPARA,\
		SQLITEDB_OPTIONRECORD_FIELDNAME1,(CString )bstrKeyName);
	CComPtr <ISqlite3Statement> spiStatement = NULL;
	HRESULT hRet = this->ExecQueryEx(CComBSTR(strSqlCmd),&spiStatement);
	strSqlCmd.Empty();
	BOOL bExistFlag = FALSE;
	if(NULL != spiStatement)
	{
		hRet = spiStatement->NextRow();
		if(SUCCEEDED(hRet))
			bExistFlag = TRUE;
		spiStatement = NULL;
	}
	if(bExistFlag)
	{
		/// 已经存在
		strSqlCmd.Format(_T("UPDATE [%s] SET %s=\'%s\' WHERE %s=\'%s\'"),\
			SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME2,\
			strNewVal,SQLITEDB_OPTIONRECORD_FIELDNAME1,(CString )bstrKeyName);
	}
	else
	{
		strSqlCmd.Format(_T("INSERT INTO [%s] (%s,%s) \
							VALUES(\'%s\',\'%s\');"),
							SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_OPTIONRECORD_FIELDNAME2,\
							(CString )bstrKeyName,strNewVal);
	}
	hRet = this->ExecCommond(CComBSTR(strSqlCmd));
	if(FAILED(hRet))
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("Sqlite3Connect::put_ParaValue 执行 %s 数据库%s失败，错误原因：%s"),\
			strSqlCmd,m_strDbFile,m_strLastErrInfo);
		WRITELOGTOFILE(strLogInfo);
#endif
	}
	strSqlCmd.Empty();
	return hRet;
}

STDMETHODIMP CSqlite3Connect::get_TableCount(VARIANT_BOOL bTemFlag,LONG* pVal)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = 0;
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	CString strSqlString(_T(""));
	if(VARIANT_FALSE == bTemFlag)
		strSqlString.Format(_T("select name from SQLITE_MASTER"));
	else
		strSqlString.Format(_T("select name from SQLITE_TEMP_MASTER"));

	CSqlite3Statement *pSqlite3Statement = m_pSqliteWraper->Statement(CComBSTR(strSqlString));
	if(!pSqlite3Statement)
		return E_FAIL;
	*pVal = pSqlite3Statement->Records();
	delete pSqlite3Statement;
	pSqlite3Statement = NULL;

	return S_OK;
}

//#define SQLITE_TEST_DB  //用于测试数据库表内容

STDMETHODIMP CSqlite3Connect::TableIsExist(BSTR bstrTableName,VARIANT_BOOL bTemFlag, VARIANT_BOOL* pVal)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!bstrTableName || !pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = VARIANT_FALSE;
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	CString strSqlString(_T(""));
#ifndef SQLITE_TEST_DB
	if(VARIANT_FALSE == bTemFlag)
		strSqlString.Format(_T("select name from SQLITE_MASTER"));
	else
		strSqlString.Format(_T("select name from SQLITE_TEMP_MASTER"));
#else
	if(VARIANT_FALSE == bTemFlag)
		strSqlString.Format(_T("select * from SQLITE_MASTER"));
	else
		strSqlString.Format(_T("select * from SQLITE_TEMP_MASTER"));
#endif

	CSqlite3Statement *pSqlite3Statement = m_pSqliteWraper->Statement(CComBSTR(strSqlString));
	if(!pSqlite3Statement)
		return E_FAIL;
#ifdef SQLITE_TEST_DB
	int iCount = pSqlite3Statement->Fields();
	for(int index = 0;index<iCount;index++)
	{
		TCHAR *szFieldName = NULL;
		CBaseFuncLib::Utf8ToUS2((LPCSTR )pSqlite3Statement->FieldName(index),&szFieldName);
		CString strFieldName(szFieldName);
		delete []szFieldName;
		szFieldName = NULL;
	}
#endif
	while(pSqlite3Statement->NextRow())
	{
#ifndef SQLITE_TEST_DB
		CString strValue(pSqlite3Statement->ValueString(0));
		if(0 == strValue.CompareNoCase(bstrTableName))
		{
			*pVal = VARIANT_TRUE;
			break;
		}
#else
		for(int index = 0;index<iCount;index++)
		{
			CString strTemValue(pSqlite3Statement->ValueString(index));
			if(0 == index)
			{
				if(0 == strTemValue.CompareNoCase(bstrTableName))
					*pVal = VARIANT_TRUE;
			}
			delete []szValue;
			szValue = NULL;
		}
#endif
	}
	delete pSqlite3Statement;
	pSqlite3Statement = NULL;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetTableSql(BSTR bstrTableName,VARIANT_BOOL bTemFlag, BSTR* pVal)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!bstrTableName || !pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	CString strSqlString(_T(""));
	if(VARIANT_FALSE == bTemFlag)
		strSqlString.Format(_T("select name,sql from SQLITE_MASTER"));
	else
		strSqlString.Format(_T("select name,sql from SQLITE_TEMP_MASTER"));
	CSqlite3Statement *pSqlite3Statement = m_pSqliteWraper->Statement(CComBSTR(strSqlString));
	if(!pSqlite3Statement)
		return E_FAIL;
	while(pSqlite3Statement->NextRow())
	{
		CString strValue(pSqlite3Statement->ValueString(0));
		if(0 == strValue.CompareNoCase(bstrTableName))
		{
			CString strSql(pSqlite3Statement->ValueString(1));
			CComBSTR bstrTem(strSql);
			bstrTem.CopyTo(pVal);
			break;
		}
	}
	delete pSqlite3Statement;
	pSqlite3Statement = NULL;

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_FieldCount(ULONG* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	(*pVal) = m_pSqlite3Statement->Fields();
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_RecordCount(ULONG* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	/// 获得数量
	while(m_pSqlite3Statement->NextRow())
	{
		(*pVal)++;
	}
	m_pSqlite3Statement->Reset();/// 恢复状态

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetFieldName(LONG lFieldIndex, BSTR* pbstrName)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pbstrName)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	TCHAR *szFieldName = NULL;
	CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqlite3Statement->FieldName(lFieldIndex),&szFieldName);
	if(!szFieldName)
	{
		m_strLastErrInfo = _T("内存不足。");
		return E_POINTER;
	}
	CString strFieldName(szFieldName);
	delete []szFieldName;
	szFieldName = NULL;

	CComBSTR bstrName(strFieldName);
	bstrName.CopyTo(pbstrName);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::NextRow(void)
{
	// TODO: 在此添加实现代码
	m_strLastErrInfo.Empty();
	//	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	if(!m_pSqlite3Statement->NextRow())
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueString(LONG lFieldIndex, BSTR* pbstrValue)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pbstrValue)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	CComBSTR bstrValue(m_pSqlite3Statement->ValueString(lFieldIndex));
	bstrValue.CopyTo(pbstrValue);
	bstrValue.Empty();
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueInt(LONG lFieldIndex,LONG* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = m_pSqlite3Statement->ValueInt(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueLong(LONG lFieldIndex,LONGLONG* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	(*pVal) = m_pSqlite3Statement->ValueLong(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueDouble(LONG lFieldIndex,DOUBLE* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("请先执行SELECT 语句操作ExecQuery。");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("无效参数错误。");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = m_pSqlite3Statement->ValueFloat(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_LastErrorInfo(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(!m_strLastErrInfo.IsEmpty())
	{
		CComBSTR bstrErr(m_strLastErrInfo);//COM组件封装提示的错误信息
		bstrErr.CopyTo(pVal);
		return S_OK;
	}
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	TCHAR *szErrInfo=NULL;
	CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqliteWraper->LastError(),&szErrInfo);
	if(!szErrInfo)
	{
		m_strLastErrInfo = _T("内存不足。");
		return E_POINTER;
	}
	CString strErrorInfo(szErrInfo);
	delete []szErrInfo;
	szErrInfo=NULL;
	CComBSTR bstrErr(strErrorInfo);//COM组件封装提示的错误信息
	bstrErr.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::put_Password(BSTR newVal)
{
	// TODO: 在此添加实现代码
	if(0 == m_dwCurrentThreadID)
		m_dwCurrentThreadID = ::GetCurrentThreadId();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(m_strPassword.GetLength() && NULL != m_pSqliteWraper)
	{
		/// 修改密码
		m_pSqliteWraper->ChangeDBPassword(newVal);
	}
	if(!m_strPassword.GetLength() && NULL != m_pSqliteWraper)
	{
		/// 设置密码
		m_pSqliteWraper->SetPassword(newVal);
	}
	m_strPassword.Empty();
	if(NULL != newVal)
		m_strPassword = newVal;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_DriverVersion(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	CComBSTR bstrVer(SQLITE_VERSION);
	bstrVer.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_DbPathFile(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	CComBSTR bstrDBFile(m_strDbFile);
	bstrDBFile.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_ReadOnly(VARIANT_BOOL* pVal)
{
	// TODO: 在此添加实现代码
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(m_bReadOnly)
		(*pVal) = VARIANT_TRUE;
	else
		(*pVal) = VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_DBVersion(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("数据库访问包装对象为空。");
		return E_POINTER;
	}
	//获得当前打开文件数据库版本
	CComBSTR bstrVer(_T("3.09"));
	bstrVer.CopyTo(pVal);

	return S_OK;
}
