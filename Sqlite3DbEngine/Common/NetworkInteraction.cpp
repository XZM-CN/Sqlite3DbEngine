
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

	//读取注册表中的ClientID
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

//创建通信接口
BOOL CNetworkInteraction::CreateConnect()
{
	BOOL bRet = FALSE;

	//创建阻塞模式下的TCP客户端
	m_pNetSock = new CNetSocket(FALSE, enumStream, enumBlock);
	if (m_pNetSock == NULL)
	{
		return FALSE;
	}


	//读取初始化配置文件
	GetConfInfo();

	// TCP 连接
	m_sock = m_pNetSock->Start(m_ServerMinorIP,m_ServerPort);
	if (m_sock == INVALID_SOCKET)
	{
		return FALSE;
	}

	OutputDebugString(_T("初始化UDP网络成功\n"));
	return bRet;
}

BOOL CNetworkInteraction::GetConfInfo()
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

BOOL CNetworkInteraction::ParseSyslogConfigFile(BYTE* pchSyslog)
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

//发送数据包到平台
BOOL CNetworkInteraction::SendJSonPacket()
{
	BOOL bRet = FALSE;


	return bRet;
}

//接收平台数据包
BOOL CNetworkInteraction::RecvJSonPacket(CString strJSon)
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析数据头
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
	//成功
	bRet = TRUE;

	spiJsonService->GetStringValue(CComBSTR(_T("ValidDay")),&bstrVal);
	if(bstrVal.Length())
		//strValidDay = bstrVal.m_str;
		bstrVal.Empty();


	spiJsonService = NULL;
	return bRet;
}

//解析平台返回状态
BOOL CNetworkInteraction::ParseJsonRegister()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析卸载命令
BOOL CNetworkInteraction::ParseJsonUnRegister()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析获取系统资源信息
BOOL CNetworkInteraction::ParseJsonSysResource()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析获取系统版本信息
BOOL CNetworkInteraction::ParseJsonSysType()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析扫描磁盘命令
BOOL CNetworkInteraction::ParseJsonScanDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析删除单个进程白名单命令
BOOL CNetworkInteraction::ParseJsonDelProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析添加单个进程白名单命令
BOOL CNetworkInteraction::ParseJsonAddProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析上传白名单返回状态命令
BOOL CNetworkInteraction::ParseJsonSendWhiteList()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析安装程序命令
BOOL CNetworkInteraction::ParseJsonInstallProgram()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析上传U盘信息返回状态命令
BOOL CNetworkInteraction::ParseJsonUDiskDevice()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析添加安全U盘命令
BOOL CNetworkInteraction::ParseJsonAddUDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析开启自身防护命令
BOOL CNetworkInteraction::ParseJsonTurnOnSelf()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析设置拦截功能命令
BOOL CNetworkInteraction::ParseJsonIntercept()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析设置安全等级命令
BOOL CNetworkInteraction::ParseJsonSafeLevel()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析U盘控制设置命令
BOOL CNetworkInteraction::ParseJsonUDiskSet()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析双因子登录设置命令
BOOL CNetworkInteraction::ParseJsonDLogin()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析授权文件命令
BOOL CNetworkInteraction::ParseJsonAuthFile()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析更新授权命令
BOOL CNetworkInteraction::ParseJsonUpdateAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析移除授权命令
BOOL CNetworkInteraction::ParseJsonRemoveAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析开启进程白名单防护命令
BOOL CNetworkInteraction::ParseJsonTurnOnServer()
{
	BOOL bRet = FALSE;


	return bRet;
}

//解析获取当前配置信息命令
BOOL CNetworkInteraction::ParseJsonGetConfig()
{
	BOOL bRet = FALSE;


	return bRet;
}
//解析上传用户信息返回状态命令
BOOL CNetworkInteraction::ParseJsonUserName()
{
	BOOL bRet = FALSE;


	return bRet;
}


//打包数据头信息
BOOL CNetworkInteraction::ConvertStringToJSon(CString strSource, CString& strJSon)
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包注册到平台的数据包
BOOL CNetworkInteraction::PacketJSonRegister1()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包注册成功信息
BOOL CNetworkInteraction::PacketJsonRegister2()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包卸载返回状态
BOOL CNetworkInteraction::PacketJsonUnRegister()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包系统资源信息
BOOL CNetworkInteraction::PacketJsonSysResource()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包系统版本信息
BOOL CNetworkInteraction::PacketJsonSysType()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包接收扫描磁盘命令的状态信息
BOOL CNetworkInteraction::PacketJsonScanDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包删除单个进程白名单执行状态信息
BOOL CNetworkInteraction::PacketJsonDelProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包添加单个进程白名单命令
BOOL CNetworkInteraction::PacketJsonAddProcess()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包上传白名单返回状态命令
BOOL CNetworkInteraction::PacketJsonSendWhiteList()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包安装程序命令
BOOL CNetworkInteraction::PacketJsonInstallProgram()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包上传U盘信息返回状态命令
BOOL CNetworkInteraction::PacketJsonUDiskDevice()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包添加安全U盘命令
BOOL CNetworkInteraction::PacketJsonAddUDisk()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包开启自身防护命令
BOOL CNetworkInteraction::PacketJsonTurnOnSelf()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包设置拦截功能命令
BOOL CNetworkInteraction::PacketJsonIntercept()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包设置安全等级命令
BOOL CNetworkInteraction::PacketJsonSafeLevel()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包U盘控制设置命令
BOOL CNetworkInteraction::PacketJsonUDiskSet()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包双因子登录设置命令
BOOL CNetworkInteraction::PacketJsonDLogin()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包授权文件命令
BOOL CNetworkInteraction::PacketJsonAuthFile()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包更新授权命令
BOOL CNetworkInteraction::PacketJsonUpdateAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包移除授权命令
BOOL CNetworkInteraction::PacketJsonRemoveAuth()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包开启进程白名单防护命令
BOOL CNetworkInteraction::PacketJsonTurnOnServer()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包获取当前配置信息命令
BOOL CNetworkInteraction::PacketJsonGetConfig()
{
	BOOL bRet = FALSE;


	return bRet;
}

//打包上传用户信息返回状态命令
BOOL CNetworkInteraction::PacketJsonUserName()	
{
	BOOL bRet = FALSE;


	return bRet;
}


//处理网络心跳包
BOOL CNetworkInteraction::PacketJsonHeartBeat()				//打包心跳包
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
		/// 写入配置
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
