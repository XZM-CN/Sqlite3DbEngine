// ParaService.cpp : CParaService 的实现

#include "stdafx.h"
#include "ParaService.h"

extern HANDLE	g_hInstance;

CComPtr <IJsonService> GetJsonService()
{
	CComPtr <IJsonService> spiJsonService = NULL;
	CBaseFuncLib::CreateInterface((HINSTANCE )g_hInstance,__uuidof(JsonService),__uuidof(IJsonService),(VOID **)&spiJsonService);
	ATLASSERT(spiJsonService);
	return spiJsonService;
}

// CParaService

STDMETHODIMP CParaService::AddNumber(/*[in]*/ BSTR bstrName,/*[in]*/ ULONG nValue)
{
	// TODO: Add your implementation code here
	if(NULL == bstrName)
		return E_POINTER;
	CString strName(bstrName);
	if(strName.IsEmpty())
		return E_INVALIDARG;
	m_ParaPair.Set(strName,nValue);
	return S_OK;
}

STDMETHODIMP CParaService::AddString(/*[in]*/ BSTR bstrName,/*[in]*/ BSTR bstrValue)
{
	// TODO: Add your implementation code here
	if(NULL == bstrName || NULL == bstrValue)
		return E_POINTER;
	CString strName(bstrName);
	if(strName.IsEmpty())
		return E_INVALIDARG;
	m_ParaPair.Set(strName,bstrValue);
	return S_OK;
}

STDMETHODIMP CParaService::IsExist(/*[in]*/ BSTR bstrName,/*[out,retval]*/ VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrName || NULL == pVal)
		return E_POINTER;
	*pVal = VARIANT_FALSE;
	CString strName(bstrName);
	if(strName.IsEmpty())
		return E_INVALIDARG;
	if(m_ParaPair.IsExist(strName))
		*pVal = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CParaService::GetNumber(/*[in]*/ BSTR bstrName,/*[out,retval]*/ ULONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrName || NULL == pVal)
		return E_POINTER;
	CString strName(bstrName);
	if(strName.IsEmpty())
		return E_INVALIDARG;
	if(!m_ParaPair.IsExist(strName))
		return E_FAIL;
	(*pVal) = (ULONG )CBaseFuncLib::StrToNum(m_ParaPair.Get(strName));
	return S_OK;
}

STDMETHODIMP CParaService::GetString(/*[in]*/ BSTR bstrName,/*[out,retval]*/ BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == bstrName || NULL == pVal)
		return E_POINTER;
	CString strName(bstrName);
	if(strName.IsEmpty())
		return E_INVALIDARG;
	if(!m_ParaPair.IsExist(strName))
		return E_FAIL;
	CComBSTR bstrVal(m_ParaPair.Get(strName));
	bstrVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CParaService::get_Count(/*[out,retval]*/ SHORT* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	*pVal = (SHORT )m_ParaPair.Size();
	return S_OK;
}

STDMETHODIMP CParaService::RemovePara(/*[in]*/ BSTR bstrName)
{
	// TODO: Add your implementation code here
	CString strName(bstrName);
	if(strName.IsEmpty())
		return E_INVALIDARG;
	if(!m_ParaPair.IsExist(strName))
		return E_FAIL;
	m_ParaPair.Remove(strName);
	return S_OK;
}

STDMETHODIMP CParaService::Clear()
{
	// TODO: Add your implementation code here
	m_ParaPair.Clear();
	return S_OK;
}

STDMETHODIMP CParaService::GetJson(IJsonService** pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	if(!m_ParaPair.Size())
		return S_OK;/// 无参数需要保存
	CComPtr <IJsonService> spiJsonService = GetJsonService();
	if(NULL == spiJsonService)
		return E_FAIL;
	CSTRING_MAP ParaMap = m_ParaPair.GetMap();
	CSTRING_MAP::iterator it = ParaMap.begin();
	while(it != ParaMap.end())
	{
		spiJsonService->put_StringValue(CComBSTR(it->first),CComBSTR(it->second));
		it++;
	}
	return spiJsonService->QueryInterface(IID_IJsonService,(LPVOID *)pVal);
}