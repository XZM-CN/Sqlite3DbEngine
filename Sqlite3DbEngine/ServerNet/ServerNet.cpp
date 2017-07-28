#include "ServerNet.h"


SOCKET sockConn;
HANDLE bufferMutex;     // 令其能互斥成功正常通信的信号量句柄  
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

		printf("%s:%d上线",inet_ntoa(tcpAddr.sin_addr),tcpAddr.sin_port);
		sprintf(t,"%s:%d",inet_ntoa(tcpAddr.sin_addr),tcpAddr.sin_port);
		string t1=t;
		if (SOCKET_ERROR != sockConn)
		{  
			clientSocketGroup.push_back(sockConn);  
			m_ipSocket[sockConn] = t1;
		} 
		HANDLE receiveThread = CreateThread(NULL, 0, RecMsgThread, (LPVOID)sockConn, 0, NULL);
		WaitForSingleObject(bufferMutex, INFINITE);     // P（资源未被占用）   
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
					printf("%s下线\n",m_ipSocket[ClientSocket].c_str());
					m_ipSocket.erase(i);
					break;
				}  
			}  
			closesocket(ClientSocket);  
			ReleaseSemaphore(bufferMutex, 1, NULL);
			break;
		}
		printf("%s Says: %s\n", m_ipSocket[ClientSocket].c_str(), recvBuf);     // 接收信息
		ReleaseSemaphore(bufferMutex, 1, NULL);
	}
	return 0;
}



int CServerNet::Init( const char* address,int port )
{
	int rlt = 0;

	//用于记录错误信息，并输出
	int iErrorMsg;

	//初始化WinSock
	WSAData wsaData;
	iErrorMsg = WSAStartup(MAKEWORD(1,1),&wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		//初始化WinSock失败
		printf("wsastartup failed with error : %d\n",iErrorMsg);
		//WSAGetLastError()
		rlt = 1;
		return rlt;
	}

	//创建服务端Socket
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
	{
		//创建Socket异常
		printf("socket failed with error : %d\n",WSAGetLastError());
		rlt = 2;
		return rlt;
	}

	//声明信息
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//绑定
	iErrorMsg = bind(m_sock,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//绑定失败
		printf("bind failed with error : %d\n",iErrorMsg);
		rlt = 3;
		return rlt;
	}



	return rlt;
}

void CServerNet::Run()
{
	//公开连接
	listen(m_sock,20);


	int len = sizeof(sockaddr);
	bufferMutex = CreateSemaphore(NULL, 1, 1, NULL);
	//HANDLE sendThread = CreateThread(NULL,0,)
	HANDLE acceptThread = CreateThread(NULL, 0, WaitAcceptThread, (LPVOID)m_sock, 0, NULL);
	//do 
	//{
	//  //接收信息
	//  newSocket = accept(m_sock,(sockaddr*)&tcpAddr,&len);

	//  
	//  if (newSocket == INVALID_SOCKET)
	//  {
	//      //非可用socket

	//  }
	//  else
	//  {
	//      //新socket连接
	//      printf("new socket connect : %d\n",newSocket);


	//      //消息处理
	//      do
	//      {
	//          printf("process\n");
	//          //接收数据
	//          memset(buf,0,sizeof(buf));
	//          rval = recv(newSocket,buf,1024,0);

	//          
	//          if (rval == SOCKET_ERROR)
	//          {
	//              //这应该是个异常，当客户端没有调用closeSocket就直接退出游戏的时候，将会进入这里
	//              printf("recv socket error\n");
	//              break;
	//          }

	//          
	//          
	//          if (rval == 0)
	//              //recv返回0表示正常退出
	//              printf("ending connection");
	//          else
	//              //显示接收到的数据
	//              printf("recv %s\n",buf);


	//      }while(rval != 0);

	//      //关闭对应Accept的socket
	//      closesocket(newSocket);
	//  }
	//  

	//  
	//} while (1);

	//关闭自身的Socket
	WaitForSingleObject(acceptThread, INFINITE);  // 等待线程结束 
	//WaitForSingleObject(sendThread, INFINITE);  // 等待线程结束  
	//CloseHandle(sendThread);  
	CloseHandle(bufferMutex);  
	WSACleanup();   // 终止对套接字库的使用 
	closesocket(m_sock);
}





CServerNet::CServerNet(void)
{
}


CServerNet::~CServerNet(void)
{
}