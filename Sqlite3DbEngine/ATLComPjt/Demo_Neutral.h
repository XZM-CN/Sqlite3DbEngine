// Demo_Neutral.h : CDemo_Neutral 的声明

#pragma once
#include "resource.h"       // 主符号



#include "ATLComPjt_i.h"


#ifdef _WIN32_WCE
#error "Windows CE 上不支持非特定语言线程的 COM 对象。"
#endif

using namespace ATL;


// CDemo_Neutral

class ATL_NO_VTABLE CDemo_Neutral :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDemo_Neutral, &CLSID_Demo_Neutral>,
	public IDispatchImpl<IDemo_Neutral, &IID_IDemo_Neutral, &LIBID_ATLComPjtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDemo_Neutral()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEMO_NEUTRAL)


BEGIN_COM_MAP(CDemo_Neutral)
	COM_INTERFACE_ENTRY(IDemo_Neutral)
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

OBJECT_ENTRY_AUTO(__uuidof(Demo_Neutral), CDemo_Neutral)
