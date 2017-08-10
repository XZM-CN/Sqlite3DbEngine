#include "StdAfx.h"
#include "StaticClass.h"
#include "SqliteHelper.h"
#include "BaseFuncLib.h"
#include "SysLog.h "

CRITICAL_SECTION csHeartBeat;
CRITICAL_SECTION csUpLoad;

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
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// while (1)
			// {
			// }
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister2(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		//// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister3(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister4(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister5(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister6(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister7(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister8(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister9(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister10(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister11(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister12(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister13(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister14(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister15(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister16(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister17(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister18(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister19(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::ApplyRegister20(LPVOID pParam)
{
	PCStaticClass pThis = (PCStaticClass)pParam;
	// 创建套节字
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址;否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port   = htons(514);

	// 注意,这里要填写服务器程序(TCPServer程序)所在机器的IP地址
	// 如果你的计算机没有联网,直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.25");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		// ::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	// 接收数据
	char JsonBuff[256];
	int nRecv = -1;
	memset(JsonBuff,0,256);
	int senfRet = SOCKET_ERROR;
	CString strClientID;
	char *szValue = NULL;
	CTime ct = CTime::GetCurrentTime();
	//strClientID.Format(_T("TDHX%d%d%d%d%d%d%s"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond(),name);
	strClientID.Format(_T("%d%d%d%d%d%d"),ct.GetYear(),ct.GetMonth(),ct.GetDay(),ct.GetHour(),ct.GetMinute(),ct.GetSecond());
	CBaseFuncLib::Us2ToChar(strClientID/*strChild*/,&szValue);
	while (1)
	{
		if (strlen(JsonBuff) == 0)
		{
			CString strSendData;
			if(TRUE){
				DWORD dwCrc32 = 0;
				CComBSTR bstrVal;

				CComPtr <IJsonService> spiJsonService = NULL;
				CComPtr <IJsonService> spiJsonService2 = NULL;
				if(NULL == spiJsonService)
					spiJsonService = CDbHelper::GetJsonService();
				if(NULL != spiJsonService)
				{
					/// 写入配置
					//spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
					spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(strClientID.GetBuffer(0)));
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
					spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
					spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
					//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
					spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),1);
					spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
					spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
					spiJsonService->get_ObjectString(&bstrVal);
					CString m_strHeartBeat = bstrVal.m_str;
					strSendData = m_strHeartBeat;
				}
			}

			char *pText = NULL;
			strSendData = strSendData + _T("\t");
			int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
			senfRet = SOCKET_ERROR;
			senfRet = ::send(xxx, pText, strlen(pText), 0);
			if(senfRet == SOCKET_ERROR)
			{
				printf("发送数据失败%s%d\n", __FILE__, __LINE__);
			}
			senfRet = SOCKET_ERROR;
			// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
			// 	{
			// 		BOOL bRet = FALSE;
			// 	}

			// 开始发心跳
			DWORD dwThreadId;
			HANDLE hThread;
			hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::HeartBeat, (LPVOID)xxx, 0, &dwThreadId);

			// 			while (1)
			// 			{
			// 			}
			nRecv = -1;
			nRecv = ::recv(xxx, JsonBuff, 256, 0);
			if(nRecv > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);
			}
		}
		else // 解析字符串
		{
			if(strlen(JsonBuff) > 0)
			{
				JsonBuff[nRecv] = '\0';
				OutputAReceiveMsg(JsonBuff);

				LONGLONG cmdID = -1000;
				Get_CmdContext(JsonBuff,CComBSTR(JSON_STRING_CMD_ID),cmdID);

				switch(cmdID){
				case 1:
					{
						char pText[256] = "想要什么";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 1"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 2:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 2"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 3:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 3"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 4:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 4"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 5:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 5"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 6:
					{
						char pText[256] = "想要什么\t";
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						cout<<endl<<"出现了命令 6"<<endl;
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
						}
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}
						break;
					}
				case 7:
					{
						char *pText = NULL;
						CString strContext ;
						CComPtr <IJsonService> spiJsonService2=SJD_Content7();
						SJD_register(strContext,strClientID,7,spiJsonService2);
						strContext = strContext + _T("\t");
						int nSendLen = CBaseFuncLib::Us2ToChar(strContext, &pText);
						senfRet = SOCKET_ERROR;
						senfRet = ::send(xxx, pText, strlen(pText), 0);
						if(senfRet == SOCKET_ERROR)
						{
							printf("发送数据失败%s%d", __FILE__, __LINE__);
							break;
						}

						// 开始发心跳
						DWORD dwThreadId;
						HANDLE hThread;
						hThread=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)CStaticClass::UpLoadSysLog, (LPVOID)xxx, 0, &dwThreadId);


						// 接收数据
						senfRet = SOCKET_ERROR;
						memset(JsonBuff,0,256);
						nRecv = -1;
						nRecv = ::recv(xxx, JsonBuff, 256, 0);
						if(nRecv > 0)
						{
							continue;
						}

					}
					break;
				default:
					{

					}
					break;
				}

			}
			else 
			{
				break;
			}
		}
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

static int count = 0;
UINT CStaticClass::HeartBeat(LPVOID pParam)
{
	SOCKET xxx = (SOCKET)pParam;

	char pText[256];
	int senfRet;
	while(1){
		EnterCriticalSection(&csHeartBeat);// 进入临界区，其它线程则无法进入
		memset(pText,0,256);
		sprintf(pText,"_HeartBeat%d\t",count);
		senfRet = SOCKET_ERROR;
		senfRet = ::send(xxx, pText, strlen(pText), 0);
		if(senfRet != SOCKET_ERROR){
			cout<<endl;
			cout<<"发送心跳成功!"<<count++<<endl;
			cout<<endl;
		}
		Sleep(1000);

		printf("发送数据失败%s%d\n", __FILE__, __LINE__);
		LeaveCriticalSection(&csHeartBeat);  // 离开临界区，其它线程可以进入
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
		
		// 做CRC校验(目前未做)

		spiJsonService->get_ObjectString(&bstrVal);
		strJson = bstrVal.m_str;
		bstrVal.Empty();
	}
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
		cout<<"获取CString的Value"<<endl<<endl;
		spiJsonService->OutputStyledJson();
		cout<<endl<<endl;
		CComBSTR bstrVal;
		spiJsonService->GetStringValue(bstrKeyName,&bstrVal);
		strJson = bstrVal.m_str;
	}
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
		cout<<"获取int的Value"<<endl<<endl;
		spiJsonService->OutputStyledJson();
		cout<<endl<<endl;
		LONGLONG bstrVal;
		spiJsonService->GetIntValue(bstrKeyName,&bstrVal);
		intJson = bstrVal;
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
	cout<<"接收到的数据是"<<endl;
	spiJsonService->OutputStyledJson();
	cout<<endl<<endl;
}

UINT CStaticClass::UpLoadSysLog(LPVOID pParam)
{
	EnterCriticalSection(&csUpLoad);// 进入临界区，其它线程则无法进入

	//创建上传日志对象
	CSysLog syslog;
	//创建通信机制
	syslog.CreateUDPCommunication();

	
	//处理上传事务
	OutputDebugString(_T("开始处理上传syslog日志"));

	//禁止更改数据库的SetSyslogFlag标志位
	syslog.GetLogDataFromDB(FALSE);



	LeaveCriticalSection(&csUpLoad);  // 离开临界区，其它线程可以进入
	return 0;
}
