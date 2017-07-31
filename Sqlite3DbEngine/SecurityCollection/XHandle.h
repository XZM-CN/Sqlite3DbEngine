//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/06/07  
// ><����ʱ�� ><  2017��:06��:07��   16ʱ:15��:31��  
// ><�ļ�     ><  Handle.h  
// ><�ļ�·�� ><  C:\Ѹ������\MySecurityTest\MySecurityTest  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  https://msdn.microsoft.com/zh-cn/library/f2c0w594(VS.80).aspx
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  
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
// ><�޸����� ><  2017��:06��:07��   16ʱ:15��:31��  
// ><ԭ��     ><    
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