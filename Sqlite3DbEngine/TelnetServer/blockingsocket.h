//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/08/01  
// ><����ʱ�� ><  2017��:08��:01��   14ʱ:12��:58��  
// ><�ļ�     ><  blockingsocket.h  
// ><�ļ�·�� ><  C:\Users\xzm\Downloads\ChessServer  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  https://www.codeproject.com/Articles/59/Multithreaded-TCP-IP-Telnet-Server-Chess-Game-Exam
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  Multithreaded TCP/IP Telnet Server - Chess Game �����߳�TCP / IP Telnet������ - ������Ϸʾ����
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:08��:01��   14ʱ:12��:58��  
// ><ԭ��     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999 Lee Patterson
// lee@antws.com
#pragma once

#include "SockAddr.h "
// blocksock.h

#define ASSERT assert
#define VERIFY ASSERT
#define DEFAULT_TIMEOUT  10
// member functions truly block and must not be used in UI threads
// use this class as an alternative to the MFC CSocket class
class CBlockingSocket
{
public:
	SOCKET m_hSocket;
	CBlockingSocket() { m_hSocket = NULL; }
	void Cleanup();
	void Create(int nType = SOCK_STREAM);
	void Close();
	void Bind(LPCSOCKADDR psa);
	void Listen();
	void Connect(LPCSOCKADDR psa);
	bool Accept(CBlockingSocket& s, LPSOCKADDR psa);
	int Send(const char* pch, const int nSize, const int nSecs=DEFAULT_TIMEOUT);
	int Write(const char* pch, const int nSize, const int nSecs=DEFAULT_TIMEOUT);
	int Receive(char* pch, const int nSize, const int nSecs=DEFAULT_TIMEOUT);
	int SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, const int nSecs=10);
	int ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, const int nSecs=10);
	void GetPeerAddr(LPSOCKADDR psa);
	void GetSockAddr(LPSOCKADDR psa);
	static CSockAddr GetHostByName(const char* pchName, const USHORT ushPort = 0);
	static const char* GetHostByAddr(LPCSOCKADDR psa);
	operator SOCKET()
	{
		return m_hSocket;
	}
};
