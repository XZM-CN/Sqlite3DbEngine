#pragma once


/// 包含STL库声明
#include <vector>
#include <list>
#include <map>

/// 使用ATL的CString
#include <atlstr.h>


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

/// 定义公司名称
#define COMPANY_NAME				_T("TDHX")

/// 定义注册表中公司的路径
#define COMPANY_REGPATH				ATL::CString(_T("Software\\"))+COMPANY_NAME

/// 定义注册表中产品节点名称
#define PRODUCT_REGNODENAME			_T("HXSafeGuard")



///////////////////////////// 模块定义 //////////////////////////////////
#define TDHXKJ_SYSCOREDLL			_T("Kernel32.dll")
#define TDHXKJ_SYNTDLL				_T("NTdll.dll")

#define TDHXKJ_SAFEDRIVER			_T("HXSafeGuard.sys")
#define TDHXKJ_SQLITEENGINEDLL		_T("HXEng.dll")
#define	TDHXKJ_BASEENGINEDLL		_T("HXBase.dll")
#define	TDHXKJ_GINADLL				_T("HXSafeGina.dll")
#define	TDHXKJ_LOGINDLL				_T("HXSafeProvider.dll")
#define	TDHXKJ_DATAENGINEDLL		_T("HXData.dll")
#define	TDHXKJ_ENCRYENGINEDLL		_T("HXEncry.dll")
#define	TDHXKJ_SKFSUPPORTDLL		_T("SKF_CC.dll")
#define	TDHXKJ_BUTTRAPDLL			_T("BugTrap.dll")
#define	TDHXKJ_GETSYSEVENTLOGDLL	_T("HXGetSysEventLog.dll")

#define	TDHXKJ_MAIN_SAFEAPP			_T("HXSafeApp.exe")
#define	TDHXKJ_SAFEMGR				_T("HXSafeMgr.exe")
#define	TDHXKJ_HOSTSERVICE			_T("HXHostService.exe")
#define	TDHXKJ_SAFEINSTALL			_T("HXSafeSetup.exe")
#define	TDHXKJ_SAFETOOL				_T("HXSafeTool.exe")

typedef void ( __stdcall *lpFN_GNSI)(LPSYSTEM_INFO);
/// 注册表查询
typedef ULONG (__stdcall *lpNtQueryKey)(HANDLE,int,PVOID,ULONG,PULONG);

///////////////////////// 公司、产品及注册表定义 /////////////////////////
#define TDHXKJ_VRSION_SINGLE
#ifdef TDHXKJ_VRSION_SINGLE

/// 定义产品中文名称
#ifdef QDAK_VRSION_SINGLE
#define TDHXKJ_APPLICATION_NAME	_T("安控主机防护系统")
#else
#define TDHXKJ_APPLICATION_NAME	_T("和兴主机安全防护系统")
#endif
#endif


#define SYS_EVENT_LOG_NAME		_T("System")
#define APP_EVENT_LOG_NAME		_T("Application")
#define SEC_EVENT_LOG_NAME		_T("Security")
#define SETUP_EVENT_LOG_NAME	_T("Setup")

typedef enum _SYSEVENTCLASS {
	enumSecurityClass,
	enumSystemClass,
	enumApplicationClass,
	enumSetupClass,
	enumUnknownClass,
} SYSEVENTCLASS;