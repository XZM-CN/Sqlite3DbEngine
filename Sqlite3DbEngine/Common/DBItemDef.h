
/**
 * @ingroup ����ͷ�ļ�������ģ�鹲��
 *
 * @brief ���ݿ�ID��������
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

/// �������ݿ��ȱʡ�汾��3��ʾSQLITE 3������SQLITE���������������޸Ĵ���
/// ����汾Ϊ3900
#define SQLITEDB_OPTIONVALUE_DEFAULTVERSION		3900
/// ���ݿ���ʳ�ʱȱʡʱ��
#define SQLITE_OPEN_DEFAULTOVERTIME				15

/// ���峬������Ա�˺�
#define	TDHX_ACCOUNT_SA							_T("_sa")
#define	TDHX_ACCOUNT_SANAME						_T("��������Ա")
#define	TDHX_ACCOUNT_SANAME_TEMP				_T("SuperAdmin")
/// �������Ա�˺�
#define	TDHX_ACCOUNT_ADMIN						_T("_admin")
#define	TDHX_ACCOUNT_ADMINNAME					_T("����Ա")
#define	TDHX_ACCOUNT_ADMINNAME_TEMP				_T("Admin")
/// ��������˺�
#define	TDHX_ACCOUNT_AUDIT						_T("_audit")
#define	TDHX_ACCOUNT_AUDITNAME					_T("���Ա")
#define	TDHX_ACCOUNT_AUDITNAME_TEMP				_T("Audit")

/// ���ܴ洢��ȷ����ȫ
#define	TDHX_SQLITEDB_SYSFILE					_T("HXSys.hdb")
/// ��ʱϵͳ�ļ�
#define	TDHX_SQLITEDB_SYSTEMPFILE				_T("HXTempSys.hdb")
/// ���ܴ洢��ȷ����ȫ
#define	TDHX_SQLITEDB_AUTHFILE					_T("HXAuth.hdb")
/// �����ܴ洢��ȷ������
#define	TDHX_SQLITEDB_LOGFILE					_T("HXLog.hdb")
/// ��װ�������ݿ�
#define	TDHX_SQLITEDB_INSFILE					_T("HXIns.hdb")

/// ����������
#define	TDHX_FILEDB_WHITELIST					_T("HXRun.dat")
/// �ܱ�������Ŀ¼
#define	TDHX_FILEDB_PROCDIR						_T("HXDir.dat")
/// �ܱ���ע���
#define	TDHX_FILEDB_PROCREG						_T("HXReg.dat")

/////////////////////     �����ı���     //////////////////////
/// ���ڱ���ѡ������ݿ������
#define SQLITEDB_TABLE_SYSPARA					_T("T_Para")

/////////////////////    �ֶζ���     //////////////////////
/// �汾��ѡ����еĵ�һ���ֶ�����
#define SQLITEDB_OPTIONRECORD_FIELDNAME1        _T("ParaName")
/// �汾��ѡ����еĵڶ����ֶ�����
#define SQLITEDB_OPTIONRECORD_FIELDNAME2        _T("ParaValue")
/// ���ݿ�汾��ǣ������Ժ��������ݿ�
#define SQLITEDB_OPTIONNAME_DBVERSION			_T("DBVersion")
/// �ļ�����������ʱ��
#define SQLITEDB_OPTIONNAME_FILEWHITETIME		_T("FWTime")

/////////////////////     ��־���ݿⶨ��     //////////////////////
/// ���ڱ���������Ƶ����ݿ������
#define SQLITEDB_TABLE_SYSLOG					_T("T_Sys")
/// ����Ԥ�������ݿ������
#define SQLITEDB_TABLE_WARNLOG					_T("T_Warn")

/// ���ڱ���ϵͳ��־�����ݿ������
#define SQLITEDB_TABLE_SYS_EVENTLOG				_T("T_Event")
/////////////////////     ϵͳ���ݿⶨ��     //////////////////////
/// ���ڱ����˺ŵ����ݿ������
#define SQLITEDB_TABLE_ACCOUNT					_T("T_Account")
/// �ļ���������
#define	SQLITEDB_TABLE_FILEWHITE				_T("T_File")
/// USB��������
#define	SQLITEDB_TABLE_USBWHITE					_T("T_USBs")
/// ��Ȩ���ݱ�
#define	SQLITEDB_TABLE_AUTHLIST					_T("T_Auths")
/// ��Ȩ�û���
#define	SQLITEDB_TABLE_AUTHUSER					_T("T_Users")
/// ����ϵͳ�����
#define	SQLITEDB_TABLE_STOPSERVICE				_T("T_StopService")

/// ��ʱ�˺ű�
#define	SQLITEDB_TABLE_TEMP_ACCOUNT				_T("T_TempAccount")


/// ��������ݿ�
#define	TDHXKJ_PACKETTABLE_PATH					_T("T_PathList")
#define	TDHXKJ_PACKETTABLE_FILE					_T("T_FileList")
#define	TDHXKJ_PACKETTABLE_CONTROL				_T("T_Control")


/////////////////////////////  ע�����Ŀ����    ///////////////////////////////////////////
/// ���ýڵ�����
#define	TDHXKJ_CONFIG_NODENAME				_T("Config")
/// ������
#define	TDHXKJ_CONFIG_SELFPROTECT			_T("SelfProtect")
/// ��ʱ��������ж�ؿ���
#define	TDHXKJ_CONFIG_CONTROLDRIVER			_T("ControlDriver")
/// ��������
#define	TDHXKJ_CONFIG_SAFEGUARD				_T("SafeGuard")
/// ��������
#define	TDHXKJ_CONFIG_SAFEFLAG				_T("SafeFlag")
/// ��ȫ����
#define	TDHXKJ_CONFIG_SAFELEVEL				_T("SafeLevel")
/// ϵͳ˫���ӵ�¼
#define	TDHXKJ_CONFIG_GINALOGIN				_T("GinaLogin")
/// ���ø�����
#define	TDHXKJ_CONFIG_SAFEUPDATE			_T("Update")
/// ���ݿ����TOKEN
#define	TDHXKJ_CONFIG_DBTOKEN				_T("DBToken")
/// ������
#define	TDHXKJ_CONFIG_SERVICEPID			_T("ServicePID")
///��װ�³�������ģʽ
#define	TDHXKJ_CONFIG_INSTALLORUPDATE		_T("SetupMode")
///ɨ���ļ�ģʽ
#define	TDHXKJ_CONFIG_SCANINGFILE			_T("ScaningFile")

/// APP���
#define	TDHXKJ_CONFIG_APPMSGWND				_T("AppHandle")
/// �����߾��
#define	TDHXKJ_CONFIG_MGRMSGWND				_T("MgrHandle")
///�豸���Ƴ����ϣֵ
#define	TDHXKJ_CONFIG_DICONTROLHASH			_T("DIControlHash")
///���س�����չ��
#define	TDHXKJ_CONFIG_EXTFLAG				_T("ExtFlag")
///�ǰ������������ر��
#define	TDHXKJ_CONFIG_RUNCONTROL			_T("RunControl")
///USB��ʹ�ÿ���
#define	TDHXKJ_CONFIG_UCONTROL				_T("UControl")
///U�̰�����
#define	TDHXKJ_CONFIG_UDISK					_T("UDisk")
///�ܱ�������
#define	TDHXKJ_CONFIG_PROTECTEXT			_T("ProtectExe")
///��������·��
#define	TDHXKJ_CONFIG_RUNDIR				_T("RunDir")
///�������ļ�·��
#define	TDHXKJ_CONFIG_RUNDATA				_T("RunData")
///�������ļ���ϣֵ
#define	TDHXKJ_CONFIG_RUNDATAHASH			_T("RunDataHash")
///��Ҫ�����Ĵ���Ŀ¼�ļ�
#define	TDHXKJ_CONFIG_DIRDATA				_T("DirData")
///��Ҫ�����Ĵ���Ŀ¼�ļ���ϣֵ
#define	TDHXKJ_CONFIG_DIRDATAHASH			_T("DirDataHash")
///��Ҫ������ע�����Ŀ�ļ�
#define	TDHXKJ_CONFIG_REGDATA				_T("RegData")
///��Ҫ������ע�����Ŀ�ļ���ϣֵ
#define	TDHXKJ_CONFIG_REGDATAHASH			_T("RegDataHash")

/// Ԥ���ڵ�����
#define	TDHXKJ_ALERT_LOGTYPE				_T("AlertType")
#define	TDHXKJ_ALERT_LOGID					_T("AlertID")
/// ��װ֪ͨ�ڵ�
#define	TDHXKJ_INSTALL_FILE					_T("InsFile")

/// �����������
#define	TDHXKJ_LOCKUI_MINUTE				_T("LockUI")
/// �������
#define	TDHXKJ_LOCKUI_SERVCIEERR			__T("ServiceErr")
/// ��������
#define	TDHXKJ_LOCKUI_DRIVERERR				__T("DriverErr")
//////////////////////////////////////////////////////////////////////////
