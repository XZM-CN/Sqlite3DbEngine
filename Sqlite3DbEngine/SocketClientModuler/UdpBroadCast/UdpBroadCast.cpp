#include <tchar.h> // _T宏
#include <stdio.h>// printf sprintf

#include <iostream>// cout fstream
using namespace std;

// windows socket
#include <winsock.h>
#pragma comment (lib,"ws2_32.lib")

bool initSocketLib();

void main()
{
	SOCKET sock; //socket套接字
	char szMsg[] = "this is a UDP test package";//被发送的字段

	// 1.启动SOCKET库，版本为2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err; 
	wVersionRequested = MAKEWORD( 2, 0 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( 0 != err ) {
		cout<<"Socket2.0初始化失败，Exit!";
		return;
	}
	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 ) { // 检查Socket库的版本是否为2.0
		WSACleanup( );
		return; 
	}

	// 2.创建socket
	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_IP/*虚拟IP*/);
	if (INVALID_SOCKET == sock ) {
		cout<<"Socket 创建失败，Exit!";
		return;
	}

	// 3.设置该套接字为广播类型，
	bool opt = true;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST/*广播*/, reinterpret_cast<char FAR *>(&opt), sizeof(opt));

	// 4.设置发往的地址
	sockaddr_in addrto;
	memset(&addrto,0,sizeof(addrto));
	addrto.sin_family = AF_INET;               // 地址类型为internetwork
	addrto.sin_addr.s_addr = INADDR_BROADCAST; // 设置ip为广播地址
	addrto.sin_port = htons(7861);             // 端口号为7861
	int nlen=sizeof(addrto);
	unsigned int uIndex = 1;

	while(true) {
		Sleep(1000);

		//向广播地址发送消息
		if( sendto(sock, szMsg, strlen(szMsg), 0, (sockaddr*)&addrto,nlen)== SOCKET_ERROR ) {
			cout<<WSAGetLastError()<<endl;
		}
		else {
			cout<<uIndex++<<":an UDP package is sended."<<endl;
		}
	}

	if (!closesocket(sock)) { // 关闭套接字
		WSAGetLastError();
		return;
	}
	if (!WSACleanup()) { // 关闭Socket库
		WSAGetLastError();
		return;
	} 

	return;
}

bool initSocketLib()
{
	WSADATA Data;

	//初始化windows Socket Dll
	int status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		printf(_T("初始化失败\n"));
		return false;
	}

	return true;
}