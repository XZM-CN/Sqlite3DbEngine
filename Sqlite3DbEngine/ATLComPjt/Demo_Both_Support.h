// Demo_Both_Support.h : CDemo_Both_Support ������

#pragma once
#include "resource.h"       // ������



#include "ATLComPjt_i.h"
#include "_IDemo_Both_SupportEvents_CP.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CDemo_Both_Support

class ATL_NO_VTABLE CDemo_Both_Support :
	// 
	public CComObjectRootEx<CComMultiThreadModel>,
	// 
	public CComCoClass<CDemo_Both_Support, &CLSID_Demo_Both_Support>,
	// ISupportErrorInfo�ӿ�֧�֣��ṩ��һ������³���Եķ��������ͻ�����server-to-client���Ĵ���㱨����
	//��ATL����ָ�ϵڶ��桷��������ϸ��������ص�ϸ��
	public ISupportErrorInfo,
	// ���ӵ�Ĭ��ʵ�ֽӿ�,���ӵ�֧�ַ������Ļؽ��źţ�callback�������¼����㣨event����
	//��ATL����ָ�ϵڶ��桷��������ϸ��������ص�ϸ��
	public IConnectionPointContainerImpl<CDemo_Both_Support>,
	// �����̷߳��Ͳ��������ṩ���ڵ����̵��߳�����Ĭ�ϵķ�ʽ�Խӿ�ָ������߳�֮��Ĳ������ȣ���Marshaling��
	//��ATL����ָ�ϵڶ��桷��ʮ����ϸ��������ص�ϸ��
	public CProxy_IDemo_Both_SupportEvents<CDemo_Both_Support>,
	// IObjectWithSiteImpl IE����֧��
	public IObjectWithSiteImpl<CDemo_Both_Support>,
	// OLE�Զ���Э�鱩¶�����Ľӿ�,����ͨC++������֧��IDispatch�Զ����ӿ�
	// class T,            // ˫�ӿ�
	// const IID* piid,    // ˫�ӿ�IID
	// const GUID* plibid, // �������Ϳ�TypeLib
	// WORD wMajor = 1,    // ���Ϳ�İ汾
	// WORD wMinor = 0,    // ���Ϳ�İ汾
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
