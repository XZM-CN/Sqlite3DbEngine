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
			/// д������
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

	// �����׽���
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
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
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		::MessageBox(NULL,_T("δ��ʼ��socket��"),_T(""),MB_OK);
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

	// ��������
	char buff[256];
	int nRecv = ::recv(xxx, buff, 256, 0);
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" ���յ����ݣ�%s", buff);
	}

	// �ر��׽���
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
			/// д������
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

	// �����׽���
	SOCKET xxx = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(xxx == INVALID_SOCKET)
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
	servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.8");

	if(::connect(xxx, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		::MessageBox(NULL,_T("δ��ʼ��socket��"),_T(""),MB_OK);
		printf(" Failed connect() \n");
		return 0;
	}

	char *pText = NULL;
	strSendData = strSendData + _T("\t");
	int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pText);
	::send(xxx, pText, strlen(pText), 0);

	// ��������
	char buff[256];
	int nRecv = ::recv(xxx, buff, 256, 0);
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" ���յ����ݣ�%s", buff);
	}

	// �ر��׽���
	::closesocket(xxx);
	return 0;
}
