//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/21  
// ><创建时间 ><  2017年:08月:21日   15时:59分:34秒  
// ><文件     ><  MsXmlScheduler.h  
// ><文件路径 ><  g:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\SchedulerCenter  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  调度中心--MsXml调度器
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:08月:21日   15时:59分:34秒  
// ><原因     ><    
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

