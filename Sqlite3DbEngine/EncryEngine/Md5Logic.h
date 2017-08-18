// Md5Logic.h : CMd5Logic 的声明

#pragma once
#include "resource.h"       // 主符号



#include "EncryEngine_i.h"

#include "Md5Wrapper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CMd5Logic

class ATL_NO_VTABLE CMd5Logic :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMd5Logic, &CLSID_Md5Logic>,
	public IDispatchImpl<IMd5Logic, &IID_IMd5Logic, &LIBID_EncryEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMd5Logic()
	{
		m_pMd5Wrapper = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MD5LOGIC)


BEGIN_COM_MAP(CMd5Logic)
	COM_INTERFACE_ENTRY(IMd5Logic)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		if (m_pMd5Wrapper == NULL) {
			m_pMd5Wrapper = new CMd5Wrapper();
		}
		return S_OK;
	}

	void FinalRelease()
	{
	}

protected:
	CMd5Wrapper*  m_pMd5Wrapper;
public:


	/**
	 * @brief             StringMD5
	 *
	 * @Function          将字符串进行MD5加密
	 *
	 * @param[  _In_   ]  bstrString 传进来的字符串
	 *
	 * @param[  _Out_  ]  pbstrMD5 输出经过md5加密的字符串
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(StringMD5)(BSTR bstrString, BSTR* pbstrMD5);

	/**
	 * @brief             
	 *
	 * @Function          
	 *
	 * @param[  _In_   ]  
	 *
	 * @param[  _Out_  ]  
	 *
	 * @param[_Out_opt_]  
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(StringSha1)(BSTR bstrString, BSTR* pbstrSha1);
};

OBJECT_ENTRY_AUTO(__uuidof(Md5Logic), CMd5Logic)
