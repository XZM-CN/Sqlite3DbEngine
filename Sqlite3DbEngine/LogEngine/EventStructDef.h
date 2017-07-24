#pragma once
#include <comsvcs.h>
using namespace ATL;

#include <Sddl.h>

#define EVENT_XML_INFO		0
#define EVENT_XML_KEY		1
#define EVENT_XML_ERROR		2
#define EVENT_XML_WARNNING	3


// typedef struct tagSystemEventLogInfo
// {
// 	unsigned long Sys_EventTime;		//时间
// 	unsigned long Sys_EventID;			//事件ID
// 	unsigned long Sys_EventType;		//日志类型：0-成功、1-错误、2-警告、3-信息
// 	unsigned long Sys_EventClass;		//日志分类：0-安全、1-系统、2-应用程序、3-设置
// 	ATL::CString Sys_EventSource;		//事件来源
// 	ATL::CString Sys_ComputerName;		//计算机名
// 	ATL::CString Sys_ComputerUser;		//用户名
// 	ATL::CString Sys_TaskCategory;		//任务类别
// 	ATL::CString Sys_EventDesc;			//描述
// }SYSEVENTINFO,*PSYSEVENTINFO;


inline DWORD GetEventClass(ATL::CString EventName);
BOOL GetUserNameForSID (TCHAR* StringSid, TCHAR* strUserName, long nUserNameLen);
