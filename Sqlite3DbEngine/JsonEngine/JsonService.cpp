// JsonService.cpp : CJsonService 的实现

#include "stdafx.h"
#include "JsonService.h"

extern HANDLE	g_hInstance;



CComPtr <IParaService> GetParaService()
{
	CComPtr <IParaService> spiParaService = NULL;
	CBaseFuncLib::CreateInterface((HINSTANCE )g_hInstance,__uuidof(ParaService),__uuidof(IParaService),(VOID **)&spiParaService);
	ATLASSERT(spiParaService);
	return spiParaService;
}

// CJsonService
CJsonService::CJsonService()
	:m_pJsonParser(NULL)
	,m_eCodingType(CODINGTYPE_UTF8)
	,m_strFilePath(_T(""))
{
}

STDMETHODIMP CJsonService::get_CodingType(ECodingType* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	if(NULL != m_pJsonParser)
		(*pVal) = m_pJsonParser->GetCodingType();
	else
		(*pVal) = m_eCodingType;
	return S_OK;
}

STDMETHODIMP CJsonService::put_CodingType(ECodingType newVal)
{
	// TODO: Add your implementation code here
	if(newVal < CODINGTYPE_ANSI || newVal > CODINGTYPE_UTF16)
		return E_INVALIDARG;
	m_eCodingType = newVal;
	if(NULL != m_pJsonParser)
		m_pJsonParser->PutCodingType(m_eCodingType);
	return S_OK;
}

STDMETHODIMP CJsonService::ParseFile(BSTR bstrFilePath, VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrFilePath || NULL == pVal)
	{
		ATLASSERT(0);
		return E_POINTER;
	}
	*pVal = VARIANT_FALSE;

	ClearParser();
	m_strFilePath = bstrFilePath;
	if(m_strFilePath.IsEmpty() || !CBaseFuncLib::IsPathExist(m_strFilePath))
	{
		ATLASSERT(0);
		return E_INVALIDARG;
	}
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	ATLASSERT(m_pJsonParser);
	if(NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_OUTOFMEMORY;
	}
	m_pJsonParser->PutCodingType(m_eCodingType);
	bool bParseFlag= m_pJsonParser->ParseFile(m_strFilePath);
	if(!bParseFlag && CBaseFuncLib::GetFileSize(m_strFilePath))
	{
		/// 转换编码保存查看
		char *szBuf = NULL;
		BYTE *pContent = NULL;
		CBaseFuncLib::ReadAllData(m_strFilePath,&pContent);
		if(NULL != pContent)
		{
			CBaseFuncLib::Utf8ToChar((const char *)pContent,&szBuf);
			delete []pContent;
			pContent = NULL;
		}
		if(NULL != szBuf)
		{
			/// 改写错误JSON文件路径
			int nFind = m_strFilePath.ReverseFind(_T('\\'));
			CString strFileName(m_strFilePath.Right(m_strFilePath.GetLength()-nFind-1));
			CString strErrPath(CBaseFuncLib::GetTmpPath()+strFileName);
			CBaseFuncLib::WriteToFile(strErrPath,(BYTE *)szBuf,(DWORD)strlen(szBuf));
			delete []szBuf;
			szBuf = NULL;
		}
		return E_FAIL;
	}
	*pVal = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CJsonService::ParseString(BSTR bstrContent, VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrContent || NULL == pVal)
	{
		ATLASSERT(0);
		return E_POINTER;
	}
	*pVal = VARIANT_FALSE;

	ClearParser();
	CString strContent(bstrContent);
	if(strContent.IsEmpty())
	{
		ATLASSERT(0);
		return E_INVALIDARG;
	}
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	ATLASSERT(m_pJsonParser);
	if(NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_OUTOFMEMORY;
	}
	m_pJsonParser->PutCodingType(m_eCodingType);
	char *szBuf = NULL;
	if(CODINGTYPE_UTF8 != m_eCodingType)
		CBaseFuncLib::US2ToUtf8(strContent,&szBuf);
	else
		CBaseFuncLib::Us2ToChar(strContent,&szBuf);
	if(NULL == szBuf)
	{
		ATLASSERT(0);
		return E_OUTOFMEMORY;
	}
	bool bParseFlag= m_pJsonParser->Parse(szBuf);
	delete []szBuf;
	szBuf = NULL;
	if(!bParseFlag)
		return E_FAIL;
	*pVal = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CJsonService::put_FilePath(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		Clear();
		return S_OK;
	}
	m_strFilePath.Empty();
	m_strFilePath = newVal;
	return S_OK;
}

STDMETHODIMP CJsonService::get_FilePath(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrValue(m_strFilePath);
	bstrValue.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CJsonService::get_Type(EJsonType* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal || NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_POINTER;
	}
	*pVal = (EJsonType )m_pJsonParser->GetType();
	return S_OK;
}

STDMETHODIMP CJsonService::get_Value(VARIANT* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal || NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_POINTER;
	}
	bool bRet = m_pJsonParser->GetValue(pVal);
	if(!bRet)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CJsonService::GetValueType(BSTR bstrKeyName, EJsonType* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_POINTER;
	}
	*pVal = JSONTYPE_NULL;
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
	{
		ATLASSERT(0);
		return E_INVALIDARG;
	}
	Json::ValueType jType = m_pJsonParser->GetChildTypeByName(bstrKeyName);
	switch(jType)
	{
	case Json::booleanValue:
		*pVal = JSONTYPE_BOOL;
		break;
	case Json::intValue:
		*pVal = JSONTYPE_INT;
		break;
	case Json::uintValue:
		*pVal = JSONTYPE_UINT;
		break;
	case Json::realValue:
		*pVal = JSONTYPE_REAL;
		break;
	case Json::stringValue:
		*pVal = JSONTYPE_STRING;
		break;
	case Json::arrayValue:
		*pVal = JSONTYPE_ARRAY;
		break;
	case Json::objectValue:
		*pVal = JSONTYPE_OBJECT;
		break;
	default:
		break;
	}
	return S_OK;
}

STDMETHODIMP CJsonService::GetStringValue(BSTR bstrKeyName, BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_POINTER;
	}
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
	{
		ATLASSERT(0);
		return E_INVALIDARG;
	}
	CString strVaule = m_pJsonParser->GetStringChildByName(strKeyName);
	CComBSTR bstrValue(strVaule);
	bstrValue.CopyTo(pVal);
	bstrValue.Empty();
	return S_OK;
}

STDMETHODIMP CJsonService::GetBoolValue(BSTR bstrKeyName, VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	bool bValue = m_pJsonParser->GetBoolChildByName(strKeyName);
	if(bValue)
		*pVal = VARIANT_TRUE;
	else
		*pVal = VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CJsonService::GetIntValue(BSTR bstrKeyName, LONGLONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	*pVal = m_pJsonParser->GetLongLongChildByName(strKeyName);

	return S_OK;
}

STDMETHODIMP CJsonService::GetUIntValue(BSTR bstrKeyName, ULONGLONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	*pVal = m_pJsonParser->GetUIntChildByName(strKeyName);

	return S_OK;
}

STDMETHODIMP CJsonService::GetDoubleValue(BSTR bstrKeyName, DOUBLE* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	*pVal = m_pJsonParser->GetDoubleChildByName(strKeyName);
	return S_OK;
}

STDMETHODIMP CJsonService::GetArrayValue(SHORT sIndex,BSTR* pKey,VARIANT* pVal)
{
	// TODO: Add your implementation code here
	if(sIndex<0 || NULL == pKey || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	CComVariant varVal;
	CComBSTR bstrKeyName;
	bool bGetFlag = m_pJsonParser->GetArrayValue(sIndex,bstrKeyName,varVal);
	if(!bGetFlag)
		return E_FAIL;
	if(VT_SAFEARRAY == varVal.vt)
	{
		/// 数组类型,特殊处理
		CJsonParser* pJsonParser = (CJsonParser* )varVal.parray;
		CComObject< CJsonService > *pJsonService=NULL;
#ifdef NEED_FINAL_CONSTRUCT
		CComObject<CJsonService>::CreateInstance(&(pJsonService));
#else
		pJsonService = new CComObject< CJsonService >;
#endif
		pJsonService->put_CodingType(pJsonParser->GetCodingType());
		HRESULT hRet = pJsonService->SetObj(pJsonParser);
		if(SUCCEEDED(hRet))
		{
			varVal.Clear();
			varVal.vt = VT_DISPATCH;	
			hRet = pJsonService->QueryInterface(__uuidof(IDispatch),(LPVOID *)&varVal.pdispVal);
			varVal.Detach(pVal);
		}
	}
	else
		varVal.Detach(pVal);
	bstrKeyName.CopyTo(pKey);

	return S_OK;
}

STDMETHODIMP CJsonService::GetVariantValue(BSTR bstrKeyName, VARIANT* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	Json::ValueType vType = m_pJsonParser->GetType();
	if(Json::objectValue != vType)
		return E_INVALIDARG;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	CComVariant varVal;
	bool bGetFlag = m_pJsonParser->GetVariantChildByName(strKeyName,varVal);
	strKeyName.Empty();
	if(!bGetFlag)
		return E_FAIL;
	varVal.Detach(pVal);
	varVal.Clear();
	return S_OK;
}

STDMETHODIMP CJsonService::PutChild(BSTR bstrKeyName, IJsonService* pVal)
{
	if(NULL == bstrKeyName || NULL == pVal)
		return E_POINTER;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	if(NULL == m_pJsonParser)
		return E_POINTER;
	/// 设置关键字名称
	CComBSTR bstrVal;
	HRESULT hRet = pVal->get_ObjectString(&bstrVal);
	if(FAILED(hRet))
		return hRet;
		bool bSetFlag = m_pJsonParser->SetObjAsChild(strKeyName,bstrVal.m_str);
	bstrVal.Empty();
	if(!bSetFlag)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CJsonService::GetChildByName(BSTR bstrKeyName, IJsonService** pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	CJsonParser *pJsonParser = m_pJsonParser->GetChildJson(strKeyName);
	if(NULL == pJsonParser)
		return E_FAIL;
	Json::ValueType vType = pJsonParser->GetType();
	if(Json::nullValue == vType)
		return E_FAIL;
#ifdef _DEBUG
	if(Json::arrayValue == vType)
	{
		ULONG ulSize = pJsonParser->GetArraySize();
		::Sleep(1);
	}
#endif
	CComObject< CJsonService > *pJsonService=NULL;
#ifdef NEED_FINAL_CONSTRUCT
	CComObject<CJsonService>::CreateInstance(&(pJsonService));
#else
	pJsonService = new CComObject< CJsonService >;
#endif
	pJsonService->put_CodingType(pJsonParser->GetCodingType());
	HRESULT hRet = pJsonService->SetObj(pJsonParser);
	if(SUCCEEDED(hRet))
		hRet = pJsonService->QueryInterface(IID_IJsonService,(LPVOID *)pVal);
	return S_OK;
}

STDMETHODIMP CJsonService::GetChildByIndex(LONG nIndex, IJsonService** pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	if(nIndex<0)
		return E_INVALIDARG;
	CJsonParser *pJsonParser = m_pJsonParser->GetChildJson(nIndex);
	if(NULL == pJsonParser)
	{
		ATLASSERT(0);
		return E_FAIL;
	}
	CComObject< CJsonService > *pJsonService=NULL;
#ifdef NEED_FINAL_CONSTRUCT
	CComObject<CJsonService>::CreateInstance(&(pJsonService));
#else
	pJsonService = new CComObject< CJsonService >;
#endif
	HRESULT hRet = pJsonService->SetObj(pJsonParser);
	if(SUCCEEDED(hRet))
		hRet = pJsonService->QueryInterface(IID_IJsonService,(LPVOID *)pVal);
	return S_OK;
}

STDMETHODIMP CJsonService::Clear(void)
{
	// TODO: Add your implementation code here
	ClearParser();
	return S_OK;
}



STDMETHODIMP CJsonService::get_ObjectString(BSTR* pVal)
{
	if(NULL == m_pJsonParser || NULL == pVal)
		return E_POINTER;
	return m_pJsonParser->GetObjectString(pVal);
}

STDMETHODIMP CJsonService::get_ErrInfo(BSTR* pVal)
{
	if(NULL == m_pJsonParser || NULL == pVal)
		return E_POINTER;
	CComBSTR bstrErrInfo(m_pJsonParser->GetErrInfo());
	bstrErrInfo.CopyTo(pVal);
	bstrErrInfo.Empty();
	return S_OK;
}

STDMETHODIMP CJsonService::Save(void)
{
	// TODO: Add your implementation code here
	if(NULL == m_pJsonParser)
		return E_POINTER;
	if(m_strFilePath.IsEmpty())
		return E_INVALIDARG;
	BOOL bSaveFlag = m_pJsonParser->SaveToFile(m_strFilePath);
	if(!bSaveFlag)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CJsonService::CreateChild(BSTR bstrKeyName, IJsonService** pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal)
		return E_POINTER;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	if(NULL == m_pJsonParser)
		return E_POINTER;
	CJsonParser *pParser = m_pJsonParser->CreateChildJson(strKeyName);
	if(NULL == pParser)
		return E_FAIL;

	CComObject< CJsonService > *pJsonService=NULL;
#ifdef NEED_FINAL_CONSTRUCT
	CComObject<CJsonService>::CreateInstance(&(pJsonService));
#else
	pJsonService = new CComObject< CJsonService >;
#endif
	HRESULT hRet = pJsonService->SetObj(pParser);
	if(SUCCEEDED(hRet))
		hRet = pJsonService->QueryInterface(IID_IJsonService,(LPVOID *)pVal);

	return hRet;
}

STDMETHODIMP CJsonService::put_StringValue(BSTR bstrKeyName, BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName)
		return E_POINTER;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	if(NULL == m_pJsonParser)
		return E_POINTER;
	bool bSetFlag = m_pJsonParser->SetStringValue(strKeyName,newVal);
	if(!bSetFlag)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CJsonService::put_ChildValue(BSTR bstrKeyName, VARIANT newVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName)
		return E_POINTER;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	if(NULL == m_pJsonParser)
		return E_POINTER;
	CComVariant varVal(newVal);
	bool bSetFlag = m_pJsonParser->SetValue(strKeyName,varVal);
	if(!bSetFlag)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CJsonService::put_IntValue(BSTR bstrKeyName, LONGLONG newVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName)
		return E_POINTER;
	CString strKeyName(bstrKeyName);
	if(strKeyName.IsEmpty())
		return E_INVALIDARG;
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	if(NULL == m_pJsonParser)
		return E_POINTER;
	bool bSetFlag = m_pJsonParser->SetIntValue(strKeyName,newVal);
	if(!bSetFlag)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CJsonService::get_ArraySize(ULONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	(*pVal) = m_pJsonParser->GetArraySize();
	return S_OK;
}

STDMETHODIMP CJsonService::get_IsExist(BSTR bstrKeyName, VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrKeyName || NULL == pVal || NULL == m_pJsonParser)
		return E_POINTER;
	*pVal = VARIANT_FALSE;
	if(m_pJsonParser->IsMember(bstrKeyName))
		*pVal = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CJsonService::GetString(IParaService** pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	ULONG ulSize = 0;
	get_ArraySize(&ulSize);
	if(!ulSize)
		return S_OK;
	CComPtr <IParaService> spiParaService = GetParaService();
	if(NULL == spiParaService)
		return E_FAIL;
	std::vector<std::string> KeyList = m_pJsonParser->GetChildNames();

	CComVariant varVal;
	for(ULONG nIndex = 0;nIndex<ulSize;nIndex++)
	{
		char *szKeyName = (char *)KeyList[nIndex].c_str();
		bool bGetFlag = m_pJsonParser->GetVariantChildByKey(szKeyName,varVal);
		ATLASSERT(bGetFlag);
		if(!bGetFlag)
			continue;
		CString strVal(_T(""));
		switch(varVal.vt)
		{
		case VT_BSTR:
			strVal = varVal.bstrVal;
			break;
		default:
			varVal.ChangeType(VT_BSTR);
			strVal = varVal.bstrVal;
			break;
		}
		varVal.Clear();
		WCHAR *szVal = NULL;
		CBaseFuncLib::Utf8ToUS2(szKeyName,&szVal);
		if(NULL != szVal)
		{
			spiParaService->AddString(CComBSTR(szVal),CComBSTR(strVal));
			delete []szVal;
			szVal = NULL;
		}
	}
	KeyList.clear();
	return spiParaService->QueryInterface(IID_IParaService,(LPVOID *)pVal);
}

// 创建Json文件,如果文件存在则删除重建
STDMETHODIMP CJsonService::TestCreateJsonToFile(BSTR bstrFilePath)
{
	if(NULL == bstrFilePath)
	{
		ATLASSERT(0);
		return E_POINTER;
	}

	m_strFilePath = bstrFilePath;
	if(m_strFilePath.IsEmpty() || CBaseFuncLib::IsPathExist(m_strFilePath))
	{
		::SetFileAttributes(bstrFilePath,FILE_ATTRIBUTE_NORMAL);
		::DeleteFile(bstrFilePath);// 如果文件存在则删除重建
	}
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	ATLASSERT(m_pJsonParser);
	if(NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_OUTOFMEMORY;
	}

	m_pJsonParser->CreateJsonDemoToFile(bstrFilePath);
	return S_OK;
}


STDMETHODIMP CJsonService::TestParseJsonFromFile(BSTR bstrFilePath)
{
	if(NULL == bstrFilePath)
	{
		ATLASSERT(0);
		return E_POINTER;
	}

	m_strFilePath = bstrFilePath;

	if(m_strFilePath.IsEmpty() || !CBaseFuncLib::IsPathExist(m_strFilePath))
	{
		ATLASSERT(0);
		return E_INVALIDARG;
	}
	if(NULL == m_pJsonParser)
		m_pJsonParser = new CJsonParser();
	ATLASSERT(m_pJsonParser);
	if(NULL == m_pJsonParser)
	{
		ATLASSERT(0);
		return E_OUTOFMEMORY;
	}

	m_pJsonParser->ParseJsonFromFile(bstrFilePath);
	return S_OK;
}

STDMETHODIMP CJsonService::TestAllJsonDesignFormat(JsonTest eJt)
{
	switch (eJt)
	{
	case e_Test00:
		m_pJsonParser->JsonDesignFormatTest00();
		break;
	case e_Test01:
		m_pJsonParser->JsonDesignFormatTest01();
		break;
	case e_Test02:
		m_pJsonParser->JsonDesignFormatTest02();
		break;
	case e_Test03:
		m_pJsonParser->JsonDesignFormatTest03();
		break;
	case e_Test04:
		m_pJsonParser->JsonDesignFormatTest04();
		break;
	case e_Test05:
		m_pJsonParser->JsonDesignFormatTest05();
		break;
	case e_Test06:
		m_pJsonParser->JsonDesignFormatTest06();
		break;
	case e_Test07:
		m_pJsonParser->JsonDesignFormatTest07();
		break;
	case e_Test08:
		m_pJsonParser->JsonDesignFormatTest08();
		break;
	case e_Test09:
		{
			break;
		}
	case e_Test10:
		{
			break;
		}
	case e_Test11:
		{
			break;
		}
	case e_Test12:
		{
			break;
		}
	case e_Test13:
		{
			break;
		}
	case e_Test14:
		{
			break;
		}
	case e_Test15:
		{
			break;
		}
	case e_Test16:
		{
			break;
		}
	case e_Test17:
		{
			break;
		}
	case e_Test18:
		{
			break;
		}
	case e_Test19:
		{
			break;
		}
	case e_Test20:
		{
			break;
		}
	default:
		break;
	}
	return S_OK;
}


STDMETHODIMP CJsonService::AddObjAsChildNode(BSTR bstrKeyName, IJsonService* pVal)
{
	HRESULT bSetFlag = PutChild(bstrKeyName,pVal);
	if(!bSetFlag)
		return E_FAIL;

	return S_OK;
}


STDMETHODIMP CJsonService::AddArrayAsChildNode(BSTR bstrKeyName, IJsonService* pVal)
{
	return S_OK;
}

STDMETHODIMP CJsonService::OutputStyledJson(void)
{
	m_pJsonParser->OutputStyledJson();
	return S_OK;
}
