#include "StdAfx.h"
#include "XzmSocket.h"
#include "BaseFuncLib.h"

CXzmSocket::CXzmSocket(void)
{
	WSADATA Data;

	//��ʼ��windows Socket Dll
	status = WSAStartup(MAKEWORD(1,1),&Data);
	if (0!=status)
	{
		OutputDebugString(_T("��ʼ��ʧ��\n"));
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
	SOCKET serversocket;  //��ʵ����һ��unsig int����,������¼�Ѿ�����������δ�������׽��ֺ�  
	SOCKET clientsocket;
	int addrlen = sizeof(SOCKADDR_IN);   //16
	int status;
	int numrcv;
	char buffer[MAXBUFLEN];

	CXzmSocket *pXzmSocket = (CXzmSocket*)pParam;
	//��0  
	memset(&(serverSockAddr),0,sizeof(serverSockAddr));
	//��ʼ��  
	serverSockAddr.sin_port = htons(8088);
	serverSockAddr.sin_family = AF_INET;    //ָ����ַЭ����  
	// serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //��ʼ��ip��ַ0,0,0,0 �κ�ip����������
	serverSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


	//�����׽���  
	serversocket = socket(AF_INET,SOCK_STREAM/*�׽�������*/,IPPROTO_TCP/*�׽���ʹ�õ��ض�Э��*/);
	if (INVALID_SOCKET == serversocket)  
	{
		OutputDebugString(_T("����socketʧ��\n"));
	}

	//��socket���ض���ַ(�ṹ���ַ)  
	status = bind(serversocket,(LPSOCKADDR)&(serverSockAddr),sizeof(serverSockAddr));
	if (SOCKET_ERROR==status)  
	{
		OutputDebugString(_T("�󶨵�ַʧ��\n"));
	}



	//��������������������,����˵���Ǽ���  
	status = ::listen(serversocket/*�׽��ֺ�*/,2/*����������ӵ��׽��ָ���*/);  
	if (SOCKET_ERROR == status)  
	{
		OutputDebugString(_T("����ʧ��\n"));
	}  


	//�������������Ǿͽ�����������  
	clientsocket = accept(serversocket,(LPSOCKADDR)&clientsocket,&addrlen);

	int i = 0;
	while (1)
	{
		numrcv = recv(clientsocket/*�Ѿ��������׽���*/,buffer/*�����������ݻ�������ָ��*/,MAXBUFLEN/*���ܻ�������С*/,0/*������Ʒ�ʽ*/);
		if ((0==numrcv)||(numrcv==SOCKET_ERROR))
		{
			OutputDebugString(_T("�����ܵ�����\n"));

			status = closesocket(clientsocket);
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("�Ͽ�����ʧ��\n"));

			status = WSACleanup();
			if (SOCKET_ERROR==status)
				OutputDebugString(_T("��������ʧ��\n"));

			return(1);  


		}

		CString str;
		str.Format(_T("���ܵ��ͻ��˷���������Ϣ�����ǣ�%S\n"),buffer);
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

	OutputDebugString(_T("�ȴ��������ܿͻ��˷�������Ϣ..........\n"));


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


	//��ȡ·��
	CString strTmpPath = CBaseFuncLib::GetAppConfigPath();
	/*strTmpPath += _T("\\");*/
	strTmpPath += TCPCONFIGFILENAME;

	CString strstr;
	strstr.Format(_T("��ini�ļ�:%s\n"), strTmpPath);
	OutputDebugString(strstr);

	//��ȡ�����ļ�������
	DWORD nReadLen = CBaseFuncLib::ReadAllData( strTmpPath, &ppData);
	if (ppData != NULL)
	{
		//��ȡ����Ч����
		OutputDebugString(_T("��ʼ����ini�ļ�\n"));
		if(ParseSyslogConfigFile(ppData))
		{
			bRet = TRUE;
		}

		//�ͷŶ��ڴ�
		delete []ppData;
		ppData = NULL;

	}

	CString strstr1;
	strstr1.Format(_T("��ini�ļ���С��:%d\n"), nReadLen);
	OutputDebugString(strstr1);

	return bRet;
}

BOOL CXzmSocket::ParseSyslogConfigFile(BYTE* pchSyslog)
{
	BOOL bRet = FALSE;

	if (pchSyslog  == NULL)
	{
		OutputDebugString(_T("����ʧ��\n"));
		return bRet;
	}

	CString strTemp(pchSyslog);



	//��ʼ����
	int nLen = strTemp.Find(TCPSECTION);
	if (nLen != -1)
	{
		OutputDebugString(_T("���ҵ���Ч�ֶ�\n"));
		int nLength = strTemp.GetLength() - nLen - sizeof(TCPSECTION)/sizeof(TCHAR);	//�õ��ַ���

		CString strStart = strTemp.Right(nLength);
		if (!strStart.IsEmpty())
		{
			OutputDebugString(_T("��ȡ����Ч�ֶ�\n"));
			//
			nLen = strStart.Find('[');
			if (nLen != -1)
			{
				//��ȡ��ǰ�ε��ַ���
				strStart = strStart.Left(nLen);

				nLen = strStart.Find('=');
				while (nLen != -1)
				{
					//��ѯ��������IP��PORT
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
						//���һ��û�л��з�
						strIP = strStart;
						strIP.TrimLeft();
						strIP.TrimRight();
					}

					CString strstr;
					strstr.Format(_T("0 ��ʼ��ȡini�ļ���������:%s=%s\n"),strIPName,strIP);
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

						//����˿ڵĴ���
						m_ServerPort = CBaseFuncLib::StrToNum(strIP);
					}

					//������һ��
					nLen = strStart.Find('=');
				}
			}
			else
			{
				nLen = strStart.Find('=');
				while (nLen != -1)
				{
					//��ѯ��������IP��PORT
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
						//���һ��û�л��з�
						strIP = strStart;
						strIP.TrimLeft();
						strIP.TrimRight();
					}
					CString strstr;
					strstr.Format(_T("1 ��ʼ��ȡini�ļ���������:%s\n"),strIP);
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

						//����˿ڵĴ���
						m_ServerPort = CBaseFuncLib::StrToNum(strIP);
					}


					//������һ��
					nLen = strStart.Find('=');
				}
			}
		}
		else
		{
			OutputDebugString(_T("û�н�ȡ����Ч�ֶ�\n"));
		}

	}
	else
	{
		OutputDebugString(_T("û�в��ҵ���Ч�ֶ�\n"));
	}



	return bRet;
}
