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

	SOCKET serversocket;  //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号  
	SOCKET clientsocket;  


	int status; //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号
};

