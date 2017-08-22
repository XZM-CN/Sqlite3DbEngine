#pragma once

//************************************************************************
// Boby Thomas Pazheparampil
// May 2006
// Implementation of CIPMessage class and main.
//************************************************************************

#include <stdio.h>
#include <winsock2.h>
#include <conio.h> 

#include <iostream>

using namespace std;


class CChatClient
{
public:
	CChatClient();
	~CChatClient();
	void Init(string sIpAddress, int iPort);
	int SendMessagePort(string sMessage);
	int RecMessagePort();
	bool IsConnected(){return m_bIsConnected;}
	static UINT  MessageRecThread(LPVOID pParam);
private:
	bool m_bIsConnected; // true - connected false - not connected
	string m_sServerIPAddress;
	int m_iServerPort;
	SOCKET conn; // socket connected to server
};
