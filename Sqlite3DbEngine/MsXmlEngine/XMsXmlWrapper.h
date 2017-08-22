// XMsXmlWrapper.h : CXMsXmlWrapper ������

#pragma once
#include "resource.h"       // ������



#include "MsXmlEngine_i.h"

#include "MsXmlBase.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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

	HRESULT FinalConstruct() // ����
	{
		m_pMsXmlBase = new CMsXmlBase();
		return S_OK;
	}

	void FinalRelease()     // ����
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
