/**
 * @file HXBaseDef.h
 *
 * Copyright (c) 2016-?,天地和兴科技
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 一些常量和基础类定义
 *
 *
 *
 * @version 
 *
 * @Date 2016-01-18
 *
 * @author 汪佐怀
 *
 * @history 
 *
 */

//  HXBaseDef.h
/////////////////////////////////////////////////////////////////

#pragma once

/// 包含STL库声明
#include <vector>
#include <list>
#include <map>

/// 使用ATL的CString
#include <atlstr.h>
#include "HXVersionDef.h"

typedef enum EHXLogType
{
	LOGTYPE_INVALID		= 0,	/// 无效日志
	LOGTYPE_DATABASE	= 1,	/// 写入数据库的日志
	LOGTYPE_NORMAL		= 2,	/// 普通日志
	LOGTYPE_WARNING		= 4,	/// 警告日志
	LOGTYPE_ERROR		= 8,	/// 错误日志
	LOGTYPE_FATAL		= 16,	/// 致命错误日志
	LOGTYPE_RUN			= 32	/// 运行日志，必须输出的
} ELogType;

#ifdef _DEBUG
#undef		APP_COPYRIGHT_CODE
#define		APP_LOG_ENABLE  LOGTYPE_NORMAL
#else
#define		APP_LOG_ENABLE  LOGTYPE_NORMAL
//#define	APP_LOG_ENABLE  LOGTYPE_WARNING
#endif

//////////////////////////////////////////////////////////////////////////

///////////////////////// 公司、产品及注册表定义 /////////////////////////

#ifdef TDHXKJ_VRSION_SINGLE
/// 定义产品中文名称
#ifdef QDAK_VRSION_SINGLE
#define TDHXKJ_APPLICATION_NAME	_T("安控主机防护系统")
#else
#define TDHXKJ_APPLICATION_NAME	_T("和兴主机安全防护系统")
#endif
#endif

#ifdef TDHXKJ_VRSION_NET
/// 定义产品中文名称
#ifdef QDAK_VRSION_SINGLE
#define TDHXKJ_APPLICATION_NAME	_T("安控主机安全防护客户端")
#else
#define TDHXKJ_APPLICATION_NAME	_T("和兴主机安全防护客户端")
#endif
#endif


/// 定义公司名称
#define COMPANY_NAME				_T("TDHX")

#ifdef QDAK_VRSION_SINGLE
/// 定义公司名称
#define COMPANY_NAME_CHS			_T("青岛安控")
#else
/// 定义公司名称
#define COMPANY_NAME_CHS			_T("天地和兴")
#endif

/// 定义注册表中公司的路径
#define COMPANY_REGPATH				ATL::CString(_T("Software\\"))+COMPANY_NAME

/// 定义崩溃后的联系邮箱
#define	TDHXKJ_EMAIL_ADDR			_T("wangzh@tdhxkj.com")
/// 定义崩溃后的主站地址
#define	TDHXKJ_WEB_ADDR				_T("http://www.tdhxkj.com")

/// 定义注册表中产品节点名称
#define PRODUCT_REGNODENAME			_T("HXSafeGuard")
/// 和兴主机安全桌面应用注册表路径
#define TDHXKJ_SAFEGUARDAPP_REGPATH	ATL::CString(COMPANY_REGPATH)+ATL::CString(_T("\\"))+PRODUCT_REGNODENAME

/// 和兴安全文件过滤驱动服务名称
#define TDHXKJ_SAFEDRIVER_NAME		_T("HXSafeGuard")
/// 和兴主机安全防护服务名称
#define TDHXKJ_HOSTSERVICE_NAME		_T("HXHostService")
/// 和兴主机安全防护服务注册表路径
#define TDHXKJ_HOSTSERVICE_REGPATH	ATL::CString(_T("SYSTEM\\CurrentControlSet\\Services\\"))+TDHXKJ_HOSTSERVICE_NAME

/// 系统事件通知服务，处理U盘等热插拔通知System Event Notification Service
#define TDHXKJ_SYSTEMSERVICE_SENS	_T("SENS")

/// 默认安装路径
#define TDHXKJ_DEFAULT_SETUP_DIR	_T("D:\\Program Files\\")
//////////////////////////////////////////////////////////////////////////


///////////////////////////// 模块定义 //////////////////////////////////
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

//定义日志文件报警大小
#define	TDHXKJ_LOG_ALERT_SIZE		80
#define	TDHXKJ_DEFAULT_PASSWORD			_T("HXSafe2016")
////////////////////////////////////////////////////////////////////////////////////

#define SYS_EVENT_LOG_NAME		_T("System")
#define APP_EVENT_LOG_NAME		_T("Application")
#define SEC_EVENT_LOG_NAME		_T("Security")
#define SETUP_EVENT_LOG_NAME	_T("Setup")

#define EVENT_LOG_SUCCESS_NAME			_T("成功")
#define EVENT_LOG_ERROR_NAME			_T("错误")
#define EVENT_LOG_WARNING_NAME			_T("警告")
#define EVENT_LOG_INFORMATION_NAME		_T("信息")
#define EVENT_LOG_AUDIT_SUCCESS_NAME	_T("审计成功")
#define EVENT_LOG_AUDIT_FAILURE_NAME	_T("审计失败")

#define FIND_LOG_CLASS_NAME		_T("CLASS")
#define FIND_LOG_ID_NAME		_T("ID")
#define FIND_LOG_TIME_NAME		_T("TIME")
#define FIND_LOG_LEVEL_NAME		_T("LEVEL")

///////////////////////////// 其它定义 //////////////////////////////////

///////////////////////////// 与平台通信 //////////////////////////////////

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
/// 64位支持函数定义
typedef BOOL ( __stdcall *lpIsWow64Process)(HANDLE,PBOOL);
typedef BOOL ( __stdcall *lpWow64RevertWow64FsRedirection)(PVOID OldValue);
typedef BOOL ( __stdcall *lpWow64DisableWow64FsRedirection)(PVOID *OldValue);
#endif
/// 注册表查询
typedef ULONG (__stdcall *lpNtQueryKey)(HANDLE,int,PVOID,ULONG,PULONG);
/// 提升调试权限接口
typedef ULONG (__stdcall *lpRtlAdjustPrivilege)(ULONG,ULONG,ULONG,PVOID);
/// 设置线程信息，避免被调试NtSetInformationThread(GetCurrentThread(), (THREADINFOCLASS)0x11, 0, 0);
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

/// 字符串vector
typedef std::vector		<ATL::CString>				CSTRING_VECTOR;
/// 字符串list
typedef std::list		<ATL::CString>				CSTRING_LIST;
/// 字符串属性对
typedef std::pair		<ATL::CString,ATL::CString>	CSTRING_PAIR;
/// 属性对列表
typedef std::vector		<CSTRING_PAIR>				PAIR_LIST;
/// 字符串集合
typedef std::map		<ATL::CString,ATL::CString>	CSTRING_MAP;
/// 字符串和句柄集合
typedef std::map		<ATL::CString,HINSTANCE>	STRINGHANDLE_MAP;
/// 数值vector
typedef std::vector		<DWORD>						NUMBER_VECTOR;
/// 数值集合
typedef std::map		<DWORD,DWORD>				NUMBER_MAP;
/// 字符串-整数
typedef std::map		<ATL::CString,DWORD>		CSTRINGDWORD_MAP;
/// 结构体list
typedef std::list		<DELETEREPEAT>				DELETEREPEAT_LIST;
////////////////////////////////////////////////////////////////////////////////////

typedef struct tagSystemEventLogInfo
{
	unsigned long Sys_EventTime;		//时间
	unsigned long Sys_EventID;			//事件ID
	unsigned long Sys_EventType;		//日志类型：0-成功、1-错误、2-警告、3-信息
	unsigned long Sys_EventClass;		//日志分类：0-安全、1-系统、2-应用程序、3-设置
	ATL::CString Sys_EventSource;		//事件来源
	ATL::CString Sys_ComputerName;		//计算机名
	ATL::CString Sys_ComputerUser;		//用户名
	ATL::CString Sys_TaskCategory;		//任务类别
	ATL::CString Sys_EventDesc;			//描述
}SYSEVENTINFO,*PSYSEVENTINFO;

typedef enum _SYSEVENTCLASS {
	enumSecurityClass,
	enumSystemClass,
	enumApplicationClass,
	enumSetupClass,
	enumUnknownClass,
} SYSEVENTCLASS;



typedef enum __OPENSQLITE3FILEFLAG {
	e_r   = 0x01, // 以只读方式打开文件，该文件必须存在。
	e_r_  = 0x02, // 以读/写方式打开文件，该文件必须存在。
	e_rb_ = 0x03, // 以读/写方式打开一个二进制文件，只允许读/写数据。
	e_rt_ = 0x04, // 以读/写方式打开一个文本文件，允许读和写。
	e_a   = 0x05, // 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留（EOF 符保留）。
	e_a_  = 0x06, // 以附加方式打开可读/写的文件。若文件不存在，则会建立该文件，如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（原来的 EOF 符不保留）。
	e_ab_ = 0x07, // 以读/写方式打开一个二进制文件，允许读或在文件末追加数据。
	e_at_ = 0x08, // 以读/写方式打开一个文本文件，允许读或在文本末追加数据。
	e_w   = 0x09, // 打开只写文件，若文件存在则长度清为 0，即该文件内容消失，若不存在则创建该文件。
	e_w_  = 0x0A, // 打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
	e_wb  = 0x0B, // 以只写方式打开或新建一个二进制文件，只允许写数据。
	e_wb_ = 0x0C, // 以读/写方式打开或建立一个二进制文件，允许读和写。
	e_wt_ = 0x0D, // 以读/写方式打开或建立一个文本文件，允许读写。
} OPENSQLITE3FILEFLAG;


