#pragma once
class CUploadLogMgt
{
public:
	CUploadLogMgt(void);
	~CUploadLogMgt(void);

	static UINT SendSyslogThread(LPVOID pParam);
public:
	//上传线程退出标志
	BOOL m_bSysLogFlag;
};

