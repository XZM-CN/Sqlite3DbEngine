// SafeService.h : CSafeService 的声明

#pragma once
#include "resource.h"       // 主符号



#include "SKFEncryLib_i.h"
#include "_ISafeServiceEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
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
