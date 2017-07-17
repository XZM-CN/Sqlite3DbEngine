#pragma once
#include "..//SqliteLib//sqlite3.h"
class CSqlite3Wrapper
{
public:
	CSqlite3Wrapper(void);
	~CSqlite3Wrapper(void);

	/// 打开数据库
	bool Open(LPCTSTR db_file,LPCTSTR db_pw);

public:
	CString	m_szName;
	sqlite3* m_db;

};

