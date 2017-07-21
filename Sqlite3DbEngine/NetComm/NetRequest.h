// NetRequest.h : CNetRequest 的声明

#pragma once
#include "resource.h"       // 主符号



#include "NetComm_i.h"
#include "_INetRequestEvents_CP.h"

#include "WinNet.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CNetRequest

class ATL_NO_VTABLE CNetRequest :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CNetRequest, &CLSID_NetRequest>,
	public IConnectionPointContainerImpl<CNetRequest>,
	public CProxy_INetRequestEvents<CNetRequest>,
	public IDispatchImpl<INetRequest, &IID_INetRequest, &LIBID_NetCommLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// 网络操作类
	CWinNet*		m_pWeiboNet;

	/// 消息窗口句柄
	HWND			m_hMsgWnd;

	/// 同步还是异步网络操作
	VARIANT_BOOL	m_bSyncConnect;
	/// COOKIE
	VARIANT_BOOL	m_bNoCookie;
	/// 是否使用IE设置
	VARIANT_BOOL	m_bUseIE;

	/// Cookie
	CString			m_strCookie;
	/// HTTP Referer
	CString			m_strHttpReferer;
	/// 代理地址
	CString			m_strProxyServer;
	/// UserAgent
	CString			m_strUserAgent;

	/// Origin
	CString			m_strOrigin;

	/// 请求方法
	CString			m_strMethod;

	/// POST数据包地址
	CString			m_strPostFile;
	/// 数据文件下载地址
	CString			m_strDownFile;
	/// 请求URL
	CString			m_strUrl;

	/// 端口
	UINT			m_nPort;
	/// 服务类型
	SHORT			m_sServiceID;
	/// 类型
	SHORT			m_sType;
	/// 压缩标志
	BOOL			m_bZipFlag;
	/// 请求头
	CString			m_strAuth;
	/// 是否为下载文件
	BOOL			m_bDownFile;

public:
	CNetRequest()
		:m_pWeiboNet(NULL)
		,m_hMsgWnd(NULL)
		,m_sServiceID(ENETREQUEST_INVALID)
		,m_bSyncConnect(VARIANT_TRUE)
		,m_bNoCookie(VARIANT_FALSE)
		,m_bUseIE(VARIANT_TRUE)
		,m_nPort(0)
		,m_sType(1)
		,m_bZipFlag(TRUE)
		,m_bDownFile(FALSE)
		,m_strMethod(_T("GET"))
	{
		m_pUnkMarshaler = NULL;

		m_strPostFile.Empty();
		m_strDownFile.Empty();
		m_strAuth.Empty();
		m_strCookie.Empty();
		m_strHttpReferer.Empty();
		m_strProxyServer.Empty();
		m_strUserAgent.Empty();
		m_strOrigin.Empty();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_NETREQUEST)


BEGIN_COM_MAP(CNetRequest)
	COM_INTERFACE_ENTRY(INetRequest)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CNetRequest)
	CONNECTION_POINT_ENTRY(__uuidof(_INetRequestEvents))
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
	STDMETHOD(get_MsgWnd)(LONGLONG* pVal);
	STDMETHOD(put_MsgWnd)(LONGLONG newVal);
	STDMETHOD(get_SyncConnect)(VARIANT_BOOL* pVal);
	STDMETHOD(put_SyncConnect)(VARIANT_BOOL newVal);
	STDMETHOD(put_NoCookie)(VARIANT_BOOL newVal);
	STDMETHOD(put_Port)(LONG newVal);
	STDMETHOD(get_Method)(BSTR* pVal);
	STDMETHOD(put_Method)(BSTR newVal);
	STDMETHOD(get_PostFile)(BSTR* pVal);
	STDMETHOD(put_PostFile)(BSTR newVal);
	STDMETHOD(get_DownFile)(BSTR* pVal);
	STDMETHOD(put_DownFile)(BSTR newVal);
	STDMETHOD(get_ServiceID)(SHORT* pVal);
	STDMETHOD(put_ServiceID)(SHORT newVal);
	STDMETHOD(get_UseIE)(VARIANT_BOOL* pVal);
	STDMETHOD(put_UseIE)(VARIANT_BOOL newVal);
	STDMETHOD(get_PrxoyServer)(BSTR* pVal);
	STDMETHOD(put_PrxoyServer)(BSTR newVal);
	STDMETHOD(get_UserAgent)(BSTR* pVal);
	STDMETHOD(put_UserAgent)(BSTR newVal);
	STDMETHOD(get_Referer)(BSTR* pVal);
	STDMETHOD(put_Referer)(BSTR newVal);
	STDMETHOD(get_Cookie)(BSTR* pVal);
	STDMETHOD(put_Cookie)(BSTR newVal);
	STDMETHOD(get_Location)(BSTR* pVal);
	STDMETHOD(get_RequestUrl)(BSTR* pVal);
	STDMETHOD(get_Origin)(BSTR* pVal);
	STDMETHOD(put_Origin)(BSTR newVal);
	STDMETHOD(get_ServerFileName)(BSTR* pVal);

	STDMETHOD(get_Busy)(VARIANT_BOOL* pVal);
	STDMETHOD(get_HttpStatus)(LONG* pVal);

	STDMETHOD(get_ErrCode)(LONG* pVal);
	STDMETHOD(get_LastErrInfo)(BSTR* pVal);

	STDMETHOD(SendHeader)(SHORT sType, VARIANT_BOOL bZipFlag, BSTR bstrAuth);
	STDMETHOD(Request)(BSTR bstrUrl);
	STDMETHOD(DownloadFile)(BSTR bstrUrl,VARIANT_BOOL bHaveName);
	STDMETHOD(Stop)(VARIANT_BOOL bReleaseConnect);
};

OBJECT_ENTRY_AUTO(__uuidof(NetRequest), CNetRequest)
