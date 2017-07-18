// Demo_Both_Support.h : CDemo_Both_Support 的声明

#pragma once
#include "resource.h"       // 主符号



#include "ATLComPjt_i.h"
#include "_IDemo_Both_SupportEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CDemo_Both_Support

class ATL_NO_VTABLE CDemo_Both_Support :
	// 
	public CComObjectRootEx<CComMultiThreadModel>,
	// 
	public CComCoClass<CDemo_Both_Support, &CLSID_Demo_Both_Support>,
	// ISupportErrorInfo接口支持，提供了一个具有鲁棒性的服务器到客户机（server-to-client）的错误汇报机制
	//《ATL开发指南第二版》第四章详细阐述了相关的细节
	public ISupportErrorInfo,
	// 连接点默认实现接口,连接点支持服务器的回叫信号（callback）或者事件爱你（event），
	//《ATL开发指南第二版》第七章详细阐述了相关的细节
	public IConnectionPointContainerImpl<CDemo_Both_Support>,
	// 自由线程封送拆收器，提供了在单进程的线程里以默认的方式对接口指针进行线程之间的参数调度（即Marshaling）
	//《ATL开发指南第二版》第十章详细阐述了相关的细节
	public CProxy_IDemo_Both_SupportEvents<CDemo_Both_Support>,
	// IObjectWithSiteImpl IE对象支持
	public IObjectWithSiteImpl<CDemo_Both_Support>,
	// OLE自动化协议暴露出来的接口,让普通C++类轻松支持IDispatch自动化接口
	// class T,            // 双接口
	// const IID* piid,    // 双接口IID
	// const GUID* plibid, // 包含类型库TypeLib
	// WORD wMajor = 1,    // 类型库的版本
	// WORD wMinor = 0,    // 类型库的版本
	// class tihclass = CComTypeInfoHolder
	public IDispatchImpl<IDemo_Both_Support, &IID_IDemo_Both_Support, &LIBID_ATLComPjtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDemo_Both_Support()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEMO_BOTH_SUPPORT)


BEGIN_COM_MAP(CDemo_Both_Support)
	COM_INTERFACE_ENTRY(IDemo_Both_Support)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CDemo_Both_Support)
	CONNECTION_POINT_ENTRY(__uuidof(_IDemo_Both_SupportEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


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

OBJECT_ENTRY_AUTO(__uuidof(Demo_Both_Support), CDemo_Both_Support)
