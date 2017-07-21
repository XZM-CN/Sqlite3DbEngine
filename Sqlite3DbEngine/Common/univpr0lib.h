/// 内核驱动IO控制命令定义
#ifndef _UNIVPRLIB_H_
#define _UNIVPRLIB_H_

//设置DeviceIoControl调用者身份
#define UvProt_SetDIControl					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x01, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置自身保护状态，不拦截程序运行等
#define UvProt_SelfSafeGuard				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x02, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置是否可卸载驱动，停止服务
#define UvProt_UnloadDriver					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x03, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置HOST进程ID
#define UvProt_HostPID						(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x04, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置是否处于安装升级卸载其他程序模式
#define UvProt_InsPID						(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x05, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置或读取文件、注册表、进程等的保护标志
#define UvProt_OperateSafeFlag			    (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x06, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置是否处于扫描程序过程中
#define UvProt_ScaningFile					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x07, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置是否处于扫描磁盘过程中
#define UvProt_ScaningDisk					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x08, METHOD_BUFFERED, FILE_WRITE_DATA )


//添加白名单文件
#define UvProt_AddWhiteFile			        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x11, METHOD_BUFFERED, FILE_WRITE_DATA )

//删除白名单文件
#define UvProt_DelWhiteFile			        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x12, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置拦截类型
#define UvProt_SetExtFlag			        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x13, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置非白名单文件运行拦截还是运行
#define UvProt_SetRunControl		        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x14, METHOD_BUFFERED, FILE_WRITE_DATA )

//添加受保护进程
#define UvProt_AddProtectExe		        (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x15, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置受保护目录
#define UvProt_SetDirProtect				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x16, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置受保护注册表
#define UvProt_SetRegProtect				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x17, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置非白名单U盘访问权限
#define UvProt_SetUDiskAccess			    (ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x18, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置U盘白名单
#define UvProt_SetUdiskWhite				(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x19, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置可信任目录，暂时不启用
#define UvProt_SetRunDir					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x31, METHOD_BUFFERED, FILE_WRITE_DATA )

//设置本地磁盘符，用于移动U盘的扫描
#define UvProt_SetLocalDisk					(ULONG) CTL_CODE( FILE_DEVICE_DISK_FILE_SYSTEM, 0x32, METHOD_BUFFERED, FILE_WRITE_DATA )

/// U盘白名单最大长度
#define MAX_UDISK_WHITE_LEN					( 26*sizeof(WCHAR) )

/*
/// 服务描述结构
typedef struct ServiceDescriptorEntry {
    unsigned int *ServiceTableBase;
    unsigned int *ServiceCounterTableBase; 
    unsigned int NumberOfServices;
    unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;

__declspec(dllimport) ServiceDescriptorTableEntry_t ServiceDescriptorTable;

#define SYSTEMSERVICE(_function) ServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)] 
*/

#ifndef DEVICE_NAME_SZ
#define DEVICE_NAME_SZ	256
#endif

typedef struct _VOLUMN_RIGHT
{
	UCHAR	uRight;
	WCHAR	szVolumeName[DEVICE_NAME_SZ];
}VOLUMN_RIGHT,*PVOLUMN_RIGHT;

#endif