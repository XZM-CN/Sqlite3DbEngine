//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/07/27  
// ><����ʱ�� ><  2017��:07��:27��   10ʱ:35��:12��  
// ><�ļ�     ><  NetworkInteraction.h  
// ><�ļ�·�� ><  G:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\Common  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  ����ע����Ϣ�ͻ���(������)
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:07��:27��   10ʱ:35��:12��  
// ><ԭ��     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _HXPARSEJSON_H
#define _HXPARSEJSON_H

#include "NetSocket.h"

#define BLANKSPACE	L" "
#define LEFTBRACE	L"{"
#define RIGHTBRACE	L"}"
#define JSONCOLON	L":"
#define LEFTBRACKET		L"["
#define RIGHTBRACKET	L"]"

#define TCPPORT	514

#define TCPCONFIGFILENAME	L"NetworkInteractionConfig.ini"
#define TCPSECTION			L"[NetworkInteraction Section]"
#define TCPSERVERMAJORIP			L"ServerMajorIP"
#define TCPSERVERMINORIP			L"ServerMinorIP"
#define TCPSERVERPORT				L"ServerPort"


typedef enum _tagCommandType
{
	enumRegistar1 = 1,
	enumRegistar2,
	enumRegistar3,
	enumUnregistar,
	enumHeartbeat,
	enumGetSysResource,
	enumGetSysType,
	enumScanDisk = 101,
	enumDelProcess,
	enumAddProcess,
	enumSendWhiteList,
	enumInstallProgram,
	enumUDiskDevice = 201,
	enumAddUDisk,
	enumTurnOnSelf = 301,
	enumIntercept,
	enumSafeLevel,
	enumUDiskSet,
	enumDLogin,
	enumAuthFile,
	enumAuthName,
	enumRemoveAuth,
	enumTurnOnServer,
	enumGetConfigStatus,
	enumUserName = 401
}COMMANDTYPE;

typedef struct _tagSysResource
{
	DWORD dwCPUUsageRate;
	DWORD dwMemoryUsageRate;
	DWORD dwMemorySize;
	DWORD dwDiskUsageRate;
	DWORD dwDiskSize;
}SYSRESOURCE;

typedef struct _tagSystemType
{
	CString strComputeName;
	CString strCurrentUserName;
	CString strSysVersion;
	CString strSysBit;
	CString strHostIP;
}SYSTEMTYPE;

typedef struct _tagClientCfgStatus
{
	DWORD	dwTurnOnSelf;		//������ر����������1������0�ر�
	DWORD	dwIntercept;		//������رս������أ�1������0�ر�
	DWORD	dwSafeLevel;		//���ð�ȫ�ȼ���1����ͼ���2�����м���3����߼�
	DWORD	dwSetUDisk;			//����U�̿��Ƽ���1�����д��2����ֻ����3�������
	DWORD	dwDLogin;			//������ر�˫���ӵ�¼��1��������0����ر�
	DWORD	dwTurnOnServer;		//������رս��̷���
}CLIENTCFGSTATUS;


class CNetworkInteraction
{
public:
	CNetworkInteraction();
	~CNetworkInteraction();


	//����ͨ��
	BOOL CreateConnect();					//����ͨ�Žӿ�
	BOOL SendJSonPacket();					//�������ݰ���ƽ̨
	BOOL RecvJSonPacket(CString strJSon);	//����ƽ̨���ݰ�

	// ��ȡ������Ϣ
	/**
	 * @brief             GetConfInfo
	 *
	 * @Function          ��ȡ�����ļ�·������ȡ�����������ļ�����ַ���
	 *
	 * @return            ��ȡ�������ļ�·������TRUE�����򷵻�FALSE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL GetConfInfo();
	/**
	 * @brief             ParseSyslogConfigFile
	 *
	 * @Function          ���������ļ�����ȡServerMajorIP��m_ServerMajorIP
	 *                                  ��ȡServerMinorIP��m_ServerMinorIP
	 *                                  ��ȡServerPort   ��m_ServerPort
	 *
	 * @param[  _In_   ]  pchSyslog�����ļ��������ı��ַ�
	 *
	 * @return            ��ȡ�������ļ���������Ϣ����TRUE�����򷵻�FALSE
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseSyslogConfigFile(BYTE* pchSyslog);

	//����
	BOOL ParseJsonFromNetwork(CString strCommand);
	BOOL ParseJsonRegister();				//����ƽ̨����״̬
	BOOL ParseJsonUnRegister();				//����ж������
	BOOL ParseJsonSysResource();			//������ȡϵͳ��Դ��Ϣ
	BOOL ParseJsonSysType();				//������ȡϵͳ�汾��Ϣ
	BOOL ParseJsonScanDisk();				//����ɨ���������
	BOOL ParseJsonDelProcess();				//����ɾ���������̰���������
	BOOL ParseJsonAddProcess();				//������ӵ������̰���������
	BOOL ParseJsonSendWhiteList();			//�����ϴ�����������״̬����
	BOOL ParseJsonInstallProgram();			//������װ��������
	BOOL ParseJsonUDiskDevice();			//�����ϴ�U����Ϣ����״̬����
	BOOL ParseJsonAddUDisk();				//������Ӱ�ȫU������
	BOOL ParseJsonTurnOnSelf();				//�������������������
	BOOL ParseJsonIntercept();				//�����������ع�������
	BOOL ParseJsonSafeLevel();				//�������ð�ȫ�ȼ�����
	BOOL ParseJsonUDiskSet();				//����U�̿�����������
	BOOL ParseJsonDLogin();					//����˫���ӵ�¼��������
	BOOL ParseJsonAuthFile();				//������Ȩ�ļ�����
	BOOL ParseJsonUpdateAuth();				//����������Ȩ����
	BOOL ParseJsonRemoveAuth();				//�����Ƴ���Ȩ����
	BOOL ParseJsonTurnOnServer();			//�����������̰�������������
	BOOL ParseJsonGetConfig();				//������ȡ��ǰ������Ϣ����
	BOOL ParseJsonUserName();				//�����ϴ��û���Ϣ����״̬����


	//���
	BOOL ConvertStringToJSon(CString strSource, CString& strJSon);		//�������ͷ��Ϣ
	BOOL PacketJSonRegister1();				//���ע�ᵽƽ̨�����ݰ�
	BOOL PacketJsonRegister2();				//���ע��ɹ���Ϣ
	BOOL PacketJsonUnRegister();			//���ж�ط���״̬
	BOOL PacketJsonSysResource();			//���ϵͳ��Դ��Ϣ
	BOOL PacketJsonSysType();				//���ϵͳ�汾��Ϣ
	BOOL PacketJsonScanDisk();				//�������ɨ����������״̬��Ϣ
	BOOL PacketJsonDelProcess();			//���ɾ���������̰�����ִ��״̬��Ϣ
	BOOL PacketJsonAddProcess();			//�����ӵ������̰���������
	BOOL PacketJsonSendWhiteList();			//����ϴ�����������״̬����
	BOOL PacketJsonInstallProgram();		//�����װ��������
	BOOL PacketJsonUDiskDevice();			//����ϴ�U����Ϣ����״̬����
	BOOL PacketJsonAddUDisk();				//�����Ӱ�ȫU������
	BOOL PacketJsonTurnOnSelf();			//������������������
	BOOL PacketJsonIntercept();				//����������ع�������
	BOOL PacketJsonSafeLevel();				//������ð�ȫ�ȼ�����
	BOOL PacketJsonUDiskSet();				//���U�̿�����������
	BOOL PacketJsonDLogin();				//���˫���ӵ�¼��������
	BOOL PacketJsonAuthFile();				//�����Ȩ�ļ�����
	BOOL PacketJsonUpdateAuth();			//���������Ȩ����
	BOOL PacketJsonRemoveAuth();			//����Ƴ���Ȩ����
	BOOL PacketJsonTurnOnServer();			//����������̰�������������
	BOOL PacketJsonGetConfig();				//�����ȡ��ǰ������Ϣ����
	BOOL PacketJsonUserName();				//����ϴ��û���Ϣ����״̬����

	//��������������
	BOOL PacketJsonHeartBeat();				//���������




private:
	CNetSocket* m_pNetSock;
	SOCKET m_sock;

	char m_ServerMajorIP[64];	// ��IP
	char m_ServerMinorIP[64];	// ����IP
	int  m_ServerPort;			// �˿�

	CString m_strHeartBeat;		//�������ַ���

	CString m_strSendBuffer;	//���ͻ�����
	CString m_strRecvBuffer;	//���ջ�����

	CString	m_strClientID;		//�ͻ����豸ID

	//�ͻ���������Ϣ
	BOOL	m_bRegisterSuccess;	//ע��ɹ�״̬

	SYSRESOURCE m_sysResource;	//ϵͳ��Դ
	SYSTEMTYPE  m_sysType;		//ϵͳ������Ϣ

	CString m_strScanPath;		//ɨ��·��
	DWORD	m_dwAddOrDelProcess;	//��ӻ�ɾ�����̣�1��ӣ�2ɾ��
	CString m_strProcessPath;	//���̾���·��

	CString m_strInstallPath;	//��Ҫ��װ�����·��
	CString m_strUDiskDevice;	//U���豸��
	CString m_strUDiskDrive;	//U���̷�
	DWORD	m_dwAddOrDelUDisk;	//��ӻ�ɾ����ȫU�̣�1��ӣ�0ɾ��

	CLIENTCFGSTATUS m_ConfigStatus;		//��ǰ������Ϣ

	CString m_AuthFile;			//��Ȩ�ļ�����ȫ·��
	CString m_AuthBuffer;		//��Ȩ�ļ�����
	DWORD	m_dwRemoveAuth;		//�Ƴ���Ȩ�ļ���־��1�����Ƴ�
	BOOL	m_bConfigStatus;	//��ǰ����״̬
};








#endif
