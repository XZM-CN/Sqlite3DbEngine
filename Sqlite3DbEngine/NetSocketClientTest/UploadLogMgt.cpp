#include "StdAfx.h"
#include "UploadLogMgt.h"
#include "XzmSyslog.h"

CUploadLogMgt::CUploadLogMgt(void)
{
	m_bSysLogFlag = 0;
}


CUploadLogMgt::~CUploadLogMgt(void)
{
}

UINT CUploadLogMgt::SendSyslogThread(LPVOID pParam)
{
	CUploadLogMgt* pThis = (CUploadLogMgt *)pParam;


	if(NULL == pThis)
		return 0;

	//�����ϴ���־����
	CSysLog XzmSysLog;
	//����ͨ�Ż���
	XzmSysLog.CreateUDPCommunication();

	while(pThis->m_bSysLogFlag)
	{
		//�����ϴ�����
		OutputDebugString(_T("��ʼ�����ϴ�syslog��־"));
		XzmSysLog.GetLogDataFromDB();

		Sleep(2000);
	}

	//���������˳�ʱ���˳���ǰ�߳�

	return 0;
}
