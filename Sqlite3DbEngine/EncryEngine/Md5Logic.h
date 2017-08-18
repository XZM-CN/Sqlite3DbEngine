// Md5Logic.h : CMd5Logic ������

#pragma once
#include "resource.h"       // ������



#include "EncryEngine_i.h"

#include "Md5Wrapper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
	 * @Function          ���ַ�������MD5����
	 *
	 * @param[  _In_   ]  bstrString ���������ַ���
	 *
	 * @param[  _Out_  ]  pbstrMD5 �������md5���ܵ��ַ���
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
