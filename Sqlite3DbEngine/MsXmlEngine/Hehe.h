// Hehe.h : CHehe ������

#pragma once
#include "resource.h"       // ������



#include "MsXmlEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CHehe

class ATL_NO_VTABLE CHehe :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHehe, &CLSID_Hehe>,
	public IDispatchImpl<IHehe, &IID_IHehe, &LIBID_MsXmlEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHehe()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HEHE)


BEGIN_COM_MAP(CHehe)
	COM_INTERFACE_ENTRY(IHehe)
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

OBJECT_ENTRY_AUTO(__uuidof(Hehe), CHehe)
