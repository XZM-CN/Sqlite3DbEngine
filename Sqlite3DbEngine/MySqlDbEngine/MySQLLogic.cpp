// MySQLLogic.cpp : CCMySQLLogic ��ʵ��

#include "stdafx.h"
#include "MySQLLogic.h"

#include "BaseFuncLib.h"

using namespace _com_util;
// CCMySQLLogic



STDMETHODIMP CMySQLLogic::OpenDB(BSTR host, BSTR user, BSTR password, BSTR dbname, ULONG port)
{
	// TODO: �ڴ����ʵ�ִ���
	HRESULT hr = E_FAIL;
	if (m_pMySQLWrapper != NULL)
	{
		bool bRet = m_pMySQLWrapper->Open(
			host,     // BSTR תchar*
			user,     // BSTR תchar*
			password, // BSTR תchar*
			dbname,   // BSTR תchar*
			port);
		if (!bRet)
		{
			return hr;
		}
	}

	return S_OK;
}


STDMETHODIMP CMySQLLogic::TestAllDesignFormat(MySQLTest e)
{
	// TODO: �ڴ����ʵ�ִ���
	switch (e)
	{
	case e_MySQL00:
		{
			MYSQL *mydata = m_pMySQLWrapper->GetConnect();
			if(mydata)// ���ݿ����ӳɹ�
			{
				printf("�������ݿ�xzmdb�ɹ�~~%s,%5d\n",__FILE__,__LINE__);

				CString sql;
				char* s_sql = NULL;
				sql.Format(_T("select * from tlb_person_info;"));
				CBaseFuncLib::US2ToUtf8(sql,&s_sql);

				if(mysql_query(mydata,s_sql) !=0 )// query�ɹ��򷵻�0�����򷵻ط�0
				{
					printf("ִ�в�ѯSQL������~~%s,%5d\n",__FILE__,__LINE__);
					return E_FAIL;
				}
				MYSQL_RES *result = mysql_store_result(mydata);
				MYSQL_ROW currow = NULL;
				while ((currow = mysql_fetch_row(result)) != NULL)
				{
					CString gp = NULL;
					CBaseFuncLib::CharToUsCString(currow[2],gp);
					gp.Empty();
					char* row1 = currow[0];
					char* row2 = currow[1];
					char* row3 = currow[2];
					wchar_t *wrow1 = NULL;
					wchar_t *wrow2 = NULL;
					wchar_t *wrow3 = NULL;
					CBaseFuncLib::CharToUS2(row1,&wrow1);
					CBaseFuncLib::CharToUS2(row2,&wrow2);
					CBaseFuncLib::CharToUS2(row3,&wrow3);

					gp.Format(_T("\t%s\t  %s\t  %s\t\n"),wrow1,wrow2,wrow3);

					CBaseFuncLib::US2ToUtf8(gp,&s_sql);
					printf(s_sql);
				}
			}
			else
			{
				printf("�������ݿ�xzmdbʧ��~~%s,%5d",__FILE__,__LINE__);
			}
		}
		break;
	case e_MySQL01:
		{
			CMySQLWrapper::Test();
		}
		break;
	case e_MySQL02:
		{
			m_pMySQLWrapper->Test2();
		}
		break;
	case e_MySQL03:
		{
		}
		break;
	case e_MySQL04:
		{
		}
		break;
	case e_MySQL05:
		{
		}
		break;
	case e_MySQL06:
		{
		}
		break;
	case e_MySQL07:
		{

		}
		break;
	case e_MySQL08:
		{
		}
		break;
	case e_MySQL09:
		{
		}
		break;
	case e_MySQL10:
		{
		}
		break;
	case e_MySQL11:
		{
		}
		break;
	case e_MySQL12:
		{
		}
		break;
	case e_MySQL13:
		{
		}
		break;
	case e_MySQL14:
		{
		}
		break;
	case e_MySQL15:
		{
		}
		break;
	case e_MySQL16:
		{
		}
		break;
	case e_MySQL17:
		{
		}
		break;
	case e_MySQL18:
		{
		}
		break;
	case e_MySQL19:
		{
		}
		break;
	case e_MySQL20:
		{
		}
		break;
	case e_MySQL21:
		{
		}
		break;
	case e_MySQL22:
		{
		}
		break;
	case e_MySQL23:
		{
		}
		break;
	case e_MySQL24:
		{
		}
		break;
	case e_MySQL25:
		{
		}
		break;
	case e_MySQL26:
		{
		}
		break;
	case e_MySQL27:
		{
		}
		break;
	case e_MySQL28:
		{
		}
		break;
	case e_MySQL29:
		{
		}
		break;
	default:
		{
			printf("δִ���κβ���\n\n");
		}
		break;
	}
	return S_OK;
}
