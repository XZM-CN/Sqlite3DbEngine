// Demo.h : CDemo ������

#pragma once
#include "resource.h"       // ������



#include "ATLComPjt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CDemo

class ATL_NO_VTABLE CDemo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDemo, &CLSID_Demo>,
	public IDispatchImpl<IDemo, &IID_IDemo, &LIBID_ATLComPjtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDemo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEMO)


BEGIN_COM_MAP(CDemo)
	COM_INTERFACE_ENTRY(IDemo)
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

public:



	STDMETHOD(dddddd)(BSTR ddd);
	STDMETHOD(asdasd)(BSTR* asdsa, BYTE asdasdaaa);
	STDMETHOD(asdasdaaa)(BSTR* asd, CY* asdaaa);
	STDMETHOD(sdasdad)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(Demo), CDemo)
