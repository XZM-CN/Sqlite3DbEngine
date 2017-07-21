// WMIDevice.h : CWMIDevice ������

#pragma once
#include <WbemCli.h> // for IWbemLocator IWbemServices

#include "resource.h"       // ������



#include "SKFEncry_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CWMIDevice

class ATL_NO_VTABLE CWMIDevice :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CWMIDevice, &CLSID_WMIDevice>,
	public IDispatchImpl<IWMIDevice, &IID_IWMIDevice, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
	/// ���ʽӿ�
	CComPtr <IWbemLocator>	m_spiLoc;
	CComPtr <IWbemServices> m_spiSvc;

	/// ��ѯ����
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
