#include "StdAfx.h"
#include "XzmSocket.h"


CXzmSocket::CXzmSocket(void)
{
	WSADATA Data;

	//初始化windows Socket Dll
	status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		OutputDebugString(_T("初始化失败\n"));
	} 

	OutputDebugString(_T("\n"));
}

CXzmSocket::~CXzmSocket(void)
{
}

UINT CXzmSocket::DoServerTest(LPVOID pParam)
{
	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;
	SOCKET serversocket;  //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号  
	SOCKET clientsocket;
	int addrlen = sizeof(SOCKADDR_IN);   //16
	int status;
	int numrcv;
	char buffer[MAXBUFLEN];

	CXzmSocket *pXzmSocket = (CXzmSocket*)pParam;
	//清0  
	memset(&(serverSockAddr),0,sizeof(serverSockAddr));
	//初始化  
	serverSockAddr.sin_port = htons(PORT);
	serverSockAddr.sin_family = AF_INET;    //指定地址协议族  
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //初始化ip地址0,0,0,0 任何ip都可以连接


	//创建套接字  
	serversocket = socket(AF_INET,SOCK_STREAM/*套接字类型*/,0/*套接字使用的特定协议*/);
	if (INVALID_SOCKET == serversocket)  
	{
		OutputDebugString(_T("创建socket失败\n"));
	}

	//绑定socket到特定地址(结构体地址)  
	status = bind(serversocket,(LPSOCKADDR)&(serverSockAddr),sizeof(serverSockAddr));
	if (SOCKET_ERROR==status)  
	{
		OutputDebugString(_T("绑定地址失败\n"));
	}



	//允许外来申请链接请求，或者说就是监听  
	status = ::listen(serversocket/*套接字号*/,2/*最大容许链接的套接字个数*/);  
	if (SOCKET_ERROR == status)  
	{
		OutputDebugString(_T("监听失败\n"));
	}  


	//当有请求申请是就接受链接请求  
	clientsocket = accept(serversocket,(LPSOCKADDR)&clientsocket,&addrlen);

	int i = 0;
	while (1)
	{
		numrcv = recv(clientsocket/*已经建立的套接字*/,buffer/*接受输入数据缓冲器的指针*/,MAXBUFLEN/*接受缓冲区大小*/,0/*传输控制方式*/);
		if ((0==numrcv)||(numrcv==SOCKET_ERROR))
		{
			OutputDebugString(_T("链接受到限制\n"));

			status = closesocket(clientsocket);
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("断开链接失败\n"));

			status = WSACleanup();
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("清理链接失败\n"));

			return(1);  


		}

		CString str;
		str.Format(_T("接受到客户端发过来的消息内容是：%S\n"),buffer);
		OutputDebugString(str);



		int numsnt;
		char *toSendtxt=new char[256];
		sprintf(toSendtxt,"Server Send Data%d",i++);

		numsnt=send(clientsocket, toSendtxt, strlen(toSendtxt) + 1, 0);
		if (numsnt != (int)strlen(toSendtxt) + 1)
		{
			OutputDebugString(_T("Connection terminated\n"));


			status=closesocket(clientsocket);
			if (status == SOCKET_ERROR)
				OutputDebugString(_T("ERROR: closesocket unsuccessful\n"));

			status=WSACleanup();
			if (status == SOCKET_ERROR)
				OutputDebugString(_T("ERROR: WSACleanup unsuccessful\n"));

			return(1);
		}
	}

	OutputDebugString(_T("等待继续接受客户端发来的消息..........\n"));


	return 0;
}

UINT CXzmSocket::DoClientTest(LPVOID pParam)
{
	return 0;
}
