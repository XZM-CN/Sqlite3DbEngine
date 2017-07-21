// LogService.cpp : CLogService ��ʵ��

#include "stdafx.h"
#include "LogService.h"
#include "BaseFuncLib.h"
#include "ComHelper.h"

// CLogService

BOOL CLogService::CreateTable()
{
	BOOL bRetFlag = FALSE;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(FALSE);
	if(NULL == spiSqlite3Connect)
		return bRetFlag;
	HRESULT hRet(S_OK);
	CString strSqlCmd(_T(""));
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	CString strTableName(SQLITEDB_TABLE_SYSLOG);
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼ϵͳ������־��LID��־ID/LTime��־ʱ��/Flag��־���/Text��־����/UID������/SyslogFlag�Ƿ����ϴ�/SyncFlagͬ�����
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			LID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
			LTime DOUBLE DEFAULT '' NOT NULL,\
			Type INTEGER DEFAULT '0' NOT NULL,\
			Text VARCHAR DEFAULT '' NOT NULL,\
			UID VARCHAR DEFAULT '' NOT NULL,\
			UName VARCHAR DEFAULT '' NOT NULL,\
			SyncFlag INTEGER DEFAULT '0' NOT NULL,\
			SyslogFlag INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		ATLASSERT(SUCCEEDED(hRet));
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	strTableName = SQLITEDB_TABLE_WARNLOG;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		///��¼Ԥ����־��LID��־ID/LTime��־ʱ��/Type��־����/Flag��־/Count����/Text��־����/UID��������/SyncFlagͬ�����/SyslogFlag�Ƿ����ϴ�
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
			LID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\
			LTime DOUBLE DEFAULT '' NOT NULL,\
			Type INTEGER DEFAULT '0' NOT NULL,\
			Flag INTEGER DEFAULT '0' NOT NULL,\
			Count INTEGER DEFAULT '1' NOT NULL,\
			Text VARCHAR DEFAULT '' NOT NULL,\
			UID VARCHAR DEFAULT '' NOT NULL,\
			SyncFlag INTEGER DEFAULT '0' NOT NULL,\
			SyslogFlag INTEGER DEFAULT '0' NOT NULL);"),\
			strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		ATLASSERT(SUCCEEDED(hRet));
		strSqlCmd.Empty();
	}
	strTableName.Empty();
	strTableName = SQLITEDB_TABLE_SYS_EVENTLOG;
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(strTableName),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		/*/��¼ϵͳ��־��LTime��־ʱ��/LEventID�¼�ID/EventType�¼�����/EventClass�¼�����/EventSource�¼���Դ/ComputerName�������
		/ComputerUser�û���/TaskCategory��������/EventDesc�¼�����/SyslogFlag�Ƿ����ϴ�*/
		strSqlCmd.Format(_T("CREATE TABLE [%s] (\
							LTime INTEGER DEFAULT '0' NOT NULL,\
							LEventID INTEGER DEFAULT '0' NOT NULL,\
							EventType INTEGER DEFAULT '0' NOT NULL,\
							EventClass INTEGER DEFAULT '0' NOT NULL,\
							EventSource VARCHAR DEFAULT '' NOT NULL,\
							ComputerName VARCHAR DEFAULT '' NOT NULL,\
							ComputerUser VARCHAR DEFAULT '' NOT NULL,\
							TaskCategory VARCHAR DEFAULT '' NOT NULL,\
							EventDesc VARCHAR DEFAULT '' NOT NULL,\
							SyslogFlag INTEGER DEFAULT '0' NOT NULL,\
							LID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE);"),\
							strTableName);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		ATLASSERT(SUCCEEDED(hRet));
		strSqlCmd.Empty();
	}

	strTableName.Empty();
	spiSqlite3Connect->Close();
	spiSqlite3Connect  = NULL;
	return bRetFlag;
}

CComPtr <ISqlite3Connect> CLogService::GetConnect(BOOL bReadFlag)
{
	CComPtr<IClassFactory>	cf = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = NULL;
	HRESULT hRet = DllGetClassObject( __uuidof(ConnectHelper), __uuidof(IClassFactory), (LPVOID *)&cf );
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(IConnectHelper), (VOID **)&spiConnectHelper );
		cf = NULL;
	}
	if(NULL == spiConnectHelper)
		return NULL;/// �����ӿ�ʧ��
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
#ifdef _DEBUG
	CString strLogFile = CBaseFuncLib::GetAppDataDir()+TDHX_SQLITEDB_LOGFILE;
#else
	CString strLogPath = CComHelper::GetAppInsPath();
	strLogPath += _T("Data\\");
	CString strLogFile = strLogPath+TDHX_SQLITEDB_LOGFILE;
#endif
	if(!bReadFlag)
	{
		hRet = spiConnectHelper->OpenDB(CComBSTR(strLogFile),VARIANT_TRUE,SQLITE_OPEN_DEFAULTOVERTIME,NULL,&spiSqlite3Connect);
		spiConnectHelper = NULL;
		return spiSqlite3Connect;
	}
#ifdef _DEBUG
	CString strReadFile = CBaseFuncLib::GetTmpPath()+TDHX_SQLITEDB_LOGFILE;
#else
	strLogPath.Empty();
	strLogPath = CComHelper::GetAppInsPath();
	strLogPath += _T("Temp\\");
	CString strReadFile = strLogPath+TDHX_SQLITEDB_LOGFILE;
#endif
	/// ���Ƶ���ʱĿ¼ֻ��ʹ��
	::CopyFile(strLogFile,strReadFile,FALSE);
	hRet = spiConnectHelper->OpenDB(CComBSTR(strReadFile),VARIANT_FALSE,SQLITE_OPEN_DEFAULTOVERTIME,NULL,&spiSqlite3Connect);
	spiConnectHelper = NULL;
	return spiSqlite3Connect;
}

// STDMETHODIMP CLogService::GetReadConnect(ISqlite3Connect** ppVal)
// {
// 	// TODO: �ڴ����ʵ�ִ���
// 	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(TRUE);
// 	if(NULL == spiSqlite3Connect)
// 		return E_FAIL;
// 	HRESULT hRet = spiSqlite3Connect->QueryInterface(IID_ISqlite3Connect,(LPVOID *)ppVal);
// 	spiSqlite3Connect = NULL;
// 	return hRet;
// }

STDMETHODIMP CLogService::Del(ESafeLogType eLogType,LONG nBeforeDay)
{
	// TODO: �ڴ����ʵ�ִ���
#ifdef _DEBUG
	if(nBeforeDay < 1)
#else
	if(nBeforeDay < 90)
#endif
		return E_INVALIDARG;
	HRESULT hRet(E_FAIL);
	COleDateTime curTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan spanTime(nBeforeDay,0,0,0);
	COleDateTime DelTime = curTime-spanTime;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(FALSE);
	if(NULL == spiSqlite3Connect)
		return hRet;
	CString strSqlCmd(_T(""));
	switch(eLogType)
	{
	case SAFELOGTYPE_UNWHITEFILERUN:
	case SAFELOGTYPE_WHITEFILERUN:
	case SAFELOGTYPE_PROTECTEXE:
	case SAFELOGTYPE_USBACCESS:
	case SAFELOGTYPE_PROTECTDIR:
	case SAFELOGTYPE_PROTECTREG:
		strSqlCmd.Format( _T("DELETE FROM [%s] WHERE LTime < %f"), \
			SQLITEDB_TABLE_WARNLOG,DelTime.m_dt);
		break;
	case SAFELOGTYPE_SELFRUN:
	case SAFELOGTYPE_OPERATION:
		strSqlCmd.Format( _T("DELETE FROM [%s] WHERE LTime < %f"), \
			SQLITEDB_TABLE_SYSLOG,DelTime.m_dt);
		break;
	default:
		break;
	}
	if(strSqlCmd.GetLength())
	{
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		ATLASSERT(SUCCEEDED(hRet));
		strSqlCmd.Empty();
		/// ѹ�����ݿ��ļ�
		spiSqlite3Connect->CompressDB();
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CLogService::WriteSysLog(SHORT nUserType,BSTR bstrLID,BSTR bstrName,DATE dtTime,BSTR bstrLog,ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrLog)
		return E_POINTER;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CString strLog(bstrLog),strLID(_T("")),strName(_T(""));
	if(NULL != bstrLID)
		strLID = bstrLID;
	if (NULL != bstrName)
	{
		strName = bstrName;
	}
	strLog.Replace(_T("'"),_T("''"));
	DATE LogTime(dtTime);
	if(LogTime < 1.0)
		LogTime = COleDateTime::GetCurrentTime().m_dt;
	CString strSqlCmd(_T(""));
	strSqlCmd.Format(_T("INSERT INTO [%s] (LTime,Type,Text,UID,UName) \
		VALUES(%f,%ld,\'%s\',\'%s\',\'%s\');"), 
		SQLITEDB_TABLE_SYSLOG,LogTime,nUserType,strLog,strLID,strName);
	HRESULT hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	if(SUCCEEDED(hRet))
	{
		/// �����־ID
		strSqlCmd.Format(_T("select last_insert_rowid() from %s"),SQLITEDB_TABLE_SYSLOG);
		hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
		hRet = spiSqlite3Connect->NextRow();
		if(SUCCEEDED(hRet))
		{
			LONG nLogID = 0;
			spiSqlite3Connect->GetValueInt(0,&nLogID);
			(*pVal) = nLogID;
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CLogService::WriteAppLog(ESafeLogType eLogType,SHORT nFlag,\
	DATE dtTime,BSTR bstrFilePath,BSTR bstrLog,ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrLog || NULL == pVal)
		return E_POINTER;
	(*pVal) = 0;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CString strLog(bstrLog),strUID(_T(""));
	if(NULL != bstrFilePath)
		strUID = bstrFilePath;
	strLog.Replace(_T("'"),_T("''"));
	/// ��ɨ�����5����֮����û����ͬ����־
	COleDateTimeSpan spanTime(0,0,5,0);
	CString strSqlCmd(_T(""));
	LONG nLogID = 0,nCount = 0;
	DATE LogTime(dtTime);
	if(LogTime < 1.0)
		LogTime = COleDateTime::GetCurrentTime().m_dt;
	COleDateTime LastTime(LogTime);
	HRESULT hRet(E_FAIL);
	/// ��������
	if(-1 == strLog.Find(_T("\\svchost.exe")) && -1 == strLog.Find(_T("\\dllhost.exe")))
	{
		strSqlCmd.Format(_T("SELECT LID,Count FROM [%s] WHERE %s='%s' AND %s='%s' AND Flag = %d AND Type = %d AND LTime > %f"),\
			SQLITEDB_TABLE_WARNLOG,_T("UID"),strUID,_T("Text"),strLog,nFlag,eLogType,(LastTime-spanTime).m_dt);
		hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&nLogID);
			spiSqlite3Connect->GetValueInt(1,&nCount);
			(*pVal) = nLogID;
			break;
		}
	}
	if(nLogID > 0)
	{
		nCount++;
		strSqlCmd.Format(_T("UPDATE [%s] SET Count=%ld,LTime=%f WHERE LID=%ld"),\
				SQLITEDB_TABLE_WARNLOG,nCount,LogTime,nLogID);
	}
	else
	{
		strSqlCmd.Format(_T("INSERT INTO [%s] (LTime,Type,Flag,Text,UID) \
			VALUES(%f,%d,%d,\'%s\',\'%s\');"), 
			SQLITEDB_TABLE_WARNLOG,LogTime,eLogType,nFlag,strLog,strUID);
	}
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	if(SUCCEEDED(hRet) && !nLogID)
	{
		/// �����־ID
		strSqlCmd.Format(_T("select last_insert_rowid() from %s"),SQLITEDB_TABLE_WARNLOG);
		hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
		hRet = spiSqlite3Connect->NextRow();
		if(SUCCEEDED(hRet))
		{
			spiSqlite3Connect->GetValueInt(0,&nLogID);
			(*pVal) = nLogID;
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CLogService::WriteUsbLog(SHORT nFlag,BSTR bstrUID,DATE dtTime,BSTR bstrLog,ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrLog || NULL == pVal)
		return E_POINTER;
	(*pVal) = 0;
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetConnect(FALSE);
	if(NULL == spiSqlite3Connect)
		return E_FAIL;
	CString strLog(bstrLog),strUID(_T(""));
	if(NULL != bstrUID)
		strUID = bstrUID;
	strLog.Replace(_T("'"),_T("''"));
	COleDateTimeSpan spanTime(0,0,5,0);
	DATE LogTime(dtTime);
	if(LogTime < 1.0)
		LogTime = COleDateTime::GetCurrentTime().m_dt;
	COleDateTime LastTime(LogTime);

	CString strSqlCmd(_T(""));
	LONG nLogID = 0,nCount = 0;

	/// ��ɨ�����5����֮����û����ͬ����־
	strSqlCmd.Format(_T("SELECT LID,Count FROM [%s] WHERE %s='%s' AND %s='%s' AND Flag = %d AND Type = %d AND LTime > %f"),\
		SQLITEDB_TABLE_WARNLOG,_T("UID"),strUID,_T("Text"),strLog,nFlag,SAFELOGTYPE_USBACCESS,(LastTime-spanTime).m_dt);
	HRESULT hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
	while(SUCCEEDED(spiSqlite3Connect->NextRow()))
	{
		spiSqlite3Connect->GetValueInt(0,&nLogID);
		spiSqlite3Connect->GetValueInt(1,&nCount);
		(*pVal) = nLogID;
		break;
	}
	if(nLogID > 0)
	{
		strSqlCmd.Format(_T("UPDATE [%s] SET Count=%ld,LTime=%f WHERE LID=%ld"),\
				SQLITEDB_TABLE_WARNLOG,++nCount,LogTime,nLogID);
	}
	else
	{
		strSqlCmd.Format(_T("INSERT INTO [%s] (LTime,Type,Flag,Text,UID) \
			VALUES(%f,%d,%d,\'%s\',\'%s\');"), 
			SQLITEDB_TABLE_WARNLOG,LogTime,SAFELOGTYPE_USBACCESS,nFlag,strLog,strUID);
	}
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	if(SUCCEEDED(hRet) && !nLogID)
	{
		/// �����־ID
		strSqlCmd.Format(_T("select last_insert_rowid() from %s"),SQLITEDB_TABLE_WARNLOG);
		hRet = spiSqlite3Connect->ExecQuery(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
		hRet = spiSqlite3Connect->NextRow();
		if(SUCCEEDED(hRet))
		{
			spiSqlite3Connect->GetValueInt(0,&nLogID);
			(*pVal) = nLogID;
		}
	}
	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;
	return hRet;
}