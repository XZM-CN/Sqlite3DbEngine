// ConnectHelper.h : CConnectHelper 的声明

#pragma once
#include "resource.h"       // 主符号



#include "Sqlite3DbEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CConnectHelper

class ATL_NO_VTABLE CConnectHelper :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CConnectHelper, &CLSID_ConnectHelper>,
	public IDispatchImpl<IConnectHelper, &IID_IConnectHelper, &LIBID_Sqlite3DbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:
/// 成员
	/// 最近一次操作错误信息
	CString m_strLastErrInfo;

/// 方法

	/**
	 * @brief 创建数据库配置选项数据库表
	 *
	 *
	 * @param spiSqlite3Connect 
	 * @return 
	 */
	HRESULT CreateOptionTable(CComPtr<ISqlite3Connect>& spiSqlite3Connect,DWORD dwVersion = SQLITEDB_OPTIONVALUE_DEFAULTVERSION);
	
	/**
	 * @brief 创建缺省数据库
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
	 * @brief 初始化数据库运行环境，主要是完成设置数据库用的临时路径
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(InitDBEnv)(BSTR bstrDbPath);
	
	/**
	 * @brief 打开指定位置的数据库
	 *
	 *
	 * @param bstrDbPath 数据库路径
	 * @param bCreateFlag 是否创建
	 * @param sOverTimeSecond 打开超时 
	 * @param bstrPW 访问密码 
	 * @param ppVal 
	 * @return 
	 */
	STDMETHOD(OpenDB)(BSTR bstrDbPath,VARIANT_BOOL bCreateFlag,short sOverTimeSecond,BSTR bstrPW,ISqlite3Connect** ppVal);

	/**
	 * @brief 获得最近一次操作的错误原因
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);

};

OBJECT_ENTRY_AUTO(__uuidof(ConnectHelper), CConnectHelper)
