
#include "stdafx.h"
#include "common.h"
#include "XzmSysLog.h"
#include "BaseFuncLib.h"
#include <stdio.h>
#include "DBItemDef.h"
#include "SqliteHelper.h "


CComPtr <ISqlite3Connect> GetLogConnect(BOOL bReadFlag)
{
	CComPtr <ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr <IConnectHelper> spiConnectHelper = CDbHelper::GetDBHelper();
	if(NULL == spiConnectHelper)
		return spiSqlite3Connect;
	HRESULT hRet(E_FAIL);
	VARIANT_BOOL bCreateFlag = VARIANT_FALSE;
	CString strDBFile(_T(""));
	if(bReadFlag)
	{
		CString strOriDBFile = CBaseFuncLib::GetAppDataDir()+TDHX_SQLITEDB_LOGFILE;
		strDBFile = CBaseFuncLib::GetTmpPath()+TDHX_SQLITEDB_LOGFILE;
		BOOL bNeedCopy = FALSE;
		if(!CBaseFuncLib::IsPathExist(strDBFile) || !CBaseFuncLib::GetFileSize(strDBFile))
			bNeedCopy = TRUE;
		else
		{
			COleDateTime FileTime1 = CBaseFuncLib::GetFileWriteTime(strOriDBFile);
			COleDateTime FileTime2 = CBaseFuncLib::GetFileWriteTime(strDBFile);
			if((FileTime1-FileTime2).GetTotalSeconds() > 0.01)
				bNeedCopy = TRUE;
		}
		if(bNeedCopy)
			::CopyFile(strOriDBFile,strDBFile,FALSE);
	}
	else
	{
		bCreateFlag = VARIANT_TRUE;
		strDBFile = CBaseFuncLib::GetAppDataDir()+TDHX_SQLITEDB_LOGFILE;
	}

	hRet = spiConnectHelper->OpenDB(CComBSTR(strDBFile),bCreateFlag,15,NULL,&spiSqlite3Connect);
	if(NULL == spiSqlite3Connect)
	{
		CComBSTR bstrErrInfo;
		spiConnectHelper->get_LastErrorInfo(&bstrErrInfo);
		if(bstrErrInfo.Length())
		{
#ifdef APP_LOG_ENABLE
			/// д��־
			WRITELOGTOFILE(bstrErrInfo.m_str);
#endif
			bstrErrInfo.Empty();
		}
	}
	spiConnectHelper = NULL;
	return spiSqlite3Connect;
}

CXzmSysLog::CXzmSysLog()
{
	m_pNetSock = NULL;
	m_sock = INVALID_SOCKET;
	memset(m_ServerMajorIP,0,64);
	memset(m_ServerMinorIP,0,64);
	m_ServerPort = 514;

}

CXzmSysLog::~CXzmSysLog()
{
	if (m_pNetSock != NULL && m_sock != INVALID_SOCKET)
	{
		m_pNetSock->Stop(m_sock);
	}
}

//������XzmSysLog��ͨ��
BOOL CXzmSysLog::CreateUDPCommunication()
{
	BOOL bRet = TRUE;
	//��ȡ��ʼ�������ļ�
	HandleXzmSysLogFile();

	//��������ģʽ�µ�UDP�ͻ���
	m_pNetSock = new CNetSocket(FALSE, enumStream, enumBlock);
	if (m_pNetSock == NULL)
	{
		return FALSE;
	}
	//UDP����ҪIP�Ͷ˿�
	m_sock = m_pNetSock->Start("127.0.0.1",514);
	if (m_sock == INVALID_SOCKET)
	{
		return FALSE;
	}

	OutputDebugString(_T("��ʼ��UDP����ɹ�\n"));
	return bRet;
}

//�����ݿ��л�ȡ��־��Ϣ
BOOL CXzmSysLog::GetLogDataFromDB()
{
	BOOL bRet = TRUE;
	HRESULT hRet = 0;
	CString strSendData;

	CString strComputerName;	//�����������
	TCHAR ComputerName[MAX_COMPUTERNAME_LENGTH + 1] = {0};
	DWORD nlength = MAX_COMPUTERNAME_LENGTH + 1;
	if(GetComputerNameEx(ComputerNameDnsHostname,ComputerName, &nlength))	//�������ѡ��ͬ���͵ļ������
	{
		strComputerName = ComputerName;

	}

	//��ȡ��ǰ�ͻ��˵��豸��
	CHXRegKey regKey(COMPANY_REGPATH,PRODUCT_REGNODENAME,HKEY_LOCAL_MACHINE,KEY_READ);
	CString strDeviceID;
	regKey.GetRegStringVal(PRODUCT_COMMAN_DEVICEID, m_strDeviceID);
	if (m_strDeviceID.IsEmpty())
	{
		OutputDebugString(_T("��ע����л�ȡ�豸��ʧ�ܣ�"));
	}

	//��ѯϵͳ��־���ݿ��
	CComPtr <ISqlite3Connect> spiSqlite3Connect = GetLogConnect(FALSE);
	if(NULL == spiSqlite3Connect)
	{
		return bRet;
	}
	CComPtr <ISqlite3Statement> spiSqlite3Statement = NULL;
	CString strSqlCmd(_T(""));
	CComBSTR bstrText;

#ifdef SYSLOG_OPERATION_ENABLE
	//��ѯ���ݿ�Ĳ�����־
	strSqlCmd.Format(_T("SELECT * FROM [%s]"),SQLITEDB_TABLE_SYSLOG);

	hRet = spiSqlite3Connect->ExecQueryEx(CComBSTR(strSqlCmd),&spiSqlite3Statement);
	if(NULL != spiSqlite3Statement)
	{
		//�ж�����
		while(SUCCEEDED(spiSqlite3Statement->NextRow()))
		{
			DOUBLE dLTime = 0.0;
			long nType = 0,nFlag = 0,nLogID = 0,nSyncFlag = 0;

			spiSqlite3Statement->GetValueInt(7,&nFlag);
			if (nFlag)
			{
				//�����Ѿ�����
				continue;
			}
			//��շ�������
			strSendData.Empty();


			spiSqlite3Statement->GetValueInt(0,&nLogID);

			spiSqlite3Statement->GetValueDouble(1,&dLTime);

			spiSqlite3Statement->GetValueInt(2,&nType);

			spiSqlite3Statement->GetValueString(3,&bstrText);
			CString strTempText = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(4,&bstrText);
			CString strUID = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(5,&bstrText);
			CString strName = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueInt(6,&nSyncFlag);

			CString strTemp;
			//ת����ʽ
			if(ConvertPRIToXzmSysLog(enumUserLevelMessages, enumInformational, strTemp))
			{
				strSendData += strTemp;
				strTemp.Empty();
			}

			//ת��ʱ���ʽ

			if(ParseAndConvertLogDateToXzmSysLog(dLTime, strTemp))
			{
				strSendData += strTemp;
				strTemp.Empty();
			}

			//��Ӽ��������IP
			strSendData += _T(" ");
			strSendData += strComputerName;

			//��ӽ�����PID
			strSendData += _T(" ");
			strSendData += strName;

			strSendData += _T(":");
			//���������Ϣ
			strSendData += _T(" ");

			strSendData += strTempText;

			//��ӿͻ��˺�
			strSendData += L"|";
			strSendData += m_strDeviceID;

			//�����־����
			strSendData += L"|";
			strSendData += LOG_CLASS_OPERATION;
			//��Ӱ汾��
			strSendData += L"|";
			strSendData += L"1";


			//���µ�ǰ�ϴ���־

			CString strSqlBuff;
			strSqlBuff.Format(_T("UPDATE [%s] SET XzmSysLogFlag=%d WHERE LID=%ld"),\
				SQLITEDB_TABLE_SYSLOG,1,nLogID);


			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlBuff));
			strSqlBuff.Empty();

			//���ʹ���syslog��־
			if (m_pNetSock != NULL)
			{

				CString strstr;
				strstr.Format(_T("���͵�������:%s\n"),strSendData);
				OutputDebugString(strstr);
				//ת����ʽ
				char* pSendData = NULL;
				int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pSendData);
				if (nSendLen > 0 && pSendData != NULL)
				{
					OutputDebugString(_T("ִ�з���"));
					//����
					m_pNetSock->SendDataTo(m_sock, pSendData, nSendLen,m_ServerMajorIP,UDPSYSLOGPORT);

					//���÷���������
					m_pNetSock->SendDataTo(m_sock, pSendData, nSendLen,m_ServerMinorIP,UDPSYSLOGPORT);

					delete[] pSendData;

					Sleep(20);
				}
			}

		}

		bstrText.Empty();
		strSqlCmd.Empty();
		spiSqlite3Statement = NULL;
	}
#endif


#ifdef SYSLOG_WARNNING_ENABLE
	//��ѯ���ݿ��Ԥ����־
	strSqlCmd.Format(_T("SELECT * FROM [%s]"),SQLITEDB_TABLE_WARNLOG);


	hRet = spiSqlite3Connect->ExecQueryEx(CComBSTR(strSqlCmd),&spiSqlite3Statement);
	if(NULL != spiSqlite3Statement)
	{
		//�ж�����
		while(SUCCEEDED(spiSqlite3Statement->NextRow()))
		{
			DOUBLE dLTime = 0.0;
			long nType = 0,nFlag = 0,nCount = 0,nLogID = 0,nSyncFlag = 0,nXzmSysLogFlag = 0;
			CString strLogType;

			spiSqlite3Statement->GetValueInt(8,&nXzmSysLogFlag);
			if (nXzmSysLogFlag)
			{
				//�����Ѿ�����
				continue;
			}
			//��շ�������
			strSendData.Empty();

			spiSqlite3Statement->GetValueInt(0,&nLogID);

			spiSqlite3Statement->GetValueDouble(1,&dLTime);

			spiSqlite3Statement->GetValueInt(2,&nType);

			spiSqlite3Statement->GetValueInt(3,&nFlag);

			spiSqlite3Statement->GetValueInt(4,&nCount);

			spiSqlite3Statement->GetValueString(5,&bstrText);
			CString strTempText = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(6,&bstrText);
			CString strUID = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueInt(7,&nSyncFlag);

			CString strTemp;
			//ת����ʽ
			if(ConvertPRIToXzmSysLog(enumKernelMessages, enumWarning, strTemp))
			{
				strSendData += strTemp;
				strTemp.Empty();
			}

			//ת��ʱ���ʽ

			if(ParseAndConvertLogDateToXzmSysLog(dLTime, strTemp))
			{
				strSendData += strTemp;
				strTemp.Empty();
			}

			//��Ӽ��������IP
			strSendData += _T(" ");
			strSendData += strComputerName;


			//��ӽ�����PID
			strSendData += _T(" ");
			//ȡ��������
			int nfind = strUID.ReverseFind('\\');
			if (nfind != -1)
			{
				int nRight = strUID.GetLength() - nfind - 1;
				strTemp = strUID.Right(nRight);
				strSendData += strTemp;
				strTemp.Empty();
			}

			strSendData += _T(":");
			//���������Ϣ
			strSendData += _T(" ");

			strTemp.Empty();
			//����Ԥ�����ͻ�ȡ����


			{
				strLogType = LOG_CLASS_SHEET;

				nfind = strUID.ReverseFind(_T('.'));
				strUID = strUID.Right(strUID.GetLength()-nfind);
				switch(nType)
				{
				case LOG_VIOLATE_DISK_ALERT:
					strTemp = _T("���̿ռ䲻��Ԥ��");
					break;
				case LOG_DENY_RUN_NON_WHITE:
					if(0 == strUID.CompareNoCase(_T(".exe")) || 0 == strUID.CompareNoCase(_T(".com"))
						|| 0 == strUID.CompareNoCase(_T(".bat")) || 0 == strUID.CompareNoCase(_T(".js"))
						|| 0 == strUID.CompareNoCase(_T(".msi")) || 0 == strUID.CompareNoCase(_T(".jar")))
						strTemp = _T("�ǰ���������ܾ�����");
					else
						strTemp = _T("�ǰ�����ģ��ܾ�����");
					break;
				case LOG_ALLOW_RUN_NON_WHITE:
					if(0 == strUID.CompareNoCase(_T(".exe")) || 0 == strUID.CompareNoCase(_T(".com"))
						|| 0 == strUID.CompareNoCase(_T(".bat")) || 0 == strUID.CompareNoCase(_T(".js"))
						|| 0 == strUID.CompareNoCase(_T(".msi")) || 0 == strUID.CompareNoCase(_T(".jar")))
					{
						strTemp.Empty();
						strTemp = _T("�ǰ���������Ԥ������");
					}
					else
					{
						strTemp.Empty();
						strTemp = _T("�ǰ�����ģ��Ԥ������");
					}
					break;
				case LOG_VIOLATE_USB_ACCESS:
					if(1 == nFlag)
					{
						strLogType = LOG_CLASS_USB;
						strTemp = _T("�ǰ�����U��д����");
					}
					else if(2 == nFlag)
					{
						strLogType = LOG_CLASS_USB;
						strTemp = _T("�ǰ�����U�̽���");
					}
					break;
				default:
					strTemp.Empty();
					break;
				}
			}

			//ֻ�ϴ�Ԥ����־
			if (strTemp.IsEmpty())
			{
				continue;
			}
			strSendData += strTemp;
			strTemp.Empty();

			//��ӿͻ��˺�
			strSendData += L"|";
			strSendData += m_strDeviceID;

			//�����־����
			strSendData += L"|";
			strSendData += strLogType;
			//��Ӱ汾��
			strSendData += L"|";
			strSendData += L"1";

			//�ж��Ƿ��ǽ��̰�������־
			if (nType == LOG_DENY_RUN_NON_WHITE || nType == LOG_ALLOW_RUN_NON_WHITE)
			{
				int nLen = 1024;
				char chBuffer[1024] = {0};
				char* pProcessPath = NULL;
				//����ļ�������˾��
				strSendData += L"|";

				CBaseFuncLib::Us2ToChar(strUID, &pProcessPath);
				if (CBaseFuncLib::GetCompanyName(pProcessPath, chBuffer,nLen))
				{
					CString strTempBuffer(chBuffer);

					strSendData += strTempBuffer;

					memset(chBuffer,0,nLen);

				}


				//����ļ��汾��
				strSendData += L"|";
				if (CBaseFuncLib::GetFileVersion(pProcessPath, chBuffer,nLen))
				{
					CString strTempBuffer(chBuffer);

					strSendData += strTempBuffer;

					memset(chBuffer,0,nLen);

				}


				//����ļ��Ĵ�С
				strSendData += L"|";
				DWORD dwFileSize = CBaseFuncLib::GetFileSize(strUID);
				//dwFileSize = CBaseFuncLib::GetSpecialFileSize(strUID);
				strTemp.Format(_T("%ld"),dwFileSize);

				strSendData += strTemp;
				strTemp.Empty();

				//����ļ���ȫ·��
				strSendData += L"|";
				strSendData += strUID;

				//�ͷŵ�ַ�ռ�
				if (pProcessPath != NULL)
				{
					delete[] pProcessPath;
				}

			}



			//���µ�ǰ�ϴ���־

			CString strSqlBuff;
			strSqlBuff.Format(_T("UPDATE [%s] SET XzmSysLogFlag=%d WHERE LID=%ld"),\
				SQLITEDB_TABLE_WARNLOG,1,nLogID);


			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlBuff));
			strSqlBuff.Empty();

			//���ʹ���syslog��־
			if (m_pNetSock != NULL)
			{

				CString strstr;
				strstr.Format(_T("���͵�������:%s\n"),strSendData);
				OutputDebugString(strstr);
				//ת����ʽ
				char* pSendData = NULL;
				int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pSendData);
				if (nSendLen > 0 && pSendData != NULL)
				{
					OutputDebugString(_T("ִ�з���"));
					//����
					m_pNetSock->SendDataTo(m_sock, pSendData, nSendLen,m_ServerMajorIP,UDPSYSLOGPORT);

					//���÷���������
					m_pNetSock->SendDataTo(m_sock, pSendData, nSendLen,m_ServerMinorIP,UDPSYSLOGPORT);

					delete[] pSendData;

					Sleep(20);
				}
			}


		}

		bstrText.Empty();
		strSqlCmd.Empty();
		spiSqlite3Statement = NULL;
	}
#endif


#ifdef SYSLOG_EVENT_ENABLE
	//��ѯ���ݿ��ϵͳ��־
	strSqlCmd.Format(_T("SELECT * FROM [%s]"),SQLITEDB_TABLE_SYS_EVENTLOG);


	hRet = spiSqlite3Connect->ExecQueryEx(CComBSTR(strSqlCmd),&spiSqlite3Statement);
	if(NULL != spiSqlite3Statement)
	{
		//�ж�����
		while(SUCCEEDED(spiSqlite3Statement->NextRow()))
		{

			long nLTime = 0,nEventID = 0,nEventType = 0,nEventClass = 0,nFlag = 0,nLogID = 0;

			spiSqlite3Statement->GetValueInt(9,&nFlag);
			if (nFlag)
			{
				//�����Ѿ�����
				continue;
			}
			//��շ�������
			strSendData.Empty();

			spiSqlite3Statement->GetValueInt(0,&nLTime);

			spiSqlite3Statement->GetValueInt(1,&nEventID);

			spiSqlite3Statement->GetValueInt(2,&nEventType);

			spiSqlite3Statement->GetValueInt(3,&nEventClass);

			spiSqlite3Statement->GetValueString(4,&bstrText);
			CString strEventSource = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(5,&bstrText);
			CString strEventComputerName = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(6,&bstrText);
			CString strEventUserName = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(7,&bstrText);
			CString strTaskCategory = bstrText.m_str;
			bstrText.Empty();

			spiSqlite3Statement->GetValueString(8,&bstrText);
			CString strEventDesc = bstrText.m_str;
			bstrText.Empty();
			//��ȡ��־ID���ڸ�������
			spiSqlite3Statement->GetValueInt(10,&nLogID);

			CString strTemp;
			//ת����ʽ
			if(ConvertPRIToXzmSysLog(enumSystemDaemons, enumNotice, strTemp))
			{
				strSendData += strTemp;
				strTemp.Empty();
			}

			//ת��ʱ���ʽ
			DOUBLE fTime = CBaseFuncLib::GetMsgTime(nLTime);

			if(ParseAndConvertLogDateToXzmSysLog(fTime, strTemp))
			{
				strSendData += strTemp;
				strTemp.Empty();
			}

			//��Ӽ��������IP
			strSendData += _T(" ");
			strSendData += strEventComputerName;

			//��ӽ�����PID
			strSendData += _T(" ");

			CBaseFuncLib::GetInfoClass(nEventClass, strTemp);
			strSendData += strTemp;
			strTemp.Empty();
			strSendData += _T(":");
			//���������Ϣ
			strSendData += _T(" ");

			strTemp.Format(_T("%s"), strEventDesc);
			strSendData += strTemp;
			strTemp.Empty();

			//��ӿͻ��˺�
			strSendData += L"|";
			strSendData += m_strDeviceID;

			//�����־����
			strSendData += L"|";
			strSendData += LOG_CLASS_WINDOWS;
			//��Ӱ汾��
			strSendData += L"|";
			strSendData += L"1";

			//�����־����
			strSendData += L"|";
			strTemp.Format(_T("%d"), nEventType);
			strSendData += strTemp;
			strTemp.Empty();

			//����¼�ID
			strSendData += L"|";
			strTemp.Format(_T("%d"), nEventID);
			strSendData += strTemp;
			strTemp.Empty();

			//����¼���Դ
			strSendData += L"|";
			strTemp.Format(_T("%s"), strEventSource);
			strSendData += strTemp;
			strTemp.Empty();

			//����û���
			strSendData += L"|";
			strTemp.Format(_T("%s"), strEventUserName);
			strSendData += strTemp;
			strTemp.Empty();


			//���µ�ǰ�ϴ���־

			CString strSqlBuff;
			strSqlBuff.Format(_T("UPDATE [%s] SET XzmSysLogFlag=%d WHERE LID=%ld"),\
				SQLITEDB_TABLE_SYS_EVENTLOG,1,nLogID);


			hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlBuff));
			strSqlBuff.Empty();

			//���ʹ���syslog��־
			if (m_pNetSock != NULL)
			{

				CString strstr;
				strstr.Format(_T("���͵�������:%s\n"),strSendData);
				OutputDebugString(strstr);

				//ת����ʽ
				char* pSendData = NULL;
				int nSendLen = CBaseFuncLib::Us2ToChar(strSendData, &pSendData);
				if (nSendLen > 0 && pSendData != NULL)
				{
					OutputDebugString(_T("ִ�з���"));
					//����
					m_pNetSock->SendDataTo(m_sock, pSendData, nSendLen,m_ServerMajorIP,UDPSYSLOGPORT);

					//���÷���������
					m_pNetSock->SendDataTo(m_sock, pSendData, nSendLen,m_ServerMinorIP,UDPSYSLOGPORT);

					delete[] pSendData;

					Sleep(20);
				}
			}


		}
		bstrText.Empty();
		strSqlCmd.Empty();
		spiSqlite3Statement = NULL;
	}
#endif


	spiSqlite3Connect->Close();
	spiSqlite3Connect = NULL;


	return bRet;
}


//ת��XzmSysLog ��PRI��
BOOL CXzmSysLog::ConvertPRIToXzmSysLog(enumFACILITY nFacility, enumSEVERITY nSeverity, CString& strPRI)
{
	BOOL bRet = TRUE;
	int nFRI = 0;

	switch(nFacility)
	{
	case enumKernelMessages:
		nFRI = enumKernelMessages;
		break;
	case enumUserLevelMessages:
		nFRI = enumUserLevelMessages;
		break;
	case enumMailSystem:
		nFRI = enumMailSystem;
		break;
	case enumSystemDaemons:
		nFRI = enumSystemDaemons;
		break;
	case enumSecurityOrAuthorizationMessages_1:
		nFRI = enumSecurityOrAuthorizationMessages_1;
		break;
	case enumMessagesGeneratedInternallyByXzmSysLogd:
		nFRI = enumMessagesGeneratedInternallyByXzmSysLogd;
		break;
	case enumLinePrinterSubsystem:
		nFRI = enumLinePrinterSubsystem;
		break;
	case enumNetworkNewsSubsystem:
		nFRI = enumNetworkNewsSubsystem;
		break;
	case enumUUCPSubsystem:
		nFRI = enumUUCPSubsystem;
		break;
	case enumClockDaemon_1:
		nFRI = enumClockDaemon_1;
		break;
	case enumSecurityOrAuthorizationMessages_2:
		nFRI = enumSecurityOrAuthorizationMessages_2;
		break;
	case enumFTPDaemon:
		nFRI = enumFTPDaemon;
		break;
	case enumNTPSubsystem:
		nFRI = enumNTPSubsystem;
		break;
	case enumLogAudit:
		nFRI = enumLogAudit;
		break;
	case enumLogAlert:
		nFRI = enumLogAlert;
		break;
	case enumClockDaemon_2:
		nFRI = enumClockDaemon_2;
		break;
	case enumLocalUse0:
		nFRI = enumLocalUse0;
		break;
	case enumLocalUse1:
		nFRI = enumLocalUse1;
		break;
	case enumLocalUse2:
		nFRI = enumLocalUse2;
		break;
	case enumLocalUse3:
		nFRI = enumLocalUse3;
		break;
	case enumLocalUse4:
		nFRI = enumLocalUse4;
		break;
	case enumLocalUse5:
		nFRI = enumLocalUse5;
		break;
	case enumLocalUse6:
		nFRI = enumLocalUse6;
		break;
	case enumLocalUse7:
		nFRI = enumLocalUse7;
		break;
	default:
		break;

	}

	//������λ
	nFRI *= 8;

	switch(nSeverity)
	{
	case enumEmergency:
		nFRI += enumEmergency;
		break;
	case enumAlert:
		nFRI += enumAlert;
		break;
	case enumCritical:
		nFRI += enumCritical;
		break;
	case enumError:
		nFRI += enumError;
		break;
	case enumWarning:
		nFRI += enumWarning;
		break;
	case enumNotice:
		nFRI += enumNotice;
		break;
	case enumInformational:
		nFRI += enumInformational;
		break;
	case enumDebug:
		nFRI += enumDebug;
		break;
	default:
		break;

	}

	CString str;
	str.Format(_T("<%d>"),nFRI);

	strPRI = str;

	return bRet;
}



//������־��ת��Ϊsyslog��ʽ
BOOL CXzmSysLog::ParseAndConvertLogDateToXzmSysLog(DOUBLE fTime, CString& strDate, BOOL bYear)
{
	BOOL bRet = TRUE;
	CString strTemp;
	//����ʱ�䣬����������ʱ����
	COleDateTime tempDate(fTime);

	//ƴ���ַ���
	if(GetMonthString(tempDate.GetMonth(),strTemp))
	{
		strDate += strTemp;
		strTemp.Empty();
	}

	strDate += _T(" ");
	strTemp.Format(_T("%d"),tempDate.GetDay());
	strDate += strTemp;
	strTemp.Empty();

	strDate += _T(" ");
	strTemp.Format(_T("%02d:%02d:%02d"),tempDate.GetHour(),tempDate.GetMinute(),tempDate.GetSecond());
	strDate += strTemp;
	strTemp.Empty();

	if (bYear)
	{
		strDate += _T(" ");
		strTemp.Format(_T("CST %04d"),tempDate.GetYear());
		strDate += strTemp;
		strTemp.Empty();
	}

	return bRet;
}

//��ȡ�·��ַ���
BOOL CXzmSysLog::GetMonthString(int nMonth,CString& strMonth)
{
	BOOL bRet = TRUE;
	if (nMonth <= 0)
	{
		return FALSE;
	}

	switch(nMonth)
	{
	case enumJanuary:
		strMonth = JUNUARY;
		break;
	case enumFebruary:
		strMonth = FEBRUARY;
		break;
	case enumMarch:
		strMonth = MARCH;
		break;
	case enumApril:
		strMonth = APRIL;
		break;
	case enumMay:
		strMonth = MAY;
		break;
	case enumJune:
		strMonth = JUNE;
		break;
	case enumJuly:
		strMonth = JULY;
		break;
	case enumAugust:
		strMonth = AUGUST;
		break;
	case enumSeptember:
		strMonth = SEPTEMBER;
		break;
	case enumOctober:
		strMonth = OCTOBER;
		break;
	case enumNovember:
		strMonth = NOVEMBER;
		break;
	case enumDecember:
		strMonth = DECEMBER;
		break;

	default:
		break;
	}

	return bRet;
}


//���������ļ�����ȡXzmSysLog������IP
BOOL CXzmSysLog::ParseXzmSysLogConfigFile(BYTE* pchXzmSysLog)
{
	BOOL bRet = FALSE;

	if (pchXzmSysLog  == NULL)
	{
		OutputDebugString(_T("����ʧ��"));
		return bRet;
	}

	CString strTemp(pchXzmSysLog);



	//��ʼ����
	int nLen = strTemp.Find(SYSLOGSECTION);
	if (nLen != -1)
	{
		OutputDebugString(_T("���ҵ���Ч�ֶ�"));
		int nLength = strTemp.GetLength() - nLen - sizeof(SYSLOGSECTION)/sizeof(TCHAR);	//�õ��ַ���

		CString strStart = strTemp.Right(nLength);
		if (!strStart.IsEmpty())
		{
			OutputDebugString(_T("��ȡ����Ч�ֶ�"));
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
					strstr.Format(_T("0 ��ʼ��ȡini�ļ���������:%s\n"),strIP);
					OutputDebugString(strstr);

					if (0 == strIPName.CompareNoCase(SERVERMAJORIP))
					{

						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMajorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}

					}
					else if (0 == strIPName.CompareNoCase(SERVERMINORIP))
					{
						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMinorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}

					}
					else if (0 == strIPName.CompareNoCase(SERVERPORT))
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


					if (0 == strIPName.CompareNoCase(SERVERMAJORIP))
					{
						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMajorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}
					}
					else if (0 == strIPName.CompareNoCase(SERVERMINORIP))
					{

						char* ptempIP = NULL;
						int nSendLen = CBaseFuncLib::Us2ToChar(strIP, &ptempIP);
						if (nSendLen > 0 && nSendLen < 64 && ptempIP != NULL)
						{

							memcpy(m_ServerMinorIP, ptempIP, nSendLen);
							delete[] ptempIP;
						}

					}
					else if (0 == strIPName.CompareNoCase(SERVERPORT))
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
			OutputDebugString(_T("û�н�ȡ����Ч�ֶ�"));
		}

	}
	else
	{
		OutputDebugString(_T("û�в��ҵ���Ч�ֶ�"));
	}



	return bRet;
}

//����XzmSysLog�ļ�
BOOL CXzmSysLog::HandleXzmSysLogFile()
{
	BOOL bRet = FALSE;
	BYTE* ppData = NULL;


	//��ȡ·��
	CString strTmpPath = CBaseFuncLib::GetAppDir();
	/*strTmpPath += _T("\\");*/
	strTmpPath += SYSLOGCONFIGFILENAME;

	CString strstr;
	strstr.Format(_T("��ini�ļ�:%s\n"), strTmpPath);
	OutputDebugString(strstr);

	//��ȡ�����ļ�������
	DWORD nReadLen = CBaseFuncLib::ReadAllData( strTmpPath, &ppData);
	if (ppData != NULL)
	{
		//��ȡ����Ч����
		OutputDebugString(_T("��ʼ����ini�ļ�"));
		if(ParseXzmSysLogConfigFile(ppData))
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


