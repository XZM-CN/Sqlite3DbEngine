#include "StdAfx.h"
#include "Sqlite3Wrapper.h"

#include "BaseFuncLib.h"


#ifndef WIN64
#ifdef _DEBUG
#pragma comment(lib, "..\\bin\\Debug\\SqliteLib.lib")
#else
#pragma comment(lib, "..\\bin\\Release\\SqliteLib.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "\\Debug\\X64\\SqliteLib.lib")
#else
#pragma comment(lib, "\\Release\\X64\\SqliteLib.lib")
#endif
#endif

CSqlite3Wrapper::CSqlite3Wrapper(void)
{
}


CSqlite3Wrapper::~CSqlite3Wrapper(void)
{
}

bool CSqlite3Wrapper::Open(LPCTSTR db_file,LPCTSTR db_pw)
{
	bool fResult = TRUE;
	m_szName = db_file;

	sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
	int nThreadSafe = sqlite3_threadsafe();

	int rc = SQLITE_OK;
	char *inbuffer=NULL;
	int nLen = CBaseFuncLib::US2ToUtf8(m_szName,&inbuffer);
	if(NULL != inbuffer)
	{
		rc = sqlite3_open(inbuffer, &m_db);
		delete []inbuffer;
		inbuffer = NULL;
		/// 有密码，需要设置才能正常获取数据
		if(NULL != db_pw && lstrlen(db_pw))
		{
			nLen = CBaseFuncLib::US2ToUtf8(db_pw,&inbuffer);
			if(NULL != inbuffer)
			{
				rc = sqlite3_key(m_db,inbuffer,nLen-1);
				delete []inbuffer;
				inbuffer = NULL;
			}
		}
	}
	else
		rc = SQLITE_NOMEM;

	if ( rc != SQLITE_OK )
		fResult = false;

	return fResult;
}

bool CSqlite3Wrapper::ChangeDBPassword(LPCTSTR db_pw)
{
	bool bRet = false;
	if(NULL == m_db)
		return bRet;
	int rc = SQLITE_OK;
	if(NULL != db_pw && lstrlen(db_pw))
	{
		char *inbuffer=NULL;
		int nLen = CBaseFuncLib::US2ToUtf8(db_pw,&inbuffer);
		if(NULL != inbuffer)
		{
			rc = sqlite3_rekey(m_db,inbuffer,nLen-1);
			delete []inbuffer;
			inbuffer = NULL;
		}
	}
	else
	{
		rc = sqlite3_rekey(m_db,NULL,0);
	}
	if(SQLITE_OK == rc)
		bRet = true;
	return bRet;
}

bool CSqlite3Wrapper::SetPassword(LPCTSTR db_pw)
{
	bool bRet = false;
	if(NULL == db_pw || NULL == m_db)
		return bRet;
	int rc = SQLITE_OK;
	if(lstrlen(db_pw))
	{
		char *inbuffer=NULL;
		int nLen = CBaseFuncLib::US2ToUtf8(db_pw,&inbuffer);
		if(NULL != inbuffer)
		{
			rc = sqlite3_key(m_db,inbuffer,nLen-1);
			delete []inbuffer;
			inbuffer = NULL;
		}
	}
	else
	{
		rc = sqlite3_key(m_db,NULL,0);
	}
	if(SQLITE_OK == rc)
		bRet = true;
	return bRet;
}

bool CSqlite3Wrapper::SelectStatement(LPCTSTR statement, ResultTable& res)
{
	LPSTR lpsz = NULL;
	int len = 0;
	int rc = 0;
	char* errmsg = NULL;
	bool fResult = TRUE;

	res.reset();
	len = (int)_tcslen(statement);
	lpsz = (LPSTR) LocalAlloc(LMEM_ZEROINIT, len+2);

	// A UTF-16 version of sqlite_exec does not exist at this time,
	// so we will convert the string if necessary.

#if defined(_UNICODE) || defined(UNICODE)
	::WideCharToMultiByte(CP_ACP, 0, statement, -1, lpsz, len, 0, 0);
#else
	memcpy(lpsz, statement, len);
#endif
	SHORT nTryCount = 50;
	while( TRUE )
	{
		if( SQLITE_OK != sqlite3_exec( m_db, lpsz, SelectCallback, static_cast<void*>(&res), &errmsg) )
		{
			if(::strstr(errmsg,"database is locked") && nTryCount--)
			{
				::Sleep(100);
				continue;
			}
			break;
		}
	}
	if ( rc != SQLITE_OK )
		fResult = false;

	LocalFree(lpsz);
	lpsz = NULL;
	return fResult;
}

bool CSqlite3Wrapper::DirectStatement(LPCTSTR statement)
{
	CSqlite3Statement* pStatement = this->Statement(statement);
	if(NULL == pStatement)
		return false;
	bool fResult = pStatement->Execute();
	delete pStatement;
	pStatement = NULL;
	return fResult;
}

CSqlite3Statement* CSqlite3Wrapper::Statement(LPCTSTR statement)
{
	return new CSqlite3Statement(statement, m_db);
}

LPCTSTR CSqlite3Wrapper::LastError()
{
	return (LPCTSTR)sqlite3_errmsg(m_db);
}

bool CSqlite3Wrapper::Begin()
{
	return DirectStatement(_T("begin"));
}

bool CSqlite3Wrapper::Commit()
{
	return DirectStatement(_T("commit"));
}

bool CSqlite3Wrapper::Rollback()
{
	return DirectStatement(_T("rollback"));
}

void CSqlite3Wrapper::SetTimeOut(int nWait)
{
	if(NULL == m_db)
		return;
	sqlite3_busy_timeout(m_db,nWait);
}

int CSqlite3Wrapper::SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
	ResultTable* res = reinterpret_cast<ResultTable*>(p_data);
	if(NULL == res)
		return -1;
	ResultRecord record;
	for ( int i=0; i < num_fields; i++ )
	{
		record.m_fields.push_back(CString(p_fields[i]));
	}
	res->m_records.push_back(record);
	return 0;
}
