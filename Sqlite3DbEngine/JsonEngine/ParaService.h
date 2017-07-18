// ParaService.h : CParaService ������

#pragma once
#include "resource.h"       // ������



#include "JsonEngine_i.h"

#include "BaseFuncLib.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CParaService

class ATL_NO_VTABLE CParaService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CParaService, &CLSID_ParaService>,
	public IDispatchImpl<IParaService, &IID_IParaService, &LIBID_JsonEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
	/// ������
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
