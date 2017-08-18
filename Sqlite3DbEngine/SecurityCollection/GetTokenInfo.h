//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/06/07  
// ><创建时间 ><  2017年:06月:07日   16时:13分:30秒  
// ><文件     ><  GetTokenInfo.h  
// ><文件路径 ><  C:\迅雷下载\MySecurityTest\MySecurityTest  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  TDHX_工程笔记.docx
// ><         ><  3.  第06章 - 访问控制 https://wenku.baidu.com/view/a2733342336c1eb91a375d04.html
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  浅析Windows安全相关的一些概念
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
// ><修改日期 ><  2017年:06月:07日   16时:13分:30秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

/*      
 * PTOKEN_USER
 * PTOKEN_GROUPS
 * PTOKEN_PRIVILEGES
 * PTOKEN_MANDATORY_LABEL
 * PTOKEN_STATISTICS
*/
template <typename P>
class LocalMemory
{
public:
	LocalMemory() :
		m_p(0)
	{
	}

	~LocalMemory()
	{
		Free();
	}

	HRESULT Allocate(size_t bytes)
	{
		Free();

		/**
		 * @brief             LocalAlloc(Allocates the specified number of bytes from the heap.)
		 *
		 * @Function          这个函数从堆中分配指定大小的字节数
		 *
		 * @param[  _In_   ]  UINT   uFlags 内存分配属性。默认值为LMEM_FIXED值。该参数可以是以下值中的一个或多个，除了特别注明的不兼容的组合
		 *
		 * @param[  _In_   ]  SIZE_T uBytes 要分配的字节数。如果此参数为零并且uFlags参数指定LMEM_MOVEABLE，该函数将返回一个被标记为已被丢弃的内存对象的句柄
		 *
		 * @return            
		 *
		 * @Date 2017/06/30  13:40:11
		*/
		m_p = static_cast<P>(::LocalAlloc(LPTR, bytes));

		/**
		 * @brief             HRESULT_FROM_WIN32(Maps a system error code to an HRESULT value.)
		 *
		 * @Function          将系统错误代码映射到HRESULT值
		 *
		 * @param  DWORD x  The system error code.
		 *
		 * @return The HRESULT value
		 *
		 * @Date 2017/06/30  13:40:11
		*/
		return 0 != m_p ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
	}

	void Attach(P p)
	{
		Free();
		m_p = p;
	}

	P Detach()
	{
		P result = m_p;
		m_p = 0;
		return result;
	}

	void Free()
	{
		if (0 != m_p)
		{
			::LocalFree(m_p);
			m_p = 0;
		}
	}

	P operator->() const
	{
		return m_p;
	}

	operator P()
	{
		return m_p;
	}

private:
	LocalMemory(const LocalMemory&);
	LocalMemory& operator=(const LocalMemory&);

private:
	P m_p;
};

/**
 * @brief    GetTokenInformation()
 *
 * @Function
 *
 * @param    HANDLE token
 *
 * @param    TOKEN_INFORMATION_CLASS infoClass
 *
 * @param    LocalMemory<InfoType*>& info
 *
 * @return   HRESULT
 *
 * @Date 2017/06/30  11:59:43
 */
template <typename InfoType>
HRESULT GetTokenInformation(HANDLE token, TOKEN_INFORMATION_CLASS infoClass, LocalMemory<InfoType*>& info)
{
	DWORD size = 0;

	HRESULT result = S_OK;

	/**
	 * @brief    GetTokenInformation(The GetTokenInformation function retrieves a specified type of information about an access token. The calling process must have appropriate access rights to obtain the information.)
	 *
	 * @Function GetTokenInformation函数检索指定类型的关于信息访问令牌。调用进程必须具有获取信息的适当访问权限
	 *
	 * @param    HANDLE                  TokenHandle (检索信息的访问令牌的句柄。如果TokenInformationClass指定TokenSource，则该句柄必须具有TOKEN_QUERY_SOURCE访问权限。对于所有其他TokenInformationClass值，句柄必须具有TOKEN_QUERY权限。)
	 *
	 * @param    TOKEN_INFORMATION_CLASS TokenInformationClass (指定来自TOKEN_INFORMATION_CLASS枚举类型的值， 以识别函数检索的信息类型。检查TokenIsAppContainer并返回0的任何呼叫者也应该验证呼叫者令牌不是识别级别的模拟令牌。如果当前令牌不是应用程序容器，而是身份级别令牌，则应返回AccessDenied。)
	 *
	 * @param    LPVOID                  TokenInformation (指向缓冲区的指针，该函数填充所请求的信息。放入此缓冲区的结构取决于由TokenInformationClass参数指定的信息类型。)
	 *
	 * @param    DWORD                   TokenInformationLength (指定由TokenInformation参数指向的缓冲区的大小（以字节为单位）。如果TokenInformation为NULL，则此参数必须为零)
	 *
	 * @param    PDWORD                  ReturnLength (指向一个变量的指针，该变量接收TokenInformation参数指向的缓冲区所需的字节数。如果该值大于TokenInformationLength参数中指定的值，则该函数将失败，并且不会在缓冲区中存储任何数据。)
	 *
	 * @return   如果函数成功，返回值不为零。如果函数失败，返回值为零。要获取扩展错误信息，请调用 GetLastError。
	 *
	 * @Date 2017/06/30  12:00:41
	 */
	// 取得存储特权信息空间的大小size
	if (!::GetTokenInformation(token, infoClass, 0, 0, &size))
	{
		const DWORD lastError = ::GetLastError();

		if (ERROR_INSUFFICIENT_BUFFER != lastError)
		{
			result = HRESULT_FROM_WIN32(lastError);
		}
	}

	if (SUCCEEDED(result))
	{
		LocalMemory<InfoType*> localMemory;
		result = localMemory.Allocate(size);

		if (SUCCEEDED(result))
		{
			if (!::GetTokenInformation(token, infoClass, localMemory, size, &size))
			{
				result = HRESULT_FROM_WIN32(::GetLastError());
			}
			else
			{
				info.Attach(localMemory.Detach());
			}
		}
	}

	return result;
}