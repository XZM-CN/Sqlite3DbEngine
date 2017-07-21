// USBDevice.h : CUSBDevice ������

#pragma once
#include "resource.h"       // ������



#include "SKFEncry_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CUSBDevice

class ATL_NO_VTABLE CUSBDevice :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CUSBDevice, &CLSID_USBDevice>,
	public IDispatchImpl<IUSBDevice, &IID_IUSBDevice, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CUSBDevice()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_USBDEVICE)


BEGIN_COM_MAP(CUSBDevice)
	COM_INTERFACE_ENTRY(IUSBDevice)
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

OBJECT_ENTRY_AUTO(__uuidof(USBDevice), CUSBDevice)
