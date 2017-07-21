#pragma once

#include "HXBaseDef.h"

//////////////////////  错误码定义  ///////////////////////////////
#define ESOCIAL_ERROR_BASECODE			70000
/////////////////  底层网络操作错误码定义  ////////////////////////
#define ERROR_HTTP_OPEN					(ESOCIAL_ERROR_BASECODE + 1)
#define ERROR_HTTP_CLOSE				(ESOCIAL_ERROR_BASECODE + 2)
#define ERROR_HTTP_DIAL					(ESOCIAL_ERROR_BASECODE + 3)
#define ERROR_HTTP_CONNECT				(ESOCIAL_ERROR_BASECODE + 4)
#define ERROR_HTTP_OPENREQUEST			(ESOCIAL_ERROR_BASECODE + 5)
#define ERROR_HTTP_SENDREQUEST			(ESOCIAL_ERROR_BASECODE + 6)
#define ERROR_HTTP_QUERYINFO			(ESOCIAL_ERROR_BASECODE + 7)
//状态错误，不是200或206，而是404或其他
#define ERROR_HTTP_STATUSCODE			(ESOCIAL_ERROR_BASECODE + 8)  
#define ERROR_HTTP_INTERNETREADFILE		(ESOCIAL_ERROR_BASECODE + 9)
#define ERROR_HTTP_OPENFILE				(ESOCIAL_ERROR_BASECODE + 10)
#define ERROR_HTTP_GETFILESIZE			(ESOCIAL_ERROR_BASECODE + 11)
//定位文件指针错误
#define ERROR_HTTP_SEEKFILE				(ESOCIAL_ERROR_BASECODE + 12)	
#define ERROR_HTTP_WRITEFILE			(ESOCIAL_ERROR_BASECODE + 13)
/// 被拦截错误
#define ERROR_HTTP_TRYDOWNLOAD			(ESOCIAL_ERROR_BASECODE + 14)
////////////////////////////////////////////////////////////////////

//////////////////////  错误处理基类  //////////////////////////
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
	/// 错误码
	long	m_lErrCode;		
	/// 错误信息
	CString	m_strMessage;
};
