#include "StdAfx.h"
#include "ChatServer.h"
#include "BaseFuncLib.h"
#include "SqliteHelper.h"

CChatServer::CChatServer()
{
	cout << "Starting up TCP Chat server\n";
	m_bIsConnected = false;

	WSADATA wsaData;

	sockaddr_in local;

	int wsaret=WSAStartup(0x101,&wsaData);

	if(wsaret!=0)
	{
		return;
	}

	local.sin_family=AF_INET; 
	local.sin_addr.s_addr=inet_addr("192.168.0.67")/*INADDR_ANY*/; 
	local.sin_port=htons((u_short)514); 

	m_SListenClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);


	if(m_SListenClient==INVALID_SOCKET)
	{
		return;
	}


	if(bind(m_SListenClient,(sockaddr*)&local,sizeof(local))!=0)
	{
		return;
	}


	if(listen(m_SListenClient,10)!=0)
	{
		return;
	}

	m_bIsConnected = true;
	return;
}

CChatServer::~CChatServer()
{
	closesocket(m_SListenClient);

	WSACleanup();
}

void CChatServer::StartListenClient()
{
	sockaddr_in from;
	int fromlen=sizeof(from);

	m_SClient=accept(m_SListenClient,
		(struct sockaddr*)&from,&fromlen);

	if(m_SClient != INVALID_SOCKET)
		m_vClientList.push_back(m_SClient);

	// AfxBeginThread(ServerRecThread,(void *)m_SClient);
	HANDLE hThread;
	DWORD dwThreadId;
	PThreadParam Param = new ThreadParam();
	Param->TP_pChatServer = this;
	Param->TP_socket      = m_SClient;
	hThread=::CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CChatServer::ServerRecThread, Param, 0, &dwThreadId);

}

int CChatServer::SendMultiplyMessagePort(string sMessage)
{
	int iStat = 0;
	list<SOCKET>::iterator itl;

	if(m_vClientList.size() == 0)
		return 0;

	for(itl = m_vClientList.begin();itl != m_vClientList.end();itl++)
	{
		iStat = send(*itl,sMessage.c_str(),sMessage.size()+1,0);
		if(iStat == -1)
			m_vClientList.remove(*itl);
	}

	if(iStat == -1)
		return 1;

	return 0;

}

int CChatServer::SendSingleMessagePort(PSendMsg pMessage)
{

	int iStat = SOCKET_ERROR;

	LONGLONG cmdID = -1000;
	CString strClientID;
	CString strSendData = _T("");
	DWORD dwCrc32 = 0;
	Get_CmdContext(pMessage->TP_string,CComBSTR(JSON_STRING_CMD_ID),cmdID);
	Get_CmdContext(pMessage->TP_string,CComBSTR(JSON_STRING_CLIENT_ID),strClientID);
	switch(cmdID){
	case 1:
		{
			if(TRUE){
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),dwCrc32);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),2);

					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_REGISTAR),2);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));

					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}
		}
		break;
	case 2:
		{
		}
		break;
	default:
		{
		}
		break;
	}

	if (strSendData.IsEmpty())
		return 0;

	char *pText = NULL;
	strSendData = strSendData + _T("\t");
	int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);

	iStat = send(pMessage->TP_socket, pText, strlen(pText), 0);
	if(iStat == -1) {
		m_vClientList.remove(pMessage->TP_socket);
		return 1;
	}

	return 0;
}

int CChatServer::RecClient(SOCKET sRecSocket)
{
	char JsonBuff[4096];
	memset(JsonBuff,0,4096);
	int iStat;

	iStat = recv(sRecSocket,JsonBuff,4096,0);
	if(iStat == -1)
	{
		m_vClientList.remove(sRecSocket);
		return 1;
	}
	else
	{
		SOCKADDR_IN from;
		int nSize = sizeof(from);
		getpeername(sRecSocket,(SOCKADDR *)&from,&nSize);

		cout <<"接收了客户端"<< inet_ntoa(from.sin_addr) <<"的发送请求"<<":"<<JsonBuff<<"\r\n"<< endl;

		if (strlen(JsonBuff) == 0)
			return 0;
		else if(-1 == CBaseFuncLib::StringFind(JsonBuff,"{"))
			return 0;

		// 点对点
		PSendMsg p = new SendMsg();
		p->TP_socket = sRecSocket;
		p->TP_string = JsonBuff;
		SendSingleMessagePort(p);

		return 0;
	}
	return 0;

}

UINT CChatServer::ServerRecThread(LPVOID pParam)
{
	PThreadParam Param = (PThreadParam)pParam;

	CChatServer* pChatServer = Param->TP_pChatServer;
	SOCKET sRecSocket        = Param->TP_socket;
	while(1)
	{
		if(pChatServer->RecClient(sRecSocket))
			break;
	}
	return 0;
}

UINT CChatServer::ServerListenThread(LPVOID pParam)
{
	CChatServer* pChatServer = (CChatServer*)pParam;

	while(1)
		pChatServer->StartListenClient();


	return 0;
}

BOOL CChatServer::GetInterfacePtr(CComPtr <IJsonService>& spiJsonService)
{
	if(NULL == spiJsonService)
		spiJsonService = CDbHelper::GetJsonService();
	if(NULL != spiJsonService)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CChatServer::Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,CString& strJson)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		VARIANT_BOOL bParseRet = FALSE;
		wchar_t *szBuf = NULL;
		CBaseFuncLib::CharToUS2(bstrContent,&szBuf);
		spiJsonService->ParseString(szBuf,&bParseRet);
		cout<<"获取CString的Value"<<endl<<endl;
		spiJsonService->OutputStyledJson();
		cout<<endl<<endl;
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(bstrKeyName,&bstrVal);
		strJson = bstrVal.m_str;
	}
}

void CChatServer::Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,LONGLONG& intJson)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		VARIANT_BOOL bParseRet = FALSE;
		wchar_t *szBuf = NULL;
		CBaseFuncLib::CharToUS2(bstrContent,&szBuf);
		spiJsonService->ParseString(szBuf,&bParseRet);
		cout<<"获取int的Value"<<endl<<endl;
		spiJsonService->OutputStyledJson();
		cout<<endl<<endl;
		LONGLONG bstrVal;
		spiJsonService->GetIntValue(bstrKeyName,&bstrVal);
		intJson = bstrVal;
	}
}