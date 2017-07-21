#pragma once

#include "HXBaseDef.h"

//////////////////////  �����붨��  ///////////////////////////////
#define ESOCIAL_ERROR_BASECODE			70000
/////////////////  �ײ�������������붨��  ////////////////////////
#define ERROR_HTTP_OPEN					(ESOCIAL_ERROR_BASECODE + 1)
#define ERROR_HTTP_CLOSE				(ESOCIAL_ERROR_BASECODE + 2)
#define ERROR_HTTP_DIAL					(ESOCIAL_ERROR_BASECODE + 3)
#define ERROR_HTTP_CONNECT				(ESOCIAL_ERROR_BASECODE + 4)
#define ERROR_HTTP_OPENREQUEST			(ESOCIAL_ERROR_BASECODE + 5)
#define ERROR_HTTP_SENDREQUEST			(ESOCIAL_ERROR_BASECODE + 6)
#define ERROR_HTTP_QUERYINFO			(ESOCIAL_ERROR_BASECODE + 7)
//״̬���󣬲���200��206������404������
#define ERROR_HTTP_STATUSCODE			(ESOCIAL_ERROR_BASECODE + 8)  
#define ERROR_HTTP_INTERNETREADFILE		(ESOCIAL_ERROR_BASECODE + 9)
#define ERROR_HTTP_OPENFILE				(ESOCIAL_ERROR_BASECODE + 10)
#define ERROR_HTTP_GETFILESIZE			(ESOCIAL_ERROR_BASECODE + 11)
//��λ�ļ�ָ�����
#define ERROR_HTTP_SEEKFILE				(ESOCIAL_ERROR_BASECODE + 12)	
#define ERROR_HTTP_WRITEFILE			(ESOCIAL_ERROR_BASECODE + 13)
/// �����ش���
#define ERROR_HTTP_TRYDOWNLOAD			(ESOCIAL_ERROR_BASECODE + 14)
////////////////////////////////////////////////////////////////////

//////////////////////  ���������  //////////////////////////
class CErrorBase
{
public:
	CErrorBase()
		:m_lErrCode(0)
		,m_strMessage(_T(""))
	{
	}
	virtual ~CErrorBase()
	{
		m_strMessage.Empty();
	}

	virtual void SetError( long lErrCode, const CString& strMessage )
	{
		m_strMessage.Empty();

		m_lErrCode = lErrCode;
		m_strMessage = strMessage;
	}

	int GetLastError()
	{
		return m_lErrCode;
	}

	LPCTSTR GetLastMessage()
	{
		return m_strMessage;
	}

protected:
	/// ������
	long	m_lErrCode;		
	/// ������Ϣ
	CString	m_strMessage;
};
