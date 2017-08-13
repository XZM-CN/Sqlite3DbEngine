#pragma once


#include "stdafx.h"
#include "mysql.h"
//#define LIBPATH(p,f) p##f
//#pragma comment(lib, LIBPATH(__FILE__, "/../lib/libmysql.lib")) 

class CMySQLWrapper
{
public:
	CMySQLWrapper();

	//
	// host     : 连接数据库地址 e.g.  localhost(本地地址)、127.0.0.1(本地地址)、etc.
	// user     : 用户名         e.g.  root(管理员)、remote(远程登录用户)、etc.
	// password : 用户登录密码   e.g.  
	// dbname   : 数据库名称     e.g.  
	// port     : 端口号         e.g.  3360
	// 
	CMySQLWrapper(LPCTSTR host, LPCTSTR user, LPCTSTR password, LPCTSTR dbname, unsigned long port = MYSQL_PORT);

	~CMySQLWrapper();

	//
	// host     : 连接数据库地址 e.g.  localhost(本地地址)、127.0.0.1(本地地址)、etc.
	// user     : 用户名         e.g.  root(管理员)、remote(远程登录用户)、etc.
	// password : 用户登录密码   e.g.  
	// dbname   : 数据库名称     e.g.  
	// port     : 端口号         e.g.  3360
	// 
	bool Open(LPCTSTR host, LPCTSTR user, LPCTSTR password, LPCTSTR dbname, unsigned long port = MYSQL_PORT);

	MYSQL* GetConnect();
	CString GetError();

	static void Test();
	void Test2();

protected:
	CString m_strHost;
	CString m_strUser;
	CString m_strPassword;
	CString m_strDbname;
	unsigned int m_Port;
private:
	//一个连接
	MYSQL *m_pMYSQL;
	static CRITICAL_SECTION m_crit; // mysql在init和connect时不是完全线程安全的，so.....
	static BOOL m_bCrit;
};