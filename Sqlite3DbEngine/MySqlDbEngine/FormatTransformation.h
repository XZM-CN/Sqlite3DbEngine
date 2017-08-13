// FormatTransformation.h : CFormatTransformation ������

#pragma once
#include "resource.h"       // ������



#include "MySqlDbEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CFormatTransformation

class ATL_NO_VTABLE CFormatTransformation :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFormatTransformation, &CLSID_FormatTransformation>,
	public IDispatchImpl<IFormatTransformation, &IID_IFormatTransformation, &LIBID_MySqlDbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFormatTransformation()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FORMATTRANSFORMATION)


BEGIN_COM_MAP(CFormatTransformation)
	COM_INTERFACE_ENTRY(IFormatTransformation)
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



	STDMETHOD(BSTR2charPtr01)(BSTR bstr);
};

OBJECT_ENTRY_AUTO(__uuidof(FormatTransformation), CFormatTransformation)
