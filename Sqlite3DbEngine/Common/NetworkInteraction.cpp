
#include "stdafx.h"
#include "common.h"
#include "BaseFuncLib.h"
#include "SKFHelper.h"
#include "SqliteHelper.h"
#include "NetworkInteraction.h"

CNetworkInteraction::CNetworkInteraction()
{

	m_pNetSock = NULL;
	m_sock = INVALID_SOCKET;
	memset(m_ServerMajorIP,0,64);
	memset(m_ServerMinorIP,0,64);
	m_ServerPort = 514;

	//��ȡע����е�ClientID
	CString strUniID;
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID,strUniID);

	if(!strUniID.IsEmpty())
	{
		m_strClientID = strUniID;
	}

	m_pNetSock = NULL;

}
CNetworkInteraction:: ~CNetworkInteraction()
{

}

//����ͨ�Žӿ�
BOOL CNetworkInteraction::CreateConnect()
{
	BOOL bRet = FALSE;

	//��������ģʽ�µ�TCP�ͻ���
	m_pNetSock = new CNetSocket(FALSE, enumStream, enumBlock);
	if (m_pNetSock == NULL)
	{
		return FALSE;
	}


	//��ȡ��ʼ�������ļ�
	GetConfInfo();

	// TCP ����
	m_sock = m_pNetSock->Start(m_ServerMinorIP,m_ServerPort);
	if (m_sock == INVALID_SOCKET)
	{
		return FALSE;
	}

	OutputDebugString(_T("��ʼ��UDP����ɹ�\n"));
	return bRet;
}

BOOL CNetworkInteraction::GetConfInfo()
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

BOOL CNetworkInteraction::ParseSyslogConfigFile(BYTE* pchSyslog)
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

//�������ݰ���ƽ̨
BOOL CNetworkInteraction::SendJSonPacket()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ƽ̨���ݰ�
BOOL CNetworkInteraction::RecvJSonPacket(CString strJSon)
{
	BOOL bRet = FALSE;


	return bRet;
}

//��������ͷ
BOOL CNetworkInteraction::ParseJsonFromNetwork(CString strCommand)
{
	BOOL bRet = FALSE;

	VARIANT_BOOL bRetFlag = VARIANT_FALSE;
	CComBSTR bstrVal;

	CComPtr <IJsonService> spiJsonService = NULL;
	if(NULL == spiJsonService)
		spiJsonService = CDbHelper::GetJsonService();
	if(NULL != spiJsonService)
	{
		spiJsonService->put_CodingType(CODINGTYPE_UTF8);
		HRESULT hRet = spiJsonService->ParseString(CComBSTR(strCommand),&bRetFlag);
	}


	if(VARIANT_FALSE == bRetFlag)
	{
		spiJsonService = NULL;

		return bRet;
	}
	//�ɹ�
	bRet = TRUE;

	spiJsonService->GetStringValue(CComBSTR(_T("ValidDay")),&bstrVal);
	if(bstrVal.Length())
		//strValidDay = bstrVal.m_str;
		bstrVal.Empty();


	spiJsonService = NULL;
	return bRet;
}

//����ƽ̨����״̬
BOOL CNetworkInteraction::ParseJsonRegister()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ж������
BOOL CNetworkInteraction::ParseJsonUnRegister()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������ȡϵͳ��Դ��Ϣ
BOOL CNetworkInteraction::ParseJsonSysResource()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������ȡϵͳ�汾��Ϣ
BOOL CNetworkInteraction::ParseJsonSysType()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ɨ���������
BOOL CNetworkInteraction::ParseJsonScanDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ɾ���������̰���������
BOOL CNetworkInteraction::ParseJsonDelProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������ӵ������̰���������
BOOL CNetworkInteraction::ParseJsonAddProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����ϴ�����������״̬����
BOOL CNetworkInteraction::ParseJsonSendWhiteList()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������װ��������
BOOL CNetworkInteraction::ParseJsonInstallProgram()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����ϴ�U����Ϣ����״̬����
BOOL CNetworkInteraction::ParseJsonUDiskDevice()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������Ӱ�ȫU������
BOOL CNetworkInteraction::ParseJsonAddUDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�������������������
BOOL CNetworkInteraction::ParseJsonTurnOnSelf()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����������ع�������
BOOL CNetworkInteraction::ParseJsonIntercept()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�������ð�ȫ�ȼ�����
BOOL CNetworkInteraction::ParseJsonSafeLevel()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����U�̿�����������
BOOL CNetworkInteraction::ParseJsonUDiskSet()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����˫���ӵ�¼��������
BOOL CNetworkInteraction::ParseJsonDLogin()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������Ȩ�ļ�����
BOOL CNetworkInteraction::ParseJsonAuthFile()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����������Ȩ����
BOOL CNetworkInteraction::ParseJsonUpdateAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����Ƴ���Ȩ����
BOOL CNetworkInteraction::ParseJsonRemoveAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����������̰�������������
BOOL CNetworkInteraction::ParseJsonTurnOnServer()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������ȡ��ǰ������Ϣ����
BOOL CNetworkInteraction::ParseJsonGetConfig()
{
	BOOL bRet = FALSE;


	return bRet;
}
//�����ϴ��û���Ϣ����״̬����
BOOL CNetworkInteraction::ParseJsonUserName()
{
	BOOL bRet = FALSE;


	return bRet;
}


//�������ͷ��Ϣ
BOOL CNetworkInteraction::ConvertStringToJSon(CString strSource, CString& strJSon)
{
	BOOL bRet = FALSE;


	return bRet;
}

//���ע�ᵽƽ̨�����ݰ�
BOOL CNetworkInteraction::PacketJSonRegister1()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���ע��ɹ���Ϣ
BOOL CNetworkInteraction::PacketJsonRegister2()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���ж�ط���״̬
BOOL CNetworkInteraction::PacketJsonUnRegister()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���ϵͳ��Դ��Ϣ
BOOL CNetworkInteraction::PacketJsonSysResource()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���ϵͳ�汾��Ϣ
BOOL CNetworkInteraction::PacketJsonSysType()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�������ɨ����������״̬��Ϣ
BOOL CNetworkInteraction::PacketJsonScanDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���ɾ���������̰�����ִ��״̬��Ϣ
BOOL CNetworkInteraction::PacketJsonDelProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����ӵ������̰���������
BOOL CNetworkInteraction::PacketJsonAddProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ϴ�����������״̬����
BOOL CNetworkInteraction::PacketJsonSendWhiteList()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����װ��������
BOOL CNetworkInteraction::PacketJsonInstallProgram()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ϴ�U����Ϣ����״̬����
BOOL CNetworkInteraction::PacketJsonUDiskDevice()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����Ӱ�ȫU������
BOOL CNetworkInteraction::PacketJsonAddUDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������������������
BOOL CNetworkInteraction::PacketJsonTurnOnSelf()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����������ع�������
BOOL CNetworkInteraction::PacketJsonIntercept()
{
	BOOL bRet = FALSE;


	return bRet;
}

//������ð�ȫ�ȼ�����
BOOL CNetworkInteraction::PacketJsonSafeLevel()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���U�̿�����������
BOOL CNetworkInteraction::PacketJsonUDiskSet()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���˫���ӵ�¼��������
BOOL CNetworkInteraction::PacketJsonDLogin()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����Ȩ�ļ�����
BOOL CNetworkInteraction::PacketJsonAuthFile()
{
	BOOL bRet = FALSE;


	return bRet;
}

//���������Ȩ����
BOOL CNetworkInteraction::PacketJsonUpdateAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����Ƴ���Ȩ����
BOOL CNetworkInteraction::PacketJsonRemoveAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����������̰�������������
BOOL CNetworkInteraction::PacketJsonTurnOnServer()
{
	BOOL bRet = FALSE;


	return bRet;
}

//�����ȡ��ǰ������Ϣ����
BOOL CNetworkInteraction::PacketJsonGetConfig()
{
	BOOL bRet = FALSE;


	return bRet;
}

//����ϴ��û���Ϣ����״̬����
BOOL CNetworkInteraction::PacketJsonUserName()	
{
	BOOL bRet = FALSE;


	return bRet;
}


//��������������
BOOL CNetworkInteraction::PacketJsonHeartBeat()				//���������
{
	BOOL bRet = FALSE;

	DWORD dwCrc32 = 0;
	CComBSTR bstrVal;

	CComPtr <IJsonService> spiJsonService = NULL;
	CComPtr <IJsonService> spiJsonService2 = NULL;
	if(NULL == spiJsonService)
		spiJsonService = CDbHelper::GetJsonService();
	if(NULL != spiJsonService)
	{
		/// д������
		spiJsonService->put_StringValue(CComBSTR(JSON_STRING_CLIENT_ID),CComBSTR(m_strClientID));
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CRC32),dwCrc32);
		spiJsonService->put_IntValue(CComBSTR(JSON_STRING_CMD_ID),enumHeartbeat);
		spiJsonService->CreateChild(CComBSTR(JSON_STRING_CONTENT),&spiJsonService2);
		spiJsonService2->put_IntValue(CComBSTR(JSON_STRING_HEART_BEAT),1);
		spiJsonService->get_ObjectString(&bstrVal);

		CString strTemp1 = bstrVal.m_str;
		bstrVal.Empty();
		spiJsonService2->get_ObjectString(&bstrVal);
		CString strTemp2 = bstrVal.m_str;
		bstrVal.Empty();

		//spiJsonService->PutChild(CComBSTR(JSON_STRING_CONTENT),spiJsonService2);
		spiJsonService->get_ObjectString(&bstrVal);

		m_strHeartBeat = bstrVal.m_str;
		bRet = TRUE;
	}

	return bRet;
}
