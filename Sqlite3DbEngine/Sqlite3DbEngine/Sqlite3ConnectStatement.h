// Sqlite3ConnectStatement.h : CSqlite3ConnectStatement ������

#pragma once
#include "resource.h"       // ������



#include "Sqlite3DbEngine_i.h"

#include "Sqlite3Statement.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;



// CSqlite3ConnectStatement

class ATL_NO_VTABLE CSqlite3ConnectStatement : 
	public CComObjectRoot,
	public ISqlite3Statement
{
public:
	CSqlite3ConnectStatement()
		: m_pSqlite3Statement(NULL)
	{
	}
	~CSqlite3ConnectStatement()
	{
		if(NULL != m_pSqlite3Statement)
		{
			delete m_pSqlite3Statement;
			m_pSqlite3Statement = NULL;
		}
	}

	/**
	 * @brief ���ð�װ����
	 *
	 *
	 * @param *pSqlite3Statement 
	 * @return 
	 */
	HRESULT SetObj(CSqlite3Statement *pSqlite3Statement)
	{
		ATLASSERT(pSqlite3Statement);
		m_pSqlite3Statement = pSqlite3Statement;
		return m_pSqlite3Statement ? S_OK:E_FAIL;
	}

BEGIN_COM_MAP(CSqlite3ConnectStatement)
	COM_INTERFACE_ENTRY(ISqlite3Statement)
	COM_INTERFACE_ENTRY2(IDispatch,ISqlite3Statement) 
END_COM_MAP()

public:
	/// IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT*)
    {
       return E_NOTIMPL;
    }

    STDMETHODIMP GetTypeInfo( UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
    { 
       return E_NOTIMPL;
    }

    STDMETHODIMP GetIDsOfNames( REFIID riid,LPOLESTR *rgszNames,UINT cNames,
                    LCID lcid,DISPID *rgDispId)
    { 
      return E_NOTIMPL;
    }

	STDMETHOD(Invoke)(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,
               VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);
	
	/// ISqlite3Statement
	/**
	 * @brief ����ֶ�����
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_FieldCount)(ULONG* pVal);

	/**
	 * @brief ��ü�¼����
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_RecordCount)(ULONG* pVal);
	
	/**
	 * @brief ����ֶ�����
	 *
	 *
	 * @param lFieldIndex 
	 * @param pbstrName 
	 * @return 
	 */
	STDMETHOD(GetFieldName)(LONG lFieldIndex,BSTR* pbstrName);

	/**
	 * @brief ����ֶ�����
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetDataType)(LONG lFieldIndex,EDBDataType* pVal);

	/**
	 * @brief ����ֶ������ַ���
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetFieldType)(LONG lFieldIndex,SHORT* pVal);

	/**
	 * @brief ��������ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueInt)(LONG lFieldIndex,LONG* pVal);

	/**
	 * @brief ��ó������ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueLong)(LONG lFieldIndex,LONGLONG* pVal);

	/**
	 * @brief ��ø����ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueDouble)(LONG lFieldIndex,DOUBLE* pVal);

	/**
	 * @brief ����ַ����ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueString)(LONG lFieldIndex, BSTR* pVal);

	/**
	 * @brief ���BLOB����
	 *
	 *
	 * @param lFieldIndex 
	 * @param ppData 
	 * @param pLen 
	 * @return 
	 */
	STDMETHOD(GetValueBlob)(LONG lFieldIndex,BYTE** ppData,ULONG* pLen);

	/**
	 * @brief ���BLOB����ָ�����ļ�
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrFilePath 
	 * @return 
	 */
	STDMETHOD(GetValueBlobFile)(LONG lFieldIndex,BSTR bstrFilePath);

	/**
	 * @brief ���ַ��������ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrValue 
	 * @return 
	 */
	STDMETHOD(BindString)(LONG lFieldIndex,BSTR bstrValue);

	/**
	 * @brief ��˫���������ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param dValue 
	 * @return 
	 */
	STDMETHOD(BindDouble)(LONG lFieldIndex,DOUBLE dValue);

	/**
	 * @brief �����������ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param lValue 
	 * @return 
	 */
	STDMETHOD(BindInt)(LONG lFieldIndex,LONG lValue);

	/**
	 * @brief �������ļ�����
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrFilePath 
	 * @return 
	 */
	STDMETHOD(BindBlobFile)(LONG lFieldIndex,BSTR bstrFilePath);

	/**
	 * @brief ����
	 *
	 *
	 * @param lFieldIndex 
	 * @param bstrFilePath 
	 * @return 
	 */
	STDMETHOD(BindBlob)(LONG lFieldIndex,BYTE* pData,ULONG nDataLen);

	/**
	 * @brief �󶨿������ֶ�ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @return 
	 */
	STDMETHOD(BindNull)(LONG lFieldIndex);

	/**
	 * @brief ��һ���������
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(NextRow)(void);

	/**
	 * @brief ִ��
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(Execute)(void);

	/**
	 * @brief ����
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(Reset)(void);

	/**
	 * @brief ��ð�װ����
	 *
	 *
	 * @param pObjVal 
	 * @return 
	 */
	STDMETHOD(get_NodeObject)(VARIANT* pObjVal);

protected:

	CSqlite3Statement *m_pSqlite3Statement;
};