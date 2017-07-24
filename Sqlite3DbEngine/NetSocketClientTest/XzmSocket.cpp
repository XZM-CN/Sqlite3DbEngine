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
	return 0;
}

UINT CXzmSocket::DoClientTest(LPVOID pParam)
{  
	SOCKADDR_IN destSockAddr;  
	SOCKET destSocket;  
	unsigned long destAddr;  
	int status;  

	/* convert IP address into in_addr form */  
	destAddr=inet_addr(DEST_IP_ADDR);

	/* copy destAddr into sockaddr_in structure */  
	memcpy(&destSockAddr.sin_addr, &destAddr, sizeof(destAddr));

	/* specify the port portion of the address */  
	destSockAddr.sin_port=htons(PORT);

	/* specify the address family as Internet */  
	destSockAddr.sin_family=AF_INET;  

	/* create a socket */  
	destSocket=socket(AF_INET, SOCK_STREAM, 0);  
	if (destSocket == INVALID_SOCKET)  
	{
		OutputDebugString(_T("ERROR: socket unsuccessful\n"));

		status=WSACleanup();  
		if (status == SOCKET_ERROR)
			OutputDebugString(_T("ERROR: WSACleanup unsuccessful\n"));

		return(1);  
	}  

	CString str;
	str.Format(_T("服务器发来的消息内容是：%S\n"),DEST_IP_ADDR);
	OutputDebugString(str);

	/* connect to the server */  
	status=connect(destSocket, (LPSOCKADDR)&destSockAddr, sizeof(destSockAddr));  
	if (status == SOCKET_ERROR)  
	{
		OutputDebugString(_T("ERROR: connect unsuccessful\n"));


		status=closesocket(destSocket);  
		if (status == SOCKET_ERROR)
			OutputDebugString(_T("ERROR: closesocket unsuccessful\n"));

		status=WSACleanup();  
		if (status == SOCKET_ERROR)
			OutputDebugString(_T("ERROR: WSACleanup unsuccessful\n"));

		return(1);  
	}  

	OutputDebugString(_T("Connected...\n"));

	int i = 0;
	while(1)  
	{  
		int numsnt;  
		char *toSendtxt=new char[256];  

		OutputDebugString(_T("输入想要发送给服务器的消息内容并按回车发送:\n"));
		sprintf(toSendtxt,"Client Send Data%d",i++);

		numsnt=send(destSocket, toSendtxt, strlen(toSendtxt) + 1, 0);  
		if (numsnt != (int)strlen(toSendtxt) + 1)  
		{
			OutputDebugString(_T("Connection terminated\n"));


			status=closesocket(destSocket);  
			if (status == SOCKET_ERROR)
				OutputDebugString(_T("ERROR: closesocket unsuccessful\n"));

			status=WSACleanup();  
			if (status == SOCKET_ERROR)
				OutputDebugString(_T("ERROR: WSACleanup unsuccessful\n"));

			return(1);  
		}  

		/* Wait before sending the message again */  
		OutputDebugString(_T("向服务器发送的消息已经成功发送现在等待接受服务器发来的消息......\n"));

		int numrcv;  
		char buffer[MAXBUFLEN];  
		numrcv = recv(destSocket/*已经建立的套接字*/,buffer/*接受输入数据缓冲器的指针*/,MAXBUFLEN/*接受缓冲区大小*/,0/*传输控制方式*/);  

		if ((0==numrcv)||(numrcv==SOCKET_ERROR))  
		{
			OutputDebugString(_T("链接受到限制\n"));

			status = closesocket(destSocket);  
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("断开链接失败\n"));

			status = WSACleanup();  
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("清理链接失败\n"));
			return(1);  
		}
		
		CString str;
		str.Format(_T("服务器发来的消息内容是：%S\n"),buffer);
		OutputDebugString(str);
		OutputDebugString(str);

		Sleep(2000);
	} /* while */  
	return 0;  
}
