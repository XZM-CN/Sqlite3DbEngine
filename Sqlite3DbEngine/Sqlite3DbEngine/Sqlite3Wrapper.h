#pragma once

#include "Sqlite3Statement.h"

class CSqlite3Wrapper
{
public:
	CSqlite3Wrapper(void);
	~CSqlite3Wrapper(void);

	/// 打开数据库
	bool Open(LPCTSTR db_file,LPCTSTR db_pw);
	/// 修改密码
	bool ChangeDBPassword(LPCTSTR db_pw);
	/// 设置密码
	bool SetPassword(LPCTSTR db_pw);

	class ResultRecord
	{
	public:
		std::vector<LPCTSTR> m_fields;
	};

	class ResultTable
	{
		friend class CSqlite3Wrapper;
	public:
		ResultTable() : ptr_cur_record_(0) {}

		std::vector<ResultRecord> m_records;

		ResultRecord* next()
		{
			if (ptr_cur_record_ < m_records.size())
			{
				return &(m_records[ptr_cur_record_++]);
			}

			return 0;
		}

	private:
		void reset()
		{
			m_records.clear();
			ptr_cur_record_=0;
		}
	private:
		unsigned int ptr_cur_record_;
	};

	bool SelectStatement(LPCTSTR statement, ResultTable& res);
	bool DirectStatement(LPCTSTR statement);
	CSqlite3Statement* Statement(LPCTSTR statement);

	LPCTSTR LastError();
	//
	// Transaction related
	//
	bool Begin();
	bool Commit();
	bool Rollback();

	void SetTimeOut(int nWait);
private:

	static int SelectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

	sqlite3* m_db;
	CString	m_szName;
	CString	m_szText;

};

