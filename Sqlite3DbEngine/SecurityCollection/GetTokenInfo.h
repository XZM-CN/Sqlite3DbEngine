//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/06/07  
// ><����ʱ�� ><  2017��:06��:07��   16ʱ:13��:30��  
// ><�ļ�     ><  GetTokenInfo.h  
// ><�ļ�·�� ><  C:\Ѹ������\MySecurityTest\MySecurityTest  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  TDHX_���̱ʼ�.docx
// ><         ><  3.  ��06�� - ���ʿ��� https://wenku.baidu.com/view/a2733342336c1eb91a375d04.html
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  ǳ��Windows��ȫ��ص�һЩ����
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
// ><�޸����� ><  2017��:06��:07��   16ʱ:13��:30��  
// ><ԭ��     ><    
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
		 * @Function          ��������Ӷ��з���ָ����С���ֽ���
		 *
		 * @param[  _In_   ]  UINT   uFlags �ڴ�������ԡ�Ĭ��ֵΪLMEM_FIXEDֵ���ò�������������ֵ�е�һ�������������ر�ע���Ĳ����ݵ����
		 *
		 * @param[  _In_   ]  SIZE_T uBytes Ҫ������ֽ���������˲���Ϊ�㲢��uFlags����ָ��LMEM_MOVEABLE���ú���������һ�������Ϊ�ѱ��������ڴ����ľ��
		 *
		 * @return            
		 *
		 * @Date 2017/06/30  13:40:11
		*/
		m_p = static_cast<P>(::LocalAlloc(LPTR, bytes));

		/**
		 * @brief             HRESULT_FROM_WIN32(Maps a system error code to an HRESULT value.)
		 *
		 * @Function          ��ϵͳ�������ӳ�䵽HRESULTֵ
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
	 * @Function GetTokenInformation��������ָ�����͵Ĺ�����Ϣ�������ơ����ý��̱�����л�ȡ��Ϣ���ʵ�����Ȩ��
	 *
	 * @param    HANDLE                  TokenHandle (������Ϣ�ķ������Ƶľ�������TokenInformationClassָ��TokenSource����þ���������TOKEN_QUERY_SOURCE����Ȩ�ޡ�������������TokenInformationClassֵ������������TOKEN_QUERYȨ�ޡ�)
	 *
	 * @param    TOKEN_INFORMATION_CLASS TokenInformationClass (ָ������TOKEN_INFORMATION_CLASSö�����͵�ֵ�� ��ʶ������������Ϣ���͡����TokenIsAppContainer������0���κκ�����ҲӦ����֤���������Ʋ���ʶ�𼶱��ģ�����ơ������ǰ���Ʋ���Ӧ�ó���������������ݼ������ƣ���Ӧ����AccessDenied��)
	 *
	 * @param    LPVOID                  TokenInformation (ָ�򻺳�����ָ�룬�ú���������������Ϣ������˻������Ľṹȡ������TokenInformationClass����ָ������Ϣ���͡�)
	 *
	 * @param    DWORD                   TokenInformationLength (ָ����TokenInformation����ָ��Ļ������Ĵ�С�����ֽ�Ϊ��λ�������TokenInformationΪNULL����˲�������Ϊ��)
	 *
	 * @param    PDWORD                  ReturnLength (ָ��һ��������ָ�룬�ñ�������TokenInformation����ָ��Ļ�����������ֽ����������ֵ����TokenInformationLength������ָ����ֵ����ú�����ʧ�ܣ����Ҳ����ڻ������д洢�κ����ݡ�)
	 *
	 * @return   ��������ɹ�������ֵ��Ϊ�㡣�������ʧ�ܣ�����ֵΪ�㡣Ҫ��ȡ��չ������Ϣ������� GetLastError��
	 *
	 * @Date 2017/06/30  12:00:41
	 */
	// ȡ�ô洢��Ȩ��Ϣ�ռ�Ĵ�Сsize
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