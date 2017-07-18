// Sqlite3Statement.cpp : CSqlite3Statement 的实现

#include "stdafx.h"
#include "Sqlite3Statement.h"

#include "BaseFuncLib.h"


//
// Default ctor.
//
CSqlite3Statement::CSqlite3Statement()
	:m_stmt(NULL)
	,m_db(NULL)
	,m_pBindValue(NULL)
	,m_strSql(_T(""))
{
}

CSqlite3Statement::CSqlite3Statement(LPCTSTR statement, sqlite3* db)
	:m_stmt(NULL)
	,m_db(db)
	,m_strSql(statement)
	,m_pBindValue(NULL)
{
	int rc = SQLITE_OK;
	char *inbuffer = NULL;
	CBaseFuncLib::US2ToUtf8(statement,&inbuffer);
	OutputDebugStringA(inbuffer);
	OutputDebugStringA("\n");
	if(NULL != inbuffer)
	{
		rc = sqlite3_prepare(db, inbuffer, -1, &m_stmt, 0 );
		delete []inbuffer;
		inbuffer = NULL;
	}
	else
		rc = SQLITE_NOMEM;

	if (rc != SQLITE_OK)
	{
		m_stmt = NULL;
		/// SQL命令错误的话，会执行到这
		ATLASSERT(0);
	}
}

//
// Do we really want to finalize or do we want to leave that up to
// the user?
//
CSqlite3Statement::~CSqlite3Statement()
{
	if(NULL != m_pBindValue)
	{
		delete []m_pBindValue;
		m_pBindValue = NULL;
	}

	m_strSql.Empty();

	if (NULL != m_stmt)
	{
		sqlite3_finalize(m_stmt);
		m_stmt = NULL;
	}
	m_db = NULL;
}

bool CSqlite3Statement::Bind(int pos_zero_indexed, LPCTSTR value)
{
	int rc = SQLITE_OK;
	bool fResult = TRUE;
	CString v = CString(value);
	char *inbuffer=NULL;
	CBaseFuncLib::US2ToUtf8(v,&inbuffer);
	if(NULL != inbuffer)
	{
		rc = sqlite3_bind_text(m_stmt, pos_zero_indexed+1,
			inbuffer, v.GetLength(), SQLITE_TRANSIENT);
		delete []inbuffer;
		inbuffer = NULL;
	}
	else
		rc = SQLITE_NOMEM;

	if (rc != SQLITE_OK )
	{
		fResult = false;
	}

	return fResult;
}

bool CSqlite3Statement::Bind(int pos_zero_indexed, double value)
{
	bool fResult = TRUE;
	int rc = sqlite3_bind_double( m_stmt, pos_zero_indexed+1,value );

	if (rc != SQLITE_OK )
	{
		fResult = false;
	}

	return fResult;
}

bool CSqlite3Statement::BindBlobData(int pos_zero_indexed,BYTE* pData,ULONG nDataLen)
{
	int iRet= sqlite3_bind_blob(m_stmt,pos_zero_indexed,pData,nDataLen,NULL);
	if(SQLITE_OK == iRet)
		return true;
	if(NULL != m_db)
	{
		TCHAR *szErrInfo=NULL;
		CBaseFuncLib::Utf8ToUS2(sqlite3_errmsg(m_db),&szErrInfo);
		if(NULL != szErrInfo)
		{
			CString strErrInfo(szErrInfo);
			delete []szErrInfo;
			szErrInfo = NULL;

#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("BindBlobData操作失败，原因：%s"),strErrInfo);
			WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
			strErrInfo.Empty();
		}
	}
	return false;
}

bool CSqlite3Statement::BindBlobFile(int pos_zero_indexed,LPCTSTR FilePath)
{
	if(NULL != m_pBindValue)
	{
		delete []m_pBindValue;
		m_pBindValue = NULL;
	}

	/// 读取文件流来绑定
	HANDLE hFile = CreateFile(FilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(INVALID_HANDLE_VALUE == hFile || NULL == hFile)
	{
		//打开文件失败
		return false;
	}

	// 取文件长度
	DWORD nFileSize = GetFileSize(hFile,NULL);
	m_pBindValue = new BYTE[nFileSize];
	if(NULL == m_pBindValue){
		///分配内存失败
		return false;
	}

	memset(m_pBindValue,0,nFileSize*sizeof(BYTE));
	DWORD dwReadSize = 0;

	// 读取数据到MemBlock中
	::ReadFile (hFile, m_pBindValue,nFileSize, &dwReadSize,NULL);

	// 调试断言:读出的数据长度等于文件长度
	ATLASSERT(nFileSize == dwReadSize);
	CloseHandle(hFile);

	int iRet= sqlite3_bind_blob(m_stmt,pos_zero_indexed,m_pBindValue,dwReadSize,NULL);
	if(SQLITE_OK == iRet)
		return true;
	if(NULL != m_db)
	{
		TCHAR *szErrInfo=NULL;
		CBaseFuncLib::Utf8ToUS2(sqlite3_errmsg(m_db),&szErrInfo);
		CString strErrInfo(szErrInfo);
		delete []szErrInfo;
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("BindBlobFile操作失败，原因：%s"),strErrInfo);
		WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
		strErrInfo.Empty();
	}
	return false;
}

bool CSqlite3Statement::Bind (int pos_zero_indexed, void *pv,ULONG uiLength)
{
	bool fResult = TRUE;
	int rc = sqlite3_bind_blob( m_stmt, pos_zero_indexed+1, pv, uiLength,NULL);

	if (rc != SQLITE_OK )
	{
		fResult = false;
	}

	return fResult;
}

bool CSqlite3Statement::Bind(int pos_zero_indexed, int value)
{
	bool fResult = TRUE;
	int rc = sqlite3_bind_int( m_stmt, pos_zero_indexed+1, value );

	if (rc != SQLITE_OK )
	{
		fResult = false;
	}

	return fResult;
}

bool CSqlite3Statement::BindNull(int pos_zero_indexed)
{
	bool fResult = TRUE;
	int rc = sqlite3_bind_null( m_stmt, pos_zero_indexed+1);

	if (rc != SQLITE_OK )
	{
		fResult = false;
	}

	return fResult;
}

bool CSqlite3Statement::Execute()
{
	int rc = sqlite3_step(m_stmt);

	if(SQLITE_BUSY == rc)
	{
		if(-1 == m_strSql.Find(_T("SELECT")) && -1 == m_strSql.Find(_T("select")))
		{
			SHORT nTryCount = 5;
			while(SQLITE_BUSY == rc && nTryCount--)
			{
				::Sleep(200);
				rc = sqlite3_step(m_stmt);
			}
		}
	}
	else if(SQLITE_LOCKED == rc)
	{
		/// 锁定，重试
		SHORT nTryCount = 20;
		while(SQLITE_LOCKED == rc && nTryCount--)
		{
			::Sleep(50);
			rc = sqlite3_step(m_stmt);
		}
	}

	if(NULL != m_pBindValue)
	{
		delete []m_pBindValue;
		m_pBindValue = NULL;
	}

	if ( rc == SQLITE_BUSY )
		return false;

	if ( rc == SQLITE_ERROR )
		return false;

	if ( rc == SQLITE_MISUSE )
		return false;

	if ( rc != SQLITE_DONE )
		return false;

	sqlite3_reset(m_stmt);
	return true;
}

int CSqlite3Statement::Fields()
{
	return sqlite3_column_count(m_stmt);
}

int CSqlite3Statement::Records()
{
	return sqlite3_data_count(m_stmt);
}

const char* CSqlite3Statement::FieldName(int pos_zero_indexed)
{
	return sqlite3_column_name(m_stmt, pos_zero_indexed);
}

CSqlite3Statement::EDataType CSqlite3Statement::DataType(int pos_zero_indexed)
{
	return EDataType(sqlite3_column_type(m_stmt, pos_zero_indexed));
}

int CSqlite3Statement::FieldType(int pos_zero_indexed)
{
	return sqlite3_column_type(m_stmt, pos_zero_indexed);
}

int CSqlite3Statement::ValueInt(int pos_zero_indexed)
{
	return sqlite3_column_int(m_stmt, pos_zero_indexed);
}

sqlite3_int64 CSqlite3Statement::ValueLong(int pos_zero_indexed)
{
	return sqlite3_column_int64(m_stmt, pos_zero_indexed);
}

double CSqlite3Statement::ValueFloat(int pos_zero_indexed)
{
	return sqlite3_column_double(m_stmt,pos_zero_indexed);
}

CString CSqlite3Statement::ValueString(int pos_zero_indexed)
{
	WCHAR *szValue = NULL;
	CString strValue(_T(""));
	const char *szText =(const char *)sqlite3_column_text(m_stmt, pos_zero_indexed);
	sqlite3_column_bytes(m_stmt, pos_zero_indexed);
	CBaseFuncLib::Utf8ToUS2(szText,&szValue);
	if(NULL != szValue)
	{
		strValue.Format(_T("%s"),szValue);
		delete []szValue;
		szValue = NULL;
	}
	return strValue;
}

void *CSqlite3Statement::ValueBlob(int pos_zero_indexed)
{
	// SQLite3利用函数sqlite3_column_blob来返回blob字段的内容的指针，
	// 并通过sqlite3_column_bytes来返回对应该字段的长度。
	const void *pBlob= sqlite3_column_blob(m_stmt, pos_zero_indexed);
	ATLASSERT(pBlob);
	return (void *)pBlob;
}

BYTE* CSqlite3Statement::ValueBlob(int pos_zero_indexed,ULONG& nLen)
{
	// SQLite3利用函数sqlite3_column_blob来返回blob字段的内容的指针，
	// 并通过sqlite3_column_bytes来返回对应该字段的长度。
	nLen = sqlite3_column_bytes(m_stmt, pos_zero_indexed);
	if(!nLen)
		return NULL;
	return (BYTE* )ValueBlob(pos_zero_indexed);
}

int CSqlite3Statement::ValueBlobFile(int pos_zero_indexed,LPCTSTR FilePath)
{
	// SQLite3利用函数sqlite3_column_blob来返回blob字段的内容的指针，
	// 并通过sqlite3_column_bytes来返回对应该字段的长度。
	int iBufferLength = sqlite3_column_bytes(m_stmt, pos_zero_indexed);
	void *pValue = ValueBlob(pos_zero_indexed);
	if(!pValue)
	{
		TCHAR *szErrInfo=NULL;
		CBaseFuncLib::Utf8ToUS2((LPCSTR )sqlite3_errmsg(m_db),&szErrInfo);
		CString strErrInfo (szErrInfo);
		delete []szErrInfo;
		return 0;
	}
	///保存到文件
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD  dwBytesWritten = 0;
	if(CBaseFuncLib::IsPathExist(FilePath))
		::SetFileAttributes(FilePath,FILE_ATTRIBUTE_NORMAL);
	hFile = CreateFile ( FilePath,	GENERIC_WRITE, 
		0,						// Do not share
		NULL,                   // No security
		CREATE_ALWAYS,          // Create file only
		FILE_ATTRIBUTE_NORMAL,  // Normal file
		NULL);                  // No template file

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwLastErr = ::GetLastError();
		return 0;
	}
	BYTE* pt=(BYTE *)pValue;
	for(;;)
	{
		if ( iBufferLength >= ZORROSOFT_BLOCKWRITE_SIZE ) {
			
			// 如果数据的长度iBufferLength大于ZORROSOFT_BLOCKWRITE_SIZE，那就每次写入ZORROSOFT_BLOCKWRITE_SIZE字节
			// 否则将iBufferLength长度的数据直接写入
			if( !WriteFile(hFile,pt,ZORROSOFT_BLOCKWRITE_SIZE,&dwBytesWritten,NULL ) )
			{
				CloseHandle ( hFile );	
				DeleteFile( FilePath );
				return 0;
			}        			

			/**
			* @brief             FlushFileBuffers(Flushes the buffers of a specified file and causes all buffered data to be written to a file.)
			*                    How to ensure all data has been physically written to disk? the answer is use FlushFileBuffers
			* @Function          刷新指定文件的缓冲区，并将所有缓冲数据写入文件。
			*
			* @param[  _In_   ]  HANDLE hFile
			*
			* @return            If the function succeeds, the return value is nonzero.
			*
			* @Date xzm_@_2017/06/30  13:40:11
			*/
			FlushFileBuffers( hFile );
			iBufferLength -= ZORROSOFT_BLOCKWRITE_SIZE;
			pt += ZORROSOFT_BLOCKWRITE_SIZE;
		}
		else
		{		
			if( !WriteFile (hFile,pt,iBufferLength,&dwBytesWritten,NULL ) )
			{
				CloseHandle ( hFile );
				DeleteFile( FilePath );
				return 0;
			}
			FlushFileBuffers( hFile );
			break;
		}
	}
	CloseHandle ( hFile );
	/// 不能删除
	//	delete []pValue;
	return 1;
}

bool CSqlite3Statement::Reset()
{
	int rc = sqlite3_reset(m_stmt);
	if ( rc == SQLITE_OK ) 
		return true;
	return false;
}

bool CSqlite3Statement::NextRow()
{
	CString szError;
	int rc = sqlite3_step(m_stmt);

	if ( rc == SQLITE_ROW )
		return true;

	if ( rc == SQLITE_DONE )
	{
		sqlite3_reset(m_stmt);
		return false;
	}
	else if ( rc == SQLITE_MISUSE )
		szError = _T("CSqlite3Statement::NextRow SQLITE_MISUSE");
	else if ( rc == SQLITE_BUSY )
		szError = _T("CSqlite3Statement::NextRow SQLITE_BUSY");
	else if ( rc == SQLITE_ERROR )
		szError = _T("CSqlite3Statement::NextRow SQLITE_ERROR");

	//   ::MessageBox(0, szError, 0, 0);
	return false;
}