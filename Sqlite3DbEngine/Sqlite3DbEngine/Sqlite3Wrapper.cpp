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
