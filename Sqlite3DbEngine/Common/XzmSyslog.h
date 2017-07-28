/*
* Copyright (c) 2017-?,��غ��˿Ƽ�
* All rights reserved.
*
* @brief XzmSysLog��ʽת���ඨ��
*
* @version 1.0
*
* @Date 2017-02-21
*
* @author ����
* 
*/

#pragma once


#include "NetSocket.h"

#define SYSLOG_OPERATION_ENABLE			//������������û�������SysLog��ʽ��־	
#define SYSLOG_WARNNING_ENABLE			//�����������Ԥ����Ϣ��SysLog��ʽ��־	
#define SYSLOG_EVENT_ENABLE				//�����������Windows��־��SysLog��ʽ��־	


#define UDPSYSLOGPORT	514

#define SYSLOGCONFIGFILENAME	L"SysLogConfig.ini"
#define SYSLOGSECTION			L"[SysLog Section]"
#define SERVERMAJORIP			L"ServerMajorIP"
#define SERVERMINORIP			L"ServerMinorIP"
#define SERVERPORT				L"ServerPort"


//��Ҫ�Խṹ
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


//����ģ��ṹ
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

//�·ݽṹ
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

//������־���ͱ�ʶ��
#define	LOG_CLASS_SHEET		_T("S")
#define	LOG_CLASS_USB		_T("U")
#define	LOG_CLASS_OPERATION	_T("O")
#define	LOG_CLASS_WINDOWS	_T("W")



class CSysLog
{
public:
	CSysLog();
	~CSysLog();

	BOOL CreateUDPCommunication();	//������XzmSysLog��ͨ��

	BOOL GetLogDataFromDB();		//�����ݿ��л�ȡ��־��Ϣ

	BOOL HandleXzmSysLogFile();		//����XzmSysLog�ļ�

private:

	BOOL ConvertPRIToXzmSysLog(enumFACILITY nFacility, enumSEVERITY nSeverity, CString& strPRI);		//ת��XzmSysLog ��PRI��

	BOOL ParseAndConvertLogDateToXzmSysLog(DOUBLE fTime, CString& strDate, BOOL bYear = TRUE);	//������־��ת��Ϊsyslog��ʽ

	BOOL ParseXzmSysLogConfigFile(BYTE* pchXzmSysLog);		//���������ļ�����ȡXzmSysLog������IP

	BOOL GetMonthString(int nMonth,CString& strMonth);	//��ȡ�·��ַ���

private:
	char m_ServerMajorIP[64];
	char m_ServerMinorIP[64];
	int m_ServerPort;

	CNetSocket* m_pNetSock;
	SOCKET m_sock;

	CString m_strDeviceID;

};


