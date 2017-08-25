// HXTestPlatformUDPServer.cpp : main source file for HXTestPlatformUDPServer.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"


CAppModule _Module;

void ConsoleEcho(BOOL bEcho)
{
	if(!bEcho)
		return;

	// �����ڵ�Ӧ�ó��򣬴���һ����ʱ�Ŀ���̨���Թ�cout���
	AllocConsole();

	HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);

	int hcin = _open_osfhandle((intptr_t)hin,_O_TEXT);
	FILE* fpin = _fdopen(hcin,"r");
	*stdin = *fpin; 

	int hcout = _open_osfhandle((intptr_t)hout,_O_TEXT);
	FILE* fpout = _fdopen(hcout,"wt");
	*stdout = *fpout;

	std::ios_base::sync_with_stdio();

	std::cout << "Test Console Echo" << endl << endl;
}


bool initSocketLib()
{
	WSADATA Data;

	//��ʼ��windows Socket Dll
	int status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		wprintf(_T("��ʼ��ʧ��\n"));
		return false;
	}

	return true;
}

void outputHostIP()
{
	printf("********************************\n");
	printf("����һ����ȷ���\n");


	int nLen=256;
	char hostname[20];
	gethostname(hostname,nLen); 
	hostent *pHost = gethostbyname(hostname);
	LPSTR lpAddr = pHost->h_addr_list[0];
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	printf("��  ��  IP��ַ��%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+4,4);
	printf("��  ��  IP��ַ��%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+8,4);
	printf("��  ��  IP��ַ��%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+12,4);
	printf("�����1 IP��ַ��%s\n",inet_ntoa(inAddr));
	memmove(&inAddr,lpAddr+16,4);
	printf("�����2 IP��ַ��%s\n",inet_ntoa(inAddr));


	printf("********************************\n\n");
	printf("********************************\n");
	printf("������,��ӡ����Ip,��֤��һ�����\n");
	char szHost[256] = {0};
	hostent *lpHost = gethostbyname(szHost);
	memset(szHost,0,sizeof(szHost));

	// ȡ�ñ�����������
	::gethostname(szHost, 256);

	// ͨ���������õ���ַ��Ϣ


	// ��ӡ������IP��ַ
	in_addr addr;
	for(int i = 0; ; i++)
	{
		char *p = lpHost->h_addr_list[i];
		if(p == NULL)
			break;
		memcpy(&addr.S_un.S_addr, p, lpHost->h_length);
		char *szIp = ::inet_ntoa(addr);//��32λ�Ķ�������ת��Ϊ�ַ���;
		printf("����IP��ַ��%s \n", szIp);
	}
	printf("********************************\n");
}

void Innermain()
{
	if(!initSocketLib())
		return;

	// ��ӡ������ַ
	outputHostIP();


	// ��ʼ�����ؿͻ����׽���
	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
	if(serSocket == INVALID_SOCKET)
	{
		printf("socket error !");
		return ;
	}

	// "����"�ķ���˵�ַӳ�䣨IP:port��
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(514);
	serAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.67");// ��Ϊ����Զ�̻������ͻ���Ҳ���ڱ����ϣ����԰�ʵ�ʵ������ַ

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
			printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			printf(recvData);
		}

		//char * sendData = "һ�����Է���˵�UDP���ݰ�\n";
		//sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);    

	}

	closesocket(serSocket); 
	WSACleanup();
	return ;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// ����̨����
	ConsoleEcho(TRUE);

	Innermain();

	::CoUninitialize();

	return 0;
}
