#include "StdAfx.h"
#include "XzmSocket.h"


CXzmSocket::CXzmSocket(void)
{
	WSADATA Data;

	//��ʼ��windows Socket Dll
	status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		OutputDebugString(_T("��ʼ��ʧ��\n"));
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
	str.Format(_T("��������������Ϣ�����ǣ�%S\n"),DEST_IP_ADDR);
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

		OutputDebugString(_T("������Ҫ���͸�����������Ϣ���ݲ����س�����:\n"));
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
		OutputDebugString(_T("����������͵���Ϣ�Ѿ��ɹ��������ڵȴ����ܷ�������������Ϣ......\n"));

		int numrcv;  
		char buffer[MAXBUFLEN];  
		numrcv = recv(destSocket/*�Ѿ��������׽���*/,buffer/*�����������ݻ�������ָ��*/,MAXBUFLEN/*���ܻ�������С*/,0/*������Ʒ�ʽ*/);  

		if ((0==numrcv)||(numrcv==SOCKET_ERROR))  
		{
			OutputDebugString(_T("�����ܵ�����\n"));

			status = closesocket(destSocket);  
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("�Ͽ�����ʧ��\n"));

			status = WSACleanup();  
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("��������ʧ��\n"));
			return(1);  
		}
		
		CString str;
		str.Format(_T("��������������Ϣ�����ǣ�%S\n"),buffer);
		OutputDebugString(str);
		OutputDebugString(str);

		Sleep(2000);
	} /* while */  
	return 0;  
}
