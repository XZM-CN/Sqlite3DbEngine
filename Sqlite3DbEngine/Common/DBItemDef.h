
/**
 * @ingroup 共用头文件，几个模块共用
 *
 * @brief 数据库ID常量定义
 *
 *
 * @Date 2016-02-15
 *
 * @author WZH
 *
 */
// DbItemDef.h
#pragma once

#define	TDHXKJ_SKFAPP_FILELEN					1024

#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif

#define	TDHXKJ_SKFAPP_NAME						_T("HXSafe")
#define	TDHXKJ_SKFAPP_CONFIGFILE				_T("HX_Access.cfg")
#define	TDHXKJ_SKFAPP_AUTHFILE					_T("HXAuth.hsa")
#define	TDHXKJ_SKFAPP_HELPFILE					_T("Help.pdf")
#define	TDHXKJ_SKFAPP_SYSLOGCONFIG				_T("SyslogConfig.ini")

#define	TDHXKJ_SERVICEEVENT_CMD					_T("Global\\HXSafeGuard")
#define	TDHXKJ_SERVICEEVENT_ALERT				_T("Global\\HXSafeAlert")
#define	TDHXKJ_SERVICEEVENT_USB					_T("Global\\HXSafeUsb")
#define	TDHXKJ_SERVICEEVENT_APP					_T("Global\\HXSafeApp")
#define	TDHXKJ_SERVICEEVENT_INS					_T("Global\\HXSafeIns")
#define	TDHXKJ_SERVICEEVENT_SETUP				_T("Global\\HXSetupEvent")

/// 定义数据库表缺省版本，3表示SQLITE 3，除非SQLITE本身升级，否则不修改此数
/// 定义版本为3900
#define SQLITEDB_OPTIONVALUE_DEFAULTVERSION		3900
/// 数据库访问超时缺省时间
#define SQLITE_OPEN_DEFAULTOVERTIME				15

/// 定义超级管理员账号
#define	TDHX_ACCOUNT_SA							_T("_sa")
#define	TDHX_ACCOUNT_SANAME						_T("超级管理员")
#define	TDHX_ACCOUNT_SANAME_TEMP				_T("SuperAdmin")
/// 定义管理员账号
#define	TDHX_ACCOUNT_ADMIN						_T("_admin")
#define	TDHX_ACCOUNT_ADMINNAME					_T("管理员")
#define	TDHX_ACCOUNT_ADMINNAME_TEMP				_T("Admin")
/// 定义审计账号
#define	TDHX_ACCOUNT_AUDIT						_T("_audit")
#define	TDHX_ACCOUNT_AUDITNAME					_T("审计员")
#define	TDHX_ACCOUNT_AUDITNAME_TEMP				_T("Audit")

/// 加密存储，确保安全
#define	TDHX_SQLITEDB_SYSFILE					_T("HXSys.hdb")
/// 临时系统文件
#define	TDHX_SQLITEDB_SYSTEMPFILE				_T("HXTempSys.hdb")
/// 加密存储，确保安全
#define	TDHX_SQLITEDB_AUTHFILE					_T("HXAuth.hdb")
/// 不加密存储，确保性能
#define	TDHX_SQLITEDB_LOGFILE					_T("HXLog.hdb")
/// 安装过程数据库
#define	TDHX_SQLITEDB_INSFILE					_T("HXIns.hdb")

/// 白名单数据
#define	TDHX_FILEDB_WHITELIST					_T("HXRun.dat")
/// 受保护磁盘目录
#define	TDHX_FILEDB_PROCDIR						_T("HXDir.dat")
/// 受保护注册表
#define	TDHX_FILEDB_PROCREG						_T("HXReg.dat")

/////////////////////     基础的表定义     //////////////////////
/// 用于保存选项的数据库表名称
#define SQLITEDB_TABLE_SYSPARA					_T("T_Para")

/////////////////////    字段定义     //////////////////////
/// 版本和选项表中的第一个字段名称
#define SQLITEDB_OPTIONRECORD_FIELDNAME1        _T("ParaName")
/// 版本和选项表中的第二个字段名称
#define SQLITEDB_OPTIONRECORD_FIELDNAME2        _T("ParaValue")
/// 数据库版本标记，方便以后升级数据库
#define SQLITEDB_OPTIONNAME_DBVERSION			_T("DBVersion")
/// 文件白名单生成时间
#define SQLITEDB_OPTIONNAME_FILEWHITETIME		_T("FWTime")

/////////////////////     日志数据库定义     //////////////////////
/// 用于保存操作控制的数据库表名称
#define SQLITEDB_TABLE_SYSLOG					_T("T_Sys")
/// 用于预警的数据库表名称
#define SQLITEDB_TABLE_WARNLOG					_T("T_Warn")

/// 用于保存系统日志的数据库表名称
#define SQLITEDB_TABLE_SYS_EVENTLOG				_T("T_Event")
/////////////////////     系统数据库定义     //////////////////////
/// 用于保存账号的数据库表名称
#define SQLITEDB_TABLE_ACCOUNT					_T("T_Account")
/// 文件白名单表
#define	SQLITEDB_TABLE_FILEWHITE				_T("T_File")
/// USB白名单表
#define	SQLITEDB_TABLE_USBWHITE					_T("T_USBs")
/// 授权数据表
#define	SQLITEDB_TABLE_AUTHLIST					_T("T_Auths")
/// 授权用户表
#define	SQLITEDB_TABLE_AUTHUSER					_T("T_Users")
/// 禁用系统服务表
#define	SQLITEDB_TABLE_STOPSERVICE				_T("T_StopService")

/// 临时账号表
#define	SQLITEDB_TABLE_TEMP_ACCOUNT				_T("T_TempAccount")


/// 软件包数据库
#define	TDHXKJ_PACKETTABLE_PATH					_T("T_PathList")
#define	TDHXKJ_PACKETTABLE_FILE					_T("T_FileList")
#define	TDHXKJ_PACKETTABLE_CONTROL				_T("T_Control")


/////////////////////////////  注册表项目定义    ///////////////////////////////////////////
/// 配置节点名称
#define	TDHXKJ_CONFIG_NODENAME				_T("Config")
/// 自身保护
#define	TDHXKJ_CONFIG_SELFPROTECT			_T("SelfProtect")
/// 临时控制驱动卸载开关
#define	TDHXKJ_CONFIG_CONTROLDRIVER			_T("ControlDriver")
/// 防护开关
#define	TDHXKJ_CONFIG_SAFEGUARD				_T("SafeGuard")
/// 保护开关
#define	TDHXKJ_CONFIG_SAFEFLAG				_T("SafeFlag")
/// 安全级别
#define	TDHXKJ_CONFIG_SAFELEVEL				_T("SafeLevel")
/// 系统双因子登录
#define	TDHXKJ_CONFIG_GINALOGIN				_T("GinaLogin")
/// 配置更新项
#define	TDHXKJ_CONFIG_SAFEUPDATE			_T("Update")
/// 数据库访问TOKEN
#define	TDHXKJ_CONFIG_DBTOKEN				_T("DBToken")
/// 服务句柄
#define	TDHXKJ_CONFIG_SERVICEPID			_T("ServicePID")
///安装新程序或更新模式
#define	TDHXKJ_CONFIG_INSTALLORUPDATE		_T("SetupMode")
///扫描文件模式
#define	TDHXKJ_CONFIG_SCANINGFILE			_T("ScaningFile")

/// APP句柄
#define	TDHXKJ_CONFIG_APPMSGWND				_T("AppHandle")
/// 管理工具句柄
#define	TDHXKJ_CONFIG_MGRMSGWND				_T("MgrHandle")
///设备控制程序哈希值
#define	TDHXKJ_CONFIG_DICONTROLHASH			_T("DIControlHash")
///拦截程序扩展名
#define	TDHXKJ_CONFIG_EXTFLAG				_T("ExtFlag")
///非白名单运行拦截标记
#define	TDHXKJ_CONFIG_RUNCONTROL			_T("RunControl")
///USB盘使用控制
#define	TDHXKJ_CONFIG_UCONTROL				_T("UControl")
///U盘白名单
#define	TDHXKJ_CONFIG_UDISK					_T("UDisk")
///受保护进程
#define	TDHXKJ_CONFIG_PROTECTEXT			_T("ProtectExe")
///可信运行路径
#define	TDHXKJ_CONFIG_RUNDIR				_T("RunDir")
///白名单文件路径
#define	TDHXKJ_CONFIG_RUNDATA				_T("RunData")
///白名单文件哈希值
#define	TDHXKJ_CONFIG_RUNDATAHASH			_T("RunDataHash")
///需要保护的磁盘目录文件
#define	TDHXKJ_CONFIG_DIRDATA				_T("DirData")
///需要保护的磁盘目录文件哈希值
#define	TDHXKJ_CONFIG_DIRDATAHASH			_T("DirDataHash")
///需要保护的注册表项目文件
#define	TDHXKJ_CONFIG_REGDATA				_T("RegData")
///需要保护的注册表项目文件哈希值
#define	TDHXKJ_CONFIG_REGDATAHASH			_T("RegDataHash")

/// 预警节点名称
#define	TDHXKJ_ALERT_LOGTYPE				_T("AlertType")
#define	TDHXKJ_ALERT_LOGID					_T("AlertID")
/// 安装通知节点
#define	TDHXKJ_INSTALL_FILE					_T("InsFile")

/// 锁定界面分钟
#define	TDHXKJ_LOCKUI_MINUTE				_T("LockUI")
/// 服务错误
#define	TDHXKJ_LOCKUI_SERVCIEERR			__T("ServiceErr")
/// 驱动错误
#define	TDHXKJ_LOCKUI_DRIVERERR				__T("DriverErr")
//////////////////////////////////////////////////////////////////////////
