//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/01  
// ><创建时间 ><  2017年:08月:01日   14时:12分:48秒  
// ><文件     ><  blockingsocket.cpp  
// ><文件路径 ><  C:\Users\xzm\Downloads\ChessServer  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  https://www.codeproject.com/Articles/59/Multithreaded-TCP-IP-Telnet-Server-Chess-Game-Exam
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  Multithreaded TCP/IP Telnet Server - Chess Game （多线程TCP / IP Telnet服务器 - 象棋游戏示例）
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:08月:01日   14时:12分:48秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1999 Lee Patterson
// lee@antws.com

// blocksock.cpp (CBlockingSocketException, CBlockingSocket, CHttpBlockingSocket)
#include <assert.h>
#include <stdio.h>
#include "blockingsocket.h"


// Class CBlockingSocket

void CBlockingSocket::Cleanup()
{
	// doesn't throw an exception because it's called in a catch block
	if(m_hSocket == NULL) return;
	closesocket(m_hSocket);
	m_hSocket = NULL;
}

void CBlockingSocket::Create(int nType /* = SOCK_STREAM */)
{
	ASSERT(m_hSocket == NULL);
	if((m_hSocket = ::socket(AF_INET, nType, 0)) == INVALID_SOCKET) {
		throw "Create";
	}
}

void CBlockingSocket::Bind(LPCSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	if(::bind(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw "Bind";
	}
}

void CBlockingSocket::Listen()
{
	ASSERT(m_hSocket != NULL);
	if(::listen(m_hSocket, 5) == SOCKET_ERROR) {
		//throw "Listen";
		throw "Listen";
	}
}

bool CBlockingSocket::Accept(CBlockingSocket& sConnect, LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	ASSERT(sConnect.m_hSocket == NULL);
	int nLengthAddr = sizeof(SOCKADDR);
	sConnect.m_hSocket = ::accept(m_hSocket, psa, &nLengthAddr);
	if(sConnect == INVALID_SOCKET) {
		// no exception if the listen was canceled
		if(WSAGetLastError() != WSAEINTR) {
			throw "Accept";
		}
		return FALSE;
	}
	return TRUE;
}

void CBlockingSocket::Close()
{
	ASSERT(m_hSocket != NULL);
	if(::closesocket(m_hSocket) == SOCKET_ERROR) {
		// should be OK to close if closed already
		throw "Close";
	}
	m_hSocket = NULL;
}

void CBlockingSocket::Connect(LPCSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	// should timeout by itself
	if(::connect(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw "Connect";
	}
}

int CBlockingSocket::Write(const char* pch, const int nSize, const int nSecs)
{
	int nBytesSent = 0;
	int nBytesThisTime;
	const char* pch1 = pch;
	do {
		nBytesThisTime = Send(pch1, nSize - nBytesSent, nSecs);
		nBytesSent += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesSent < nSize);
	return nBytesSent;
}

int CBlockingSocket::Send(const char* pch, const int nSize, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	// returned value will be less than nSize if client cancels the reading
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(::select(0, NULL, &fd, NULL, &tv) == 0) {
		throw "Send timeout";
	}
	int nBytesSent;
	if((nBytesSent = ::send(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw "Send";
	}
	return nBytesSent;
}

int CBlockingSocket::Receive(char* pch, const int nSize, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(::select(0, &fd, NULL, NULL, &tv) == 0) {
		throw "Receive timeout";
	}

	int nBytesReceived;
	if((nBytesReceived = ::recv(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw "Receive";
	}
	return nBytesReceived;
}

int CBlockingSocket::ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(::select(0, &fd, NULL, NULL, &tv) == 0) {
		throw "Receive timeout";
	}

	// input buffer should be big enough for the entire datagram
	int nFromSize = sizeof(SOCKADDR);
	int nBytesReceived = ::recvfrom(m_hSocket, pch, nSize, 0, psa, &nFromSize);
	if(nBytesReceived == SOCKET_ERROR) {
		throw "ReceiveDatagram";
	}
	return nBytesReceived;
}

int CBlockingSocket::SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(::select(0, NULL, &fd, NULL, &tv) == 0) {
		throw "Send timeout";
	}

	int nBytesSent = ::sendto(m_hSocket, pch, nSize, 0, psa, sizeof(SOCKADDR));
	if(nBytesSent == SOCKET_ERROR) {
		throw "SendDatagram";
	}
	return nBytesSent;
}

void CBlockingSocket::GetPeerAddr(LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	// gets the address of the socket at the other end
	int nLengthAddr = sizeof(SOCKADDR);
	if(::getpeername(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw "GetPeerName";
	}
}

void CBlockingSocket::GetSockAddr(LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	// gets the address of the socket at this end
	int nLengthAddr = sizeof(SOCKADDR);
	if(::getsockname(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw "GetSockName";
	}
}

//static
CSockAddr CBlockingSocket::GetHostByName(const char* pchName, const USHORT ushPort /* = 0 */)
{
	hostent* pHostEnt = gethostbyname(pchName);
	if(pHostEnt == NULL) {
		throw "GetHostByName";
	}
	ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
	SOCKADDR_IN sockTemp;
	sockTemp.sin_family = AF_INET;
	sockTemp.sin_port = ::htons(ushPort);
	sockTemp.sin_addr.s_addr = *pulAddr; // address is already in network byte order
	return sockTemp;
}

//static
const char* CBlockingSocket::GetHostByAddr(LPCSOCKADDR psa)
{
	hostent* pHostEnt = gethostbyaddr((char*) &((LPSOCKADDR_IN) psa)
				->sin_addr.s_addr, 4, PF_INET);
	if(pHostEnt == NULL) {
		throw "GetHostByAddr";
	}
	return pHostEnt->h_name; // caller shouldn't delete this memory
}