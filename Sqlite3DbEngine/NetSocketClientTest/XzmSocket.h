#pragma once

#define MAXBUFLEN		1024  
#define PORT			514  
#define DEST_IP_ADDR	"127.0.0.1" //��Ҫ���ӵ�Ŀ��Server address

//
// http://blog.csdn.net/ghevinn/article/details/9970029
//
//////////////////////////////////////////////////////////////////////////
class CXzmSocket
{
public:
	CXzmSocket(void);
	~CXzmSocket(void);

	static UINT DoServerTest(LPVOID pParam);
	static UINT DoClientTest(LPVOID pParam);
public:
	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;

	SOCKET serversocket;  //��ʵ����һ��unsig int���ͣ�������¼�Ѿ�����������δ�������׽��ֺ�  
	SOCKET clientsocket;  


	int status; //��ʵ����һ��unsig int���ͣ�������¼�Ѿ�����������δ�������׽��ֺ�
};

