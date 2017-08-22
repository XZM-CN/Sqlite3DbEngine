//************************************************************************
// Boby Thomas Pazheparampil
// May 2006
// Implementation of CIPMessage class and main.
//************************************************************************
#pragma once


#include <stdio.h>
#include <winsock2.h>
#include <conio.h> 
#include<list>
#include <iostream>
using namespace std;

#pragma comment (lib,"ws2_32.lib")



typedef struct tagSendMsg 
{
	char*  TP_string;
	SOCKET TP_socket;
}SendMsg,*PSendMsg;

class CChatServer
{
public:
	CChatServer();
	~CChatServer();
	bool IsConnected(){return m_bIsConnected;} // returns connection status
	void StartListenClient(); // Listen to client
	int SendMultiplyMessagePort(string sMessage); // Send message to sll clients.
	int SendSingleMessagePort(PSendMsg pMessage); // Send message to sll clients.
	int RecClient(SOCKET sRecSocket); // receive message for a particulat socket

public:
	static UINT  ServerRecThread(LPVOID pParam);
	static UINT  ServerListenThread(LPVOID pParam);

public:
	BOOL GetInterfacePtr(CComPtr <IJsonService>& spiJsonService);
	void Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,CString& strJson);
	void Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,LONGLONG& intJson);

private:
	bool m_bIsConnected; // true - connected false - not connected
	int m_iServerPort;
	list<SOCKET> m_vClientList; // All socket connected to client
	SOCKET m_SClient;
	SOCKET m_SListenClient; // socket listening for client calls
};

typedef struct tagThreadParam{
	CChatServer* TP_pChatServer;
	SOCKET       TP_socket;
	tagThreadParam::tagThreadParam(){
		TP_pChatServer = NULL;
		TP_socket      = NULL;
	}
}ThreadParam,*PThreadParam;
