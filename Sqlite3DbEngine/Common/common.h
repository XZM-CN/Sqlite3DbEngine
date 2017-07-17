
#define IOCTL_MSG_READ		0xbf682000
#define IOCTL_MSG_WRITE		0xbf682004
#define IOCTL_MSG_GET_SPEED	0xbf682020

#define UCTRL_DENIED		0x2
#define UCTRL_READ			0x1
#define UCTRL_WRITE			0x0

#define ANSI_HX_CONFIG_KEY	"System\\CurrentControlSet\\Services\\HXHostService\\Config"
#define ANSI_HXSAFE_KEY		"SOFTWARE\\TDHX\\HXSafeGuard"

//#pragma pack()

enum LOG_MSG_FLAGS
{
	/// 向上报告的错误运行日志
	LOG_RUN_ERR_TOHOSTSERVER	= 0x00000000,
	/// 模块加载通知
	LOG_LOADIMAGE_NAME_FILE		= 0x00000001,
	/// 试图运行非白名单文件，被拒绝后所记的日志
	LOG_DENY_RUN_NON_WHITE		= 0x00000004,
	/// 运行非白名单文件，被允许后所记的日志
	LOG_ALLOW_RUN_NON_WHITE		= 0x00000005,
	/// 运行白名单文件，所记的日志	
	LOG_RUN_WHITE				= 0x00000006,

	/// 进程保护被尝试终止通知	
	LOG_TERMINATE_PROTECT_EXE	= 0x00000008,
	/// USB违例访问通知	
	LOG_VIOLATE_USB_ACCESS		= 0x00000009,
	/// 保护目录违例通知	
	LOG_VIOLATE_PROTECT_DIR		= 0x00000010,
	/// 注册表保护违例通知	
	LOG_VIOLATE_PROTECT_REG		= 0x00000011,

	/// 进程保护被尝试终止通知	
	LOG_VIOLATE_DISK_ALERT		= 0x00000003,
	/// 记录安装文件	
	LOG_INSTALL_DISKFILE		= 0x00000050,

	//U盘操作日志，创建文件、写入文件、删除文件，重命名文件、读取文件
	LOG_USB_OP_CREATE_FILE		= 0x00000080,
	LOG_USB_OP_WRITE_FILE		= 0x00000081,
	LOG_USB_OP_DELETE_FILE		= 0x00000082,
	LOG_USB_OP_RENAME_FILE		= 0x00000083,
	LOG_USB_OP_READ_FILE		= 0x00000084
};

enum HSG_SAFE_FLAGS
{
	FL_ENABLE_DIR_PROTECT			= 1,	/// 启用目录保护
	FL_ENABLE_REG_PROTECT			= 2,	/// 启用注册表保护
	FL_ENABLE_PROCESSEND_PROTECT	= 4,	/// 启用进程防杀的保护

	FL_ENABLE_ONLYSELFENABLE		= 512	/// 启用只保护自身项目，与前面对项目配合使用
};

enum DIRREGITEM_TYPE
{
	DIRREG_ITEMTYPE_NOSELF		= 1,	/// 不是自己的项目
	DIRREG_ITEMTYPE_NOCHILD		= 2,	/// 不包含子项目
	DIRREG_ITEMTYPE_SYSTEM		= 4,	/// 系统的项目
	DIRREG_ITEMTYPE_SERVICESTOP	= 8,	/// 服务禁用项目
	DIRREG_ITEMTYPE_OTHER		= 16	/// 第三方的，界面配置

};

enum INTERCEPT_FLAGS
{
	FL_INTERCEPT_EXE		= 1,
	FL_INTERCEPT_BAT		= 2,
	FL_INTERCEPT_JS			= 4,
	FL_INTERCEPT_JAR		= 8,
	FL_INTERCEPT_COM		= 16,
	FL_INTERCEPT_MSI		= 32,
	FL_INTERCEPT_DLL_OCX	= 64,
	FL_INTERCEPT_CAB_INF	= 128,
	FL_INTERCEPT_SYS		= 256
};

#define FL_INTERCEPT_ALL (\
	FL_INTERCEPT_EXE |\
	FL_INTERCEPT_BAT |\
	FL_INTERCEPT_JS |\
	FL_INTERCEPT_JAR |\
	FL_INTERCEPT_COM |\
	FL_INTERCEPT_MSI |\
	FL_INTERCEPT_DLL_OCX |\
	FL_INTERCEPT_CAB_INF |\
	FL_INTERCEPT_SYS \
	)

#define HASH_LEN 16

#ifndef _UE_MAX_PATH
#define _UE_MAX_PATH 296
#endif

typedef struct _HSG_MSG_LOG 
{
	DWORD			m_LogFlag;				/// 做LOG时的标志
	DWORD			m_Pid;					/// 进程ID
	LARGE_INTEGER	m_MsgTime;				/// 产生时间
	WCHAR			m_Parent[_UE_MAX_PATH];	/// 父进程或当前进程路径
	WCHAR			m_Target[_UE_MAX_PATH];	/// 目标进程路径
} HSG_MSG_LOG, *PHSG_MSG_LOG;

#define HSG_MSG_BUF_SIZE 2048
/// 采用下面的定义，可能导致蓝屏
//#define HSG_MSG_BUF_SIZE sizeof(HSG_MSG_LOG)+1

// we do ioctl r/w of up to this many items in each direction
#define MAX_IOCTL_MSG_READ		30	// 读取缓存区大小
#define MAX_IOCTL_MSG_WRITE		30	// 写入缓存区大小
#define IOCTL_MSG_READ_FROM_HSG IOCTL_MSG_READ

#define	DRIVER_IOBUFFLEN		MAX_IOCTL_MSG_READ*HSG_MSG_BUF_SIZE+sizeof(DWORD)

//the structure used for sending IO from R3 to R0 to add white file .
typedef struct _WHITE_ITEM_FOR_IO
{
	BYTE	digest[HASH_LEN];		/// MD5码
	WCHAR	szPath[_UE_MAX_PATH];	/// 只是文件名，不包含路径
}WHITE_ITEM_FOR_IO, *PWHITE_ITEM_FOR_IO;

//Set the caller process identity of DeviceIoControl, MUST matched with process full path and HASH value.
//This setting can be set only ONCE everytime the driver loaded.
typedef struct _SET_DI_CONTROL
{
	BYTE	digest[HASH_LEN];		/// MD5码
	WCHAR	szPath[_UE_MAX_PATH];	/// 全路径
}SET_DI_CONTROL, *PSET_DI_CONTROL;