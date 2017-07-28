#include "StdAfx.h"
#include "StaticClass.h"
#include "SqliteHelper.h"
#include "BaseFuncLib.h"
CStaticClass::CStaticClass(void)
{
}


CStaticClass::~CStaticClass(void)
{
}

UINT CStaticClass::ApplyRegister(LPVOID pParam)
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
			spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
			spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
			spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
			spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
			//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
			spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_REGISTAR),1);
			spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
			spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
			spiJsonService->get_ObjectString(&bstrVal);

			CString strTemp1 = bstrVal.m_str;
			bstrVal.Empty();
			spiJsonService2->get_ObjectString(&bstrVal);
			CString strTemp2 = bstrVal.m_str;
			bstrVal.Empty();

			//spiJsonService->PutChild(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
			spiJsonService->get_ObjectString(&bstrVal);

			CString m_strHeartBeat = bstrVal.m_str;
			strSendData = m_strHeartBeat;
		}

	}
	//return 0;

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
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	char *pText = NULL;
	strSendData = strSendData + _T("\t");
	int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
	::send(xxx, pText, strlen(pText), 0);

// 	if(::sendto(xxx, pText, 10,0,(sockaddr*)&servAddr,sizeof(pText)) == SOCKET_ERROR)
// 	{
// 		BOOL bRet = FALSE;
// 	}

	// 接收数据
	char buff[256];
	int nRecv = ::recv(xxx, buff, 256, 0);
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" 接收到数据：%s", buff);
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}

UINT CStaticClass::AuditRegister(LPVOID pParam)
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
			spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR("E7C37BD7256059233D9566243203C12D"));
			spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),123456);
			spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),1);
			spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
			//spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
			spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_CLIENTTYPE),2);
			spiJsonService2->put_StringValue(CComBSTR(JSON_STRING_AUTHCODE),CComBSTR("HX1001-98254-30A9D-277AB"));
			spiJsonService->AddObjAsChildNode(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
			spiJsonService->get_ObjectString(&bstrVal);

			CString strTemp1 = bstrVal.m_str;
			bstrVal.Empty();
			spiJsonService2->get_ObjectString(&bstrVal);
			CString strTemp2 = bstrVal.m_str;
			bstrVal.Empty();

			//spiJsonService->PutChild(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
			spiJsonService->get_ObjectString(&bstrVal);

			CString m_strHeartBeat = bstrVal.m_str;
			strSendData = m_strHeartBeat;
		}

	}
	//return 0;

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
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		::MessageBox(NULL,_T("未初始化socket库"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	char *pText = NULL;
	strSendData = strSendData + _T("\t");
	int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
	::send(xxx, pText, strlen(pText), 0);

	// 接收数据
	char buff[256];
	int nRecv = ::recv(xxx, buff, 256, 0);
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" 接收到数据：%s", buff);
	}

	// 关闭套节字
	::closesocket(xxx);
	return 0;
}
