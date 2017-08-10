#include <tchar.h> // _T��
#include <stdio.h>// printf sprintf

#include <iostream>// cout fstream
using namespace std;

// windows socket
#include <winsock.h>
#pragma comment (lib,"ws2_32.lib")

bool initSocketLib();

void main()
{
	SOCKET sock; //socket�׽���
	char szMsg[] = "this is a UDP test package";//�����͵��ֶ�

	// 1.����SOCKET�⣬�汾Ϊ2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err; 
	wVersionRequested = MAKEWORD( 2, 0 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( 0 != err ) {
		cout<<"Socket2.0��ʼ��ʧ�ܣ�Exit!";
		return;
	}
	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) { // ���Socket��İ汾�Ƿ�Ϊ2.0
		WSACleanup( );
		return; 
	}

	// 2.����socket
	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_IP/*����IP*/);
	if (INVALID_SOCKET == sock ) {
		cout<<"Socket ����ʧ�ܣ�Exit!";
		return;
	}

	// 3.���ø��׽���Ϊ�㲥���ͣ�
	bool opt = true;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST/*�㲥*/, reinterpret_cast<char FAR *>(&opt), sizeof(opt));

	// 4.���÷����ĵ�ַ
	sockaddr_in addrto;
	memset(&addrto,0,sizeof(addrto));
	addrto.sin_family = AF_INET;               // ��ַ����Ϊinternetwork
	addrto.sin_addr.s_addr = INADDR_BROADCAST; // ����ipΪ�㲥��ַ
	addrto.sin_port = htons(7861);             // �˿ں�Ϊ7861
	int nlen=sizeof(addrto);
	unsigned int uIndex = 1;

	while(true) {
		Sleep(1000);

		//��㲥��ַ������Ϣ
		if( sendto(sock, szMsg, strlen(szMsg), 0, (sockaddr*)&addrto,nlen)== SOCKET_ERROR ) {
			cout<<WSAGetLastError()<<endl;
		}
		else {
			cout<<uIndex++<<":an UDP package is sended."<<endl;
		}
	}

	if (!closesocket(sock)) { // �ر��׽���
		WSAGetLastError();
		return;
	}
	if (!WSACleanup()) { // �ر�Socket��
		WSAGetLastError();
		return;
	} 

	return;
}

bool initSocketLib()
{
	WSADATA Data;

	//��ʼ��windows Socket Dll
	int status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		printf(_T("��ʼ��ʧ��\n"));
		return false;
	}

	return true;
}