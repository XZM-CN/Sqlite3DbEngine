// Sqlite3ConnectStatement.cpp : CSqlite3ConnectStatement ��ʵ��

#include "stdafx.h"
#include "Sqlite3ConnectStatement.h"

#include "BaseFuncLib.h"


// CSqlite3ConnectStatement


#pragma warning( disable : 4311 )
//////////////////////////////////////////
STDMETHODIMP CSqlite3ConnectStatement::Invoke( DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,
	DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr)
{
	/// ��������C++���Ե���������ʵ��
	HRESULT hRet(S_OK);
	switch( dispIdMember )
	{
	case 0x1:
		break;
	default:
		break;
	}
	return hRet;
}

STDMETHODIMP CSqlite3ConnectStatement::get_FieldCount(ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	ATLASSERT(m_pSqlite3Statement);
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	(*pVal) = m_pSqlite3Statement->Fields();

	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::get_RecordCount(ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	(*pVal) = 0;
	ATLASSERT(m_pSqlite3Statement);
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	while(m_pSqlite3Statement->NextRow())
	{
		(*pVal)++;
	}
	m_pSqlite3Statement->Reset();/// �ָ�״̬

	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetFieldName(LONG lFieldIndex,BSTR* pbstrName)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pbstrName)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	TCHAR *szFieldName = NULL;
	/// ����ֶ����֣���Ҫ���б���ת��
	CBaseFuncLib::Utf8ToUS2((LPCSTR )m_pSqlite3Statement->FieldName(lFieldIndex),&szFieldName);
	if(!szFieldName)
		return E_POINTER;
	CString strFieldName(szFieldName);
	delete []szFieldName;
	szFieldName = NULL;
	CComBSTR bstrName(strFieldName);
	bstrName.CopyTo(pbstrName);

	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetDataType(LONG lFieldIndex,EDBDataType* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;/// ������Ч
	*pVal = (EDBDataType )m_pSqlite3Statement->DataType(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetFieldType(LONG lFieldIndex,SHORT* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	(*pVal) = m_pSqlite3Statement->FieldType(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetValueInt(LONG lFieldIndex,LONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	*pVal = m_pSqlite3Statement->ValueInt(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetValueLong(LONG lFieldIndex,LONGLONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	(*pVal) = m_pSqlite3Statement->ValueLong(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetValueDouble(LONG lFieldIndex,DOUBLE* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	*pVal = m_pSqlite3Statement->ValueFloat(lFieldIndex);
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetValueString(LONG lFieldIndex, BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;

	CComBSTR bstrValue(m_pSqlite3Statement->ValueString(lFieldIndex));
	bstrValue.CopyTo(pVal);
	bstrValue.Empty();
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetValueBlob(LONG lFieldIndex,BYTE** ppData,ULONG* pLen)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == ppData && NULL == pLen)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	try
	{
		///�����쳣����
		(*ppData) = m_pSqlite3Statement->ValueBlob(lFieldIndex,*pLen);
	}
	catch ( ... )
	{
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::GetValueBlobFile(LONG lFieldIndex, BSTR bstrFilePath)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == bstrFilePath)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	try
	{
		///д�ļ��������쳣����
		if(!m_pSqlite3Statement->ValueBlobFile(lFieldIndex,bstrFilePath))
			return E_FAIL;
	}
	catch ( ... )
	{
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::BindString(LONG lFieldIndex, BSTR bstrValue)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!bstrValue)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	if(!m_pSqlite3Statement->Bind(lFieldIndex,bstrValue))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::BindDouble(LONG lFieldIndex, DOUBLE dValue)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	if(!m_pSqlite3Statement->Bind(lFieldIndex,dValue))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::BindBlob(LONG lFieldIndex, BYTE* pData,ULONG nDataLen)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pData || !nDataLen)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	if(!m_pSqlite3Statement->BindBlobData(lFieldIndex,pData,nDataLen))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::BindBlobFile(LONG lFieldIndex, BSTR bstrFilePath)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!bstrFilePath)
		return ERROR_INVALID_PARAMETER;
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	if(!m_pSqlite3Statement->BindBlobFile(lFieldIndex,bstrFilePath))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::BindInt(LONG lFieldIndex, LONG lValue)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	if(!m_pSqlite3Statement->Bind(lFieldIndex,lValue))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::BindNull(LONG lFieldIndex)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	int iCount = m_pSqlite3Statement->Fields();
	if(lFieldIndex>=iCount || lFieldIndex<0)
		return ERROR_INVALID_PARAMETER;
	if(!m_pSqlite3Statement->BindNull(lFieldIndex))
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::NextRow(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	if(!m_pSqlite3Statement->NextRow())
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::Execute(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	if(!m_pSqlite3Statement->Execute())
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::Reset(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	if(!m_pSqlite3Statement->Reset())
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CSqlite3ConnectStatement::get_NodeObject(VARIANT* pObjVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pObjVal)
		return ERROR_INVALID_PARAMETER;
	ATLASSERT(m_pSqlite3Statement);
	if(NULL == m_pSqlite3Statement)
		return E_POINTER;
	CComVariant varObj;
	varObj.vt = VT_UI4;
	varObj.ulVal = (unsigned long )m_pSqlite3Statement;

	varObj.Detach(pObjVal);
	return S_OK;
}