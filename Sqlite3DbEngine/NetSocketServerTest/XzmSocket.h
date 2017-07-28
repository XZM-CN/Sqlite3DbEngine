#pragma once

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

#define		MAXBUFLEN	1024
#define		PORT		514

class CXzmSocket
{
public:
	CXzmSocket(void);
	~CXzmSocket(void);

	static UINT DoServerTest(LPVOID pParam);
	static UINT DoClientTest(LPVOID pParam);

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

public:

	char m_ServerMajorIP[64];	// 主IP
	char m_ServerMinorIP[64];	// 备用IP
	int  m_ServerPort;			// 端口

	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;

	SOCKET serversocket;  //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号  
	SOCKET clientsocket;  


	int status; //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号
};

