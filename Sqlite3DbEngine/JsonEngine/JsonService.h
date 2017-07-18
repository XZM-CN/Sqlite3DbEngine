// JsonService.h : CJsonService 的声明

#pragma once
#include "resource.h"       // 主符号



#include "JsonEngine_i.h"
#include "JsonParser.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)            \
	上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA  \
	可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。       \
	rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CJsonService
//
// 接口风格参看图片JsonEngine--接口--2017-07-18_14-21-30--JsonService.png
//
//////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE CJsonService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CJsonService, &CLSID_JsonService>,
	public IDispatchImpl<IJsonService, &IID_IJsonService, &LIBID_JsonEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// 解析器对象
	CJsonParser *m_pJsonParser;

	/// 缓存的文件解析路径
	CString		m_strFilePath;

	/// 编码类型
	ECodingType	m_eCodingType;

	void ClearParser()
	{
		m_strFilePath.Empty();
		if(NULL != m_pJsonParser)
		{
			delete m_pJsonParser;
			m_pJsonParser = NULL;
		}
	}

public:
	CJsonService();

	/**
	 * @brief 设置包装对象
	 *
	 *
	 * @param *pSqlStatement 
	 * @return 
	 */
	HRESULT SetObj(CJsonParser *pJsonParser)
	{
		ATLASSERT(pJsonParser);
		m_pJsonParser = pJsonParser;
		return m_pJsonParser ? S_OK:E_FAIL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_JSONSERVICE)


BEGIN_COM_MAP(CJsonService)
	COM_INTERFACE_ENTRY(IJsonService)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		ClearParser();
	}

public:

public:
	/// IJsonService
	STDMETHOD(get_CodingType)(ECodingType* pVal);
	STDMETHOD(put_CodingType)(ECodingType newVal);
	STDMETHOD(get_FilePath)(BSTR* pVal);
	STDMETHOD(put_FilePath)(BSTR newVal);
	STDMETHOD(get_ArraySize)(ULONG* pVal);
	STDMETHOD(get_IsExist)(BSTR bstrKeyName, VARIANT_BOOL* pVal);

	STDMETHOD(ParseString)(BSTR bstrContent, VARIANT_BOOL* pVal);
	STDMETHOD(ParseFile)(BSTR bstrFilePath, VARIANT_BOOL* pVal);
	STDMETHOD(get_Type)(EJsonType* pVal);
	STDMETHOD(get_Value)(VARIANT* pVal);
	STDMETHOD(put_IntValue)(BSTR bstrKeyName, LONGLONG newVal);
	STDMETHOD(put_StringValue)(BSTR bstrKeyName, BSTR newVal);
	STDMETHOD(put_ChildValue)(BSTR bstrKeyName, VARIANT newVal);
	STDMETHOD(get_ObjectString)(BSTR* pVal);
	STDMETHOD(get_ErrInfo)(BSTR* pVal);

	STDMETHOD(GetValueType)(BSTR bstrKeyName, EJsonType* pVal);
	STDMETHOD(GetStringValue)(BSTR bstrKeyName, BSTR* pVal);
	STDMETHOD(GetBoolValue)(BSTR bstrKeyName, VARIANT_BOOL* pVal);
	STDMETHOD(GetIntValue)(BSTR bstrKeyName, LONGLONG* pVal);
	STDMETHOD(GetUIntValue)(BSTR bstrKeyName, ULONGLONG* pVal);
	STDMETHOD(GetDoubleValue)(BSTR bstrKeyName, DOUBLE* pVal);
	STDMETHOD(GetVariantValue)(BSTR bstrKeyName, VARIANT* pVal);
	STDMETHOD(GetArrayValue)(SHORT sIndex,BSTR* pKey,VARIANT* pVal);

	STDMETHOD(GetChildByName)(BSTR bstrKeyName, IJsonService** pVal);
	STDMETHOD(GetChildByIndex)(LONG nIndex, IJsonService** pVal);
	STDMETHOD(CreateChild)(BSTR bstrKeyName, IJsonService** pVal);
	STDMETHOD(PutChild)(BSTR bstrKeyName, IJsonService* pVal);
	STDMETHOD(GetString)(IParaService** pVal);

	STDMETHOD(Save)(void);
	STDMETHOD(Clear)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(JsonService), CJsonService)
