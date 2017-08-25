#include "StdAfx.h"
#include "StaticClass.h"
#include "SqliteHelper.h"
#include "BaseFuncLib.h"
#include "SysLog.h "

CRITICAL_SECTION csHeartBeat;
CRITICAL_SECTION csUpLoad;

#define SERVERIP   "192.168.0.67"

BOOL  CStaticClass::m_gbSendHeartBeat = FALSE;

CString  g_strClientID;
char *   g_cClientID = NULL;
CTime ct = CTime::GetCurrentTime();


CStaticClass::CStaticClass(void)
{
	m_spiJsonService = NULL;

	if(m_spiJsonService == NULL)
		GetInterfacePtr(m_spiJsonService);
}


CStaticClass::~CStaticClass(void)
{
}

char* CStaticClass::rand_str(char *str,const int len)
{
	srand((unsigned int)time(NULL));
	int i;
	for(i=0;i<len;++i)
	{
		switch((rand()%3))
		{
		case 1:
			str[i]='A'+rand()%26;
			break;
		case 2:
			str[i]='a'+rand()%26;
			break;
		default:
			str[i]='0'+rand()%10;
			break;
		}
	}
	str[++i]='\0';
	return str;
}

UINT CStaticClass::ApplyRegister(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// �����׽���
	SOCKET ServerSocket = ::socket(AF_INET, SOCK_STREAM, 0);

	if(ServerSocket == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// Ҳ�������������bind������һ�����ص�ַ;����ϵͳ�����Զ�����

	// ��дԶ�̵�ַ��Ϣ
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// ע��,����Ҫ��д����������(TCPServer����)���ڻ�����IP��ַ
	// �����ļ����û������,ֱ��ʹ��127.0.0.1����
	servAddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);

	int cRet = 0;
	cRet = ::connect(ServerSocket, (sockaddr*)&servAddr, sizeof(servAddr));
	if(cRet == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("δ��ʼ��socket��"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// ��������
	char JsonBuff[256];
	memset(JsonBuff,0,256);
	int sendRet = SOCKET_ERROR;
	int recvRet = SOCKET_ERROR;

	g_strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(g_strClientID/*strChild*/,&g_cClientID);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonParent = NULL;
				CComPtr <IJsonService> spiJsonChild = NULL;

				if(NULL == spiJsonParent)
					spiJsonParent = CDbHelper::GetJsonService();

				if(NULL != spiJsonParent)
				{
					/// д������
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonParent->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(g_strClientID.GetBuffer(0)));
					spiJsonParent->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonParent->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonParent->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonChild);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonChild->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonChild->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonParent->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonChild);
					spiJsonParent->get_ObjectString(&bstrVal);
					strSendData = bstrVal.m_str;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);

			sendRet = SOCKET_ERROR;
			sendRet = ::send(ServerSocket, pText, strlen(pText), 0);
			if(sendRet == SOCKET_ERROR)
			{
				printf("��������ʧ��%s%d", __FILE__, __LINE__);
				return 0;
			}
			sendRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// ��ʼ������
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)ServerSocket, 0, &dwThreadId);

			recvRet = SOCKET_ERROR;
			recvRet = ::recv(ServerSocket, JsonBuff, 256, 0);
			if(recvRet > 0) {
			}
			else{
				printf("��������ʧ��%s%d", __FILE__, __LINE__);
				return 0;
			}
		}
		else // �����ַ���
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[recvRet] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
					}
				case 2:
					{
						LONGLONG RegisterID = -1000;
						Get_CmdContext2Level(JsonBuff,CComBSTR(JSON_STRING_CONTENT),CComBSTR(JSON_STRING_REGISTAR),RegisterID);
						switch (RegisterID)
						{
						case 1:
							{
								char *pText = NULL;
								CString strContext ;
								CComPtr <IJsonService> spiJsonService2=SJD_Content3();
								SJD_register(strContext,g_strClientID,3,spiJsonService2);
								strContext = strContext + _T("\t");
								int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
								sendRet = SOCKET_ERROR;

								CStaticClass::m_gbSendHeartBeat = TRUE;
								Sleep(1000);
								sendRet = ::send(ServerSocket, pText, strlen(pText), 0);
								if(sendRet == SOCKET_ERROR)
								{
									CStaticClass::m_gbSendHeartBeat = FALSE;
									printf("��������ʧ��%s%d", __FILE__, __LINE__);
									return 0;
									break;
								}
								CStaticClass::m_gbSendHeartBeat = FALSE;

								// ��ʼ��SystemLog
								DWORD dwThreadId;
								HANDLE hThread;
								hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)ServerSocket, 0, &dwThreadId);
							}
							break;
						case 2:
							{
							}
							break;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,g_strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						sendRet = SOCKET_ERROR;
						CStaticClass::m_gbSendHeartBeat = TRUE;
						Sleep(1000);
						sendRet = ::send(ServerSocket, pText, strlen(pText), 0);
						if(sendRet == SOCKET_ERROR)
						{
							CStaticClass::m_gbSendHeartBeat = FALSE;
							printf("��������ʧ��%s%d", __FILE__, __LINE__);
							return 0;
							break;
						}
						CStaticClass::m_gbSendHeartBeat = FALSE;

					}
					break;
				default:
					{
						char pText[256] = "û��ƥ�������";
						sendRet = ::send(ServerSocket, pText, strlen(pText), 0);
						cout<<endl<<"û��ƥ�������"<<endl;
						if(sendRet == SOCKET_ERROR)
						{
							printf("��������ʧ��%s%d", __FILE__, __LINE__);
							return 0;
						}
					}
					break;
				}

				// ��������
				sendRet = SOCKET_ERROR;
				memset(JsonBuff,0,256);
				recvRet = -1;
				recvRet = ::recv(ServerSocket, JsonBuff, 256, 0);
				if(recvRet > 0)
				{
					continue;
				}
				else
					return 0;
			}
			else 
			{
				break;
			}
		}
	}

	// �ر��׽���
	::closesocket(ServerSocket);
	return 0;
}


static int count = 0;
UINT CStaticClass::HeartBeat(LPVOID pParam)
{
	SOCKET xxx = (SOCKET)pParam;

	int senfRet;
	while(1){
		EnterCriticalSection(&csHeartBeat);// �����ٽ����������߳����޷�����

		if (m_gbSendHeartBeat)
		{
			Sleep(1000);
			continue;
		}

		char *pText = NULL;
		CString strSend ;
		CComPtr <IJsonService> spiContent = CStaticClass::SJD_Content5();
		SJD_register(strSend,g_strClientID,5,spiContent);
		strSend = strSend + _T("\t");
		int nSendLen = CBaseFuncLib::Us2ToChar(strSend, &pText);

		senfRet = SOCKET_ERROR;
		senfRet = ::send(xxx, pText, strlen(pText), 0);
		if(senfRet != SOCKET_ERROR){
			cout<<endl;
			cout<<"���������ɹ�!"<<count++<<endl;
			cout<<endl;
		}
		else
			return 0;

		Sleep(1000);

		//printf("��������ʧ��%s%d\n", __FILE__, __LINE__);
		LeaveCriticalSection(&csHeartBeat);  // �뿪�ٽ����������߳̿��Խ���
	}
	return 0;
}

BOOL CStaticClass::GetInterfacePtr(CComPtr <IJsonService>& spiJsonService)
{
	if(NULL == spiJsonService)
		spiJsonService = CDbHelper::GetJsonService();
	if(NULL != spiJsonService)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// ��װ�ɴ�
// -------------------------------------------
// strJson         ���ص��ַ���
// CmdID           ��һ������ID
// strClientID     �ͻ��˹̶�ID
// spiJsonService2 �ִ��ӿ�
void CStaticClass::SJD_register(CString& strJson,CString& strClientID,int CmdID,CComPtr <IJsonService>& spiJsonService2)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		CComBSTR bstrVal;
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),CmdID);
		spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);

		spiJsonService->OutputStyledJson();
		
		// ��CRCУ��(Ŀǰδ��)

		spiJsonService->get_ObjectString(&bstrVal);
		strJson = bstrVal.m_str;
		bstrVal.Empty();
	}
}

CComPtr <IJsonService> CStaticClass::SJD_Content3()
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_GETSYS_TYPE),1);
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_COMPUTRENAME),CComBSTR("xzm-PC"));
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CURRENTUSER),CComBSTR("123"));
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_SYSVERSION),CComBSTR("win10"));
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_SYSBIT),64);
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_HOSTIP),CComBSTR("192.168.0.56"));

		//spiJsonService->OutputStyledJson();

		return spiJsonService;
	}

	return NULL;
}

CComPtr <IJsonService> CStaticClass::SJD_Content5()
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
		return spiJsonService;
	}

	return NULL;
}

CComPtr <IJsonService> CStaticClass::SJD_Content7()
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_GETSYS_TYPE),1);
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_COMPUTRENAME),CComBSTR("xzm-PC"));
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CURRENTUSER),CComBSTR("123"));
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_SYSVERSION),CComBSTR("win10"));
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_SYSBIT),64);
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_HOSTIP),CComBSTR("192.168.0.56"));

		//spiJsonService->OutputStyledJson();

		return spiJsonService;
	}

	return NULL;
}

void CStaticClass::Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,CString& strJson)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		VARIANT_BOOL bParseRet = FALSE;
		wchar_t *szBuf = NULL;
		CBaseFuncLib::CharToUS2(bstrContent,&szBuf);
		spiJsonService->ParseString(szBuf,&bParseRet);
		cout<<"��ȡCString��Value"<<endl<<endl;
		spiJsonService->OutputStyledJson();
		cout<<endl<<endl;
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(bstrKeyName,&bstrVal);
		strJson = bstrVal.m_str;
	}
}

void CStaticClass::Get_CmdContext2Level(const char* bstrContent,BSTR bstrParentKeyName,BSTR bstrKeyName,CString& strJson)
{
}

void CStaticClass::Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,LONGLONG& intJson)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		VARIANT_BOOL bParseRet = FALSE;
		wchar_t *szBuf = NULL;
		CBaseFuncLib::CharToUS2(bstrContent,&szBuf);
		spiJsonService->ParseString(szBuf,&bParseRet);
		cout<<"��ȡint��Value"<<endl<<endl;
		spiJsonService->OutputStyledJson();
		cout<<endl<<endl;
		LONGLONG bstrVal;
		spiJsonService->GetIntValue(bstrKeyName,&bstrVal);
		intJson = bstrVal;
	}
}

void CStaticClass::Get_CmdContext2Level(const char* bstrContent,BSTR bstrParentKeyName,BSTR bstrKeyName,LONGLONG& intJson)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	BOOL bRet = GetInterfacePtr(spiJsonService);
	if (bRet)
	{
		VARIANT_BOOL bParseRet = FALSE;
		wchar_t *szBuf = NULL;
		CBaseFuncLib::CharToUS2(bstrContent,&szBuf);
		spiJsonService->ParseString(szBuf,&bParseRet);

		CComPtr <IJsonService> spi = NULL;
		if (bRet)
		{
			spiJsonService->GetChildByName(bstrParentKeyName,(IJsonService **)&spi);
			LONGLONG bstrVal;
			spi->GetIntValue(bstrKeyName,&bstrVal);
			intJson = bstrVal;
		}
	}
}

void CStaticClass::OutputAReceiveMsg(const char *JsonBuff)
{
	CComPtr <IJsonService> spiJsonService = NULL;
	GetInterfacePtr(spiJsonService);

	VARIANT_BOOL bParseRet = FALSE;
	wchar_t *szBuf = NULL;
	CBaseFuncLib::CharToUS2(JsonBuff,&szBuf);
	spiJsonService->ParseString(szBuf,&bParseRet);

	cout<<endl<<endl;
	cout<<"���յ���������"<<endl;
	spiJsonService->OutputStyledJson();
	cout<<endl<<endl;
}

UINT CStaticClass::UpLoadSysLog(LPVOID pParam)
{
	EnterCriticalSection(&csUpLoad);// �����ٽ����������߳����޷�����

	//�����ϴ���־����
	CSysLog syslog;
	//����ͨ�Ż���
	syslog.CreateUDPCommunication();

	
	//�����ϴ�����
	OutputDebugString(_T("��ʼ�����ϴ�syslog��־"));

	//��ֹ�������ݿ��SetSyslogFlag��־λ
	syslog.GetLogDataFromDB(FALSE);



	LeaveCriticalSection(&csUpLoad);  // �뿪�ٽ����������߳̿��Խ���
	return 0;
}
