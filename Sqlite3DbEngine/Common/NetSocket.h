/*
* Copyright (c) 2017-?,天地和兴科技
* All rights reserved.
*
* @brief 网络的基础类定义
*
* @version 1.0
*
* @Date 2017-02-21
*
* @author 梁杰
* 
*/

#pragma once

#include <stdio.h>
#include <WinSock2.h>


#define MAXLISTEN 5
#define MAX_RECV_LENGTH	4096
#define MAX_SEND_LENGTH	4096
#define MAX_CLIENT_NUMBER	1024


//协议类型
typedef enum _tagProtocol
{
	enumRAW,				//原始套节字
	enumStream,				//流套节字
	enumDgram				//报文套节字
}ENUMPROTOCOL;


//通信模型
typedef enum _tagBlocking
{
	enumBlock,					//阻塞模式
	enumSelect,					//SELECT模式
	enumAsyncSelect,			//异步SELECT模式
	enumEventSelect,			//事件SELECT模式
	enumOverLapped,				//重叠模式
	enumIOCP					//完成端口模式
}ENUMBLOCKING;



class CLIENT
{
public:
	SOCKET		m_Sock;
	sockaddr_in m_addr;

	PCHAR		m_pRecvBuf;
	DWORD		m_dwRecvLen;
	DWORD		m_dwMaxRecvLen;
	PCHAR		m_pSendBuf;
	DWORD		m_dwSendLen;
	DWORD		m_dwMaxSendLen;
	DWORD		m_dwIdleTick;

	CRITICAL_SECTION	m_RecvCriticalSection;
	CRITICAL_SECTION	m_SendCriticalSection;

	HANDLE		m_hRecvThread;
	BOOL		m_bRecvThreadFlag;



	CLIENT();
	~CLIENT();
	bool IsAlive();
	BOOL CreateRecvThread();
	DWORD GetRecvBufferSize();
	BOOL GetRecvBuffer(char* strRecvData ,int& RecvDataSize);

	static unsigned int WINAPI RecvThread(LPVOID lpParam);

};



class CNetSocket
{

public:
	/*初始化socket库版本,bServerOrClient为TRUE代表服务端,FALSE代表客户端。ProtoType为enumRAW时代表原始套节字，
	为enumStream时代表流套节字，为enumDgram时代表报文套节字。BlockType为enumBlock时代表阻塞模式，为enumSelect时代表SELECT模式，
	为enumAsyncSelect时代表异步SELECT模式，为enumEventSelect代表事件SELECT模式，为enumOverLapped代表重叠模式，
	为enumIOCP代表完成端口模式*/
	CNetSocket(BOOL bServerOrClient,ENUMPROTOCOL ProtoType,ENUMBLOCKING BlockType, char major = 2, char minor = 2);
	~CNetSocket();		//释放库
	

	SOCKET Start( char* pchIP = "127.0.0.1", short nPort = 0);		//根据构造对象的参数确定创建客户端或服务端、UDP或TCP、阻塞或非阻塞
	BOOL Stop(SOCKET sock);						//关闭相应的Socket


	BOOL SendData(SOCKET sock,char* strSendData ,int Length);
	BOOL RecvData(SOCKET sock,char* strRecvData,int& nRecvLen);

	BOOL SendDataTo(SOCKET sock,char* strSendData,int Length, char* strIP, int nPort);
	BOOL RecvDataFrom(SOCKET sock,char* strRecvData,int& nRecvLen, struct sockaddr_in* saddrin);

	BOOL ServerToClientSendData(SOCKET sock,char* strSendData ,int Length);
	DWORD GetClientRecvData(SOCKET sock,char* strRecvData ,int& RecvDataSize);
	

protected:
	bool SetBlocking(SOCKET ClientSock, bool bBlocking);		//设置TCP阻塞与非阻塞状态
	bool SetDelay(SOCKET ClientSock, bool bDelay);				//设置TCP延时
	int GetProtocolType(ENUMPROTOCOL ProtoType, int& nType);	//获取Socket创建协议类型
	static unsigned int WINAPI AcceptThread(LPVOID lpParam);	//服务端等待客户端连接线程
	static unsigned int WINAPI IsClientAliveThread(LPVOID lpParam);	//判断客户端是否超时线程
	static unsigned int WINAPI AcceptClientThread(LPVOID lpParam);	//EventSelect服务端处理线程



private:
	SOCKET m_ServerSock;
	/*Socket m_ClientSock;*/

	DWORD				m_dwTickCount;
	DWORD				m_ErrorCode;
	static unsigned int m_nRefCount;	//引用计数，用于初始化加载库

	BOOL				m_bServerOrClient;	//设置是服务端（TRUE）或客户端（FALSE）
	ENUMPROTOCOL		m_ProtoType;		//设置TCP、UDP、ICMP等
	ENUMBLOCKING		m_BlockType;		//阻塞与非阻塞设置

	char				m_ServerIp[32];		//服务端IP，用于创建客户端时使用
	int					m_nServerPort;		//服务端端口，用于创建客户端时使用

	PCHAR				m_pRecvBuf;			//接收缓冲区，用于创建客户端时使用
	DWORD				m_dwRecvLen;		//接收缓冲区里当前的使用量，用于创建客户端时使用
	DWORD				m_dwMaxRecvLen;		//接收缓冲区大小，用于创建客户端时使用
	PCHAR				m_pSendBuf;			//发送缓冲区，用于创建客户端时使用
	DWORD				m_dwSendLen;		//发送缓冲区当前的使用量，用于创建客户端时使用
	DWORD				m_dwMaxSendLen;		//发送缓冲区大小，用于创建客户端时使用
	
	HANDLE				m_hAcceptThread;			//服务端accept线程句柄
	BOOL				m_bAcceptThreadFlag;		//线程结束标志

	HANDLE				m_hClientAliveThread;			//服务端判断客户端是否超时线程句柄
	BOOL				m_bClientAliveThreadFlag;		//线程结束标志
	
	CRITICAL_SECTION	m_CriticalSection;

public:
	CLIENT*		m_pClients[MAX_CLIENT_NUMBER];
	DWORD		m_dwClientCount;

	HANDLE		m_hServerEvents[WSA_MAXIMUM_WAIT_EVENTS];		//用于服务端EventSelect模型事件
	DWORD		m_dwServerEventCount;
};




