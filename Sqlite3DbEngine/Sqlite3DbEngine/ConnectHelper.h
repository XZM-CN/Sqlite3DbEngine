// ConnectHelper.h : CConnectHelper ������

#pragma once
#include "resource.h"       // ������



#include "Sqlite3DbEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CConnectHelper

class ATL_NO_VTABLE CConnectHelper :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CConnectHelper, &CLSID_ConnectHelper>,
	public IDispatchImpl<IConnectHelper, &IID_IConnectHelper, &LIBID_Sqlite3DbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
/// ��Ա
	/// ���һ�β���������Ϣ
	CString m_strLastErrInfo;

/// ����

	/**
	 * @brief �������ݿ�����ѡ�����ݿ��
	 *
	 *
	 * @param spiSqlite3Connect 
	 * @return 
	 */
	HRESULT CreateOptionTable(CComPtr<ISqlite3Connect>& spiSqlite3Connect,DWORD dwVersion = SQLITEDB_OPTIONVALUE_DEFAULTVERSION);
	
	/**
	 * @brief ����ȱʡ���ݿ�
	 *
	 *
	 * @param bstrDBPath 
	 * @param ppVal 
	 * @return 
	 */
	HRESULT CreateDefaultDB(CComBSTR bstrDBPath,CComBSTR bstrPW,ISqlite3Connect** ppVal);

public:
	CConnectHelper()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CONNECTHELPER)


BEGIN_COM_MAP(CConnectHelper)
	COM_INTERFACE_ENTRY(IConnectHelper)
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
	/// IConnectHelper
	/**
	 * @brief ��ʼ�����ݿ����л�������Ҫ������������ݿ��õ���ʱ·��
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(InitDBEnv)(BSTR bstrDbPath);
	
	/**
	 * @brief ��ָ��λ�õ����ݿ�
	 *
	 *
	 * @param bstrDbPath ���ݿ�·��
	 * @param bCreateFlag �Ƿ񴴽�
	 * @param sOverTimeSecond �򿪳�ʱ 
	 * @param bstrPW �������� 
	 * @param ppVal 
	 * @return 
	 */
	STDMETHOD(OpenDB)(BSTR bstrDbPath,VARIANT_BOOL bCreateFlag,short sOverTimeSecond,BSTR bstrPW,ISqlite3Connect** ppVal);

	/**
	 * @brief ������һ�β����Ĵ���ԭ��
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(ConnectHelper), CConnectHelper)
