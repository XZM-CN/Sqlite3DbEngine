// Md5Logic.cpp : CMd5Logic 的实现

#include "stdafx.h"
#include "Md5Logic.h"


// CMd5Logic



STDMETHODIMP CMd5Logic::StringMD5(BSTR bstrString, BSTR* pbstrMD5)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrString)
		return E_POINTER;
	CString strMD5 = m_pMd5Wrapper->GetStringMD5(bstrString);
	CComBSTR bstrVal(strMD5);
	bstrVal.CopyTo(pbstrMD5);
	bstrVal.Empty();
	strMD5.Empty();
	return S_OK;
}


STDMETHODIMP CMd5Logic::StringSha1(BSTR bstrString, BSTR* pbstrSha1)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrString)
		return E_POINTER;
	CString strSha1 = m_pMd5Wrapper->GetStringSha1(bstrString);
	CComBSTR bstrVal(strSha1);
	bstrVal.CopyTo(pbstrSha1);
	bstrVal.Empty();
	strSha1.Empty();
	return S_OK;
}
