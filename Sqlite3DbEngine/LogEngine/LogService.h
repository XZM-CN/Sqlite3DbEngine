// LogService.h : CLogService ������

#pragma once
#include "resource.h"       // ������



#include "LogEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CLogService

class ATL_NO_VTABLE CLogService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLogService, &CLSID_LogService>,
	public IDispatchImpl<ILogService, &IID_ILogService, &LIBID_LogEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// ������
	BOOL CreateTable();
	/// ������ݿ�����
	CComPtr <ISqlite3Connect> GetConnect(BOOL bReadFlag = FALSE);

public:
	CLogService()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_LOGSERVICE)

	BEGIN_COM_MAP(CLogService)
		COM_INTERFACE_ENTRY(ILogService)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		CreateTable();
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(WriteSysLog)(SHORT nUserType,BSTR bstrLID,BSTR bstrName,DATE dtTime,BSTR bstrLog,ULONG* pVal);
	STDMETHOD(WriteAppLog)(ESafeLogType eLogType,SHORT nFlag,DATE dtTime,BSTR bstrFilePath,BSTR bstrLog,ULONG* pVal);
	STDMETHOD(WriteUsbLog)(SHORT nFlag,BSTR bstrUID,DATE dtTime,BSTR bstrLog,ULONG* pVal);

	// STDMETHOD(GetReadConnect)(ISqlite3Connect** ppVal);
	STDMETHOD(Del)(ESafeLogType eLogType,LONG nBeforeDay);

};

OBJECT_ENTRY_AUTO(__uuidof(LogService), CLogService)
