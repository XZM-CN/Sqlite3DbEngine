//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/06/07  
// ><创建时间 ><  2017年:06月:07日   16时:15分:31秒  
// ><文件     ><  Handle.h  
// ><文件路径 ><  C:\迅雷下载\MySecurityTest\MySecurityTest  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  https://msdn.microsoft.com/zh-cn/library/f2c0w594(VS.80).aspx
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  
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
// ><修改日期 ><  2017年:06月:07日   16时:15分:31秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class CXHandle
{
public:
	CXHandle(HANDLE h = NULL)
		:m_handle(h)
	{

	}

	operator HANDLE()
	{
		return m_handle;
	}

	HANDLE* ref()
	{
		return &m_handle;
	}

	~CXHandle()
	{
		if(m_handle != NULL)
		{
			CloseHandle(m_handle);
		}
	}

	BOOL IsNull() const 
	{
		return m_handle == NULL;
	}

private:
	CXHandle(const CXHandle& h);
	CXHandle& operator = (const CXHandle& h);

private:
	HANDLE m_handle;
};