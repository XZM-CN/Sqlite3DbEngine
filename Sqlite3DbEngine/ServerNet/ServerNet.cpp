#include "ServerNet.h"


SOCKET sockConn;
HANDLE bufferMutex;     // �����ܻ���ɹ�����ͨ�ŵ��ź������  
vector<SOCKET> clientSocketGroup; 
DWORD WINAPI WaitAcceptThread(LPVOID IpParameter);
DWORD WINAPI RecMsgThread(LPVOID IpParameter);
DWORD WINAPI SendMsgThread(LPVOID IpParameter);
map<SOCKET,string> m_ipSocket;


DWORD WINAPI WaitAcceptThread(LPVOID IpParameter)
{
	SOCKET m_socket = (SOCKET)IpParameter;
	while(true)
	{
		sockaddr_in tcpAddr;
		int len = sizeof(sockaddr_in);
		sockConn = accept(m_socket, (sockaddr*)&tcpAddr, &len);
		char t[20];

		printf("%s:%d����",inet_ntoa(tcpAddr.sin_addr),tcpAddr.sin_port);
		sprintf(t,"%s:%d",inet_ntoa(tcpAddr.sin_addr),tcpAddr.sin_port);
		string t1=t;
		if (SOCKET_ERROR != sockConn)
		{  
			clientSocketGroup.push_back(sockConn);  
			m_ipSocket[sockConn] = t1;
		} 
		HANDLE receiveThread = CreateThread(NULL, 0, RecMsgThread, (LPVOID)sockConn, 0, NULL);
		WaitForSingleObject(bufferMutex, INFINITE);     // P����Դδ��ռ�ã�   
		if(NULL == receiveThread) {   
			printf("\nCreatThread AnswerThread() failed.\n");   
		}   
		else{   
			printf("\nCreate Receive Client Thread OK.\n");   
		}   
		ReleaseSemaphore(bufferMutex, 1, NULL);
	}
}

DWORD WINAPI RecMsgThread(LPVOID IpParameter)
{
	SOCKET ClientSocket=(SOCKET)(LPVOID)IpParameter;
	int rval;
	while(1)
	{
		char recvBuf[1024];  
		rval = recv(ClientSocket, recvBuf, 1024, 0);  
		WaitForSingleObject(bufferMutex, INFINITE);
		if (rval == SOCKET_ERROR)
		{
			printf("ONE Client Exit\n");
			vector<SOCKET>::iterator result = find(clientSocketGroup.begin(), clientSocketGroup.end(), ClientSocket);  
			clientSocketGroup.erase(result);  
			for (map<SOCKET, string>::iterator i=m_ipSocket.begin(); i!=m_ipSocket.end(); i++)  
			{  
				if (i->first == ClientSocket)  
				{  
					printf("%s����\n",m_ipSocket[ClientSocket].c_str());
					m_ipSocket.erase(i);
					break;
				}  
			}  
			closesocket(ClientSocket);  
			ReleaseSemaphore(bufferMutex, 1, NULL);
			break;
		}
		printf("%s Says: %s\n", m_ipSocket[ClientSocket].c_str(), recvBuf);     // ������Ϣ
		ReleaseSemaphore(bufferMutex, 1, NULL);
	}
	return 0;
}



int CServerNet::Init( const char* address,int port )
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�������
	int iErrorMsg;

	//��ʼ��WinSock
	WSAData wsaData;
	iErrorMsg = WSAStartup(MAKEWORD(1,1),&wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		//��ʼ��WinSockʧ��
		printf("wsastartup failed with error : %d\n",iErrorMsg);
		//WSAGetLastError()
		rlt = 1;
		return rlt;
	}

	//���������Socket
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
	{
		//����Socket�쳣
		printf("socket failed with error : %d\n",WSAGetLastError());
		rlt = 2;
		return rlt;
	}

	//������Ϣ
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//��
	iErrorMsg = bind(m_sock,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//��ʧ��
		printf("bind failed with error : %d\n",iErrorMsg);
		rlt = 3;
		return rlt;
	}



	return rlt;
}

void CServerNet::Run()
{
	//��������
	listen(m_sock,20);


	int len = sizeof(sockaddr);
	bufferMutex = CreateSemaphore(NULL, 1, 1, NULL);
	//HANDLE sendThread = CreateThread(NULL,0,)
	HANDLE acceptThread = CreateThread(NULL, 0, WaitAcceptThread, (LPVOID)m_sock, 0, NULL);
	//do 
	//{
	//  //������Ϣ
	//  newSocket = accept(m_sock,(sockaddr*)&tcpAddr,&len);

	//  
	//  if (newSocket == INVALID_SOCKET)
	//  {
	//      //�ǿ���socket

	//  }
	//  else
	//  {
	//      //��socket����
	//      printf("new socket connect : %d\n",newSocket);


	//      //��Ϣ����
	//      do
	//      {
	//          printf("process\n");
	//          //��������
	//          memset(buf,0,sizeof(buf));
	//          rval = recv(newSocket,buf,1024,0);

	//          
	//          if (rval == SOCKET_ERROR)
	//          {
	//              //��Ӧ���Ǹ��쳣�����ͻ���û�е���closeSocket��ֱ���˳���Ϸ��ʱ�򣬽����������
	//              printf("recv socket error\n");
	//              break;
	//          }

	//          
	//          
	//          if (rval == 0)
	//              //recv����0��ʾ�����˳�
	//              printf("ending connection");
	//          else
	//              //��ʾ���յ�������
	//              printf("recv %s\n",buf);


	//      }while(rval != 0);

	//      //�رն�ӦAccept��socket
	//      closesocket(newSocket);
	//  }
	//  

	//  
	//} while (1);

	//�ر������Socket
	WaitForSingleObject(acceptThread, INFINITE);  // �ȴ��߳̽��� 
	//WaitForSingleObject(sendThread, INFINITE);  // �ȴ��߳̽���  
	//CloseHandle(sendThread);  
	CloseHandle(bufferMutex);  
	WSACleanup();   // ��ֹ���׽��ֿ��ʹ�� 
	closesocket(m_sock);
}





CServerNet::CServerNet(void)
{
}


CServerNet::~CServerNet(void)
{
}