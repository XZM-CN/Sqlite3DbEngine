// JsonService.h : CJsonService ������

#pragma once
#include "resource.h"       // ������



#include "JsonEngine_i.h"
#include "JsonParser.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)            \
	���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA  \
	��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�       \
	rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CJsonService
//
// �ӿڷ��ο�ͼƬJsonEngine--�ӿ�--2017-07-18_14-21-30--JsonService.png
//
//////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE CJsonService :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CJsonService, &CLSID_JsonService>,
	public IDispatchImpl<IJsonService, &IID_IJsonService, &LIBID_JsonEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// ����������
	CJsonParser *m_pJsonParser;

	/// ������ļ�����·��
	CString		m_strFilePath;

	/// ��������
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
	 * @brief ���ð�װ����
	 *
	 *
	 * @param *pSqlite3Statement 
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

	// ��ȡ�������ļ�·��
	STDMETHOD(get_FilePath)(BSTR* pVal);
	STDMETHOD(put_FilePath)(BSTR newVal);

	STDMETHOD(get_ArraySize)(ULONG* pVal);
	STDMETHOD(get_IsExist)(BSTR bstrKeyName, VARIANT_BOOL* pVal);

	// �����ַ���|�ļ���Json
	STDMETHOD(ParseString)(BSTR bstrContent, VARIANT_BOOL* pVal);
	STDMETHOD(ParseFile)(BSTR bstrFilePath, VARIANT_BOOL* pVal);
	STDMETHOD(TraversalJson)(BSTR bstrFilePath);

	// ��ȡ�ڵ������(������JsonLib�ڲ�����)
	STDMETHOD(get_Type)(EJsonType* pVal);

	// ��ȡ����Json�ַ���
	STDMETHOD(get_Value)(VARIANT* pVal);

	// ����ڵ�׷���ֶ�ֵkey-value
	STDMETHOD(put_IntValue)(BSTR bstrKeyName, LONGLONG newVal);
	STDMETHOD(put_StringValue)(BSTR bstrKeyName, BSTR newVal);
	STDMETHOD(put_ChildValue)(BSTR bstrKeyName, VARIANT newVal);

	// ��ȡ����Json��BSTR* pVal
	STDMETHOD(get_ObjectString)(BSTR* pVal);

	STDMETHOD(get_ErrInfo)(BSTR* pVal);

	// ��ȡkey��value����
	STDMETHOD(GetValueType)(BSTR bstrKeyName, EJsonType* pVal);

	// ��ȡ�ֶε�value
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

	/**
	 * @brief             PutChild
	 *
	 * @Function          ����ӽڵ�
	 *
	 * @param[  _In_   ]  bstrKeyName key������
	 *
	 * @param[  _In_   ]  pVal �ӿ�ָ��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(PutChild)(BSTR bstrKeyName, IJsonService* pVal);
	STDMETHOD(GetString)(IParaService** pVal);

	STDMETHOD(Save)(void);
	STDMETHOD(Clear)(void);

	/**
	 * @brief             TestCreateJsonToFile
	 *
	 * @Function          ����һ��Json���ļ�
	 *
	 * @param[  _In_   ]  bstrFilePath �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(TestCreateJsonToFile)(BSTR bstrFilePath);

	/**
	 * @brief             TestParseJsonFromFile
	 *
	 * @Function          ����Json���ļ���
	 *
	 * @param[  _In_   ]  bstrFilePath �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(TestParseJsonFromFile)(BSTR bstrFilePath);

	/**
	 * @brief             TestAllJsonDesignFormat
	 *
	 * @Function          �������й���Json�Ĺ��ܺ���Ƽ��ɣ����л��ͷ����л���
	 *
	 * @param[  _In_   ]  eJt ��������
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(TestAllJsonDesignFormat)(JsonTest eJt);

	/**
	 * @brief             AddObjAsChildNode
	 *
	 * @Function          ����ӽڵ㣬�ڵ��Զ�����ʽ���
	 *
	 * @param[  _In_   ]  bstrKeyName key������
	 *
	 * @param[  _In_   ]  pVal �ӿ�ָ��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(AddObjAsChildNode)(BSTR bstrKeyName, IJsonService* pVal);

	/**
	 * @brief             AddArrayAsChildNode
	 *
	 * @Function          ����ӽڵ㣬�ڵ���������ʽ���
	 *
	 * @param[  _In_   ]  bstrKeyName key������
	 *
	 * @param[  _In_   ]  pVal �ӿ�ָ��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(AddArrayAsChildNode)(BSTR bstrKeyName, IJsonService* pVal);

	/**
	 * @brief             OutputStyledJson
	 *
	 * @Function          ��ʽ�����Json����
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	STDMETHOD(OutputStyledJson)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(JsonService), CJsonService)
