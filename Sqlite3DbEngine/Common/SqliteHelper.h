#pragma once
#include "ComHelper.h"
#include "DbItemDef.h"

class CDbHelper : public CComHelper
{
protected:
	/// 模块名称
	static ATL::CString m_strModelName;

public:

	/**
	 * @brief 释放数据库接口
	 *
	 *
	 */
	static void FreeDB();


	/**
	 * @brief  获得数据库帮助接口
	 *
	 *
	 * @return 数据库帮助接口
	 */
	static CComPtr <IConnectHelper> GetDBHelper(ATL::CString m_strModelName = TDHXKJ_SQLITE3DBENGINE);

	/**
	 * @brief              获得整形数值
	 *
	 *
	 * @param spiSqlite3Connect  数据库访问接口
	 * @param strSqlString SQL语句字符串
	 * @return 
	 */
	static long GetSingleLongValue(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString);

	/**
	 * @brief               获得整形数值
	 *
	 *
	 * @param strDBPath     数据库路径
	 * @param strSqlString  SQL语句字符串
	 * @return 
	 */
	static long GetSingleLongValueEx(const ATL::CString &strDBPath,const ATL::CString &strSqlString,const ATL::CString& strPW = _T(""));

	/**
	 * @brief 获得字符串数值
	 *
	 *
	 * @param spiSqlite3Connect 数据库路径
	 * @param strSqlString SQL语句字符串
	 * @return 
	 */
	static ATL::CString GetSingleStringValue(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString);

	/**
	 * @brief               获得字符串数值
	 *
	 *
	 * @param strDBPath     数据库路径
	 * @param strSqlString  SQL语句字符串
	 * @return 
	 */
	static ATL::CString GetSingleStringValueEx(const ATL::CString &strDBPath,const ATL::CString &strSqlString,\
		const ATL::CString& strPW = _T(""),int nOverTime = SQLITE_OPEN_DEFAULTOVERTIME);

	/**
	 * @brief                判断指定记录是否存在
	 *
	 *
	 * @param spiSqlite3Connect    数据库接口
	 * @param strSqlString   SQL语句字符串
	 * @return 
	 */
	static ULONG GetRecordCount(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString);

	/**
	 * @brief                判断指定记录是否存在
	 *
	 *
	 * @param strDBPath      数据库路径
	 * @param strSqlString   SQL语句字符串
	 * @return 
	 */
	static BOOL RecordHaveExist(const ATL::CString &strDBPath,const ATL::CString &strSqlString,const ATL::CString& strPW = _T(""));

	/**
	 * @brief              批量执行命令
	 *
	 *
	 * @param spiSqlite3Connect  数据库访问接口指针
	 * @param StringList   SQL命令列表
	 * @return 
	 */
	static HRESULT ExecDBCommond(CComPtr <ISqlite3Connect> &spiSqlite3Connect,CSTRING_VECTOR &StringList);

	/**
	 * @brief              通过批量执行SQL命令操作数据库
	 *
	 *
	 * @param strDBPath    数据库路径
	 * @param StringList   SQL命令列表
	 * @param bCompressDB  执行完成后是否压缩数据库，用于删除记录有减小文件尺寸
	 * @return 
	 */
	static HRESULT OperateDB(const ATL::CString &strDBPath,CSTRING_VECTOR &StringList,const ATL::CString& strPW = _T(""),BOOL bCompressDB = FALSE);


	// 获取数据库接口
	static CComPtr <ISqlite3Connect> GetSqlite3Connect();
	// 获取Json接口
	static CComPtr <IJsonService> GetJsonService(ATL::CString m_strModelName = TDHXKJ_JSONENGINE);
	// 获得参数服务接口
	static CComPtr <IParaService> GetParaService();
	// 获得日志服务接口
	static CComPtr <ILogService> GetLogService();
	// 写系统操作日志
	static ULONG WriteSysLog(short nUserType,const CString& strLID,const CString& strName,const CString& strLog);
	// 写预警日志
	static ULONG WriteWarnLog(EHXSafeLogType eLogType,DATE dtTime,short nFlag,const CString& strID,const CString& strLog);
};
