/*
* Copyright (c) 2017-?,天地和兴科技
* All rights reserved.
*
* @brief XzmSysLog格式转换类定义
*
* @version 1.0
*
* @Date 2017-02-21
*
* @author 梁杰
* 
*/

#pragma once


#include "NetSocket.h"

#define SYSLOG_OPERATION_ENABLE			//向服务器发送用户操作的SysLog格式日志	
#define SYSLOG_WARNNING_ENABLE			//向服务器发送预警信息的SysLog格式日志	
#define SYSLOG_EVENT_ENABLE				//向服务器发送Windows日志的SysLog格式日志	


#define UDPSYSLOGPORT	514

#define SYSLOGCONFIGFILENAME	L"SysLogConfig.ini"
#define SYSLOGSECTION			L"[SysLog Section]"
#define SERVERMAJORIP			L"ServerMajorIP"
#define SERVERMINORIP			L"ServerMinorIP"
#define SERVERPORT				L"ServerPort"


//重要性结构
typedef enum _tagSeverity
{
	enumEmergency,		//system is unusable
	enumAlert,			//action must be taken immediately
	enumCritical,		// critical conditions
	enumError,			// error conditions
	enumWarning,		// warning conditions
	enumNotice,			// normal but significant condition
	enumInformational,	// informational messages
	enumDebug			// debug-level messages

}enumSEVERITY;


//程序模块结构
typedef enum _tagFacility
{
	enumKernelMessages,
	enumUserLevelMessages,
	enumMailSystem,
	enumSystemDaemons,
	enumSecurityOrAuthorizationMessages_1,
	enumMessagesGeneratedInternallyBySysLogd,
	enumLinePrinterSubsystem,
	enumNetworkNewsSubsystem,
	enumUUCPSubsystem,
	enumClockDaemon_1,
	enumSecurityOrAuthorizationMessages_2,
	enumFTPDaemon,
	enumNTPSubsystem,
	enumLogAudit,
	enumLogAlert,
	enumClockDaemon_2,
	enumLocalUse0,
	enumLocalUse1,
	enumLocalUse2,
	enumLocalUse3,
	enumLocalUse4,
	enumLocalUse5,
	enumLocalUse6,
	enumLocalUse7
}enumFACILITY;

//月份结构
typedef enum _tagMonth
{
	enumJanuary = 1,
	enumFebruary,
	enumMarch,
	enumApril,
	enumMay,
	enumJune,
	enumJuly,
	enumAugust,
	enumSeptember,
	enumOctober,
	enumNovember,
	enumDecember
}STRMONTH;

#define	JUNUARY		_T("Jan")
#define	FEBRUARY	_T("Feb")
#define	MARCH		_T("Mar")
#define	APRIL		_T("Apr")
#define	MAY			_T("May")
#define	JUNE		_T("Jun")
#define	JULY		_T("Jul")
#define	AUGUST		_T("Aug")
#define	SEPTEMBER	_T("Sep")
#define	OCTOBER		_T("Oct")
#define	NOVEMBER	_T("Nov")
#define	DECEMBER	_T("Dec")

//定义日志类型标识符
#define	LOG_CLASS_SHEET		_T("S")
#define	LOG_CLASS_USB		_T("U")
#define	LOG_CLASS_OPERATION	_T("O")
#define	LOG_CLASS_WINDOWS	_T("W")



class CSysLog
{
public:
	CSysLog();
	~CSysLog();

	BOOL CreateUDPCommunication();	//创建与XzmSysLog的通信

	BOOL GetLogDataFromDB();		//从数据库中获取日志信息

	BOOL HandleXzmSysLogFile();		//处理XzmSysLog文件

private:

	BOOL ConvertPRIToXzmSysLog(enumFACILITY nFacility, enumSEVERITY nSeverity, CString& strPRI);		//转换XzmSysLog 的PRI段

	BOOL ParseAndConvertLogDateToXzmSysLog(DOUBLE fTime, CString& strDate, BOOL bYear = TRUE);	//解析日志并转换为syslog格式

	BOOL ParseXzmSysLogConfigFile(BYTE* pchXzmSysLog);		//解析配置文件，获取XzmSysLog服务器IP

	BOOL GetMonthString(int nMonth,CString& strMonth);	//获取月份字符串

private:
	char m_ServerMajorIP[64];
	char m_ServerMinorIP[64];
	int m_ServerPort;

	CNetSocket* m_pNetSock;
	SOCKET m_sock;

	CString m_strDeviceID;

};


