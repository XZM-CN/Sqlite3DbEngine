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
	if(!initSocketLib())
		return;

	// ��ʼ�����ؿͻ����׽���
	SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sclient == INVALID_SOCKET)
	{
		printf("socket error !");
		return;
	}

	// ���صķ���˵�ַӳ�䣨IP:port��
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(514);
	sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int len = sizeof(sin);

	// Զ�̵ķ���˵�ַӳ�䣨IP:port��
	sockaddr_in sinEx;
	sinEx.sin_family = AF_INET;
	sinEx.sin_port = htons(514);
	sinEx.sin_addr.S_un.S_addr = inet_addr("192.168.0.53");
	int lenEx = sizeof(sinEx);


	char sendData[512];
	UINT iCount = 0;
	while (true)
	{
		memset(sendData,0,512);
		sprintf(sendData,"client Send %3d\n",iCount++);
		int senRet = sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);
		if(senRet == SOCKET_ERROR ){
			printf("send is error");
		}
		senRet = sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sinEx, lenEx);
		if(senRet == SOCKET_ERROR ){
			printf("send is error");
		}
		printf(sendData);
		/* ���ע�͵Ĵ�����Լ��ϣ�Ϊ�����UDP���û��Ҫ
		char recvData[255];
		int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
		if(ret > 0)
		{
			recvData[ret] = 0x00;
			printf(recvData);
		}
		*/

		Sleep(1000);// ����̫��Ҳûɶ��˼
	}

	closesocket(sclient);
	WSACleanup();

	return;
}

bool initSocketLib()
{
	WSADATA Data;

	//��ʼ��windows Socket Dll
	int status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0 != status)
	{
		printf(_T("��ʼ��ʧ��\n"));
		return false;
	}

	return true;
}