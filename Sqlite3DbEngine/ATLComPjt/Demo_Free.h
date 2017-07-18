// Demo_Free.h : CDemo_Free µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ



#include "ATLComPjt_i.h"



using namespace ATL;


// CDemo_Free

class ATL_NO_VTABLE CDemo_Free :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CDemo_Free, &CLSID_Demo_Free>,
	public IDispatchImpl<IDemo_Free, &IID_IDemo_Free, &LIBID_ATLComPjtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDemo_Free()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEMO_FREE)


BEGIN_COM_MAP(CDemo_Free)
	COM_INTERFACE_ENTRY(IDemo_Free)
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

OBJECT_ENTRY_AUTO(__uuidof(Demo_Free), CDemo_Free)
