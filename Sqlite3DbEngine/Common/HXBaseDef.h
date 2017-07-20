#pragma once


/// ����STL������
#include <vector>
#include <list>
#include <map>

/// ʹ��ATL��CString
#include <atlstr.h>


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

/// ���幫˾����
#define COMPANY_NAME				_T("TDHX")

/// ����ע����й�˾��·��
#define COMPANY_REGPATH				ATL::CString(_T("Software\\"))+COMPANY_NAME

/// ����ע����в�Ʒ�ڵ�����
#define PRODUCT_REGNODENAME			_T("HXSafeGuard")



///////////////////////////// ģ�鶨�� //////////////////////////////////
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
/// ע����ѯ
typedef ULONG (__stdcall *lpNtQueryKey)(HANDLE,int,PVOID,ULONG,PULONG);

///////////////////////// ��˾����Ʒ��ע����� /////////////////////////
#define TDHXKJ_VRSION_SINGLE
#ifdef TDHXKJ_VRSION_SINGLE

/// �����Ʒ��������
#ifdef QDAK_VRSION_SINGLE
#define TDHXKJ_APPLICATION_NAME	_T("������������ϵͳ")
#else
#define TDHXKJ_APPLICATION_NAME	_T("����������ȫ����ϵͳ")
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