// Sqlite3Connect.h : CSqlite3Connect 的声明

#pragma once
#include "resource.h"       // 主符号



#include "Sqlite3DbEngine_i.h"

#include "Sqlite3Wrapper.h"

class Sqlite3Wrapper;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CSqlite3Connect

class ATL_NO_VTABLE CSqlite3Connect :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSqlite3Connect, &CLSID_Sqlite3Connect>,
	public IDispatchImpl<ISqlite3Connect, &IID_ISqlite3Connect, &LIBID_Sqlite3DbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSqlite3Connect()
	{

		m_hSyncEvent	= NULL;
		m_pSqliteWraper = NULL;
		m_pSqlite3Statement = NULL;

		m_lOverTimeSecond = 15;
		m_dwCurrentThreadID = 0;

		m_bReadOnly = FALSE;
		m_bBeginTrans = FALSE;
		m_strDbFile.Empty();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SQLITECONNECT)


BEGIN_COM_MAP(CSqlite3Connect)
	COM_INTERFACE_ENTRY(ISqlite3Connect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		Close();
	}


protected:

	/// 是否开启了事务处理
	BOOL	m_bBeginTrans;

	/// 是否只读方式打开
	BOOL	m_bReadOnly;

	/// 数据库文件路径
	CString m_strDbFile;
	/// 打开文件密码
	CString m_strPassword;
	/// 最近一次操作错误信息
	CString m_strLastErrInfo;

	/// Sqlite功能封装类
	CSqlite3Wrapper *m_pSqliteWraper;
	/// 查询语句对象
	CSqlite3Statement *m_pSqlite3Statement;

	/// 数据库文件打开后的同步事件句柄
	HANDLE m_hSyncEvent;
	/// 创建数据库连接的线程ID
	DWORD m_dwCurrentThreadID;

    /// 超时秒数
	long m_lOverTimeSecond;

	/**
	 * @brief 释放查询语句执行环境
	 *
	 *
	 * @return 
	 */
	HRESULT FinalizeStatement();


public:
	/// ISqlite3Connect
	/**
	 * @brief 设置数据库密码
	 *
	 *
	 * @param newVal 
	 * @return 
	 */
	STDMETHOD(put_Password)(BSTR newVal);

	/**
	 * @brief 设置超时
	 *
	 *
	 * @param newVal 
	 * @return 
	 */
	STDMETHOD(put_TimeoutSecond)(long newVal);

	/**
	 * @brief 获得超时
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_TimeoutSecond)(long* pVal);

	/**
	 * @brief 设置参数
	 *
	 *
	 * @param newVal 
	 * @return 
	 */
	STDMETHOD(put_ParaValue)(BSTR bstrKeyName,BSTR newVal);

	/**
	 * @brief 获取参数
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_ParaValue)(BSTR bstrKeyName,BSTR* pVal);

	
	/** ReadOpen
	 * @brief 只读方式打开数据库
	 *
	 *
	 * @param bstrDbFile 
	 * @param bstrPW 
	 * @param bCreateFlag 
	 * @return 
	 */
	STDMETHOD(ReadOpen)(BSTR bstrDbFile,BSTR bstrPW);

	/**
	 * @brief 打开数据库
	 *
	 *
	 * @param bstrDbFile 
	 * @param bstrPW 
	 * @param bCreateFlag 
	 * @return 
	 */
	STDMETHOD(Open)(BSTR bstrDbFile,BSTR bstrPW,VARIANT_BOOL bCreateFlag);

	/**
	 * @brief 获得数据库文件中表个数
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_TableCount)(VARIANT_BOOL bTemFlag,LONG* pVal);
	/**
	 * @brief 判断给定表名称是否存在
	 *
	 *
	 * @param bstrTableName 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(TableIsExist)(BSTR bstrTableName,VARIANT_BOOL bTemFlag, VARIANT_BOOL* pVal);

	/**
	 * @brief 获得指定表名称创建时的SQL语句
	 *
	 *
	 * @param bstrTableName 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetTableSql)(BSTR bstrTableName,VARIANT_BOOL bTemFlag, BSTR* pVal);
	
	/**
	 * @brief 开始事务操作
	 *
	 *
	 * @return 
	 */
	STDMETHOD(BeginTransaction)();

	/**
	 * @brief 执行SQL命令，比如插入记录等
	 *
	 *
	 * @param bstrSqlString 
	 * @return 
	 */
	STDMETHOD(ExecCommond)(BSTR bstrSqlString);

	/**
	 * @brief 准备命令
	 *
	 *
	 * @param bstrSqlString 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(PrepareCommond)(BSTR bstrSqlString,ISqlite3Statement** pVal);

	/**
	 * @brief 提交事务操作
	 *
	 *
	 * @return 
	 */
	STDMETHOD(CommitTransaction)();

	/**
	 * @brief 回滚事务操作
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(RollbackTransaction)(void);

	/**
	 * @brief 获得是否处于事务中
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_TransactionStatus)(VARIANT_BOOL* pVal);
	
	/**
	 * @brief 执行查询语句
	 *
	 *
	 * @param bstrSqlString SQL语句
	 * @return 
	 */
	STDMETHOD(ExecQuery)(BSTR bstrSqlString);

	/**
	 * @brief 执行查询语句
	 *
	 *
	 * @param bstrSqlString SQL语句
	 * @param pVal 返回语句接口对象
	 * @return 
	 */
	STDMETHOD(ExecQueryEx)(BSTR bstrSqlString,ISqlite3Statement** pVal);

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
	 * @brief 获取是否只读打开
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_ReadOnly)(VARIANT_BOOL* pVal);

	/**
	 * @brief 根据指定字段序号获得字段名称
	 *
	 *
	 * @param lFieldIndex 
	 * @param pbstrName 
	 * @return 
	 */
	STDMETHOD(GetFieldName)(LONG lFieldIndex, BSTR* pbstrName);

	/**
	 * @brief 移动记录位置到下一行
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(NextRow)(void);

	/**
	 * @brief 指定字段序号获得字段对应的记录值
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueString)(LONG lFieldIndex, BSTR* pVal);

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
	 * @brief 获得最近一次操作的出错信息
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);

	/**
	 * @brief 关闭数据库
	 *
	 *
	 * @return 
	 */
	STDMETHOD(Close)();
	
	/**
	 * @brief 获得数据库驱动版本
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_DriverVersion)(BSTR* pVal);
	
	/**
	 * @brief 获得数据库版本
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_DBVersion)(BSTR* pVal);

	/**
	 * @brief 获得当前打开的数据文件路径
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_DbPathFile)(BSTR* pVal);
	
	/**
	 * @brief 系统备份
	 *
	 *
	 * @return 
	 */
	STDMETHOD(SysBackup)(BSTR bstrFileName);

	/**
	 * @brief 压缩数据库
	 *
	 *
	 * @return 
	 */
	STDMETHOD(CompressDB)();
};

OBJECT_ENTRY_AUTO(__uuidof(Sqlite3Connect), CSqlite3Connect)
