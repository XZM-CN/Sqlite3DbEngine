
#include "stdafx.h"
#include <time.h>
#include "ParseEventLogXML.h "

//#pragma comment(lib, "Advapi32.lib")

CParseEventLogXML::CParseEventLogXML(void)
{
	CoInitialize(NULL);
	m_XMLDoc.CoCreateInstance(CLSID_DOMDocument);
}

CParseEventLogXML::~CParseEventLogXML(void)
{
	//CoUninitialize();
}

DWORD CParseEventLogXML::LoadFromString(LPCTSTR lpszXMLBuffer)
{
	DWORD dwError = 0;

	CString csErrMsg;

	VARIANT_BOOL vbSuccess = VARIANT_FALSE;

	if ((S_OK != m_XMLDoc->loadXML(_bstr_t(lpszXMLBuffer), &vbSuccess)) || (vbSuccess == VARIANT_FALSE))
	{
		IXMLDOMParseErrorPtr parseError;
		long lError = 0;
		_bstr_t errormsg;

		m_XMLDoc->get_parseError(&parseError);
		parseError->get_errorCode(&lError);
		if (lError != 0)
		{
			dwError = 1;
			parseError->get_reason(errormsg.GetAddress());
			csErrMsg.Format(_T("%ws"), (LPCTSTR)errormsg);
			m_csErrMsg = csErrMsg;
		}
	}


	return dwError;
}

BOOL CParseEventLogXML::ParseSetupEventLog(SYSEVENTINFO& stEvnetLog)
{
	BOOL bRet = FALSE;

	CComQIPtr<NodeType> xmlNode;

	if (SUCCEEDED(m_XMLDoc->selectSingleNode(_bstr_t(_T("//System")), &xmlNode)))
	{
		bRet = TRUE;

		BEGIN_TRAVEL_CHILDREN(xmlNode, childNode, childNodeName);

		if (!_tcsicmp(childNodeName, _T("Provider")))
		{
			//解析来源
			Parse_Source(childNode, stEvnetLog);
		}
		else if (!_tcsicmp(childNodeName, _T("EventID")))
		{
			//解析事件ID
			Parse_EventID(childNode, stEvnetLog);
		}
		else if (!_tcsicmp(childNodeName, _T("Level")))
		{
			//解析事件ID
			Parse_Level(childNode, stEvnetLog);
		}
		else if (!_tcsicmp(childNodeName, _T("Task")))
		{
			//解析事件任务
			Parse_EventTask(childNode, stEvnetLog);
		}
		else if (!_tcsicmp(childNodeName, _T("TimeCreated")))
		{
			//解析时间
			Parse_EventTime(childNode, stEvnetLog);
		}
		else if(!_tcsicmp(childNodeName, _T("Channel")))
		{
			//解析日志类型
			Parse_EventClass(childNode, stEvnetLog);
		}
		else if(!_tcsicmp(childNodeName, _T("Computer")))
		{
			//解析计算机名
			Parse_EventComputer(childNode, stEvnetLog);
		}
		else if(!_tcsicmp(childNodeName, _T("Security")))
		{
			//解析日志类型
			Parse_EventUser(childNode, stEvnetLog);
		}

		END_TRAVEL_CHILDREN;

	}

	CComQIPtr<NodeType> xmlNode1;
	if (SUCCEEDED(m_XMLDoc->selectSingleNode(_bstr_t(_T("//UserData")), &xmlNode1)))
	{
		BEGIN_TRAVEL_CHILDREN(xmlNode1, childNode, childNodeName);

		if (!_tcsicmp(childNodeName, _T("CbsPackageChangeState")))
		{
			//解析描述
			Parse_Discription(childNode, stEvnetLog);
		}
		else if(!_tcsicmp(childNodeName, _T("CbsUpdateChangeState")))
		{
			//解析描述
			Parse_Discription(childNode, stEvnetLog);
		}
		else if(!_tcsicmp(childNodeName, _T("EventXML")))
		{
			//解析描述
			Parse_DiscriptionSP(childNode, stEvnetLog);
		}

		END_TRAVEL_CHILDREN;

	}

	return bRet;
}


DWORD CParseEventLogXML::Parse_Source(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	BEGIN_TRAVEL_ATTRIBUTES(childNode, attrName, attrValue);


	if (!_tcsicmp(attrName, _T("Name")))
	{

		CString strTempSource = (LPCTSTR)_bstr_t(attrValue);

		//如果存在，去除Microsoft-Windows-字符串

		RemoveMSFlagFrom(strTempSource);

		stEvnetLog.Sys_EventSource = strTempSource;

	}

	END_TRAVEL_ATTRIBUTES;

	return dwRet;
}

DWORD CParseEventLogXML::Parse_EventID(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	_bstr_t _value;

	childNode->get_text(_value.GetAddress());

	stEvnetLog.Sys_EventID = _tcstoul((LPCTSTR) _value,NULL,10);

	return dwRet;
}

DWORD CParseEventLogXML::Parse_Level(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	_bstr_t _value;

	childNode->get_text(_value.GetAddress());

	DWORD EventType = _tcstoul((LPCTSTR) _value,NULL,10);
	stEvnetLog.Sys_EventType = ConvertXMLClassToEvent(EventType);

	return dwRet;
}

DWORD CParseEventLogXML::Parse_EventTask(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	_bstr_t _value;

	childNode->get_text(_value.GetAddress());

	stEvnetLog.Sys_TaskCategory = (LPCTSTR) _value;

	return dwRet;
}

DWORD CParseEventLogXML::Parse_EventTime(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	BEGIN_TRAVEL_ATTRIBUTES(childNode, attrName, attrValue);

	if (!_tcsicmp(attrName, _T("SystemTime")))
	{
		CString strTempTime = (LPCTSTR)_bstr_t(attrValue);

		//获取时间字符串，转换为本地时间，再转换为1970年1月1日后的秒数

		stEvnetLog.Sys_EventTime = ConvertTimeToSecond(strTempTime);

	}

	END_TRAVEL_ATTRIBUTES;


	return dwRet;
}

DWORD CParseEventLogXML::Parse_EventClass(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	_bstr_t _value;

	childNode->get_text(_value.GetAddress());
	CString strClass = (LPCTSTR)_value;

	stEvnetLog.Sys_EventClass = GetEventClass(strClass);


	return dwRet;
}

DWORD CParseEventLogXML::Parse_EventComputer(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	_bstr_t _value;

	childNode->get_text(_value.GetAddress());

	stEvnetLog.Sys_ComputerName = (LPCTSTR) _value;

	return dwRet;
}

DWORD CParseEventLogXML::Parse_EventUser(NodeType* childNode,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;


	BEGIN_TRAVEL_ATTRIBUTES(childNode, attrName, attrValue);

	if (!_tcsicmp(attrName, _T("UserID")))
	{
		CString strUser = (LPCTSTR)_bstr_t(attrValue);

		if (strUser.IsEmpty())
		{
			stEvnetLog.Sys_ComputerUser = _T("N/A");
		}
		else
		{
			TCHAR pchUserName[MAX_PATH] = {0};

			if(GetUserNameForSID ((TCHAR*)strUser.GetString(), pchUserName, MAX_PATH))
			{
				stEvnetLog.Sys_ComputerUser = pchUserName;
			}

		}

	}

	END_TRAVEL_ATTRIBUTES;

	return dwRet;
}

DWORD CParseEventLogXML::Parse_Discription(NodeType* node,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;
	CString strPackageId, strUpdateName, strInitState, strIntendeSate, strClient;
	DWORD dwErrorCode = 0;

	BEGIN_TRAVEL_CHILDREN(node, childNode, childNodeName);

	_bstr_t _value;
	if (!_tcsicmp(childNodeName, _T("PackageIdentifier")))
	{
		//解析描述
		childNode->get_text(_value.GetAddress());

		strPackageId = (LPCTSTR) _value;

	}
	if (!_tcsicmp(childNodeName, _T("UpdateName")))
	{
		//解析描述

		childNode->get_text(_value.GetAddress());

		strUpdateName = (LPCTSTR) _value;

	}
	if (!_tcsicmp(childNodeName, _T("InitialPackageState")))
	{
		//解析描述
		childNode->get_text(_value.GetAddress());

		strInitState = (LPCTSTR) _value;

	}
	if (!_tcsicmp(childNodeName, _T("IntendedPackageState")))
	{
		//解析描述
		childNode->get_text(_value.GetAddress());

		strIntendeSate = (LPCTSTR) _value;

	}
	if (!_tcsicmp(childNodeName, _T("ErrorCode")))
	{
		//解析描述
		childNode->get_text(_value.GetAddress());

		dwErrorCode = _tcstoul((LPCTSTR) _value,NULL,10);

	}
	if (!_tcsicmp(childNodeName, _T("Client")))
	{
		//解析描述
		childNode->get_text(_value.GetAddress());

		strClient = (LPCTSTR) _value;

	}

	END_TRAVEL_CHILDREN;

	stEvnetLog.Sys_EventDesc = strPackageId + _T(" ") + strUpdateName + _T(" ")
		+ strInitState + _T(" ") + strIntendeSate + _T(" ") + strClient;

	return dwRet;
}

DWORD CParseEventLogXML::Parse_DiscriptionSP(NodeType* node,SYSEVENTINFO& stEvnetLog)
{
	DWORD dwRet = 0;

	return dwRet;
}


DWORD CParseEventLogXML::ConvertXMLClassToEvent(DWORD dwLevel)
{
	DWORD dwRet = 0;

	switch(dwLevel)
	{
	case EVENT_XML_INFO:
		dwRet = EVENTLOG_INFORMATION_TYPE;
		break;
	case EVENT_XML_KEY:
		dwRet = 0x0020;
		break;
	case EVENT_XML_ERROR:
		dwRet = EVENTLOG_ERROR_TYPE;
		break;
	case EVENT_XML_WARNNING:
		dwRet = EVENTLOG_WARNING_TYPE;
		break;
	default:
		break;
	}
	return dwRet;
}

BOOL CParseEventLogXML::RemoveMSFlagFrom(CString& strSource)
{
	BOOL bRet = FALSE;

	if (!strSource.IsEmpty())
	{
		int nLen = strSource.Find(_T("Microsoft-Windows-"));
		if (nLen != -1)
		{
			bRet = TRUE;

			CString strleft = strSource.Left(nLen);
			nLen = strSource.GetLength() - nLen - 18;
			CString strRight = strSource.Right(nLen);

			strSource.Empty();
			strSource = strleft + strRight;
		}
	}


	return bRet;
}

DWORD CParseEventLogXML::ConvertTimeToSecond(CString strTime)
{
	DWORD dwRet = 0;
	DWORD dwYear = 0,dwMonth = 0,dwDay = 0,dwHour = 0,dwMinute = 0,dwSecond = 0;
	CString strDate,strHourMinSec;
	CString strTemp,strTempTime;

	if (strTime.IsEmpty())
	{
		return dwRet;
	}
	strTempTime = strTime;

	//转换字符串中小写到大写

	strTime.MakeUpper();
	int nLen = strTime.Find('T');
	if (nLen != -1)
	{
		strDate = strTime.Left(nLen);
		nLen = strTime.GetLength() - nLen -1;
		strHourMinSec = strTime.Right(nLen);

		//处理日期
		int nLength = strDate.Find('-');
		if (nLength != -1)
		{
			strTemp = strDate.Left(nLength);
			dwYear = _tcstoul(strTemp,NULL,10);
			strTemp.Empty();

			int ntemp = strDate.GetLength() - nLength -1;
			strDate = strDate.Right(ntemp);

			int nMonth = strDate.Find('-');
			if (nMonth != -1)
			{

				strTemp = strDate.Left(nMonth);
				dwMonth = _tcstoul(strTemp,NULL,10);
				strTemp.Empty();

				ntemp = strDate.GetLength() - nMonth - 1;
				strTemp = strDate.Right(ntemp);
				dwDay = _tcstoul(strTemp,NULL,10);
				strTemp.Empty();
			}

		}

		//处理时、分、秒
		int nHMS = strHourMinSec.Find('.');
		if (nHMS != -1)
		{
			strHourMinSec = strHourMinSec.Left(nHMS);

			int nHour = strHourMinSec.Find(':');
			if (nHour != -1)
			{
				strTemp = strHourMinSec.Left(nHour);

				dwHour = _tcstoul(strTemp,NULL,10);
				strTemp.Empty();
				//转换为本地时间
				//dwHour += 8;

				int ntemp = strHourMinSec.GetLength() - nHour -1;
				strHourMinSec = strHourMinSec.Right(ntemp);

				int nMin = strHourMinSec.Find(':');
				if (nMin != -1)
				{
					strTemp = strHourMinSec.Left(nMin);
					dwMinute = _tcstoul(strTemp,NULL,10);
					strTemp.Empty();

					ntemp = strHourMinSec.GetLength() - nMin - 1;
					strTemp = strHourMinSec.Right(ntemp);
					dwSecond = _tcstoul(strTemp,NULL,10);
					strTemp.Empty();

				}
			}


		}

		if (dwYear > 0 && dwMonth > 0 && dwMonth < 13 && dwDay > 0 && dwDay < 32 
			&& dwHour > 0 && dwHour < 24 && dwMinute > 0 && dwSecond >0)
		{
			struct tm time = {0};
			time.tm_year = dwYear - 1900;
			time.tm_mon = dwMonth - 1;
			time.tm_mday = dwDay;
			time.tm_hour = dwHour;
			time.tm_min = dwMinute;
			time.tm_sec = dwSecond;

			/*time.tm_isdst = -1;*/


			dwRet = (DWORD)mktime(&time);

			TIME_ZONE_INFORMATION   tzi;  
			GetTimeZoneInformation(&tzi);  
			int zone = - tzi.Bias/60; /// 时区，如果是中国标准时间则得到8
			int nZoneSecond = zone*60*60;
			dwRet += nZoneSecond;

		}
	}

	return dwRet;
}
