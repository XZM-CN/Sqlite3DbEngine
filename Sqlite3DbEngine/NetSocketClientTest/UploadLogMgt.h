#pragma once
class CUploadLogMgt
{
public:
	CUploadLogMgt(void);
	~CUploadLogMgt(void);

	static UINT SendSyslogThread(LPVOID pParam);
public:
	//�ϴ��߳��˳���־
	BOOL m_bSysLogFlag;
};

