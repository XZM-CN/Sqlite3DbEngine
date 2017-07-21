// SafeService.h : CSafeService ������

#pragma once
#include "resource.h"       // ������



#include "SKFEncryLib_i.h"
#include "_ISafeServiceEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSafeService

class ATL_NO_VTABLE CSafeService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSafeService, &CLSID_SafeService>,
	public IConnectionPointContainerImpl<CSafeService>,
	public CProxy_ISafeServiceEvents<CSafeService>,
	public IDispatchImpl<ISafeService, &IID_ISafeService, &LIBID_SKFEncryLibLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSafeService()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SAFESERVICE)


BEGIN_COM_MAP(CSafeService)
	COM_INTERFACE_ENTRY(ISafeService)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CSafeService)
	CONNECTION_POINT_ENTRY(__uuidof(_ISafeServiceEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:



};

OBJECT_ENTRY_AUTO(__uuidof(SafeService), CSafeService)
