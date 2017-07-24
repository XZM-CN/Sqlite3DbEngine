#pragma once
#include "ComHelper.h"
#include "DbItemDef.h"

class CDbHelper : public CComHelper
{
protected:
	/// ģ������
	static ATL::CString m_strModelName;

public:

	/**
	 * @brief �ͷ����ݿ�ӿ�
	 *
	 *
	 */
	static void FreeDB();

	/**
	 * @brief  ������ݿ�ӿ�
	 *
	 *
	 * @return ���ݿ�ӿ�
	 */
	static CComPtr <ISqlite3Connect> GetSqlite3Connect();

	/**
	 * @brief  ������ݿ�����ӿ�
	 *
	 *
	 * @return ���ݿ�����ӿ�
	 */
	static CComPtr <IConnectHelper> GetDBHelper();

	/**
	 * @brief              ���������ֵ
	 *
	 *
	 * @param spiSqlite3Connect  ���ݿ���ʽӿ�
	 * @param strSqlString SQL����ַ���
	 * @return 
	 */
	static long GetSingleLongValue(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString);

	/**
	 * @brief               ���������ֵ
	 *
	 *
	 * @param strDBPath     ���ݿ�·��
	 * @param strSqlString  SQL����ַ���
	 * @return 
	 */
	static long GetSingleLongValueEx(const ATL::CString &strDBPath,const ATL::CString &strSqlString,const ATL::CString& strPW = _T(""));

	/**
	 * @brief ����ַ�����ֵ
	 *
	 *
	 * @param spiSqlite3Connect ���ݿ�·��
	 * @param strSqlString SQL����ַ���
	 * @return 
	 */
	static ATL::CString GetSingleStringValue(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString);

	/**
	 * @brief               ����ַ�����ֵ
	 *
	 *
	 * @param strDBPath     ���ݿ�·��
	 * @param strSqlString  SQL����ַ���
	 * @return 
	 */
	static ATL::CString GetSingleStringValueEx(const ATL::CString &strDBPath,const ATL::CString &strSqlString,\
		const ATL::CString& strPW = _T(""),int nOverTime = SQLITE_OPEN_DEFAULTOVERTIME);

	/**
	 * @brief                �ж�ָ����¼�Ƿ����
	 *
	 *
	 * @param spiSqlite3Connect    ���ݿ�ӿ�
	 * @param strSqlString   SQL����ַ���
	 * @return 
	 */
	static ULONG GetRecordCount(CComPtr <ISqlite3Connect> &spiSqlite3Connect,const ATL::CString &strSqlString);

	/**
	 * @brief                �ж�ָ����¼�Ƿ����
	 *
	 *
	 * @param strDBPath      ���ݿ�·��
	 * @param strSqlString   SQL����ַ���
	 * @return 
	 */
	static BOOL RecordHaveExist(const ATL::CString &strDBPath,const ATL::CString &strSqlString,const ATL::CString& strPW = _T(""));

	/**
	 * @brief              ����ִ������
	 *
	 *
	 * @param spiSqlite3Connect  ���ݿ���ʽӿ�ָ��
	 * @param StringList   SQL�����б�
	 * @return 
	 */
	static HRESULT ExecDBCommond(CComPtr <ISqlite3Connect> &spiSqlite3Connect,CSTRING_VECTOR &StringList);

	/**
	 * @brief              ͨ������ִ��SQL����������ݿ�
	 *
	 *
	 * @param strDBPath    ���ݿ�·��
	 * @param StringList   SQL�����б�
	 * @param bCompressDB  ִ����ɺ��Ƿ�ѹ�����ݿ⣬����ɾ����¼�м�С�ļ��ߴ�
	 * @return 
	 */
	static HRESULT OperateDB(const ATL::CString &strDBPath,CSTRING_VECTOR &StringList,const ATL::CString& strPW = _T(""),BOOL bCompressDB = FALSE);

	/**
	 * @brief  ���JSON����ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <IJsonService> GetJsonService();

	/**
	 * @brief  ��ò�������ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <IParaService> GetParaService();

	/**
	 * @brief  �����־����ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <ILogService> GetLogService();

	/**
	 * @brief  дϵͳ������־
	 *
	 *
	 * @return 
	 */
	static ULONG WriteSysLog(short nUserType,const CString& strLID,const CString& strName,const CString& strLog);

	/**
	 * @brief  дԤ����־
	 *
	 *
	 * @return 
	 */
	static ULONG WriteWarnLog(EHXSafeLogType eLogType,DATE dtTime,short nFlag,const CString& strID,const CString& strLog);
};