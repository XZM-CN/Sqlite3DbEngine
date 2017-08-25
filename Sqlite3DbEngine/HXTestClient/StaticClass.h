#pragma once

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

extern CRITICAL_SECTION csHeartBeat;
extern CRITICAL_SECTION csUpLoad;

class CStaticClass
{
public:
	CStaticClass(void);
	~CStaticClass(void);

	CComPtr <IJsonService> m_spiJsonService;

	static char* CStaticClass::rand_str(char *str,const int len);


	static UINT UpLoadSysLog(LPVOID pParam);

	// ע��
	static UINT ApplyRegister(LPVOID pParam);
	static UINT ApplyRegisterA(LPVOID pParam){
		int i = 0;
		while (1) {
			printf("%d\n",i++);
			if (i == 50) {
				break;
			}
			Sleep(500);
		}
		return 0;
	}
	
	// ����
	static UINT HeartBeat(LPVOID pParam);

	// ��ȡ�ӿ�
	static BOOL GetInterfacePtr(CComPtr <IJsonService>& spiJsonService);

	/**
	 * @brief             SJD_register
	 *
	 * @Function          ƴ��Json����StitchingJsonData
	 *
	 * @param[  _In_   ]  strClientID     �ͻ��˵��豸ID��ÿ���ͻ��˰�װ�����ɵ�Ψһ��ʶ��eg.C5A0847E756B140DFE9656CAF754EB4A��
	 *
	 * @param[  _In_   ]  CmdID           ����ID(����һ����ֵ)
	 *
	 * @param[  _In_   ]  spiJsonService2 �Ӵ��ӿ�ָ��
	 *
	 * @param[  _Out_  ]  strJson         ������ɵõ�������Json�ַ���
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	static void SJD_register(CString& strJson,CString& strClientID,int CmdID,CComPtr <IJsonService>& spiJsonService2);
	static CComPtr <IJsonService> SJD_Content3();
	static CComPtr <IJsonService> SJD_Content5();
	static CComPtr <IJsonService> SJD_Content7();

	// ��ȡ������Ϣ
	static void Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,CString& strJson);
	static void Get_CmdContext2Level(const char* bstrContent,BSTR bstrParentKeyName,BSTR bstrKeyName,CString& strJson);
	static void Get_CmdContext(const char* bstrContent,BSTR bstrKeyName,LONGLONG& intJson);
	static void Get_CmdContext2Level(const char* bstrContent,BSTR bstrParentKeyName,BSTR bstrKeyName,LONGLONG& intJson);

	//
	static void OutputAReceiveMsg(const char *JsonBuff);
	static BOOL  m_gbSendHeartBeat;
};

typedef CStaticClass* PCStaticClass;

class CMainDlg;

typedef struct tagThreadParam 
{
	int           TP_ThreadCount;
	CMainDlg*     TP_pCMainDlg;
	CStaticClass* TP_pCStaticClass;
	int           TP_RadioID ;
	tagThreadParam::tagThreadParam(){
		TP_ThreadCount      = 0;
		TP_pCMainDlg     = NULL;
		TP_pCStaticClass = NULL;
		TP_RadioID   = 0;
	}
}ThreadParam ,*PThreadParam ;