// MySQLLogic.h : CCMySQLLogic ������

#pragma once
#include "resource.h"       // ������



#include "MySqlDbEngine_i.h"

#include "MySQLWrapper.h"

#include "comutil.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CCMySQLLogic

class ATL_NO_VTABLE CMySQLLogic :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMySQLLogic, &CLSID_MySQLLogic>,
	public IDispatchImpl<IMySQLLogic, &IID_IMySQLLogic, &LIBID_MySqlDbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMySQLLogic()
	{
		m_pMySQLWrapper = NULL;
	}
	CMySQLLogic(BSTR host, BSTR user, BSTR password, BSTR dbname, unsigned int port = MYSQL_PORT)
	{
		m_pMySQLWrapper = NULL;
		if (m_pMySQLWrapper == NULL)
		{
			m_pMySQLWrapper = new CMySQLWrapper(
				(_bstr_t)host,
				(_bstr_t)user,
				(_bstr_t)password,
				(_bstr_t)dbname,
				port);
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CMYSQLLOGIC)


BEGIN_COM_MAP(CMySQLLogic)
	COM_INTERFACE_ENTRY(IMySQLLogic)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		if (m_pMySQLWrapper == NULL)
			m_pMySQLWrapper = new CMySQLWrapper();
		return S_OK;
	}

	void FinalRelease()
	{
	}

protected:
	CMySQLWrapper* m_pMySQLWrapper;
public:



	STDMETHOD(OpenDB)(BSTR host, BSTR user, BSTR password, BSTR dbname, ULONG port);
	STDMETHOD(TestAllDesignFormat)(MySQLTest e);
};

OBJECT_ENTRY_AUTO(__uuidof(MySQLLogic), CMySQLLogic)
