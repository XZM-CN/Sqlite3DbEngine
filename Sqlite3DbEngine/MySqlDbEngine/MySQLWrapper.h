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
	// host     : �������ݿ��ַ e.g.  localhost(���ص�ַ)��127.0.0.1(���ص�ַ)��etc.
	// user     : �û���         e.g.  root(����Ա)��remote(Զ�̵�¼�û�)��etc.
	// password : �û���¼����   e.g.  
	// dbname   : ���ݿ�����     e.g.  
	// port     : �˿ں�         e.g.  3360
	// 
	CMySQLWrapper(LPCTSTR host, LPCTSTR user, LPCTSTR password, LPCTSTR dbname, unsigned long port = MYSQL_PORT);

	~CMySQLWrapper();

	//
	// host     : �������ݿ��ַ e.g.  localhost(���ص�ַ)��127.0.0.1(���ص�ַ)��etc.
	// user     : �û���         e.g.  root(����Ա)��remote(Զ�̵�¼�û�)��etc.
	// password : �û���¼����   e.g.  
	// dbname   : ���ݿ�����     e.g.  
	// port     : �˿ں�         e.g.  3360
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
	//һ������
	MYSQL *m_pMYSQL;
	static CRITICAL_SECTION m_crit; // mysql��init��connectʱ������ȫ�̰߳�ȫ�ģ�so.....
	static BOOL m_bCrit;
};