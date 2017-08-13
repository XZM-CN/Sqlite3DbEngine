#include "StdAfx.h"
#include "MySqlWrapper.h "

#include "BaseFuncLib.h"

CRITICAL_SECTION CMySQLWrapper::m_crit;
BOOL             CMySQLWrapper::m_bCrit= FALSE;
CMySQLWrapper::CMySQLWrapper()
	:m_pMYSQL(NULL)
{
	if (CMySQLWrapper::m_bCrit == FALSE) {
		InitializeCriticalSection(&(CMySQLWrapper::m_crit));
		CMySQLWrapper::m_bCrit = TRUE;
	}
	
	m_pMYSQL = NULL;
}

CMySQLWrapper::CMySQLWrapper(LPCTSTR host, LPCTSTR user, LPCTSTR password, LPCTSTR dbname, unsigned long port/* = MYSQL_PORT*/)
	:m_pMYSQL(NULL)
{
	if (CMySQLWrapper::m_bCrit == FALSE) {
		InitializeCriticalSection(&(CMySQLWrapper::m_crit));
		CMySQLWrapper::m_bCrit = TRUE;
	}

	::EnterCriticalSection(&m_crit); 
	m_strHost     = host;
	m_strUser     = user;
	m_strPassword = password;
	m_strDbname   = dbname;
	m_Port        = (unsigned int)port;

	char *strHost     = NULL;
	char *strUser     = NULL;
	char *strPassword = NULL;
	char *strDbname   = NULL;
	CBaseFuncLib::US2ToUtf8(m_strHost    ,&strHost);
	CBaseFuncLib::US2ToUtf8(m_strUser    ,&strUser);
	CBaseFuncLib::US2ToUtf8(m_strPassword,&strPassword);
	CBaseFuncLib::US2ToUtf8(m_strDbname  ,&strDbname);

	m_pMYSQL = mysql_init(m_pMYSQL);
	if (!m_pMYSQL)
	{
		::LeaveCriticalSection(&m_crit);
		return ;
	}

	char value = 1;
	mysql_options(m_pMYSQL, MYSQL_OPT_RECONNECT, &value);

	if (!mysql_real_connect(m_pMYSQL, strHost, strUser, strPassword, strDbname, m_Port, NULL, 0))
	{
		::LeaveCriticalSection(&m_crit); 
		return ;
	}

	mysql_options(m_pMYSQL, MYSQL_OPT_RECONNECT, &value);


	// �����ַ���
	if (!mysql_set_character_set(m_pMYSQL, "gb2312"))
	{
		::LeaveCriticalSection(&m_crit);
		return ;
	}

	::LeaveCriticalSection(&m_crit);
}

CMySQLWrapper::~CMySQLWrapper()
{
	if (m_pMYSQL)
	{
		mysql_close(m_pMYSQL);
		m_pMYSQL = NULL;
	}
}

bool CMySQLWrapper::Open(LPCTSTR host, LPCTSTR user, LPCTSTR password, LPCTSTR dbname, unsigned long port/* = MYSQL_PORT*/)
{
	::EnterCriticalSection(&m_crit); 
	m_strHost     = host;
	m_strUser     = user;
	m_strPassword = password;
	m_strDbname   = dbname;
	m_Port        = (unsigned int)port;

	char *strHost     = NULL;
	char *strUser     = NULL;
	char *strPassword = NULL;
	char *strDbname   = NULL;
	CBaseFuncLib::US2ToUtf8(m_strHost    ,&strHost);
	CBaseFuncLib::US2ToUtf8(m_strUser    ,&strUser);
	CBaseFuncLib::US2ToUtf8(m_strPassword,&strPassword);
	CBaseFuncLib::US2ToUtf8(m_strDbname  ,&strDbname);

	m_pMYSQL = mysql_init(m_pMYSQL);
	if (!m_pMYSQL)
	{
		::LeaveCriticalSection(&m_crit);
		return false;
	}

	char value = 1;
	mysql_options(m_pMYSQL, MYSQL_OPT_RECONNECT, &value);

	if (!mysql_real_connect(m_pMYSQL, strHost, strUser, strPassword, strDbname, m_Port, NULL, 0))
	{
		::LeaveCriticalSection(&m_crit); 
		return false;
	}

	mysql_options(m_pMYSQL, MYSQL_OPT_RECONNECT, &value);

	CString strCsName = _T("gb2312");
	char*   strcsname     = NULL;
	CBaseFuncLib::US2ToUtf8(strCsName    ,&strcsname);
	// �����ַ���
	if (!mysql_set_character_set(m_pMYSQL, strcsname))
	{
		printf("New client character set: %s\n", mysql_character_set_name(m_pMYSQL));
		::LeaveCriticalSection(&m_crit);
		return false;
	}

	::LeaveCriticalSection(&m_crit);
	return true;
}

MYSQL* CMySQLWrapper::GetConnect()
{
	return m_pMYSQL;
}

CString CMySQLWrapper::GetError()
{
	const char *error = NULL;
	if (m_pMYSQL){
		error = mysql_error(m_pMYSQL);
		CString str;
		CBaseFuncLib::CharToUsCString(error,str);
		return str;
	}
	else
		return _T("���ݿ������޷���ʼ��"); 
}

void CMySQLWrapper::Test()
{
	CMySQLWrapper *myconn = NULL;

	//�������ݿ�
	myconn = new CMySQLWrapper(_T("192.168.1.105"), _T("root"), _T("qwer123124&S"), _T("sensvrdb"), MYSQL_PORT);
	if (myconn == NULL)
	{
		printf("allocate memory exception");
		return;
	}
	if (myconn->GetConnect() == NULL)
	{
		//cout<<"connect database fail"<< endl<<myconn->What().c_str()<<endl;
		return;
	}

	MYSQL *mydata = myconn->GetConnect();
	//��ɾ�����ݱ�
	CString str_sql = _T("drop table hzhtest");

	char* s_sql = NULL;
	CBaseFuncLib::US2ToUtf8(str_sql    ,&s_sql);

	if (mysql_query(mydata, s_sql) != 0)
	{
		//ɾ����ʧ��
		cout<<"drop table fail"<<endl<<mysql_error(mydata)<<endl;
	}
	else
	{
		cout<<"drop table success"<<endl;
	}

	//�������ݱ�,�ֶ� myid ����������������
	str_sql = _T("create table hzhtest(");
	str_sql += _T("myid integer not null auto_increment,");
	str_sql += _T("mytime datetime null,myname varchar(30),");
	str_sql += _T(" primary key(myid)) DEFAULT CHARSET=gb2312");

	s_sql = NULL;
	CBaseFuncLib::US2ToUtf8(str_sql    ,&s_sql);

	if (mysql_query(mydata, s_sql) != 0)
	{
		//������ʧ��
		cout<<"create table fail"<<endl;
		return;
	}
	else
	{
		cout<<"create table success"<<endl;
		cout<<s_sql<<endl;
	}

	//����в�������
	for(int k = 1; k < 30; ++k)
	{
		str_sql = _T("insert into hzhtest(mytime,myname) values");
		str_sql += _T("('2006-06-");
		char buff[20];
		memset(buff,0,sizeof(buff));
		itoa(k,buff,10);
		str_sql += buff;
		str_sql += " ";
		int i = k % 3;
		memset(buff,0,sizeof(buff));
		itoa(i,buff,10);
		str_sql += buff;
		str_sql += _T(":01:01'");

		if (i == 0)
		{
			str_sql += _T(",NULL");
		}
		else
		{
			str_sql += _T(", '������ʾ����Chinese�ãȣɣΣ�");
			str_sql += buff;
			str_sql += _T("'");
		}
		str_sql += _T(")");

		s_sql = NULL;
		CBaseFuncLib::US2ToUtf8(str_sql    ,&s_sql);

		if (mysql_query(mydata, s_sql) != 0)
		{
			//ִ��SQL������
			std::cout<<"execute insert syntax fail"<<endl;
			return;
		}
	}
	cout<<"insert data success"<<endl;

	//��ѯ���ݲ���ʾ
	str_sql = _T("select myid,mytime,myname from hzhtest");

	s_sql = NULL;
	CBaseFuncLib::US2ToUtf8(str_sql    ,&s_sql);

	if (mysql_query(mydata, s_sql) != 0)
	{
		//ִ��SQL������
		return;
	}

	MYSQL_RES *result = mysql_store_result(mydata);

	int rowcount = mysql_num_rows(result);
	//ȡ����Ч��¼��

	MYSQL_FIELD *fields = NULL;
	//ȡ�ø��ֶ���
	for (int i = 0; fields = mysql_fetch_field(result); i++)
	{
		CString str = NULL;
		CBaseFuncLib::CharToUsCString(fields->name,str);
	}

	//���ζ�ȡ������¼
	MYSQL_ROW currrow = NULL;
	while((currrow = mysql_fetch_row(result)) != NULL)
	{
		//���еļ�¼
		CString s;
		for(unsigned int i = 0; i < mysql_num_fields(result); ++i)
		{
			if (currrow[i])
				s = currrow[i];
			else
				s = _T("NULL");
		}
		std::cout<<endl;
	}
	mysql_free_result(result) ;
}

void CMySQLWrapper::Test2()
{
	MYSQL *mydata = GetConnect();
	if(mydata == NULL)
	{
		printf("�������ݿ�xzmdbʧ��~~%s,%5d",__FILE__,__LINE__);
		return;
	}

	//��ɾ�����ݱ�
	CString str_sql = _T("drop table hzhtest");
	char* s_sql = NULL;
	CBaseFuncLib::US2ToUtf8(str_sql,&s_sql);
	if (mysql_query(mydata, s_sql) != 0)
	{
		//ɾ����ʧ��
		//printf("drop table fail"<<endl<<mysql_error(mydata)<<endl;
		printf("ɾ����ʧ��~~%s,%5d",__FILE__,__LINE__);
	}
	else
	{
		//printf("drop table success"<<endl;
		printf("ɾ����ɹ�~~%s,%5d",__FILE__,__LINE__);
	}

	//�������ݱ�,�ֶ� myid ����������������
	str_sql = L"create table hzhtest(";
	str_sql += L"myid integer not null auto_increment,";
	str_sql += L"mytime datetime null,myname varchar(30),";
	str_sql += L" primary key(myid)) DEFAULT CHARSET=gb2312";

	CBaseFuncLib::US2ToUtf8(str_sql,&s_sql);
	if (mysql_query(mydata, s_sql) != 0)
	{
		//������ʧ��
		cout<<"create table fail"<<endl;
		printf("������ʧ��~~%s,%5d",__FILE__,__LINE__);
		return;
	}
	else
	{
		cout<<"create table success"<<endl;
		//printf(s_sql.c_str()<<endl;
		printf("������ɹ�~~%s,%5d",__FILE__,__LINE__);
	}

	//����в�������
	for(int k = 1; k < 30; ++k)
	{
		str_sql = L"insert into hzhtest(mytime,myname) values";
		str_sql += L"('2006-06-";
		char buff[20];
		memset(buff,0,sizeof(buff));
		itoa(k,buff,10);
		str_sql += buff;
		str_sql += L" ";
		int i = k % 3;
		memset(buff,0,sizeof(buff));
		itoa(i,buff,10);
		str_sql += buff;
		str_sql += L":01:01'";

		if (i == 0)
		{
			str_sql += L",NULL";
		}
		else
		{
			str_sql += L", '������ʾ����Chinese�ãȣɣΣ�";
			str_sql += buff;
			str_sql += "'";
		}
		str_sql += L")";
		CBaseFuncLib::US2ToUtf8(str_sql,&s_sql);
		if (mysql_query(mydata, s_sql) != 0)  
		{
			//ִ��SQL������
			std::cout<<"execute insert syntax fail"<<endl;
			printf("ִ��SQL������~~%s,%5d",__FILE__,__LINE__);
			return;
		}
	}
	//printf("insert data success"<<endl;
	//��ѯ���ݲ���ʾ
	str_sql = L"select myid,mytime,myname from hzhtest";
	CBaseFuncLib::US2ToUtf8(str_sql,&s_sql);
	if (mysql_query(mydata, s_sql) != 0)
	{
		//ִ��SQL������
		printf("ִ��SQL������~~%s,%5d",__FILE__,__LINE__);
		return;
	}

	MYSQL_RES *result = mysql_store_result(mydata);

	int rowcount = mysql_num_rows(result);
	//ȡ����Ч��¼��

	MYSQL_FIELD *fields = NULL;
	//ȡ�ø��ֶ���
	for (int i = 0; fields = mysql_fetch_field(result); i++)
	{
		CString str = NULL;
		CBaseFuncLib::CharToUsCString(fields->name,str);
	}

	//���ζ�ȡ������¼
	MYSQL_ROW currrow = NULL;
	while((currrow = mysql_fetch_row(result)) != NULL)
	{
		//���еļ�¼
		CString s;
		for(int i = 0; i < mysql_num_fields(result); ++i)
		{
			if (currrow[i])
				s = currrow[i];
			else
				s = L"NULL";
		}
		std::cout<<endl;
	}
	mysql_free_result(result) ;
}
