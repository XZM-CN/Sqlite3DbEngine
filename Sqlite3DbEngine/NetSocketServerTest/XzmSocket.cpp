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
	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;
	SOCKET serversocket;  //��ʵ����һ��unsig int���ͣ�������¼�Ѿ�����������δ�������׽��ֺ�  
	SOCKET clientsocket;
	int addrlen = sizeof(SOCKADDR_IN);   //16
	int status;
	int numrcv;
	char buffer[MAXBUFLEN];

	CXzmSocket *pXzmSocket = (CXzmSocket*)pParam;
	//��0  
	memset(&(serverSockAddr),0,sizeof(serverSockAddr));
	//��ʼ��  
	serverSockAddr.sin_port = htons(PORT);
	serverSockAddr.sin_family = AF_INET;    //ָ����ַЭ����  
	serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //��ʼ��ip��ַ0,0,0,0 �κ�ip����������


	//�����׽���  
	serversocket = socket(AF_INET,SOCK_STREAM/*�׽�������*/,0/*�׽���ʹ�õ��ض�Э��*/);
	if (INVALID_SOCKET == serversocket)  
	{
		OutputDebugString(_T("����socketʧ��\n"));
	}

	//��socket���ض���ַ(�ṹ���ַ)  
	status = bind(serversocket,(LPSOCKADDR)&(serverSockAddr),sizeof(serverSockAddr));
	if (SOCKET_ERROR==status)  
	{
		OutputDebugString(_T("�󶨵�ַʧ��\n"));
	}



	//�������������������󣬻���˵���Ǽ���  
	status = ::listen(serversocket/*�׽��ֺ�*/,2/*����������ӵ��׽��ָ���*/);  
	if (SOCKET_ERROR == status)  
	{
		OutputDebugString(_T("����ʧ��\n"));
	}  


	//�������������Ǿͽ�����������  
	clientsocket = accept(serversocket,(LPSOCKADDR)&clientsocket,&addrlen);

	int i = 0;
	while (1)
	{
		numrcv = recv(clientsocket/*�Ѿ��������׽���*/,buffer/*�����������ݻ�������ָ��*/,MAXBUFLEN/*���ܻ�������С*/,0/*������Ʒ�ʽ*/);
		if ((0==numrcv)||(numrcv==SOCKET_ERROR))
		{
			OutputDebugString(_T("�����ܵ�����\n"));

			status = closesocket(clientsocket);
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("�Ͽ�����ʧ��\n"));

			status = WSACleanup();
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("��������ʧ��\n"));

			return(1);  


		}

		CString str;
		str.Format(_T("���ܵ��ͻ��˷���������Ϣ�����ǣ�%S\n"),buffer);
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

	OutputDebugString(_T("�ȴ��������ܿͻ��˷�������Ϣ..........\n"));


	return 0;
}

UINT CXzmSocket::DoClientTest(LPVOID pParam)
{
	return 0;
}
