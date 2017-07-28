/*
* Copyright (c) 2017-?,��غ��˿Ƽ�
* All rights reserved.
*
* @brief ����Ļ����ඨ��
*
* @version 1.0
*
* @Date 2017-02-21
*
* @author ����
* 
*/

#pragma once

#include <stdio.h>
#include <WinSock2.h>


#define MAXLISTEN 5
#define MAX_RECV_LENGTH	4096
#define MAX_SEND_LENGTH	4096
#define MAX_CLIENT_NUMBER	1024


//Э������
typedef enum _tagProtocol
{
	enumRAW,				//ԭʼ�׽���
	enumStream,				//���׽���
	enumDgram				//�����׽���
}ENUMPROTOCOL;


//ͨ��ģ��
typedef enum _tagBlocking
{
	enumBlock,					//����ģʽ
	enumSelect,					//SELECTģʽ
	enumAsyncSelect,			//�첽SELECTģʽ
	enumEventSelect,			//�¼�SELECTģʽ
	enumOverLapped,				//�ص�ģʽ
	enumIOCP					//��ɶ˿�ģʽ
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
	/*��ʼ��socket��汾,bServerOrClientΪTRUE��������,FALSE����ͻ��ˡ�ProtoTypeΪenumRAWʱ����ԭʼ�׽��֣�
	ΪenumStreamʱ�������׽��֣�ΪenumDgramʱ�������׽��֡�BlockTypeΪenumBlockʱ��������ģʽ��ΪenumSelectʱ����SELECTģʽ��
	ΪenumAsyncSelectʱ�����첽SELECTģʽ��ΪenumEventSelect�����¼�SELECTģʽ��ΪenumOverLapped�����ص�ģʽ��
	ΪenumIOCP������ɶ˿�ģʽ*/
	CNetSocket(BOOL bServerOrClient,ENUMPROTOCOL ProtoType,ENUMBLOCKING BlockType, char major = 2, char minor = 2);
	~CNetSocket();		//�ͷſ�
	

	SOCKET Start( char* pchIP = "127.0.0.1", short nPort = 0);		//���ݹ������Ĳ���ȷ�������ͻ��˻����ˡ�UDP��TCP�������������
	BOOL Stop(SOCKET sock);						//�ر���Ӧ��Socket


	BOOL SendData(SOCKET sock,char* strSendData ,int Length);
	BOOL RecvData(SOCKET sock,char* strRecvData,int& nRecvLen);

	BOOL SendDataTo(SOCKET sock,char* strSendData,int Length, char* strIP, int nPort);
	BOOL RecvDataFrom(SOCKET sock,char* strRecvData,int& nRecvLen, struct sockaddr_in* saddrin);

	BOOL ServerToClientSendData(SOCKET sock,char* strSendData ,int Length);
	DWORD GetClientRecvData(SOCKET sock,char* strRecvData ,int& RecvDataSize);
	

protected:
	bool SetBlocking(SOCKET ClientSock, bool bBlocking);		//����TCP�����������״̬
	bool SetDelay(SOCKET ClientSock, bool bDelay);				//����TCP��ʱ
	int GetProtocolType(ENUMPROTOCOL ProtoType, int& nType);	//��ȡSocket����Э������
	static unsigned int WINAPI AcceptThread(LPVOID lpParam);	//����˵ȴ��ͻ��������߳�
	static unsigned int WINAPI IsClientAliveThread(LPVOID lpParam);	//�жϿͻ����Ƿ�ʱ�߳�
	static unsigned int WINAPI AcceptClientThread(LPVOID lpParam);	//EventSelect����˴����߳�



private:
	SOCKET m_ServerSock;
	/*Socket m_ClientSock;*/

	DWORD				m_dwTickCount;
	DWORD				m_ErrorCode;
	static unsigned int m_nRefCount;	//���ü��������ڳ�ʼ�����ؿ�

	BOOL				m_bServerOrClient;	//�����Ƿ���ˣ�TRUE����ͻ��ˣ�FALSE��
	ENUMPROTOCOL		m_ProtoType;		//����TCP��UDP��ICMP��
	ENUMBLOCKING		m_BlockType;		//���������������

	char				m_ServerIp[32];		//�����IP�����ڴ����ͻ���ʱʹ��
	int					m_nServerPort;		//����˶˿ڣ����ڴ����ͻ���ʱʹ��

	PCHAR				m_pRecvBuf;			//���ջ����������ڴ����ͻ���ʱʹ��
	DWORD				m_dwRecvLen;		//���ջ������ﵱǰ��ʹ���������ڴ����ͻ���ʱʹ��
	DWORD				m_dwMaxRecvLen;		//���ջ�������С�����ڴ����ͻ���ʱʹ��
	PCHAR				m_pSendBuf;			//���ͻ����������ڴ����ͻ���ʱʹ��
	DWORD				m_dwSendLen;		//���ͻ�������ǰ��ʹ���������ڴ����ͻ���ʱʹ��
	DWORD				m_dwMaxSendLen;		//���ͻ�������С�����ڴ����ͻ���ʱʹ��
	
	HANDLE				m_hAcceptThread;			//�����accept�߳̾��
	BOOL				m_bAcceptThreadFlag;		//�߳̽�����־

	HANDLE				m_hClientAliveThread;			//������жϿͻ����Ƿ�ʱ�߳̾��
	BOOL				m_bClientAliveThreadFlag;		//�߳̽�����־
	
	CRITICAL_SECTION	m_CriticalSection;

public:
	CLIENT*		m_pClients[MAX_CLIENT_NUMBER];
	DWORD		m_dwClientCount;

	HANDLE		m_hServerEvents[WSA_MAXIMUM_WAIT_EVENTS];		//���ڷ����EventSelectģ���¼�
	DWORD		m_dwServerEventCount;
};




