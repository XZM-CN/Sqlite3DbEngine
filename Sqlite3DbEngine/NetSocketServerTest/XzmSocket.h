#pragma once



#define		MAXBUFLEN	1024
#define		PORT		514

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

