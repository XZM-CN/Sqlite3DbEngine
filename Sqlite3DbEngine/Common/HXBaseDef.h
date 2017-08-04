/**
 * @file HXBaseDef.h
 *
 * Copyright (c) 2016-?,��غ��˿Ƽ�
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief һЩ�����ͻ����ඨ��
 *
 *
 *
 * @version 
 *
 * @Date 2016-01-18
 *
 * @author ������
 *
 * @history 
 *
 */

//  HXBaseDef.h
/////////////////////////////////////////////////////////////////

#pragma once

/// ����STL������
#include <vector>
#include <list>
#include <map>

/// ʹ��ATL��CString
#include <atlstr.h>
#include "HXVersionDef.h"

typedef enum EHXLogType
{
	LOGTYPE_INVALID		= 0,	/// ��Ч��־
	LOGTYPE_DATABASE	= 1,	/// д�����ݿ����־
	LOGTYPE_NORMAL		= 2,	/// ��ͨ��־
	LOGTYPE_WARNING		= 4,	/// ������־
	LOGTYPE_ERROR		= 8,	/// ������־
	LOGTYPE_FATAL		= 16,	/// ����������־
	LOGTYPE_RUN			= 32	/// ������־�����������
} ELogType;

#ifdef _DEBUG
#undef		APP_COPYRIGHT_CODE
#define		APP_LOG_ENABLE  LOGTYPE_NORMAL
#else
#define		APP_LOG_ENABLE  LOGTYPE_NORMAL
//#define	APP_LOG_ENABLE  LOGTYPE_WARNING
#endif

//////////////////////////////////////////////////////////////////////////

///////////////////////// ��˾����Ʒ��ע����� /////////////////////////

#ifdef TDHXKJ_VRSION_SINGLE
/// �����Ʒ��������
#ifdef QDAK_VRSION_SINGLE
#define TDHXKJ_APPLICATION_NAME	_T("������������ϵͳ")
#else
#define TDHXKJ_APPLICATION_NAME	_T("����������ȫ����ϵͳ")
#endif
#endif

#ifdef TDHXKJ_VRSION_NET
/// �����Ʒ��������
#ifdef QDAK_VRSION_SINGLE
#define TDHXKJ_APPLICATION_NAME	_T("����������ȫ�����ͻ���")
#else
#define TDHXKJ_APPLICATION_NAME	_T("����������ȫ�����ͻ���")
#endif
#endif


/// ���幫˾����
#define COMPANY_NAME				_T("TDHX")

#ifdef QDAK_VRSION_SINGLE
/// ���幫˾����
#define COMPANY_NAME_CHS			_T("�ൺ����")
#else
/// ���幫˾����
#define COMPANY_NAME_CHS			_T("��غ���")
#endif

/// ����ע����й�˾��·��
#define COMPANY_REGPATH				ATL::CString(_T("Software\\"))+COMPANY_NAME

/// ������������ϵ����
#define	TDHXKJ_EMAIL_ADDR			_T("wangzh@tdhxkj.com")
/// ������������վ��ַ
#define	TDHXKJ_WEB_ADDR				_T("http://www.tdhxkj.com")

/// ����ע����в�Ʒ�ڵ�����
#define PRODUCT_REGNODENAME			_T("HXSafeGuard")
/// ����������ȫ����Ӧ��ע���·��
#define TDHXKJ_SAFEGUARDAPP_REGPATH	ATL::CString(COMPANY_REGPATH)+ATL::CString(_T("\\"))+PRODUCT_REGNODENAME

/// ���˰�ȫ�ļ�����������������
#define TDHXKJ_SAFEDRIVER_NAME		_T("HXSafeGuard")
/// ����������ȫ������������
#define TDHXKJ_HOSTSERVICE_NAME		_T("HXHostService")
/// ����������ȫ��������ע���·��
#define TDHXKJ_HOSTSERVICE_REGPATH	ATL::CString(_T("SYSTEM\\CurrentControlSet\\Services\\"))+TDHXKJ_HOSTSERVICE_NAME

/// ϵͳ�¼�֪ͨ���񣬴���U�̵��Ȳ��֪ͨSystem Event Notification Service
#define TDHXKJ_SYSTEMSERVICE_SENS	_T("SENS")

/// Ĭ�ϰ�װ·��
#define TDHXKJ_DEFAULT_SETUP_DIR	_T("D:\\Program Files\\")
//////////////////////////////////////////////////////////////////////////


///////////////////////////// ģ�鶨�� //////////////////////////////////
#define TDHXKJ_SYSCOREDLL			_T("Kernel32.dll")
#define TDHXKJ_SYNTDLL				_T("NTdll.dll")

#define TDHXKJ_SAFEDRIVER			_T("HXSafeGuard.sys")
#define TDHXKJ_SQLITEENGINEDLL		_T("HXEng.dll")
#define TDHXKJ_JSONENGINE			_T("JsonEngine.dll")
#define	TDHXKJ_BASEENGINEDLL		_T("DispatchModuleCenter.dll")
#define	TDHXKJ_GINADLL				_T("HXSafeGina.dll")
#define	TDHXKJ_LOGINDLL				_T("HXSafeProvider.dll")
#define	TDHXKJ_DATAENGINEDLL		_T("HXData.dll")
#define	TDHXKJ_ENCRYENGINEDLL		_T("SKFEncry.dll")
#define	TDHXKJ_SKFSUPPORTDLL		_T("SKF_CC.dll")
#define	TDHXKJ_BUTTRAPDLL			_T("BugTrap.dll")
#define	TDHXKJ_GETSYSEVENTLOGDLL	_T("HXGetSysEventLog.dll")

#define	TDHXKJ_MAIN_SAFEAPP			_T("HXSafeApp.exe")
#define	TDHXKJ_SAFEMGR				_T("HXSafeMgr.exe")
#define	TDHXKJ_HOSTSERVICE			_T("HXHostService.exe")
#define	TDHXKJ_SAFEINSTALL			_T("HXSafeSetup.exe")
#define	TDHXKJ_SAFETOOL				_T("HXSafeTool.exe")

//������־�ļ�������С
#define	TDHXKJ_LOG_ALERT_SIZE		80
#define	TDHXKJ_DEFAULT_PASSWORD			_T("HXSafe2016")
////////////////////////////////////////////////////////////////////////////////////

#define SYS_EVENT_LOG_NAME		_T("System")
#define APP_EVENT_LOG_NAME		_T("Application")
#define SEC_EVENT_LOG_NAME		_T("Security")
#define SETUP_EVENT_LOG_NAME	_T("Setup")

#define EVENT_LOG_SUCCESS_NAME			_T("�ɹ�")
#define EVENT_LOG_ERROR_NAME			_T("����")
#define EVENT_LOG_WARNING_NAME			_T("����")
#define EVENT_LOG_INFORMATION_NAME		_T("��Ϣ")
#define EVENT_LOG_AUDIT_SUCCESS_NAME	_T("��Ƴɹ�")
#define EVENT_LOG_AUDIT_FAILURE_NAME	_T("���ʧ��")

#define FIND_LOG_CLASS_NAME		_T("CLASS")
#define FIND_LOG_ID_NAME		_T("ID")
#define FIND_LOG_TIME_NAME		_T("TIME")
#define FIND_LOG_LEVEL_NAME		_T("LEVEL")

///////////////////////////// �������� //////////////////////////////////

///////////////////////////// ��ƽ̨ͨ�� //////////////////////////////////

#define JSON_STRING_CLIENT_ID		_T("CLIENTID")
#define JSON_STRING_CRC32			_T("CRC32")
#define JSON_STRING_CMD_ID			_T("CMDID")
#define JSON_STRING_CONTENT			_T("CONTENT")
#define JSON_STRING_REGISTAR		_T("REGISTAR")
#define JSON_STRING_CLIENTTYPE		_T("CLIENTTYPE")

#define JSON_STRING_AUTHCODE		_T("AUTHCODE")
#define JSON_STRING_UNREGISTAR		_T("UNREGISTAR")
#define JSON_STRING_HEART_BEAT		_T("HEARTBEAT")
#define JSON_STRING_GETSYS_RESOURCE	_T("GETSYSRESOURCE")
#define JSON_STRING_GETSYS_TYPE		_T("GETSYSTYPE")

#define JSON_STRING_SCAN_DISK		_T("SCANDISK")
#define JSON_STRING_SCAN_ALL		_T("ALL")
#define JSON_STRING_DEL_PROCESS		_T("DELPROCESS")
#define JSON_STRING_PROCSS_PATH		_T("PROCSSPATH")
#define JSON_STRING_ADD_PROCESS		_T("ADDPROCESS")

#define JSON_STRING_SEND_WHITELIST		_T("SENDWHITELIST")
#define JSON_STRING_INSTALL_PROGRAM		_T("INSTALLPROGRAM")
#define JSON_STRING_UDISK_DEVICE		_T("UDISKDEVICE")
#define JSON_STRING_UDISK_CHAR			_T("UDISKCHAR")
#define JSON_STRING_ADD_UDISK		_T("ADDUDISK")

#define JSON_STRING_TURNON_SELF		_T("TURNONSELF")
#define JSON_STRING_INTERCEPT		_T("INTERCEPT")
#define JSON_STRING_SAFE_LEVEL		_T("SAFELEVEL")
#define JSON_STRING_UDISK_SET		_T("UDISKSET")
#define JSON_STRING_DLOGIN			_T("DLOGIN")

#define JSON_STRING_AUTH_FILE		_T("AUTHFILE")
#define JSON_STRING_AUTH_BUFFER		_T("AUTHBUFFER")
#define JSON_STRING_AUTH_NAME		_T("AUTHNAME")
#define JSON_STRING_REMOVE_AUTH		_T("REMOVEAUTH")
#define JSON_STRING_TURNON_SERVER	_T("TURNONSERVER")

#define JSON_STRING_GET_CONFIG_STATUS		_T("GETCONFIGSTATUS")
#define JSON_STRING_USER_NAME		_T("USERNAME")
#define JSON_STRING_PASSWD			_T("PASSWD")

#define JSON_STRING_COMPUTRENAME	_T("COMPUTERNAME")
#define JSON_STRING_CURRENTUSER		_T("CURRENTUSER")
#define JSON_STRING_SYSVERSION		_T("SYSVERSION")
#define JSON_STRING_SYSBIT			_T("SYSBIT")
#define JSON_STRING_HOSTIP			_T("HOSTIP")





#ifndef THREADINFOCLASS
typedef enum _THREADINFOCLASS {
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger,
    ThreadBreakOnTermination,
    MaxThreadInfoClass
} THREADINFOCLASS;
#endif

#ifndef TIME_FIELDS
typedef struct _TIME_FIELDS {
    WORD Year;        // range [1601...]
    WORD Month;       // range [1..12]
    WORD Day;         // range [1..31]
    WORD Hour;        // range [0..23]
    WORD Minute;      // range [0..59]
    WORD Second;      // range [0..59]
    WORD Milliseconds;// range [0..999]
    WORD Weekday;     // range [0..6] == [Sunday..Saturday]
} TIME_FIELDS;
typedef TIME_FIELDS *PTIME_FIELDS;
#endif

typedef void ( __stdcall *lpFN_GNSI)(LPSYSTEM_INFO); 
#ifndef WIN64
/// 64λ֧�ֺ�������
typedef BOOL ( __stdcall *lpIsWow64Process)(HANDLE,PBOOL);
typedef BOOL ( __stdcall *lpWow64RevertWow64FsRedirection)(PVOID OldValue);
typedef BOOL ( __stdcall *lpWow64DisableWow64FsRedirection)(PVOID *OldValue);
#endif
/// ע����ѯ
typedef ULONG (__stdcall *lpNtQueryKey)(HANDLE,int,PVOID,ULONG,PULONG);
/// ��������Ȩ�޽ӿ�
typedef ULONG (__stdcall *lpRtlAdjustPrivilege)(ULONG,ULONG,ULONG,PVOID);
/// �����߳���Ϣ�����ⱻ����NtSetInformationThread(GetCurrentThread(), (THREADINFOCLASS)0x11, 0, 0);
typedef ULONG (__stdcall *lpSetInformationThreadPtr)(HANDLE threadHandle,
							THREADINFOCLASS threadInformationClass,PVOID threadInformation,ULONG threadInformationLength);

#ifndef SE_DEBUG_PRIVILEGE
#define SE_DEBUG_PRIVILEGE 20
#endif

typedef struct _tagDelelteRepeat
{
	ATL::CString strFileName;
	ATL::CString strMd5;
	ATL::CString	strFid;
	BOOL	bFlag;
	long	lTurnOn;
}DELETEREPEAT;

/// �ַ���vector
typedef std::vector		<ATL::CString>				CSTRING_VECTOR;
/// �ַ���list
typedef std::list		<ATL::CString>				CSTRING_LIST;
/// �ַ������Զ�
typedef std::pair		<ATL::CString,ATL::CString>	CSTRING_PAIR;
/// ���Զ��б�
typedef std::vector		<CSTRING_PAIR>				PAIR_LIST;
/// �ַ�������
typedef std::map		<ATL::CString,ATL::CString>	CSTRING_MAP;
/// �ַ����;������
typedef std::map		<ATL::CString,HINSTANCE>	STRINGHANDLE_MAP;
/// ��ֵvector
typedef std::vector		<DWORD>						NUMBER_VECTOR;
/// ��ֵ����
typedef std::map		<DWORD,DWORD>				NUMBER_MAP;
/// �ַ���-����
typedef std::map		<ATL::CString,DWORD>		CSTRINGDWORD_MAP;
/// �ṹ��list
typedef std::list		<DELETEREPEAT>				DELETEREPEAT_LIST;
////////////////////////////////////////////////////////////////////////////////////

typedef struct tagSystemEventLogInfo
{
	unsigned long Sys_EventTime;		//ʱ��
	unsigned long Sys_EventID;			//�¼�ID
	unsigned long Sys_EventType;		//��־���ͣ�0-�ɹ���1-����2-���桢3-��Ϣ
	unsigned long Sys_EventClass;		//��־���ࣺ0-��ȫ��1-ϵͳ��2-Ӧ�ó���3-����
	ATL::CString Sys_EventSource;		//�¼���Դ
	ATL::CString Sys_ComputerName;		//�������
	ATL::CString Sys_ComputerUser;		//�û���
	ATL::CString Sys_TaskCategory;		//�������
	ATL::CString Sys_EventDesc;			//����
}SYSEVENTINFO,*PSYSEVENTINFO;

typedef enum _SYSEVENTCLASS {
	enumSecurityClass,
	enumSystemClass,
	enumApplicationClass,
	enumSetupClass,
	enumUnknownClass,
} SYSEVENTCLASS;



typedef enum __OPENSQLITE3FILEFLAG {
	e_r   = 0x01, // ��ֻ����ʽ���ļ������ļ�������ڡ�
	e_r_  = 0x02, // �Զ�/д��ʽ���ļ������ļ�������ڡ�
	e_rb_ = 0x03, // �Զ�/д��ʽ��һ���������ļ���ֻ�����/д���ݡ�
	e_rt_ = 0x04, // �Զ�/д��ʽ��һ���ı��ļ����������д��
	e_a   = 0x05, // �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�����ļ�ԭ�ȵ����ݻᱻ������EOF ����������
	e_a_  = 0x06, // �Ը��ӷ�ʽ�򿪿ɶ�/д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������ԭ���� EOF ������������
	e_ab_ = 0x07, // �Զ�/д��ʽ��һ���������ļ�������������ļ�ĩ׷�����ݡ�
	e_at_ = 0x08, // �Զ�/д��ʽ��һ���ı��ļ�������������ı�ĩ׷�����ݡ�
	e_w   = 0x09, // ��ֻд�ļ������ļ������򳤶���Ϊ 0�������ļ�������ʧ�����������򴴽����ļ���
	e_w_  = 0x0A, // �򿪿ɶ�/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������������ļ���
	e_wb  = 0x0B, // ��ֻд��ʽ�򿪻��½�һ���������ļ���ֻ����д���ݡ�
	e_wb_ = 0x0C, // �Զ�/д��ʽ�򿪻���һ���������ļ����������д��
	e_wt_ = 0x0D, // �Զ�/д��ʽ�򿪻���һ���ı��ļ��������д��
} OPENSQLITE3FILEFLAG;


