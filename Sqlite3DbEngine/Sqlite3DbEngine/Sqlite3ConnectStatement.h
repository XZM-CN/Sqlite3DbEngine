// Sqlite3ConnectStatement.h : CSqlite3ConnectStatement 的声明

#pragma once
#include "resource.h"       // 主符号



#include "Sqlite3DbEngine_i.h"

#include "Sqlite3Statement.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;



// CSqlite3ConnectStatement

class ATL_NO_VTABLE CSqlite3ConnectStatement : 
	public CComObjectRoot,
	public ISqlite3Statement
{
public:
	CSqlite3ConnectStatement()
		: m_pSqlite3Statement(NULL)
	{
	}
	~CSqlite3ConnectStatement()
	{
		if(NULL != m_pSqlite3Statement)
		{
			delete m_pSqlite3Statement;
			m_pSqlite3Statement = NULL;
		}
	}

	/**
	 * @brief 设置包装对象
	 *
	 *
	 * @param *pSqlite3Statement 
	 * @return 
	 */
	HRESULT SetObj(CSqlite3Statement *pSqlite3Statement)
	{
		ATLASSERT(pSqlite3Statement);
		m_pSqlite3Statement = pSqlite3Statement;
		return m_pSqlite3Statement ? S_OK:E_FAIL;
	}

BEGIN_COM_MAP(CSqlite3ConnectStatement)
	COM_INTERFACE_ENTRY(ISqlite3Statement)
	COM_INTERFACE_ENTRY2(IDispatch,ISqlite3Statement) 
END_COM_MAP()

public:
	/// IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT*)
    {
       return E_NOTIMPL;
    }

    STDMETHODIMP GetTypeInfo( UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
    { 
       return E_NOTIMPL;
    }

    STDMETHODIMP GetIDsOfNames( REFIID riid,LPOLESTR *rgszNames,UINT cNames,
                    LCID lcid,DISPID *rgDispId)
    { 
      return E_NOTIMPL;
    }

	STDMETHOD(Invoke)(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,
               VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);
	
	/// ISqlite3Statement
	/**
	 * @brief 获得字段数量
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_FieldCount)(ULONG* pVal);

	/**
	 * @brief 获得记录数量
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_RecordCount)(ULONG* pVal);
	
	/**
	 * @brief 获得字段名字
	 *
	 *
	 * @param lFieldIndex 
	 * @param pbstrName 
	 * @return 
	 */
	STDMETHOD(GetFieldName)(LONG lFieldIndex,BSTR* pbstrName);

	/**
	 * @brief 获得字段类型
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetDataType)(LONG lFieldIndex,EDBDataType* pVal);

	/**
	 * @brief 获得字段类型字符串
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetFieldType)(LONG lFieldIndex,SHORT* pVal);

	/**
	 * @brief 获得整形字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueInt)(LONG lFieldIndex,LONG* pVal);

	/**
	 * @brief 获得长整形字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueLong)(LONG lFieldIndex,LONGLONG* pVal);

	/**
	 * @brief 获得浮点字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueDouble)(LONG lFieldIndex,DOUBLE* pVal);

	/**
	 * @brief 获得字符串字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueString)(LONG lFieldIndex, BSTR* pVal);

	/**
	 * @brief 获得BLOB对象
	 *
	 *
	 * @param lFieldIndex 
	 * @param ppData 
	 * @param pLen 
	 * @return 
	 */
	STDMETHOD(GetValueBlob)(LONG lFieldIndex,BYTE** ppData,ULONG* pLen);

	/**
	 * @brief 获得BLOB对象到指定的文件
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrFilePath 
	 * @return 
	 */
	STDMETHOD(GetValueBlobFile)(LONG lFieldIndex,BSTR bstrFilePath);

	/**
	 * @brief 绑定字符串类型字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrValue 
	 * @return 
	 */
	STDMETHOD(BindString)(LONG lFieldIndex,BSTR bstrValue);

	/**
	 * @brief 绑定双精度类型字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param dValue 
	 * @return 
	 */
	STDMETHOD(BindDouble)(LONG lFieldIndex,DOUBLE dValue);

	/**
	 * @brief 绑定整形类型字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @param lValue 
	 * @return 
	 */
	STDMETHOD(BindInt)(LONG lFieldIndex,LONG lValue);

	/**
	 * @brief 绑定流到文件对象
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrFilePath 
	 * @return 
	 */
	STDMETHOD(BindBlobFile)(LONG lFieldIndex,BSTR bstrFilePath);

	/**
	 * @brief 绑定流
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrFilePath 
	 * @return 
	 */
	STDMETHOD(BindBlob)(LONG lFieldIndex,BYTE* pData,ULONG nDataLen);

	/**
	 * @brief 绑定空类型字段值
	 *
	 *
	 * @param lFieldIndex 
	 * @return 
	 */
	STDMETHOD(BindNull)(LONG lFieldIndex);

	/**
	 * @brief 下一个结果集合
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(NextRow)(void);

	/**
	 * @brief 执行
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(Execute)(void);

	/**
	 * @brief 重置
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(Reset)(void);

	/**
	 * @brief 获得包装对象
	 *
	 *
	 * @param pObjVal 
	 * @return 
	 */
	STDMETHOD(get_NodeObject)(VARIANT* pObjVal);

protected:

	CSqlite3Statement *m_pSqlite3Statement;
};