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

	//创建上传日志对象
	CSysLog XzmSysLog;
	//创建通信机制
	XzmSysLog.CreateUDPCommunication();

	while(pThis->m_bSysLogFlag)
	{
		//处理上传事务
		OutputDebugString(_T("开始处理上传syslog日志"));
		XzmSysLog.GetLogDataFromDB();

		Sleep(2000);
	}

	//在主界面退出时，退出当前线程

	return 0;
}
