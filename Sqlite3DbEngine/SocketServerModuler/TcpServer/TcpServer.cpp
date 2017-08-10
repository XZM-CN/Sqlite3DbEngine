// TcpServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <tchar.h> // _T宏
#include <stdio.h>// printf sprintf

#include <iostream>// cout fstream
using namespace std;

// windows socket
#include <winsock.h>
#pragma comment (lib,"ws2_32.lib")

bool initSocketLib();
void outputHostIP();


void _tmain(int argc, _TCHAR* argv[])
{
	if(!initSocketLib())
		return;

	// 打印本机地址
	outputHostIP();


	// 初始化本地客户端套接字
	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
	if(serSocket == INVALID_SOCKET)
	{
		printf("socket error !");
		return ;
	}

	// "本地"的服务端地址映射（IP:port）
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(514);
	serAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.67");// 因为这是远程机器，客户端也不在本机上，所以绑定实际的物理地址

	if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("bind error !");
		closesocket(serSocket);
		return ;
	}

	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr); 
	while (true)
	{
		char recvData[255];
		int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
		if (ret > 0)
		{
			recvData[ret] = 0x00;
			printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			printf(recvData);
		}

		//char * sendData = "一个来自服务端的UDP数据包\n";
		//sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);    

	}

	closesocket(serSocket); 
	WSACleanup();
	return ;

}

bool initSocketLib()
{
	WSADATA Data;

	//初始化windows Socket Dll
	int status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		wprintf(_T("初始化失败\n"));
		return false;
	}

	return true;
}

void outputHostIP()
{
	printf("********************************\n");
	printf("方法一，精确输出\n");


	int nLen=256;
	char hostname[20];
	gethostname(hostname,nLen); 
	hostent *pHost = gethostbyname(hostname);
	LPSTR lpAddr = pHost->h_addr_list[0];
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	printf("有  线  IP地址：%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+4,4);
	printf("无  线  IP地址：%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+8,4);
	printf("环  回  IP地址：%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+12,4);
	printf("虚拟机1 IP地址：%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+16,4);
	printf("虚拟机2 IP地址：%s\n",inet_ntoa(inAddr));


	printf("********************************\n\n");
	printf("********************************\n");
	printf("方法二,打印所有Ip,验证第一次输出\n");
	char szHost[256] = {0};
	hostent *lpHost = gethostbyname(szHost);
	memset(szHost,0,sizeof(szHost));

	// 取得本地主机名称
	::gethostname(szHost, 256);

	// 通过主机名得到地址信息


	// 打印出所有IP地址
	in_addr addr;
	for(int i = 0; ; i++)
	{
		char *p = lpHost->h_addr_list[i];
		if(p == NULL)
			break;
		memcpy(&addr.S_un.S_addr, p, lpHost->h_length);
		char *szIp = ::inet_ntoa(addr);//将32位的二进制数转化为字符串;
		printf("本机IP地址：%s \n", szIp);
	}
	printf("********************************\n");
}

