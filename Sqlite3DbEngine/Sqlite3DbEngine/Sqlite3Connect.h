// Sqlite3Connect.h : CSqlite3Connect 的声明

#pragma once
#include "resource.h"       // 主符号



#include "Sqlite3DbEngine_i.h"

#include "Sqlite3Wrapper.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
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
	/// Sqlite功能封装类
	CSqlite3Wrapper *m_pSqlite3Wrapper;
public:
	STDMETHOD(Open)(BSTR bstrDbFile, BSTR bstrPW, VARIANT_BOOL bCreateFlag);
};

OBJECT_ENTRY_AUTO(__uuidof(Sqlite3Connect), CSqlite3Connect)
