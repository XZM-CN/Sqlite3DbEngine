// ParaService.h : CParaService 的声明

#pragma once
#include "resource.h"       // 主符号



#include "JsonEngine_i.h"

#include "BaseFuncLib.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CParaService

class ATL_NO_VTABLE CParaService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CParaService, &CLSID_ParaService>,
	public IDispatchImpl<IParaService, &IID_IParaService, &LIBID_JsonEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
	/// 参数对
	CBasePairString		m_ParaPair;

public:
	CParaService()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PARASERVICE)


BEGIN_COM_MAP(CParaService)
	COM_INTERFACE_ENTRY(IParaService)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		Clear();
	}

public:

	/// IParaService
	STDMETHOD(AddNumber)(/*[in]*/ BSTR bstrName,/*[in]*/ ULONG nValue);
	STDMETHOD(AddString)(/*[in]*/ BSTR bstrName,/*[in]*/ BSTR bstrValue);
	STDMETHOD(IsExist)(/*[in]*/ BSTR bstrName,/*[out,retval]*/ VARIANT_BOOL* pVal);
	STDMETHOD(GetNumber)(/*[in]*/ BSTR bstrName,/*[out,retval]*/ ULONG* pVal);
	STDMETHOD(GetString)(/*[in]*/ BSTR bstrName,/*[out,retval]*/ BSTR* pVal);
	STDMETHOD(get_Count)(/*[out,retval]*/ SHORT* pVal);
	STDMETHOD(RemovePara)(/*[in]*/ BSTR bstrName);
	STDMETHOD(Clear)();
	STDMETHOD(GetJson)(/*[out,retval]*/ IJsonService** pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(ParaService), CParaService)
