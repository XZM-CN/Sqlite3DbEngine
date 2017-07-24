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
// 	unsigned long Sys_EventTime;		//ʱ��
// 	unsigned long Sys_EventID;			//�¼�ID
// 	unsigned long Sys_EventType;		//��־���ͣ�0-�ɹ���1-����2-���桢3-��Ϣ
// 	unsigned long Sys_EventClass;		//��־���ࣺ0-��ȫ��1-ϵͳ��2-Ӧ�ó���3-����
// 	ATL::CString Sys_EventSource;		//�¼���Դ
// 	ATL::CString Sys_ComputerName;		//�������
// 	ATL::CString Sys_ComputerUser;		//�û���
// 	ATL::CString Sys_TaskCategory;		//�������
// 	ATL::CString Sys_EventDesc;			//����
// }SYSEVENTINFO,*PSYSEVENTINFO;


inline DWORD GetEventClass(ATL::CString EventName);
BOOL GetUserNameForSID (TCHAR* StringSid, TCHAR* strUserName, long nUserNameLen);
