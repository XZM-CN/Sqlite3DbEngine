// XMsXmlWrapper.h : CXMsXmlWrapper 的声明

#pragma once
#include "resource.h"       // 主符号



#include "MsXmlEngine_i.h"

#include "MsXmlBase.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CXMsXmlWrapper

class ATL_NO_VTABLE CXMsXmlWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CXMsXmlWrapper, &CLSID_XMsXmlWrapper>,
	public IDispatchImpl<IXMsXmlWrapper, &IID_IXMsXmlWrapper, &LIBID_MsXmlEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CXMsXmlWrapper()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_XMSXMLWRAPPER)


BEGIN_COM_MAP(CXMsXmlWrapper)
	COM_INTERFACE_ENTRY(IXMsXmlWrapper)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct() // 构造
	{
		m_pMsXmlBase = new CMsXmlBase();
		return S_OK;
	}

	void FinalRelease()     // 析构
	{
		if(m_pMsXmlBase != NULL)
			delete m_pMsXmlBase;
	}

protected:
	CMsXmlBase*  m_pMsXmlBase;


public:



	STDMETHOD(TestAllMsxmlDesignFormat)(MsXmlTest eMsXml);
	// system configuration
	STDMETHOD(SystemCfg)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(XMsXmlWrapper), CXMsXmlWrapper)
