//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/08/21  
// ><����ʱ�� ><  2017��:08��:21��   15ʱ:59��:34��  
// ><�ļ�     ><  MsXmlScheduler.h  
// ><�ļ�·�� ><  g:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\SchedulerCenter  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  ��������--MsXml������
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:08��:21��   15ʱ:59��:34��  
// ><ԭ��     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "schedulerbase.h"


class CMsXmlScheduler :
	public CSchedulerBase
{
public:
	CMsXmlScheduler(void);
	CMsXmlScheduler(CString strModulerName);
	virtual ~CMsXmlScheduler(void);
	BOOL GetMsXmlWrapperPtr(void ** ppv );

private:
	CComPtr<IXMsXmlWrapper>  m_MsXmlWrapper;
};

