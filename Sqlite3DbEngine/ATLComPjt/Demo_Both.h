// Demo_Both.h : CDemo_Both ������

#pragma once
#include "resource.h"       // ������



#include "ATLComPjt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)           \
	���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA \
	��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�      \
	rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CDemo_Both

class ATL_NO_VTABLE CDemo_Both :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDemo_Both, &CLSID_Demo_Both>,
	public IDispatchImpl<IDemo_Both, &IID_IDemo_Both, &LIBID_ATLComPjtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDemo_Both()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEMO_BOTH)


BEGIN_COM_MAP(CDemo_Both)
	COM_INTERFACE_ENTRY(IDemo_Both)
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



};

OBJECT_ENTRY_AUTO(__uuidof(Demo_Both), CDemo_Both)
