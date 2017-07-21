// Sqlite3Connect.cpp : CSqlite3Connect ��ʵ��

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
		/// ���ļ�ʧ��
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
	// TODO: �ڴ����ʵ�ִ���
	if(!bstrDbFile)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	/// �ж������ļ��Ϸ���
	if(!CBaseFuncLib::IsPathExist(bstrDbFile))
	{
		m_strLastErrInfo = bstrDbFile;
		m_strLastErrInfo += _T(" ���ݿ��ļ������ڡ�");
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
		m_strLastErrInfo = _T("�ڴ治�㡣");
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
			/// �Ѿ��б仯
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
		/// �򿪱���
		::CopyFile(bstrDbFile,strReadDB,FALSE);
	}

	BOOL bOpenFlag = m_pSqliteWraper->Open(strReadDB,bstrPW);
	if(!bOpenFlag)
	{
		m_strLastErrInfo = strReadDB + _T(" ��ʧ�ܡ�");
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
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	if(NULL == bstrDbFile)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	/// �ж������ļ��Ϸ���
	if(VARIANT_FALSE == bCreateFlag && !CBaseFuncLib::IsPathExist(bstrDbFile))
	{
		m_strLastErrInfo = bstrDbFile;
		m_strLastErrInfo += _T(" ���ݿ��ļ������ڡ�");
		return E_FAIL;
	}
	if(VARIANT_TRUE == bCreateFlag && CBaseFuncLib::IsPathExist(bstrDbFile))
	{
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(bstrDbFile);
		if(!dwFileSize)
		{
			/// �������ڴ���������ɾ��
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("Sqlite::Open �޴�С���ݿ��ļ� %s"),\
				CString(bstrDbFile));
			WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
			//			::SetFileAttributes(bstrDbFile,FILE_ATTRIBUTE_NORMAL);
			//			::DeleteFile(bstrDbFile);
			/// �ȴ��������򴴽�
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
	/// ȫ���¼�����֤��������������򶼿��Է���ʹ��
	CString strEventPath(_T("Global\\")),strDBName(bstrDbFile);
	/// ȥ�����Ϸ����ַ���
	strDBName.Replace(_T("\\"),_T(""));
	strDBName.Replace(_T(":"),_T(""));
	strDBName.Replace(_T(" "),_T(""));
	if(strDBName.IsEmpty())
		return E_FAIL;
	strEventPath += strDBName;

	/// �����¼��İ�ȫ����ȷ��ϵͳ�����������̶����Բ���
	SECURITY_DESCRIPTOR secutityDese;
	::InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION);
	/// �Ѳ���������ΪNULL����ʵ�൱�ڽ�object�İ�ȫ���𽵵�����ͣ����еķ������󶼽��ɹ�
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
			/// �Ѿ����򿪣���Ҫ�ȴ�
			DWORD dwRet = WaitForSingleObject(m_hSyncEvent,1000*m_lOverTimeSecond);///��ʱ�ȴ�
			if(WAIT_OBJECT_0 != dwRet)
			{
				/// �ͷ��¼����
				::CloseHandle(m_hSyncEvent);
				m_hSyncEvent = NULL;
				/// �򿪳�ʱ
				m_strLastErrInfo.Format(_T("���ݿ�%s��ʱ %d ��δ��"),\
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
				strLogInfo.Format(_T("�����ݿ� %s �ȴ� %d ��"),\
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
		m_strLastErrInfo = _T("�ڴ治�㡣");
		return E_POINTER;
	}
	BOOL bOpenFlag = m_pSqliteWraper->Open(bstrDbFile,bstrPW);
	if(!bOpenFlag)
	{
		m_strLastErrInfo = strEventPath + _T(" ��ʧ�ܡ�ԭ��");
		m_strLastErrInfo += m_pSqliteWraper->LastError();
		Close();
		return E_FAIL;
	}
	m_strDbFile.Empty();
	m_strDbFile = bstrDbFile;
	m_bReadOnly = FALSE;
	if(NULL != bstrPW)
		m_strPassword = bstrPW;
	/// �������ݿⳬʱ�ȴ�ʱ��
	m_pSqliteWraper->SetTimeOut(3000);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::BeginTransaction()
{
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(m_bBeginTrans)
	{
		m_strLastErrInfo = _T("�Ѿ���ʼ���񣬲���Ƕ�����������");
		return E_FAIL;//�Ѿ���ʼ����
	}
	if(!m_pSqliteWraper->Begin())
		return E_FAIL;
	m_bBeginTrans = TRUE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::CommitTransaction()
{
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(!m_bBeginTrans)
	{
		m_strLastErrInfo = _T("û�п��������ܡ�");
		return E_FAIL;
	}
	FinalizeStatement();///�ύ����ǰ���ͷ�ԭ���Ĳ�ѯ���
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
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(!m_bBeginTrans)
	{
		m_strLastErrInfo = _T("û�п��������ܡ�");
		return E_FAIL;
	}
	if(!m_pSqliteWraper->Rollback())
		return E_FAIL;
	m_bBeginTrans = FALSE;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::CompressDB(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(m_bBeginTrans)
	{
		m_strLastErrInfo = _T("�������в���ִ�д����");
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
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == bstrSqlString)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()>7 && 0==strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("����ִ��SELECT ��ѯ��䡣");
		return E_FAIL;//������ô���
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
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(!bstrSqlString)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()>7 && 0==strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("����ִ��SELECT ��ѯ��䡣");
		return E_FAIL;//������ô���
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
	// TODO: �ڴ����ʵ�ִ���
	if(m_bReadOnly)
		return E_FAIL;
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
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
	// TODO: �ڴ����ʵ�ִ���
	if(m_strDbFile.IsEmpty() || m_bReadOnly)
		return ERROR_INVALID_PARAMETER;
	ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(m_strDbFile);
	if(!dwFileSize)
		return E_FAIL;/// �ļ��쳣
	CString strSysDB(CBaseFuncLib::GetSpecialFolderPath(CSIDL_COMMON_APPDATA)+bstrFileName);
	::SetFileAttributes(strSysDB,FILE_ATTRIBUTE_NORMAL);
	::CopyFile(m_strDbFile,strSysDB,FALSE);
	::SetFileAttributes(strSysDB,FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_SYSTEM);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::Close()
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	FinalizeStatement();
	try
	{
		if(m_pSqliteWraper && m_bBeginTrans)
			RollbackTransaction();//ȱʡ��������
	}
	catch ( ... )
	{
		m_strLastErrInfo = _T("�ع������쳣��");
		return E_FAIL;
	}
	if(NULL != m_pSqliteWraper)
	{
		delete m_pSqliteWraper;
		m_pSqliteWraper = NULL;

		if(NULL != m_hSyncEvent && INVALID_HANDLE_VALUE != m_hSyncEvent)
		{
			/// �ر��ˣ������źţ����ߵȴ����ʵ����ӿ��Լ�������
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
		//strLogInfo.Format(_T("Sqlite3Connect::Close �ɹ��رգ�%s"),m_strDbFile);
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
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(!bstrSqlString)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()<7 || 0 != strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("����ִ�з�SELECT ������䡣");
		return E_FAIL;//������ô��󣬷ǲ�ѯ���
	}
	FinalizeStatement();
	m_pSqlite3Statement = m_pSqliteWraper->Statement(bstrSqlString);
	if(!m_pSqlite3Statement)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::ExecQueryEx(BSTR bstrSqlString,ISqlite3Statement** pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!m_pSqliteWraper)
	{
		CString strDBFile = m_strDbFile;
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	if(!bstrSqlString || !pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	CString strSqlString(bstrSqlString);
	if(strSqlString.GetLength()<7 || 0 != strSqlString.Left(7).CompareNoCase(_T("SELECT ")))
	{
		m_strLastErrInfo = _T("����ִ�з�SELECT ������䡣");
		return E_FAIL;//������ô��󣬷ǲ�ѯ���
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
	// TODO: �ڴ����ʵ�ִ���
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
		/// �Ѿ�����
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
		strLogInfo.Format(_T("Sqlite3Connect::put_ParaValue ִ�� %s ���ݿ�%sʧ�ܣ�����ԭ��%s"),\
			strSqlCmd,m_strDbFile,m_strLastErrInfo);
		WRITELOGTOFILE(strLogInfo);
#endif
	}
	strSqlCmd.Empty();
	return hRet;
}

STDMETHODIMP CSqlite3Connect::get_TableCount(VARIANT_BOOL bTemFlag,LONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = 0;
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
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

//#define SQLITE_TEST_DB  //���ڲ������ݿ������

STDMETHODIMP CSqlite3Connect::TableIsExist(BSTR bstrTableName,VARIANT_BOOL bTemFlag, VARIANT_BOOL* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!bstrTableName || !pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = VARIANT_FALSE;
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
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
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!bstrTableName || !pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
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
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	(*pVal) = m_pSqlite3Statement->Fields();
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_RecordCount(ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	/// �������
	while(m_pSqlite3Statement->NextRow())
	{
		(*pVal)++;
	}
	m_pSqlite3Statement->Reset();/// �ָ�״̬

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetFieldName(LONG lFieldIndex, BSTR* pbstrName)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pbstrName)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	TCHAR *szFieldName = NULL;
	CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqlite3Statement->FieldName(lFieldIndex),&szFieldName);
	if(!szFieldName)
	{
		m_strLastErrInfo = _T("�ڴ治�㡣");
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
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	//	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	if(!m_pSqlite3Statement->NextRow())
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueString(LONG lFieldIndex, BSTR* pbstrValue)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pbstrValue)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	CComBSTR bstrValue(m_pSqlite3Statement->ValueString(lFieldIndex));
	bstrValue.CopyTo(pbstrValue);
	bstrValue.Empty();
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueInt(LONG lFieldIndex,LONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = m_pSqlite3Statement->ValueInt(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueLong(LONG lFieldIndex,LONGLONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	(*pVal) = m_pSqlite3Statement->ValueLong(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::GetValueDouble(LONG lFieldIndex,DOUBLE* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	m_strLastErrInfo.Empty();
	if(NULL == m_pSqlite3Statement)
	{
		m_strLastErrInfo = _T("����ִ��SELECT ������ExecQuery��");
		return E_POINTER;
	}
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	*pVal = m_pSqlite3Statement->ValueFloat(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_LastErrorInfo(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(!m_strLastErrInfo.IsEmpty())
	{
		CComBSTR bstrErr(m_strLastErrInfo);//COM�����װ��ʾ�Ĵ�����Ϣ
		bstrErr.CopyTo(pVal);
		return S_OK;
	}
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	TCHAR *szErrInfo=NULL;
	CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqliteWraper->LastError(),&szErrInfo);
	if(!szErrInfo)
	{
		m_strLastErrInfo = _T("�ڴ治�㡣");
		return E_POINTER;
	}
	CString strErrorInfo(szErrInfo);
	delete []szErrInfo;
	szErrInfo=NULL;
	CComBSTR bstrErr(strErrorInfo);//COM�����װ��ʾ�Ĵ�����Ϣ
	bstrErr.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::put_Password(BSTR newVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(0 == m_dwCurrentThreadID)
		m_dwCurrentThreadID = ::GetCurrentThreadId();
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(m_strPassword.GetLength() && NULL != m_pSqliteWraper)
	{
		/// �޸�����
		m_pSqliteWraper->ChangeDBPassword(newVal);
	}
	if(!m_strPassword.GetLength() && NULL != m_pSqliteWraper)
	{
		/// ��������
		m_pSqliteWraper->SetPassword(newVal);
	}
	m_strPassword.Empty();
	if(NULL != newVal)
		m_strPassword = newVal;
	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_DriverVersion(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	CComBSTR bstrVer(SQLITE_VERSION);
	bstrVer.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_DbPathFile(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	CComBSTR bstrDBFile(m_strDbFile);
	bstrDBFile.CopyTo(pVal);

	return S_OK;
}

STDMETHODIMP CSqlite3Connect::get_ReadOnly(VARIANT_BOOL* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
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
	// TODO: �ڴ����ʵ�ִ���
	ATLASSERT(m_dwCurrentThreadID == ::GetCurrentThreadId());
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(!m_pSqliteWraper)
	{
		m_strLastErrInfo = _T("���ݿ���ʰ�װ����Ϊ�ա�");
		return E_POINTER;
	}
	//��õ�ǰ���ļ����ݿ�汾
	CComBSTR bstrVer(_T("3.09"));
	bstrVer.CopyTo(pVal);

	return S_OK;
}
