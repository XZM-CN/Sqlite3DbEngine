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

	// ��ȡ������Ϣ
	/**
	 * @brief             GetConfInfo
	 *
	 * @Function          ��ȡ�����ļ�·������ȡ�����������ļ�����ַ���
	 *
	 * @return            ��ȡ�������ļ�·������TRUE�����򷵻�FALSE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL GetConfInfo();
	/**
	 * @brief             ParseSyslogConfigFile
	 *
	 * @Function          ���������ļ�����ȡServerMajorIP��m_ServerMajorIP
	 *                                  ��ȡServerMinorIP��m_ServerMinorIP
	 *                                  ��ȡServerPort   ��m_ServerPort
	 *
	 * @param[  _In_   ]  pchSyslog�����ļ��������ı��ַ�
	 *
	 * @return            ��ȡ�������ļ���������Ϣ����TRUE�����򷵻�FALSE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseSyslogConfigFile(BYTE* pchSyslog);

public:

	char m_ServerMajorIP[64];	// ��IP
	char m_ServerMinorIP[64];	// ����IP
	int  m_ServerPort;			// �˿�

	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;

	SOCKET serversocket;  //��ʵ����һ��unsig int���ͣ�������¼�Ѿ�����������δ�������׽��ֺ�  
	SOCKET clientsocket;  


	int status; //��ʵ����һ��unsig int���ͣ�������¼�Ѿ�����������δ�������׽��ֺ�
};

