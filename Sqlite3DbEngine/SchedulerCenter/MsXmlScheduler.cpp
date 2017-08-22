#include "StdAfx.h"
#include "MsXmlScheduler.h"


CMsXmlScheduler::CMsXmlScheduler(void)
	: CSchedulerBase()
{
}


CMsXmlScheduler::CMsXmlScheduler(CString strModulerName)
	: CSchedulerBase(strModulerName)
{
}

CMsXmlScheduler::~CMsXmlScheduler(void)
{
}

BOOL CMsXmlScheduler::GetMsXmlWrapperPtr(void ** ppv)
{
	CString strModulePath = GetPtrPath(MODULARPATH_MSXML);


	if(!strModulePath.IsEmpty())
	{
		CreateInstance( strModulePath.GetBuffer(),__uuidof(XMsXmlWrapper),__uuidof(IXMsXmlWrapper),ppv);
		ATLASSERT(ppv);
		return TRUE;
	}

	return FALSE;
}
