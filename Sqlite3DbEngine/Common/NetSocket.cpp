
#include "stdafx.h"
#include "NetSocket.h"
#include "BaseFuncLib.h"

#pragma comment(lib,"ws2_32.lib")


unsigned int CNetSocket::m_nRefCount = 0;


CLIENT::CLIENT()
{
	m_Sock = INVALID_SOCKET;

	m_dwRecvLen = 0;
	m_dwMaxRecvLen = MAX_RECV_LENGTH;
	m_dwSendLen = 0;
	m_dwMaxSendLen = MAX_SEND_LENGTH;
	m_pRecvBuf = new CHAR[MAX_RECV_LENGTH];
	m_pSendBuf = new CHAR[MAX_SEND_LENGTH];
	m_dwIdleTick = GetTickCount();

	m_hRecvThread = NULL;
	m_bRecvThreadFlag = TRUE;

	//初始化临界区
	InitializeCriticalSection(&m_RecvCriticalSection);
	InitializeCriticalSection(&m_SendCriticalSection);


}

CLIENT::~CLIENT()
{
	if (m_Sock != INVALID_SOCKET)
	{
		closesocket(m_Sock);
		m_Sock = INVALID_SOCKET;
	}

	if (m_pRecvBuf != NULL)
	{
		delete[] m_pRecvBuf;
	}

	if (m_pSendBuf != NULL)
	{
		delete[] m_pSendBuf;
	}

	DeleteCriticalSection(&m_RecvCriticalSection);
	DeleteCriticalSection(&m_SendCriticalSection);

	//退出接收线程
	if (m_hRecvThread != NULL)
	{
		CloseHandle(m_hRecvThread);
		m_bRecvThreadFlag = FALSE;
	}
}

bool CLIENT::IsAlive()
{
	DWORD dwTickCount = GetTickCount();
	if (dwTickCount < m_dwIdleTick)
	{
		if (0xFFFFFFFF - m_dwIdleTick + dwTickCount > 5000)
		{
			return false;
		}
	}
	else
	{
		if (dwTickCount - m_dwIdleTick > 5000)
		{
			return false;
		}
	}

	return true;
}


DWORD CLIENT::GetRecvBufferSize()
{

	return m_dwRecvLen;
}
BOOL CLIENT::GetRecvBuffer(char* strRecvData ,int& RecvDataSize)
{
	if (strRecvData == NULL)
	{
		return FALSE;
	}

	int nLen = 0;
	if (RecvDataSize > m_dwRecvLen)
	{
		nLen = m_dwRecvLen;
	}
	else
	{
		nLen = RecvDataSize;
	}

	strcpy_s(strRecvData, nLen,m_pRecvBuf);

	return TRUE;
}

BOOL CLIENT::CreateRecvThread()
{

	//创建客户端接收线程
	unsigned int threadId = 0;
	m_hRecvThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,RecvThread,this,0,&threadId));
	if (m_hRecvThread == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	return TRUE;
}


unsigned int CLIENT::RecvThread(LPVOID lpParam)
{
	CLIENT* pthis = (CLIENT*)lpParam;

	while(pthis->m_bRecvThreadFlag)
	{
		//开始接收
		if (pthis->m_Sock != INVALID_SOCKET)
		{
			char cBuffer[MAX_RECV_LENGTH] = {0};
			int nLength = MAX_RECV_LENGTH;

			int nLen = recv(pthis->m_Sock,cBuffer,nLength,0);


			if(nLen > 0)
			{
				//更新时间
				pthis->m_dwIdleTick = GetTickCount();

				//临界区
				EnterCriticalSection(&pthis->m_RecvCriticalSection);


				if (nLen + pthis->m_dwRecvLen < pthis->m_dwMaxRecvLen)
				{
					strcpy_s(pthis->m_pRecvBuf + pthis->m_dwRecvLen, nLen, cBuffer);

					pthis->m_dwRecvLen += nLen;
				}
				else
				{
					//超出现有空间，重新分配
					DWORD dwReAllocLength = pthis->m_dwMaxRecvLen * 2;
					PCHAR pwTempBuffer = new CHAR[dwReAllocLength];
					if (pwTempBuffer != NULL)
					{
						if (pthis->m_pRecvBuf != NULL)
						{
							//拷贝原来数据到新内存中
							strcpy_s(pwTempBuffer, pthis->m_dwRecvLen, pthis->m_pRecvBuf);

							delete[] pthis->m_pRecvBuf;
							pthis->m_pRecvBuf = pwTempBuffer;
							pthis->m_dwMaxRecvLen = pthis->m_dwMaxRecvLen * 2;
						}
					}

					if (pthis->m_pRecvBuf != NULL && dwReAllocLength < pthis->m_dwMaxRecvLen)
					{

						strcpy_s(pthis->m_pRecvBuf + pthis->m_dwRecvLen, nLen, cBuffer);
						pthis->m_dwRecvLen += nLen;
					}
				}

				//
				LeaveCriticalSection(&pthis->m_RecvCriticalSection);
			}


		}

	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//初始化socket库版本
CNetSocket::CNetSocket(BOOL bServerOrClient,ENUMPROTOCOL ProtoType,ENUMBLOCKING BlockType, char majorVer, char minorVer)
{
	m_nRefCount++;
	m_bServerOrClient = bServerOrClient;
	m_ProtoType = ProtoType;
	m_BlockType = BlockType;


	//根据引用计数，只加载一次
	if (m_nRefCount <= 1)
	{
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if(::WSAStartup(sockVersion, &wsaData) != 0)
		{
			exit(0);
		}
	}

	m_dwTickCount = GetTickCount();

	memset(m_ServerIp,0,32);
	m_nServerPort = 0;

	if (!m_bServerOrClient)
	{

		m_dwRecvLen = 0;
		m_dwMaxRecvLen = MAX_RECV_LENGTH;
		m_dwSendLen = 0;
		m_dwMaxSendLen = MAX_SEND_LENGTH;
		m_pRecvBuf = new CHAR[MAX_RECV_LENGTH];
		m_pSendBuf = new CHAR[MAX_SEND_LENGTH];
	}
	else
	{
		m_dwRecvLen = 0;
		m_dwMaxRecvLen = 0;
		m_dwSendLen = 0;
		m_dwMaxSendLen = 0;
		m_pRecvBuf = NULL;
		m_pSendBuf = NULL;
	}

	m_dwClientCount = 0;
	memset(m_pClients, 0,sizeof(CLIENT*	)*MAX_CLIENT_NUMBER);
	m_hAcceptThread = NULL;
	m_bAcceptThreadFlag = TRUE;

	m_hClientAliveThread = NULL;
	m_bClientAliveThreadFlag = TRUE;

	for (int i = 0;i < WSA_MAXIMUM_WAIT_EVENTS;i++)
	{
		m_hServerEvents[i] = NULL;
	}
	m_dwServerEventCount = 0;

	InitializeCriticalSection(&m_CriticalSection);


}

//释放库
CNetSocket::~CNetSocket()
{
	DWORD dwExitCode = 0;

	this->m_nRefCount--;

	//如果是服务端，退出线程
	if (m_hClientAliveThread != NULL)
	{
		//CloseHandle(m_hClientAliveThread);
		m_bClientAliveThreadFlag = FALSE;

		do
		{
			Sleep(50);
			GetExitCodeThread(m_hClientAliveThread, &dwExitCode);
		} while (dwExitCode == STILL_ACTIVE);
	}

	if (m_hAcceptThread != NULL)
	{
		//CloseHandle(m_hAcceptThread);
		m_bAcceptThreadFlag = FALSE;

		do
		{
			Sleep(50);
			GetExitCodeThread(m_hAcceptThread, &dwExitCode);
		} while (dwExitCode == STILL_ACTIVE);
	}

	//如果是服务端，则清除客户端数据
	if (m_bServerOrClient && m_dwClientCount > 0)
	{
		for (DWORD i=0;i<m_dwClientCount;i++)
		{
			if (m_pClients[i] != NULL)
			{
				delete m_pClients[i];
				m_pClients[i] = NULL;
			}
		}

		m_dwClientCount = 0;
	}

	if (!m_bServerOrClient)
	{
		//为客户端时清除
		if (m_pRecvBuf != NULL)
		{
			delete[] m_pRecvBuf;
			m_pRecvBuf = NULL;
		}

		if (m_pSendBuf != NULL)
		{
			delete[] m_pSendBuf;
			m_pSendBuf = NULL;
		}
	}

	//处理事件句柄
	if (m_dwServerEventCount > 0)
	{
		for (DWORD i = 0;i < m_dwServerEventCount;i++)
		{
			CloseHandle(m_hServerEvents[i]);
			m_hServerEvents[i] = NULL;
		}

	}


	DeleteCriticalSection(&m_CriticalSection);


	//当引用计数为0时，释放库
	if (m_nRefCount == 0)
	{
		::WSACleanup();
	}
}


//根据bServerOrClient为FALSE创建客户端或为TRUE创建服务端
SOCKET CNetSocket::Start( char* pchIP, short nPort)
{
	SOCKET sock = INVALID_SOCKET;
	int nProtocol = 0,nType = 0;

	if (nPort == 0)
	{
		return sock;
	}

	switch(m_BlockType)
	{
	case enumBlock:
		{
			//创建Socket
			//判断创建的协议类型
			nProtocol = GetProtocolType(m_ProtoType, nType);

			sock = socket(AF_INET, nType, nProtocol);
			if (sock == INVALID_SOCKET)
			{
				return sock;
			}

			sockaddr_in addr_sin;
			addr_sin.sin_family = AF_INET;
			addr_sin.sin_port = htons(nPort);

			//判断是创建服务端还是客户端
			if (m_bServerOrClient)
			{
				//创建服务端
				addr_sin.sin_addr.S_un.S_addr = INADDR_ANY;

				if(::bind(sock,(sockaddr*)&addr_sin,sizeof(addr_sin)) == SOCKET_ERROR)
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return sock;
				}

				if (enumStream == m_ProtoType)
				{
					//只有TCP协议才做监听
					if(::listen(sock,MAXLISTEN) == SOCKET_ERROR)
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}

					//创建线程处理accept
					unsigned int threadId = 0;
					m_hAcceptThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,AcceptThread,this,0,&threadId));
					if (m_hAcceptThread == NULL)
					{
						m_bAcceptThreadFlag = FALSE;

						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}

					//创建检测客户端超时线程
					unsigned int threadclientId = 0;
					m_hClientAliveThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,IsClientAliveThread,this,0,&threadclientId));
					if (m_hClientAliveThread == NULL)
					{
						m_bAcceptThreadFlag = FALSE;

						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}
				}

			}
			else
			{
				//创建客户端
				addr_sin.sin_addr.S_un.S_addr = inet_addr(pchIP);

				if (enumStream == m_ProtoType)
				{
					//只有TCP协议才做连接
					if(::connect(sock,(sockaddr*)&addr_sin,sizeof(addr_sin)) == SOCKET_ERROR)
					{

						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}
				}
			}
		}
		break;
	case enumSelect:
		{

		}
		break;
	case enumAsyncSelect:
		{

		}
		break;
	case enumEventSelect:
		{
			//创建Socket
			//判断创建的协议类型
			nProtocol = GetProtocolType(m_ProtoType, nType);

			sock = socket(AF_INET, nType, nProtocol);
			if (sock == INVALID_SOCKET)
			{
				return sock;
			}

			HANDLE hEvent = WSACreateEvent();
			if (hEvent == WSA_INVALID_EVENT)
			{
				closesocket(sock);
				sock = INVALID_SOCKET;
				return sock;
			}

			sockaddr_in addr_sin;
			addr_sin.sin_family = AF_INET;
			addr_sin.sin_port = htons(nPort);


			//判断是创建服务端还是客户端
			if (m_bServerOrClient)
			{
				//服务端
				addr_sin.sin_addr.S_un.S_addr = INADDR_ANY;

				if(::bind(sock,(sockaddr*)&addr_sin,sizeof(addr_sin)) == SOCKET_ERROR)
				{
					closesocket(sock);
					sock = INVALID_SOCKET;
					return sock;
				}

				if (enumStream == m_ProtoType)
				{

					//SetBlocking(sock,false);
					SetDelay(sock,false);

					//只有TCP协议才做监听
					if(::listen(sock,MAXLISTEN) == SOCKET_ERROR)
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}

					if(WSAEventSelect(sock, hEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
					{
						WSACloseEvent(hEvent);
						hEvent = WSA_INVALID_EVENT;
						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}

				}
				else
				{
					if(WSAEventSelect(sock, hEvent, FD_READ | FD_CLOSE) == SOCKET_ERROR)
					{
						WSACloseEvent(hEvent);
						hEvent = WSA_INVALID_EVENT;
						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}
				}

			}
			else
			{
				//客户端
				addr_sin.sin_addr.S_un.S_addr = inet_addr(pchIP);

				if(WSAEventSelect(sock, hEvent, FD_READ | FD_WRITE| FD_CONNECT | FD_CLOSE) == SOCKET_ERROR)
				{
					WSACloseEvent(hEvent);
					hEvent = WSA_INVALID_EVENT;
					closesocket(sock);
					sock = INVALID_SOCKET;
					return sock;
				}

				if (enumStream == m_ProtoType)
				{
					//SetBlocking(sock,false);
					SetDelay(sock,false);

					//只有TCP协议才做连接
					if(::connect(sock,(sockaddr*)&addr_sin,sizeof(addr_sin)) == SOCKET_ERROR)
					{
						WSACloseEvent(hEvent);
						hEvent = WSA_INVALID_EVENT;
						closesocket(sock);
						sock = INVALID_SOCKET;
						return sock;
					}
				}

			}

			m_hServerEvents[m_dwServerEventCount] = hEvent;
			m_dwServerEventCount++;
			m_pClients[m_dwClientCount] = new CLIENT;
			m_pClients[m_dwClientCount++]->m_Sock = sock;

			//创建等待事件线程
			unsigned int threadId = 0;
			m_hAcceptThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL,0,AcceptClientThread,this,0,&threadId));
			if (m_hAcceptThread == NULL)
			{
				m_bAcceptThreadFlag = FALSE;

				WSACloseEvent(hEvent);
				hEvent = WSA_INVALID_EVENT;
				closesocket(sock);
				sock = INVALID_SOCKET;
				return sock;
			}


		}
		break;
	case enumOverLapped:
		{

		}
		break;
	case enumIOCP:
		{

		}
		break;
	default:
		break;
	}

	//保存相应SOCKET
	m_ServerSock = sock;

	return sock;
}


//关闭相应的Socket
BOOL CNetSocket::Stop(SOCKET sock)
{
	BOOL bRet = FALSE;
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
	}

	return bRet;
}


BOOL CNetSocket::SendData(SOCKET sock,char* strSendData ,int Length)
{

	BOOL bRet = TRUE;

	if (strSendData == NULL)
	{
		return FALSE;
	}
	CString strSend(strSendData);
	//转换UNICODE到UTF-8编码
	char* pSendData = NULL;
	int nLen = CBaseFuncLib::US2ToUtf8(strSend, &pSendData);

	//发送数据
	if(::send(sock, pSendData, nLen,0) == SOCKET_ERROR)
	{
		bRet = FALSE;
	}
	//释放转换格式时分配的内存
	delete[] pSendData;

	return bRet;
}


BOOL CNetSocket::RecvData(SOCKET sock,char* strRecvData,int& nRecvLen)
{
	BOOL bRet = TRUE;

	if (strRecvData == NULL)
	{
		return FALSE;
	}

	//接收数据
	int nLen = recv(sock, strRecvData, nRecvLen,0);
	if (nLen <= 0)
	{
		return FALSE;
	}

	////转换UTF-8到UNICODE编码
	//PTCHAR	pBuffer = NULL;

	//nLen = CBaseFuncLib::Utf8ToUS2(recvBuff, &pBuffer);
	//if(nLen > 0 && pBuffer != NULL && strRecvData !=NULL)
	//{
	//	//在处理完接收到的数据后，需要释放动态分配的内存
	//	
	//	_tcscpy_s(strRecvData, nLen, pBuffer);

	//	//清除内存
	//	delete pBuffer;

	//}

	nRecvLen = nLen;
	return bRet;
}


BOOL CNetSocket::SendDataTo(SOCKET sock,char* strSendData,int Length, char* strIP, int nPort)
{
	BOOL bRet = TRUE;

	if (strSendData == NULL)
	{
		return FALSE;
	}

	CString strSend(strSendData);
	//转换UNICODE到UTF-8编码
	char* pSendData = NULL;
	int nLen = CBaseFuncLib::US2ToUtf8(strSend, &pSendData);
	if (nLen <= 0 || pSendData == NULL)
	{
		return FALSE;
	}

	//指定连接的IP
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(nPort);
	saddr.sin_addr.S_un.S_addr = inet_addr(strIP);

	//发送数据
	if(::sendto(sock, pSendData, nLen,0,(sockaddr*)&saddr,sizeof(saddr)) == SOCKET_ERROR)
	{
		bRet = FALSE;
	}
	//释放转换格式时分配的内存
	delete[] pSendData;

	return bRet;
}

BOOL CNetSocket::RecvDataFrom(SOCKET sock,char* strRecvData,int& nRecvLen,struct sockaddr_in* saddrin)
{
	BOOL bRet = TRUE;


	if (saddrin == NULL)
	{
		return FALSE;
	}

	//指定连接的IP
	sockaddr_in saddr;

	int nRLength = sizeof(saddr);
	//接收数据
	int nLen = recvfrom(sock, strRecvData, nRecvLen,0,(sockaddr*)&saddr, &nRLength);
	if (nLen <= 0)
	{
		return FALSE;
	}

	//接收到的IP地址、端口
	memcpy(saddrin, &saddr,sizeof(saddr));


	////转换UTF-8到UNICODE编码
	//PTCHAR	pBuffer = NULL;

	//nLen = CBaseFuncLib::Utf8ToUS2(recvBuff, &pBuffer);
	//if(nLen > 0 && pBuffer != NULL && strRecvData !=NULL)
	//{
	//	//在处理完接收到的数据后，需要释放动态分配的内存
	//	_tcscpy_s(strRecvData, nLen, pBuffer);

	//	//清除内存
	//	delete pBuffer;

	//	
	//}

	nRecvLen = nLen;
	return bRet;
}


int CNetSocket::GetProtocolType(ENUMPROTOCOL ProtoType, int& nType)
{
	int nProtocol = 0;

	switch(ProtoType)
	{
	case enumRAW:
		nProtocol = IPPROTO_RAW;
		nType = SOCK_RAW;
		break;
	case enumStream:
		nProtocol = IPPROTO_TCP;
		nType = SOCK_STREAM;
		break;
	case enumDgram:
		nProtocol = IPPROTO_UDP;
		nType = SOCK_DGRAM;
		break;
	default:
		break;
	}

	return nProtocol;
}


//Accept接受客户端连接线程
unsigned int CNetSocket::AcceptThread(LPVOID lpParam)
{
	CNetSocket* pthis = (CNetSocket*)lpParam;

	while(pthis->m_bAcceptThreadFlag)
	{
		sockaddr_in clientaddr;
		int addrLen = sizeof(clientaddr);
		SOCKET ClientSock = ::accept(pthis->m_ServerSock, (sockaddr*)&clientaddr, &addrLen);
		if (ClientSock != INVALID_SOCKET)
		{
			BOOL bFlag = TRUE;

			for (DWORD i=0;i < pthis->m_dwClientCount;i++)
			{

				if (pthis->m_pClients[i]->m_Sock == ClientSock)
				{
					bFlag = FALSE;
					break;
				}
			}

			if (bFlag)
			{
				//
				EnterCriticalSection(&pthis->m_CriticalSection);

				pthis->m_pClients[pthis->m_dwClientCount] = new CLIENT;
				if (pthis->m_pClients[pthis->m_dwClientCount] != NULL)
				{
					pthis->m_pClients[pthis->m_dwClientCount]->m_Sock = ClientSock;
					pthis->m_pClients[pthis->m_dwClientCount]->m_addr = clientaddr;


					if (pthis->m_dwClientCount >= MAX_CLIENT_NUMBER)
					{
						pthis->m_dwClientCount = MAX_CLIENT_NUMBER;
					}
					else
					{
						pthis->m_dwClientCount++;

						//创建接收线程
						pthis->m_pClients[pthis->m_dwClientCount]->CreateRecvThread();

					}

				}
				else
				{
					//没有成功，则关闭此SOCKET，让客户端重新连接

					closesocket(ClientSock);
					ClientSock = INVALID_SOCKET;
				}

				//
				LeaveCriticalSection(&pthis->m_CriticalSection);
			}
		}


	}

	return 0;
}


//检查客户端是否超时,1s查一次
unsigned int CNetSocket::IsClientAliveThread(LPVOID lpParam)
{
	CNetSocket* pthis = (CNetSocket*)lpParam;

	while(pthis->m_bClientAliveThreadFlag)
	{

		//服务端循环接收数据
		if (pthis->m_bServerOrClient && pthis->m_dwClientCount > 0)
		{

			//开始接收
			for (DWORD i=0;i<pthis->m_dwClientCount;)
			{
				if (pthis->m_pClients[i] != NULL)
				{

					//判断客户端是否超时
					if(!pthis->m_pClients[i]->IsAlive())
					{
						//
						EnterCriticalSection(&pthis->m_CriticalSection);

						//当前客户端超时，关闭此客户端


						delete pthis->m_pClients[i];
						pthis->m_pClients[i] = NULL;


						//重新排列数组

						pthis->m_pClients[i] = pthis->m_pClients[pthis->m_dwClientCount-1];
						pthis->m_pClients[pthis->m_dwClientCount-1] = NULL;
						pthis->m_dwClientCount--;

						continue;

						//
						LeaveCriticalSection(&pthis->m_CriticalSection);

					}
				}

				i++;
			}

		}

		Sleep(1000);
	}

	return 0;
}

//发送数据到客户端
BOOL CNetSocket::ServerToClientSendData(SOCKET sock,char* strSendData ,int Length)
{
	if (!m_bServerOrClient || m_dwClientCount <= 0)
	{
		return FALSE;
	}

	if (sock == INVALID_SOCKET)
	{
		//发送到所有客户端

		for (DWORD i=0;i<m_dwClientCount;i++)
		{
			if (m_pClients[i] != NULL)
			{
				if(SendData(m_pClients[i]->m_Sock, strSendData, Length))
				{
					OutputDebugString(_T("发送数据成功！"));
				}
			}
		}

	}
	else
	{
		//发送到指定客户端
		for (DWORD i=0;i<m_dwClientCount;i++)
		{
			if (m_pClients[i] != NULL)
			{
				if (m_pClients[i]->m_Sock == sock)
				{
					if(SendData(m_pClients[i]->m_Sock, strSendData, Length))
					{
						OutputDebugString(_T("发送数据成功！"));
					}
				}

			}
		}

	}


	return TRUE;
}

//获取接收到的指定客户端的数据，用于解析,
//第一次调用时，strRecvData为NULL，获取缓冲区大小，存储在RecvDataSize中
//第二次调用前，分配足够大小的内存，获取数据，返回实际的大小
DWORD CNetSocket::GetClientRecvData(SOCKET sock,char* strRecvData ,int& RecvDataSize)
{
	DWORD nLen = 0;

	if (!m_bServerOrClient || m_dwClientCount <= 0)
	{
		return 0;
	}

	if (sock == INVALID_SOCKET)
	{
		return 0;
	}

	if (strRecvData == NULL || RecvDataSize == 0)
	{
		//获取接收缓冲区大小
		for (DWORD i=0;i<m_dwClientCount;i++)
		{
			if (m_pClients[i] != NULL)
			{
				if (m_pClients[i]->m_Sock == sock)
				{
					RecvDataSize = m_pClients[i]->GetRecvBufferSize();

					nLen = RecvDataSize;
				}

			}
		}

	}
	else
	{
		//获取数据
		for (DWORD i=0;i<m_dwClientCount;i++)
		{
			if (m_pClients[i] != NULL)
			{
				if (m_pClients[i]->m_Sock == sock)
				{
					if(m_pClients[i]->GetRecvBuffer(strRecvData ,RecvDataSize))
					{

						nLen = RecvDataSize;
					}
				}

			}
		}

	}

	return nLen;
}


unsigned int CNetSocket::AcceptClientThread(LPVOID lpParam)
{
	CNetSocket* pthis = (CNetSocket*)lpParam;

	while(pthis->m_bAcceptThreadFlag)
	{

		if (pthis->m_bServerOrClient)
		{
			//服务端

			DWORD dwIndex = WSAWaitForMultipleEvents(pthis->m_dwServerEventCount, pthis->m_hServerEvents, FALSE, 5000, FALSE);

			if (dwIndex == WSA_WAIT_FAILED)
			{
				pthis->m_ErrorCode = WSAGetLastError();
				break;
			}

			if (dwIndex == WAIT_IO_COMPLETION)
			{
				continue;
			}
			else if (dwIndex == WSA_WAIT_TIMEOUT)
			{
				for (DWORD i = 1; i < pthis->m_dwClientCount; i++)
				{
					if (pthis->m_pClients[i] && pthis->m_pClients[i]->IsAlive() == false)
					{

						delete pthis->m_pClients[i];
						pthis->m_pClients[i] = NULL;

						WSACloseEvent(pthis->m_hServerEvents[i]);
						pthis->m_hServerEvents[i] = WSA_INVALID_EVENT;

						pthis->m_dwClientCount--;
						pthis->m_dwServerEventCount--;

						for (int k = i; k < WSA_MAXIMUM_WAIT_EVENTS - 1; k++)
						{
							pthis->m_pClients[k] = pthis->m_pClients[k + 1];
							pthis->m_hServerEvents[k] = pthis->m_hServerEvents[k + 1];
						}

					}
				}
			}
			else
			{
				dwIndex -= WSA_WAIT_EVENT_0;

				if (dwIndex == 0) break;

				if (pthis->m_pClients[dwIndex] == NULL) continue;

				WSANETWORKEVENTS wsaEvent;
				if (WSAEnumNetworkEvents(pthis->m_pClients[dwIndex]->m_Sock, pthis->m_hServerEvents[dwIndex], &wsaEvent) == SOCKET_ERROR)
					break;

				if (wsaEvent.lNetworkEvents & FD_WRITE)
				{
					if (wsaEvent.iErrorCode[FD_WRITE_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_WRITE_BIT];

					}
					else
					{

						int nSendBytes = -1;

						if (enumStream == pthis->m_ProtoType)
						{
							//TCP

							//发送数据
							if (pthis->m_pClients[dwIndex]->m_dwSendLen > 0)
							{

								nSendBytes = pthis->SendData(pthis->m_pClients[dwIndex]->m_Sock, 
									pthis->m_pClients[dwIndex]->m_pSendBuf,pthis->m_pClients[dwIndex]->m_dwSendLen);
								if (nSendBytes == -1)
								{
									pthis->m_ErrorCode = WSAGetLastError();

								}
							}

						}
						else
						{
							//UDP
							if (pthis->m_pClients[dwIndex]->m_dwSendLen > 0)
							{

								nSendBytes = pthis->SendDataTo(pthis->m_pClients[dwIndex]->m_Sock,
									pthis->m_pClients[dwIndex]->m_pSendBuf,pthis->m_pClients[dwIndex]->m_dwSendLen,
									inet_ntoa(pthis->m_pClients[dwIndex]->m_addr.sin_addr), pthis->m_pClients[dwIndex]->m_addr.sin_port);
								if (nSendBytes == -1)
								{
									pthis->m_ErrorCode = WSAGetLastError();

								}
							}
						}


						//处理发送缓冲区
						if ((nSendBytes > 0) && (nSendBytes <= pthis->m_pClients[dwIndex]->m_dwSendLen))
						{
							pthis->m_pClients[dwIndex]->m_dwSendLen -= nSendBytes;
							memmove_s(pthis->m_pClients[dwIndex]->m_pSendBuf, pthis->m_pClients[dwIndex]->m_dwMaxSendLen,
								pthis->m_pClients[dwIndex]->m_pSendBuf+nSendBytes, pthis->m_pClients[dwIndex]->m_dwSendLen);
						}


					}
				}

				if (wsaEvent.lNetworkEvents & FD_READ)
				{
					if (wsaEvent.iErrorCode[FD_READ_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_READ_BIT];

					}
					else
					{
						pthis->m_pClients[dwIndex]->m_dwIdleTick = GetTickCount();

						char cBuffer[MAX_RECV_LENGTH] = {0};
						int nRecvBytes = -1 , nTempLen = MAX_RECV_LENGTH;

						if (enumStream == pthis->m_ProtoType)
						{
							//TCP
							nRecvBytes = pthis->RecvData(pthis->m_pClients[dwIndex]->m_Sock, cBuffer, nTempLen);
							if (nRecvBytes == -1)
							{
								pthis->m_ErrorCode = WSAGetLastError();

							}
						}
						else
						{
							//UDP
							int nTempPort = pthis->m_pClients[dwIndex]->m_addr.sin_port;
							nRecvBytes = pthis->RecvDataFrom(pthis->m_pClients[dwIndex]->m_Sock, cBuffer, nTempLen,
								&pthis->m_pClients[dwIndex]->m_addr);
							if (nRecvBytes == -1)
							{
								pthis->m_ErrorCode = WSAGetLastError();

							}
						}


						//收到的数据保存到接收缓冲区
						if (nRecvBytes + pthis->m_pClients[dwIndex]->m_dwRecvLen < pthis->m_pClients[dwIndex]->m_dwMaxRecvLen)
						{
							strcpy_s(pthis->m_pClients[dwIndex]->m_pRecvBuf + pthis->m_pClients[dwIndex]->m_dwRecvLen,
								nRecvBytes, cBuffer);

							pthis->m_pClients[dwIndex]->m_dwRecvLen += nRecvBytes;
						}
						else
						{
							//超出现有空间，重新分配
							DWORD dwReAllocLength = pthis->m_dwMaxRecvLen * 2;
							PCHAR pwTempBuffer = new CHAR[dwReAllocLength];
							if (pwTempBuffer != NULL)
							{
								if (pthis->m_pRecvBuf != NULL)
								{
									//拷贝原来数据到新内存中
									strcpy_s(pwTempBuffer, pthis->m_pClients[dwIndex]->m_dwRecvLen, 
										pthis->m_pClients[dwIndex]->m_pRecvBuf);

									delete[] pthis->m_pClients[dwIndex]->m_pRecvBuf;
									pthis->m_pClients[dwIndex]->m_pRecvBuf = pwTempBuffer;
									pthis->m_pClients[dwIndex]->m_dwMaxRecvLen = pthis->m_pClients[dwIndex]->m_dwMaxRecvLen * 2;
								}
							}

							if (pthis->m_pClients[dwIndex]->m_pRecvBuf != NULL &&
								dwReAllocLength < pthis->m_pClients[dwIndex]->m_dwMaxRecvLen)
							{

								strcpy_s(pthis->m_pClients[dwIndex]->m_pRecvBuf + pthis->m_pClients[dwIndex]->m_dwRecvLen,
									nRecvBytes, cBuffer);
								pthis->m_pClients[dwIndex]->m_dwRecvLen += nRecvBytes;
							}
						}



					}
				}

				if (wsaEvent.lNetworkEvents & FD_ACCEPT)
				{
					if (wsaEvent.iErrorCode[FD_ACCEPT_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_ACCEPT_BIT];

					}
					else
					{
						if (pthis->m_dwClientCount == WSA_MAXIMUM_WAIT_EVENTS) continue;

						int nIndex = pthis->m_dwClientCount;

						SOCKET ClientSock = INVALID_SOCKET;
						sockaddr_in saddr;
						int naddrSize = sizeof(saddr);


						try
						{
							ClientSock = ::accept(pthis->m_ServerSock, (sockaddr*)&saddr, &naddrSize);
							if (ClientSock != INVALID_SOCKET)
							{


								pthis->SetBlocking(ClientSock, false);

								pthis->m_pClients[nIndex] = new CLIENT;

								pthis->m_pClients[nIndex]->m_Sock = ClientSock;
								pthis->m_pClients[nIndex]->m_addr = saddr;
								pthis->m_pClients[nIndex]->m_dwIdleTick = GetTickCount();

								pthis->m_hServerEvents[nIndex] = WSACreateEvent();
								if (pthis->m_hServerEvents[nIndex] == WSA_INVALID_EVENT)
								{
									delete pthis->m_pClients[nIndex];
									pthis->m_pClients[nIndex] = NULL;
									continue;
								}

								if (WSAEventSelect(pthis->m_pClients[nIndex]->m_Sock, pthis->m_hServerEvents[nIndex], FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
								{
									delete pthis->m_pClients[nIndex];
									pthis->m_pClients[nIndex] = NULL;

									WSACloseEvent(pthis->m_hServerEvents[nIndex]);
									pthis->m_hServerEvents[nIndex] = WSA_INVALID_EVENT;

									continue;
								}
							}
						}
						catch (...)
						{
							continue;
						}

						pthis->m_dwClientCount++;


					}
				}

				if (wsaEvent.lNetworkEvents & FD_CLOSE)
				{
					if (wsaEvent.iErrorCode[FD_CLOSE_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_CLOSE_BIT];

					}
					else
					{

						delete pthis->m_pClients[dwIndex];
						pthis->m_pClients[dwIndex] = NULL;

						WSACloseEvent(pthis->m_hServerEvents[dwIndex]);
						pthis->m_hServerEvents[dwIndex] = WSA_INVALID_EVENT;

						pthis->m_dwClientCount--;
						pthis->m_dwServerEventCount--;

						for (int k = dwIndex; k < WSA_MAXIMUM_WAIT_EVENTS - 1; k++)
						{
							pthis->m_pClients[k] = pthis->m_pClients[k + 1];
							pthis->m_hServerEvents[k] = pthis->m_hServerEvents[k + 1];
						}

					}
				}
			}


		}
		else
		{
			//客户端

			DWORD dwIndex = WSAWaitForMultipleEvents(pthis->m_dwServerEventCount, pthis->m_hServerEvents, FALSE, 5000, FALSE);

			if (dwIndex == WSA_WAIT_FAILED)
			{
				break;
			}

			if (dwIndex == WSA_WAIT_TIMEOUT)
			{
				DWORD dwTickCount = GetTickCount();
				if (dwTickCount < pthis->m_dwTickCount)
				{
					if (0xFFFFFFFF - pthis->m_dwTickCount + dwTickCount > 5000)
					{
						//重新连接服务端

						break;
					}
				}
				else
				{
					if (dwTickCount - pthis->m_dwTickCount > 5000)
					{
						//重新连接服务端

						break;
					}
				}
			}
			else
			{
				dwIndex -= WSA_WAIT_EVENT_0;

				if (dwIndex == 0) break;

				WSANETWORKEVENTS wsaEvent;
				if (WSAEnumNetworkEvents(pthis->m_pClients[dwIndex]->m_Sock, pthis->m_hServerEvents[dwIndex], &wsaEvent) == SOCKET_ERROR)
					break;

				if (wsaEvent.lNetworkEvents & FD_CONNECT)
				{
					if (wsaEvent.iErrorCode[FD_CONNECT_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_CONNECT_BIT];
					}
					else
					{
						pthis->m_dwTickCount = GetTickCount();

						//重新连接服务端

					}
				}

				if (wsaEvent.lNetworkEvents & FD_CLOSE)
				{
					if (wsaEvent.iErrorCode[FD_CLOSE_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_CLOSE_BIT];

					}
					else
					{
						//重新连接服务端

						break;
					}
				}

				if (wsaEvent.lNetworkEvents & FD_WRITE)
				{
					if (wsaEvent.iErrorCode[FD_WRITE_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_WRITE_BIT];

					}
					else
					{
						int nRecvBytes = -1;

						if (enumStream == pthis->m_ProtoType)
						{
							//TCP
							//发送心跳包
							char chHeart = 0;

							nRecvBytes = pthis->SendData(pthis->m_pClients[dwIndex]->m_Sock, &chHeart, sizeof(chHeart));
							if (nRecvBytes == -1)
							{
								pthis->m_ErrorCode = WSAGetLastError();

							}

							//发送数据
							if (pthis->m_dwSendLen > 0)
							{

								nRecvBytes = pthis->SendData(pthis->m_pClients[dwIndex]->m_Sock, pthis->m_pSendBuf, pthis->m_dwSendLen);
								if (nRecvBytes == -1)
								{
									pthis->m_ErrorCode = WSAGetLastError();

								}
							}

						}
						else
						{
							//UDP
							if (pthis->m_dwSendLen > 0)
							{

								nRecvBytes = pthis->SendDataTo(pthis->m_pClients[dwIndex]->m_Sock, pthis->m_pSendBuf,
									pthis->m_dwSendLen, pthis->m_ServerIp, pthis->m_nServerPort);
								if (nRecvBytes == -1)
								{
									pthis->m_ErrorCode = WSAGetLastError();

								}
							}
						}


						//处理发送缓冲区
						if ((nRecvBytes > 0) && (nRecvBytes <= pthis->m_dwSendLen))
						{
							pthis->m_dwSendLen -= nRecvBytes;
							memmove_s(pthis->m_pSendBuf, pthis->m_dwMaxSendLen,
								pthis->m_pSendBuf+nRecvBytes, pthis->m_dwSendLen);
						}


					}
				}

				if (wsaEvent.lNetworkEvents & FD_READ)
				{
					if (wsaEvent.iErrorCode[FD_READ_BIT])
					{
						pthis->m_ErrorCode = wsaEvent.iErrorCode[FD_READ_BIT];
					}
					else
					{
						char cBuffer[MAX_RECV_LENGTH] = {0};
						int nRecvBytes = -1 , nTempLen = MAX_RECV_LENGTH;

						pthis->m_dwTickCount = GetTickCount();

						if (enumStream == pthis->m_ProtoType)
						{
							//TCP
							nRecvBytes = pthis->RecvData(pthis->m_pClients[dwIndex]->m_Sock, cBuffer, nTempLen);
							if (nRecvBytes == -1)
							{
								pthis->m_ErrorCode = WSAGetLastError();

							}
						}
						else
						{
							//UDP
							sockaddr_in saddr;
							nRecvBytes = pthis->RecvDataFrom(pthis->m_pClients[dwIndex]->m_Sock, cBuffer, nTempLen, &saddr);
							if (nRecvBytes == -1)
							{
								pthis->m_ErrorCode = WSAGetLastError();

							}

							strcpy_s(pthis->m_ServerIp,32,inet_ntoa(saddr.sin_addr));
							pthis->m_nServerPort = saddr.sin_port;
						}


						//收到的数据保存到接收缓冲区
						if (nRecvBytes + pthis->m_dwRecvLen < pthis->m_dwMaxRecvLen)
						{
							strcpy_s(pthis->m_pRecvBuf + pthis->m_dwRecvLen, nRecvBytes, cBuffer);

							pthis->m_dwRecvLen += nRecvBytes;
						}
						else
						{
							//超出现有空间，重新分配
							DWORD dwReAllocLength = pthis->m_dwMaxRecvLen * 2;
							PCHAR pwTempBuffer = new CHAR[dwReAllocLength];
							if (pwTempBuffer != NULL)
							{
								if (pthis->m_pRecvBuf != NULL)
								{
									//拷贝原来数据到新内存中
									strcpy_s(pwTempBuffer, pthis->m_dwRecvLen, pthis->m_pRecvBuf);

									delete[] pthis->m_pRecvBuf;
									pthis->m_pRecvBuf = pwTempBuffer;
									pthis->m_dwMaxRecvLen = pthis->m_dwMaxRecvLen * 2;
								}
							}

							if (pthis->m_pRecvBuf != NULL && dwReAllocLength < pthis->m_dwMaxRecvLen)
							{

								strcpy_s(pthis->m_pRecvBuf + pthis->m_dwRecvLen, nRecvBytes, cBuffer);
								pthis->m_dwRecvLen += nRecvBytes;
							}
						}




					}
				}
			}

		}

	}

	return 0;
}





bool CNetSocket::SetBlocking(SOCKET ClientSock, bool bBlocking)
{

	ULONG param = bBlocking ? 1 : 0;

	if (ioctlsocket(ClientSock, FIONBIO, &param) == SOCKET_ERROR)
	{
		return false;
	}


	return true;
}

bool CNetSocket::SetDelay(SOCKET ClientSock, bool bDelay)
{
	int param = bDelay ? 0 : 1;

	if (setsockopt(ClientSock, IPPROTO_TCP, TCP_NODELAY, (char*)&param, sizeof(param)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

