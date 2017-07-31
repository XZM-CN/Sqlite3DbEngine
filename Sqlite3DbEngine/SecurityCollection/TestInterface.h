// TestInterface.h : CTestInterface ������

#pragma once
#include "resource.h"       // ������



#include "SecurityCollection_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CTestInterface

class ATL_NO_VTABLE CTestInterface :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTestInterface, &CLSID_TestInterface>,
	public IDispatchImpl<ITestInterface, &IID_ITestInterface, &LIBID_SecurityCollectionLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTestInterface()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TESTINTERFACE)


BEGIN_COM_MAP(CTestInterface)
	COM_INTERFACE_ENTRY(ITestInterface)
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

protected:
	BOOL Am_I_In_Admin_Group(BOOL bCheckAdminMode = FALSE);
	BOOL EnablePrivilege(HANDLE hToken, LPCTSTR lpszPrivilegeName);
	VOID  TestRestrictToken();

public:
	STDMETHOD(Test)(SecurityCollectionTest eJt);
};

OBJECT_ENTRY_AUTO(__uuidof(TestInterface), CTestInterface)
