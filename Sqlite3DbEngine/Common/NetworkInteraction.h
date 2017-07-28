//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/07/27  
// ><创建时间 ><  2017年:07月:27日   10时:35分:12秒  
// ><文件     ><  NetworkInteraction.h  
// ><文件路径 ><  G:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\Common  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  网络注册信息客户端(服务器)
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:07月:27日   10时:35分:12秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _HXPARSEJSON_H
#define _HXPARSEJSON_H

#include "NetSocket.h"

#define BLANKSPACE	L" "
#define LEFTBRACE	L"{"
#define RIGHTBRACE	L"}"
#define JSONCOLON	L":"
#define LEFTBRACKET		L"["
#define RIGHTBRACKET	L"]"

#define TCPPORT	514

#define TCPCONFIGFILENAME	L"NetworkInteractionConfig.ini"
#define TCPSECTION			L"[NetworkInteraction Section]"
#define TCPSERVERMAJORIP			L"ServerMajorIP"
#define TCPSERVERMINORIP			L"ServerMinorIP"
#define TCPSERVERPORT				L"ServerPort"


typedef enum _tagCommandType
{
	enumRegistar1 = 1,
	enumRegistar2,
	enumRegistar3,
	enumUnregistar,
	enumHeartbeat,
	enumGetSysResource,
	enumGetSysType,
	enumScanDisk = 101,
	enumDelProcess,
	enumAddProcess,
	enumSendWhiteList,
	enumInstallProgram,
	enumUDiskDevice = 201,
	enumAddUDisk,
	enumTurnOnSelf = 301,
	enumIntercept,
	enumSafeLevel,
	enumUDiskSet,
	enumDLogin,
	enumAuthFile,
	enumAuthName,
	enumRemoveAuth,
	enumTurnOnServer,
	enumGetConfigStatus,
	enumUserName = 401
}COMMANDTYPE;

typedef struct _tagSysResource
{
	DWORD dwCPUUsageRate;
	DWORD dwMemoryUsageRate;
	DWORD dwMemorySize;
	DWORD dwDiskUsageRate;
	DWORD dwDiskSize;
}SYSRESOURCE;

typedef struct _tagSystemType
{
	CString strComputeName;
	CString strCurrentUserName;
	CString strSysVersion;
	CString strSysBit;
	CString strHostIP;
}SYSTEMTYPE;

typedef struct _tagClientCfgStatus
{
	DWORD	dwTurnOnSelf;		//开启或关闭自身防护，1开启，0关闭
	DWORD	dwIntercept;		//开启或关闭进程拦截，1开启，0关闭
	DWORD	dwSafeLevel;		//设置安全等级，1代表低级，2代表中级，3代表高级
	DWORD	dwSetUDisk;			//设置U盘控制级别，1代表读写，2代表只读，3代表禁用
	DWORD	dwDLogin;			//开启或关闭双因子登录，1代表开启，0代表关闭
	DWORD	dwTurnOnServer;		//开启或关闭进程防护
}CLIENTCFGSTATUS;


class CNetworkInteraction
{
public:
	CNetworkInteraction();
	~CNetworkInteraction();


	//网络通信
	BOOL CreateConnect();					//创建通信接口
	BOOL SendJSonPacket();					//发送数据包到平台
	BOOL RecvJSonPacket(CString strJSon);	//接收平台数据包

	// 获取配置信息
	/**
	 * @brief             GetConfInfo
	 *
	 * @Function          获取配置文件路径，读取出整个配置文件里的字符串
	 *
	 * @return            获取到配置文件路径返回TRUE，否则返回FALSE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL GetConfInfo();
	/**
	 * @brief             ParseSyslogConfigFile
	 *
	 * @Function          解析配置文件，获取ServerMajorIP给m_ServerMajorIP
	 *                                  获取ServerMinorIP给m_ServerMinorIP
	 *                                  获取ServerPort   给m_ServerPort
	 *
	 * @param[  _In_   ]  pchSyslog配置文件的整个文本字符
	 *
	 * @return            获取到配置文件的配置信息返回TRUE，否则返回FALSE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseSyslogConfigFile(BYTE* pchSyslog);

	//解析
	BOOL ParseJsonFromNetwork(CString strCommand);
	BOOL ParseJsonRegister();				//解析平台返回状态
	BOOL ParseJsonUnRegister();				//解析卸载命令
	BOOL ParseJsonSysResource();			//解析获取系统资源信息
	BOOL ParseJsonSysType();				//解析获取系统版本信息
	BOOL ParseJsonScanDisk();				//解析扫描磁盘命令
	BOOL ParseJsonDelProcess();				//解析删除单个进程白名单命令
	BOOL ParseJsonAddProcess();				//解析添加单个进程白名单命令
	BOOL ParseJsonSendWhiteList();			//解析上传白名单返回状态命令
	BOOL ParseJsonInstallProgram();			//解析安装程序命令
	BOOL ParseJsonUDiskDevice();			//解析上传U盘信息返回状态命令
	BOOL ParseJsonAddUDisk();				//解析添加安全U盘命令
	BOOL ParseJsonTurnOnSelf();				//解析开启自身防护命令
	BOOL ParseJsonIntercept();				//解析设置拦截功能命令
	BOOL ParseJsonSafeLevel();				//解析设置安全等级命令
	BOOL ParseJsonUDiskSet();				//解析U盘控制设置命令
	BOOL ParseJsonDLogin();					//解析双因子登录设置命令
	BOOL ParseJsonAuthFile();				//解析授权文件命令
	BOOL ParseJsonUpdateAuth();				//解析更新授权命令
	BOOL ParseJsonRemoveAuth();				//解析移除授权命令
	BOOL ParseJsonTurnOnServer();			//解析开启进程白名单防护命令
	BOOL ParseJsonGetConfig();				//解析获取当前配置信息命令
	BOOL ParseJsonUserName();				//解析上传用户信息返回状态命令


	//打包
	BOOL ConvertStringToJSon(CString strSource, CString& strJSon);		//打包数据头信息
	BOOL PacketJSonRegister1();				//打包注册到平台的数据包
	BOOL PacketJsonRegister2();				//打包注册成功信息
	BOOL PacketJsonUnRegister();			//打包卸载返回状态
	BOOL PacketJsonSysResource();			//打包系统资源信息
	BOOL PacketJsonSysType();				//打包系统版本信息
	BOOL PacketJsonScanDisk();				//打包接收扫描磁盘命令的状态信息
	BOOL PacketJsonDelProcess();			//打包删除单个进程白名单执行状态信息
	BOOL PacketJsonAddProcess();			//打包添加单个进程白名单命令
	BOOL PacketJsonSendWhiteList();			//打包上传白名单返回状态命令
	BOOL PacketJsonInstallProgram();		//打包安装程序命令
	BOOL PacketJsonUDiskDevice();			//打包上传U盘信息返回状态命令
	BOOL PacketJsonAddUDisk();				//打包添加安全U盘命令
	BOOL PacketJsonTurnOnSelf();			//打包开启自身防护命令
	BOOL PacketJsonIntercept();				//打包设置拦截功能命令
	BOOL PacketJsonSafeLevel();				//打包设置安全等级命令
	BOOL PacketJsonUDiskSet();				//打包U盘控制设置命令
	BOOL PacketJsonDLogin();				//打包双因子登录设置命令
	BOOL PacketJsonAuthFile();				//打包授权文件命令
	BOOL PacketJsonUpdateAuth();			//打包更新授权命令
	BOOL PacketJsonRemoveAuth();			//打包移除授权命令
	BOOL PacketJsonTurnOnServer();			//打包开启进程白名单防护命令
	BOOL PacketJsonGetConfig();				//打包获取当前配置信息命令
	BOOL PacketJsonUserName();				//打包上传用户信息返回状态命令

	//处理网络心跳包
	BOOL PacketJsonHeartBeat();				//打包心跳包




private:
	CNetSocket* m_pNetSock;
	SOCKET m_sock;

	char m_ServerMajorIP[64];	// 主IP
	char m_ServerMinorIP[64];	// 备用IP
	int  m_ServerPort;			// 端口

	CString m_strHeartBeat;		//心跳包字符串

	CString m_strSendBuffer;	//发送缓冲区
	CString m_strRecvBuffer;	//接收缓冲区

	CString	m_strClientID;		//客户端设备ID

	//客户端配置信息
	BOOL	m_bRegisterSuccess;	//注册成功状态

	SYSRESOURCE m_sysResource;	//系统资源
	SYSTEMTYPE  m_sysType;		//系统类型信息

	CString m_strScanPath;		//扫描路径
	DWORD	m_dwAddOrDelProcess;	//添加或删除进程，1添加，2删除
	CString m_strProcessPath;	//进程绝对路径

	CString m_strInstallPath;	//需要安装程序的路径
	CString m_strUDiskDevice;	//U盘设备号
	CString m_strUDiskDrive;	//U盘盘符
	DWORD	m_dwAddOrDelUDisk;	//添加或删除安全U盘，1添加，0删除

	CLIENTCFGSTATUS m_ConfigStatus;		//当前配置信息

	CString m_AuthFile;			//授权文件名，全路径
	CString m_AuthBuffer;		//授权文件内容
	DWORD	m_dwRemoveAuth;		//移除授权文件标志，1代表移除
	BOOL	m_bConfigStatus;	//当前配置状态
};








#endif
