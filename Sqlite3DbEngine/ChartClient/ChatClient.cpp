#include "StdAfx.h"
#include "ChatClient.h"


CChatClient::CChatClient()
{
	m_bIsConnected = false;
}

void CChatClient::Init(string sIpAddress, int iPort)
{

	m_sServerIPAddress = sIpAddress;
	m_iServerPort = iPort;
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in servAddr;


	WSADATA wsaData;

	int wsaret=WSAStartup(0x101,&wsaData);


	if(wsaret!=0)
	{
		return;
	}

	conn=socket(AF_INET,SOCK_STREAM,0);
	if(conn==INVALID_SOCKET)
		return;

	addr=inet_addr(m_sServerIPAddress.c_str());
	hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);

	if(hp==NULL)
	{
		closesocket(conn);
		return;
	}

	servAddr.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
	servAddr.sin_family=AF_INET;
	servAddr.sin_port=htons(m_iServerPort);

	if(connect(conn,(struct sockaddr*)&servAddr,sizeof(servAddr)))
	{
		closesocket(conn);
		return;	
	}
	m_bIsConnected = true;
	return;
}

CChatClient::~CChatClient()
{
	if(m_bIsConnected)
		closesocket(conn);
}

int CChatClient::SendMessagePort(string sMessage)
{
	int iStat = 0;

	iStat = send(conn,sMessage.c_str(),sMessage.size()+1,0);
	if(iStat == -1)
		return 1;

	return 0;

}

int CChatClient::RecMessagePort()
{

	char acRetData[4096];
	int iStat = 0;

	iStat = recv(conn,acRetData,4096,0);
	if(iStat == -1)
		return 1;
	cout<<"-->"<<acRetData<<"\n";

	return 0;

}

UINT CChatClient::MessageRecThread(LPVOID pParam)
{
	CChatClient* pChatServer = (CChatClient*)pParam;
	while(1)
	{
		if(pChatServer->RecMessagePort())
			break;
	}
	return 0;
}
