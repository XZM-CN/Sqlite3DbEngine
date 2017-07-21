// WMIDevice.h : CWMIDevice 的声明

#pragma once
#include <WbemCli.h> // for IWbemLocator IWbemServices

#include "resource.h"       // 主符号



#include "SKFEncry_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CWMIDevice

class ATL_NO_VTABLE CWMIDevice :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CWMIDevice, &CLSID_WMIDevice>,
	public IDispatchImpl<IWMIDevice, &IID_IWMIDevice, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
	/// 访问接口
	CComPtr <IWbemLocator>	m_spiLoc;
	CComPtr <IWbemServices> m_spiSvc;

	/// 查询集合
	CSTRING_VECTOR			m_QueryIDList;
	CSTRING_VECTOR			m_QueryNameList;
	CSTRING_VECTOR			m_QueryKeyList;

	BOOL WMI_DoWithPNPDeviceID( const TCHAR *PNPDeviceID, TCHAR *MacAddress, UINT uSize );

	BOOL WMI_DoWithHarddiskSerialNumber( TCHAR *SerialNumber, UINT uSize );

	BOOL WMI_DoWithProperty( INT iQueryType, TCHAR *szProperty, UINT uSize );
public:
	CWMIDevice()
		:m_spiLoc(NULL)
		,m_spiSvc(NULL)
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_WMIDEVICE)

	BEGIN_COM_MAP(CWMIDevice)
		COM_INTERFACE_ENTRY(IWMIDevice)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		Close();
	}

public:

	STDMETHOD(Connect)(void);
	STDMETHOD(Query)(EWMIType eWMIType,BSTR bstrKey,SHORT* pVal);
	STDMETHOD(GetValue)(SHORT nIndex,BSTR* pVal);
	STDMETHOD(GetName)(SHORT nIndex,BSTR* pVal);
	STDMETHOD(GetKeyValue)(SHORT nIndex,BSTR* pVal);
	STDMETHOD(Close)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(WMIDevice), CWMIDevice)
