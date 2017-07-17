// Sqlite3Connect.h : CSqlite3Connect ������

#pragma once
#include "resource.h"       // ������



#include "Sqlite3DbEngine_i.h"

#include "Sqlite3Wrapper.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSqlite3Connect

class ATL_NO_VTABLE CSqlite3Connect :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSqlite3Connect, &CLSID_Sqlite3Connect>,
	public IDispatchImpl<ISqlite3Connect, &IID_ISqlite3Connect, &LIBID_Sqlite3DbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSqlite3Connect()
	{
		m_pSqlite3Wrapper = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SQLITECONNECT)


BEGIN_COM_MAP(CSqlite3Connect)
	COM_INTERFACE_ENTRY(ISqlite3Connect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

protected:
	/// Sqlite���ܷ�װ��
	CSqlite3Wrapper *m_pSqlite3Wrapper;
public:
	STDMETHOD(Open)(BSTR bstrDbFile, BSTR bstrPW, VARIANT_BOOL bCreateFlag);
};

OBJECT_ENTRY_AUTO(__uuidof(Sqlite3Connect), CSqlite3Connect)
