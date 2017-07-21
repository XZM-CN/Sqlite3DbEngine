// LogService.h : CLogService 的声明

#pragma once
#include "resource.h"       // 主符号



#include "LogEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CLogService

class ATL_NO_VTABLE CLogService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLogService, &CLSID_LogService>,
	public IDispatchImpl<ILogService, &IID_ILogService, &LIBID_LogEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// 创建表
	BOOL CreateTable();
	/// 获得数据库连接
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
