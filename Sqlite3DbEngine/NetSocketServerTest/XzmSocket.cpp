#include "StdAfx.h"
#include "XzmSocket.h"
#include "BaseFuncLib.h"

CXzmSocket::CXzmSocket(void)
{
	WSADATA Data;

	//初始化windows Socket Dll
	status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		OutputDebugString(_T("初始化失败\n"));
	} 


	memset(m_ServerMajorIP,0,64);
	memset(m_ServerMinorIP,0,64);
	m_ServerPort = 514;

	OutputDebugString(_T("\n"));
}

CXzmSocket::~CXzmSocket(void)
{
}

UINT CXzmSocket::DoServerTest(LPVOID pParam)
{
	SOCKADDR_IN serverSockAddr;
	SOCKADDR_IN clientSockAddr;
	SOCKET serversocket;  //其实就是一个unsig int类型,用来记录已经建立或者尚未建立的套接字号  
	SOCKET clientsocket;
	int addrlen = sizeof(SOCKADDR_IN);   //16
	int status;
	int numrcv;
	char buffer[MAXBUFLEN];

	CXzmSocket *pXzmSocket = (CXzmSocket*)pParam;
	//清0  
	memset(&(serverSockAddr),0,sizeof(serverSockAddr));
	//初始化  
	serverSockAddr.sin_port = htons(8088);
	serverSockAddr.sin_family = AF_INET;    //指定地址协议族  
	// serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //初始化ip地址0,0,0,0 任何ip都可以连接
	serverSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	//创建套接字  
	serversocket = socket(AF_INET,SOCK_STREAM/*套接字类型*/,IPPROTO_TCP/*套接字使用的特定协议*/);
	if (INVALID_SOCKET == serversocket)  
	{
		OutputDebugString(_T("创建socket失败\n"));
	}

	//绑定socket到特定地址(结构体地址)  
	status = bind(serversocket,(LPSOCKADDR)&(serverSockAddr),sizeof(serverSockAddr));
	if (SOCKET_ERROR==status)  
	{
		OutputDebugString(_T("绑定地址失败\n"));
	}



	//允许外来申请链接请求,或者说就是监听  
	status = ::listen(serversocket/*套接字号*/,2/*最大容许链接的套接字个数*/);  
	if (SOCKET_ERROR == status)  
	{
		OutputDebugString(_T("监听失败\n"));
	}  


	//当有请求申请是就接受链接请求  
	clientsocket = accept(serversocket,(LPSOCKADDR)&clientsocket,&addrlen);

	int i = 0;
	while (1)
	{
		numrcv = recv(clientsocket/*已经建立的套接字*/,buffer/*接受输入数据缓冲器的指针*/,MAXBUFLEN/*接受缓冲区大小*/,0/*传输控制方式*/);
		if ((0==numrcv)||(numrcv==SOCKET_ERROR))
		{
			OutputDebugString(_T("链接受到限制\n"));

			status = closesocket(clientsocket);
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("断开链接失败\n"));

			status = WSACleanup();
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("清理链接失败\n"));

			return(1);  


		}

		CString str;
		str.Format(_T("接受到客户端发过来的消息内容是：%S\n"),buffer);
		OutputDebugString(str);



		int numsnt;
		char *toSendtxt=new char[256];
		sprintf(toSendtxt,"Server Send Data%d",i++);

		numsnt=send(clientsocket, toSendtxt, strlen(toSendtxt) + 1, 0);
		if (numsnt != (int)strlen(toSendtxt) + 1)
		{
			OutputDebugString(_T("Connection terminated\n"));


			status=closesocket(clientsocket);
			if (status == SOCKET_ERROR)
				OutputDebugString(_T("ERROR: closesocket unsuccessful\n"));

			status=WSACleanup();
			if (status == SOCKET_ERROR)
				OutputDebugString(_T("ERROR: WSACleanup unsuccessful\n"));

			return(1);
		}
	}

	OutputDebugString(_T("等待继续接受客户端发来的消息..........\n"));


	return 0;
}

UINT CXzmSocket::DoClientTest(LPVOID pParam)
{
	return 0;
}

BOOL CXzmSocket::GetConfInfo()
{

	BOOL bRet = FALSE;
	BYTE* ppData = NULL;


	//获取路径
	CString strTmpPath = CBaseFuncLib::GetAppConfigPath();
	/*strTmpPath += _T("\\");*/
	strTmpPath += TCPCONFIGFILENAME;

	CString strstr;
	strstr.Format(_T("打开ini文件:%s\n"), strTmpPath);
	OutputDebugString(strstr);

	//读取配置文件的内容
	DWORD nReadLen = CBaseFuncLib::ReadAllData( strTmpPath, &ppData);
	if (ppData != NULL)
	{
		//读取到有效数据
		OutputDebugString(_T("开始解析ini文件\n"));
		if(ParseSyslogConfigFile(ppData))
		{
			bRet = TRUE;
		}

		//释放堆内存
		delete []ppData;
		ppData = NULL;

	}

	CString strstr1;
	strstr1.Format(_T("打开ini文件大小是:%d\n"), nReadLen);
	OutputDebugString(strstr1);

	return bRet;
}

BOOL CXzmSocket::ParseSyslogConfigFile(BYTE* pchSyslog)
{
	BOOL bRet = FALSE;

	if (pchSyslog  == NULL)
	{
		OutputDebugString(_T("解析失败\n"));
		return bRet;
	}

	CString strTemp(pchSyslog);



	//开始解析
	int nLen = strTemp.Find(TCPSECTION);
	if (nLen != -1)
	{
		OutputDebugString(_T("查找到有效字段\n"));
		int nLength = strTemp.GetLength() - nLen - sizeof(TCPSECTION)/sizeof(TCHAR);	//得到字符数

		CString strStart = strTemp.Right(nLength);
		if (!strStart.IsEmpty())
		{
			OutputDebugString(_T("截取到有效字段\n"));
			//
			nLen = strStart.Find('[');
			if (nLen != -1)
			{
				//截取当前段的字符串
				strStart = strStart.Left(nLen);

				nLen = strStart.Find('=');
				while (nLen != -1)
				{
					//查询到服务器IP或PORT
					CString strIPName = strStart.Left(nLen);
					strIPName.TrimLeft();
					strIPName.TrimRight();

					int nRightLen = strStart.GetLength() - nLen -1;
					strStart = strStart.Right(nRightLen);

					CString strIP;
					int nLastLen = strStart.Find('\n');
					if (nLastLen != -1)
					{
						strIP = strStart.Left(nLastLen);
						strIP.TrimLeft();
						strIP.TrimRight();

						nRightLen = strStart.GetLength() - nLastLen;
						strStart = strStart.Right(nRightLen);

					}
					else
					{
						//最后一行没有换行符
						strIP = strStart;
						strIP.TrimLeft();
						strIP.TrimRight();
					}

					CString strstr;
					strstr.Format(_T("0 开始读取ini文件的内容是:%s=%s\n"),strIPName,strIP);
					OutputDebugString(strstr);

					if (0 == strIPName.CompareNoCase(TCPSERVERMAJORIP))
					{

						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMajorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}

					}
					else if (0 == strIPName.CompareNoCase(TCPSERVERMINORIP))
					{
						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMinorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}

					}
					else if (0 == strIPName.CompareNoCase(TCPSERVERPORT))
					{

						//网络端口的处理
						m_ServerPort = CBaseFuncLib::StrToNum(strIP);
					}

					//查找下一个
					nLen = strStart.Find('=');
				}
			}
			else
			{
				nLen = strStart.Find('=');
				while (nLen != -1)
				{
					//查询到服务器IP或PORT
					CString strIPName = strStart.Left(nLen);
					strIPName.TrimLeft();
					strIPName.TrimRight();

					int nRightLen = strStart.GetLength() - nLen -1;
					strStart = strStart.Right(nRightLen);

					CString strIP;
					int nLastLen = strStart.Find('\n');
					if (nLastLen != -1)
					{
						strIP = strStart.Left(nLastLen);
						strIP.TrimLeft();
						strIP.TrimRight();

						nRightLen = strStart.GetLength() - nLastLen;
						strStart = strStart.Right(nRightLen);

					}
					else
					{
						//最后一行没有换行符
						strIP = strStart;
						strIP.TrimLeft();
						strIP.TrimRight();
					}
					CString strstr;
					strstr.Format(_T("1 开始读取ini文件的内容是:%s\n"),strIP);
					OutputDebugString(strstr);


					if (0 == strIPName.CompareNoCase(TCPSERVERMAJORIP))
					{
						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMajorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}
					}
					else if (0 == strIPName.CompareNoCase(TCPSERVERMINORIP))
					{

						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMinorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}

					}
					else if (0 == strIPName.CompareNoCase(TCPSERVERPORT))
					{

						//网络端口的处理
						m_ServerPort = CBaseFuncLib::StrToNum(strIP);
					}


					//查找下一个
					nLen = strStart.Find('=');
				}
			}
		}
		else
		{
			OutputDebugString(_T("没有截取到有效字段\n"));
		}

	}
	else
	{
		OutputDebugString(_T("没有查找到有效字段\n"));
	}



	return bRet;
}
