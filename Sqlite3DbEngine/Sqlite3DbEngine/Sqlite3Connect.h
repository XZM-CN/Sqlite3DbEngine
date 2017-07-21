// Sqlite3Connect.h : CSqlite3Connect ������

#pragma once
#include "resource.h"       // ������



#include "Sqlite3DbEngine_i.h"

#include "Sqlite3Wrapper.h"

class Sqlite3Wrapper;


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSqlite3Connect

class ATL_NO_VTABLE CSqlite3Connect :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSqlite3Connect, &CLSID_Sqlite3Connect>,
	public IDispatchImpl<ISqlite3Connect, &IID_ISqlite3Connect, &LIBID_Sqlite3DbEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSqlite3Connect()
	{

		m_hSyncEvent	= NULL;
		m_pSqliteWraper = NULL;
		m_pSqlite3Statement = NULL;

		m_lOverTimeSecond = 15;
		m_dwCurrentThreadID = 0;

		m_bReadOnly = FALSE;
		m_bBeginTrans = FALSE;
		m_strDbFile.Empty();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SQLITECONNECT)


BEGIN_COM_MAP(CSqlite3Connect)
	COM_INTERFACE_ENTRY(ISqlite3Connect)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		Close();
	}


protected:

	/// �Ƿ�����������
	BOOL	m_bBeginTrans;

	/// �Ƿ�ֻ����ʽ��
	BOOL	m_bReadOnly;

	/// ���ݿ��ļ�·��
	CString m_strDbFile;
	/// ���ļ�����
	CString m_strPassword;
	/// ���һ�β���������Ϣ
	CString m_strLastErrInfo;

	/// Sqlite���ܷ�װ��
	CSqlite3Wrapper *m_pSqliteWraper;
	/// ��ѯ������
	CSqlite3Statement *m_pSqlite3Statement;

	/// ���ݿ��ļ��򿪺��ͬ���¼����
	HANDLE m_hSyncEvent;
	/// �������ݿ����ӵ��߳�ID
	DWORD m_dwCurrentThreadID;

    /// ��ʱ����
	long m_lOverTimeSecond;

	/**
	 * @brief �ͷŲ�ѯ���ִ�л���
	 *
	 *
	 * @return 
	 */
	HRESULT FinalizeStatement();


public:
	/// ISqlite3Connect
	/**
	 * @brief �������ݿ�����
	 *
	 *
	 * @param newVal 
	 * @return 
	 */
	STDMETHOD(put_Password)(BSTR newVal);

	/**
	 * @brief ���ó�ʱ
	 *
	 *
	 * @param newVal 
	 * @return 
	 */
	STDMETHOD(put_TimeoutSecond)(long newVal);

	/**
	 * @brief ��ó�ʱ
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_TimeoutSecond)(long* pVal);

	/**
	 * @brief ���ò���
	 *
	 *
	 * @param newVal 
	 * @return 
	 */
	STDMETHOD(put_ParaValue)(BSTR bstrKeyName,BSTR newVal);

	/**
	 * @brief ��ȡ����
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_ParaValue)(BSTR bstrKeyName,BSTR* pVal);

	
	/** ReadOpen
	 * @brief ֻ����ʽ�����ݿ�
	 *
	 *
	 * @param bstrDbFile 
	 * @param bstrPW 
	 * @param bCreateFlag 
	 * @return 
	 */
	STDMETHOD(ReadOpen)(BSTR bstrDbFile,BSTR bstrPW);

	/**
	 * @brief �����ݿ�
	 *
	 *
	 * @param bstrDbFile 
	 * @param bstrPW 
	 * @param bCreateFlag 
	 * @return 
	 */
	STDMETHOD(Open)(BSTR bstrDbFile,BSTR bstrPW,VARIANT_BOOL bCreateFlag);

	/**
	 * @brief ������ݿ��ļ��б����
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_TableCount)(VARIANT_BOOL bTemFlag,LONG* pVal);
	/**
	 * @brief �жϸ����������Ƿ����
	 *
	 *
	 * @param bstrTableName 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(TableIsExist)(BSTR bstrTableName,VARIANT_BOOL bTemFlag, VARIANT_BOOL* pVal);

	/**
	 * @brief ���ָ�������ƴ���ʱ��SQL���
	 *
	 *
	 * @param bstrTableName 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetTableSql)(BSTR bstrTableName,VARIANT_BOOL bTemFlag, BSTR* pVal);
	
	/**
	 * @brief ��ʼ�������
	 *
	 *
	 * @return 
	 */
	STDMETHOD(BeginTransaction)();

	/**
	 * @brief ִ��SQL�����������¼��
	 *
	 *
	 * @param bstrSqlString 
	 * @return 
	 */
	STDMETHOD(ExecCommond)(BSTR bstrSqlString);

	/**
	 * @brief ׼������
	 *
	 *
	 * @param bstrSqlString 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(PrepareCommond)(BSTR bstrSqlString,ISqlite3Statement** pVal);

	/**
	 * @brief �ύ�������
	 *
	 *
	 * @return 
	 */
	STDMETHOD(CommitTransaction)();

	/**
	 * @brief �ع��������
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(RollbackTransaction)(void);

	/**
	 * @brief ����Ƿ���������
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_TransactionStatus)(VARIANT_BOOL* pVal);
	
	/**
	 * @brief ִ�в�ѯ���
	 *
	 *
	 * @param bstrSqlString SQL���
	 * @return 
	 */
	STDMETHOD(ExecQuery)(BSTR bstrSqlString);

	/**
	 * @brief ִ�в�ѯ���
	 *
	 *
	 * @param bstrSqlString SQL���
	 * @param pVal �������ӿڶ���
	 * @return 
	 */
	STDMETHOD(ExecQueryEx)(BSTR bstrSqlString,ISqlite3Statement** pVal);

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
	 * @brief ��ȡ�Ƿ�ֻ����
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_ReadOnly)(VARIANT_BOOL* pVal);

	/**
	 * @brief ����ָ���ֶ���Ż���ֶ�����
	 *
	 *
	 * @param lFieldIndex 
	 * @param pbstrName 
	 * @return 
	 */
	STDMETHOD(GetFieldName)(LONG lFieldIndex, BSTR* pbstrName);

	/**
	 * @brief �ƶ���¼λ�õ���һ��
	 *
	 *
	 * @param void 
	 * @return 
	 */
	STDMETHOD(NextRow)(void);

	/**
	 * @brief ָ���ֶ���Ż���ֶζ�Ӧ�ļ�¼ֵ
	 *
	 *
	 * @param lFieldIndex 
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(GetValueString)(LONG lFieldIndex, BSTR* pVal);

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
	 * @brief ������һ�β����ĳ�����Ϣ
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);

	/**
	 * @brief �ر����ݿ�
	 *
	 *
	 * @return 
	 */
	STDMETHOD(Close)();
	
	/**
	 * @brief ������ݿ������汾
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_DriverVersion)(BSTR* pVal);
	
	/**
	 * @brief ������ݿ�汾
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_DBVersion)(BSTR* pVal);

	/**
	 * @brief ��õ�ǰ�򿪵������ļ�·��
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_DbPathFile)(BSTR* pVal);
	
	/**
	 * @brief ϵͳ����
	 *
	 *
	 * @return 
	 */
	STDMETHOD(SysBackup)(BSTR bstrFileName);

	/**
	 * @brief ѹ�����ݿ�
	 *
	 *
	 * @return 
	 */
	STDMETHOD(CompressDB)();
};

OBJECT_ENTRY_AUTO(__uuidof(Sqlite3Connect), CSqlite3Connect)
