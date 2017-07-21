// ConnectHelper.cpp : CConnectHelper ��ʵ��

#include "stdafx.h"
#include "ConnectHelper.h"
#include "BaseFuncLib.h"

// CConnectHelper

HRESULT CConnectHelper::CreateOptionTable(CComPtr<ISqlite3Connect>& spiSqlite3Connect,DWORD dwVersion)
{
	HRESULT hRet(E_FAIL);
	ATLASSERT(spiSqlite3Connect);
	VARIANT_BOOL bExistFlag = VARIANT_FALSE;
	CString strSqlCmd(_T("")),strTemp(_T(""));
	hRet = spiSqlite3Connect->TableIsExist(CComBSTR(SQLITEDB_TABLE_SYSPARA),VARIANT_FALSE,&bExistFlag);
	if(VARIANT_TRUE != bExistFlag)
	{
		strSqlCmd.Format(_T("CREATE TABLE [%s] (%s VARCHAR DEFAULT \'\' NOT NULL PRIMARY KEY UNIQUE,%s VARCHAR DEFAULT \'\' NOT NULL);"),\
			SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_OPTIONRECORD_FIELDNAME2);
		hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
		strSqlCmd.Empty();
		if(FAILED(hRet))
		{
#ifdef _DEBUG
			CComBSTR bstrErrInfo;
			spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
			::Sleep(1);
#endif
			return hRet;///�޷��������ݿ��
		}
	}
	/// �����¼,���浱ǰ���ݿ����汾
	strTemp.Format(_T("%ld"),dwVersion);
	strSqlCmd.Format(_T("insert into [%s] values (\'%s\',\'%s\');"),\
		SQLITEDB_TABLE_SYSPARA,SQLITEDB_OPTIONNAME_DBVERSION,strTemp);
	hRet = spiSqlite3Connect->ExecCommond(CComBSTR(strSqlCmd));
	strSqlCmd.Empty();
	return hRet;
}

HRESULT CConnectHelper::CreateDefaultDB(CComBSTR bstrDBPath,CComBSTR bstrPW,ISqlite3Connect** ppVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	if(!bstrDBPath || !ppVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	HRESULT hRet(E_FAIL);
	CComPtr<ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr<IClassFactory>	cf = NULL;
	hRet = DllGetClassObject( __uuidof(Sqlite3Connect), __uuidof(IClassFactory), (LPVOID *)&cf );
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(ISqlite3Connect), (VOID **)&spiSqlite3Connect );
		cf = NULL;
	}
	if(FAILED(hRet) || !spiSqlite3Connect)
	{
		/// �������ݿ���ʽӿ�ʧ��
		m_strLastErrInfo = _T("�������ݿ���ʽӿ�ʧ�ܡ�");
		return E_FAIL;
	}
	try
	{
		hRet = spiSqlite3Connect->Open(bstrDBPath,bstrPW,VARIANT_TRUE);
	}
	catch ( ... )
	{
		/// �쳣
		hRet = E_FAIL;
	}
	if(FAILED(hRet))
	{
		/// �������ļ�ʧ�ܣ���ó�����Ϣ
		CComBSTR bstrErrInfo;
		hRet = spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
		if(SUCCEEDED(hRet))
		{
			if(bstrErrInfo.Length())
				m_strLastErrInfo = bstrErrInfo.m_str;
		}
		if(m_strLastErrInfo.IsEmpty())
			m_strLastErrInfo = _T("δ֪ԭ������ݿ��ļ�ʧ�ܡ�");
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	hRet = CreateOptionTable(spiSqlite3Connect);
	if(FAILED(hRet))
	{
		//����ȱʡ���ݿ�ʧ��
		spiSqlite3Connect->Close();
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	/// ��ѯ���ؽӿ�ָ��
	hRet = spiSqlite3Connect->QueryInterface(IID_ISqlite3Connect,(LPVOID *)ppVal);
	spiSqlite3Connect = NULL;
	return hRet;
}

STDMETHODIMP CConnectHelper::OpenDB(BSTR bstrDbPath,VARIANT_BOOL bCreateFlag,\
	short sOverTimeSecond,BSTR bstrPW,ISqlite3Connect** ppVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErrInfo.Empty();
	if(!bstrDbPath || !ppVal)
	{
		m_strLastErrInfo = _T("��Ч��������");
		return ERROR_INVALID_PARAMETER;
	}
	ULONGLONG dwFileSize = 0;
	BOOL bExistFlag = CBaseFuncLib::IsPathExist(bstrDbPath);
	if(bExistFlag)
	{
		/// ������Ϊ�������⵼�´�ʧ��
		dwFileSize = CBaseFuncLib::GetFileSize(bstrDbPath);
		int nIndex = 100;
		while(0 == dwFileSize && nIndex > 0)
		{
			::Sleep(100);
			nIndex--;
			dwFileSize = CBaseFuncLib::GetFileSize(bstrDbPath);
		}
	}
	if(!bExistFlag || !dwFileSize)
	{
		if(bExistFlag && !dwFileSize)
		{
			/// �쳣���ݿ�
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("ConnectHelper::OpenDB %s �쳣���ݿ�"),\
				CString(bstrDbPath));
			WRITELOGTOFILE(strLogInfo);
#endif
		}

		if(VARIANT_FALSE == bCreateFlag)
		{

			m_strLastErrInfo.Format(_T("���ݿ��ļ� %s �����ڻ��쳣"),CString(bstrDbPath));
			//�����Ȩ���һ�ΰ�װʱ����û�����ݿ��ļ�����ȡ��Ȩ��ʧ�ܵ�����
			if (!bExistFlag)
			{
				return ERROR_FILE_NOT_FOUND;
			}

			return E_FAIL;
		}
		return CreateDefaultDB(bstrDbPath,bstrPW,ppVal);
	}
	HRESULT hRet(E_FAIL);
	CComPtr<ISqlite3Connect> spiSqlite3Connect = NULL;
	CComPtr<IClassFactory>	cf = NULL;
	hRet = DllGetClassObject(__uuidof(Sqlite3Connect),__uuidof(IClassFactory),(LPVOID *)&cf);
	if(cf != NULL)
	{
		hRet = cf->CreateInstance( NULL, __uuidof(ISqlite3Connect), (VOID **)&spiSqlite3Connect );
		cf = NULL;
	}
	if(FAILED(hRet) || !spiSqlite3Connect)
	{
		/// �������ݿ���ʽӿ�ʧ��
		m_strLastErrInfo.Format(_T("%s��ʧ�ܣ������룺%ld"),CString(bstrDbPath),hRet);
		return E_FAIL;
	}
	try
	{
		if(sOverTimeSecond)
			spiSqlite3Connect->put_TimeoutSecond(sOverTimeSecond);

		hRet = spiSqlite3Connect->Open(bstrDbPath,bstrPW,bCreateFlag);
	}
	catch ( ... )
	{
		/// �쳣
		hRet = E_FAIL;
		m_strLastErrInfo.Format(_T("%s ���ݿ���쳣"),CString(bstrDbPath));
	}
	if(FAILED(hRet))
	{
		/// �������ļ�ʧ�ܣ���ó�����Ϣ
		CComBSTR bstrErrInfo;
		if(m_strLastErrInfo.IsEmpty())
			hRet = spiSqlite3Connect->get_LastErrorInfo(&bstrErrInfo);
		if(SUCCEEDED(hRet) && bstrErrInfo.Length())
		{
			m_strLastErrInfo = bstrErrInfo.m_str;
			bstrErrInfo.Empty();
		}
		if(m_strLastErrInfo.IsEmpty())
			m_strLastErrInfo.Format(_T("%s��ʧ�ܣ�δ֪����ԭ��"),CString(bstrDbPath));
		spiSqlite3Connect = NULL;
		return E_FAIL;
	}
	/// ������ݿ������������Ҫȡ�汾�����жϣ��޸İ汾��SQLITEDB_OPTIONVALUE_DEFAULTVERSION��ͨ�������������ݿ�
	long lDBVersion = 0;
	CString strSqlString(_T(""));
	strSqlString.Format(_T("SELECT %s FROM [%s] WHERE %s='%s'"),\
		SQLITEDB_OPTIONRECORD_FIELDNAME2,SQLITEDB_TABLE_SYSPARA,\
		SQLITEDB_OPTIONRECORD_FIELDNAME1,SQLITEDB_OPTIONNAME_DBVERSION);
	if(SUCCEEDED(spiSqlite3Connect->ExecQuery(CComBSTR(strSqlString))))
	{
		while(SUCCEEDED(spiSqlite3Connect->NextRow()))
		{
			spiSqlite3Connect->GetValueInt(0,&lDBVersion);
			break;
		}
	}
	if(!lDBVersion && VARIANT_FALSE == bCreateFlag)
	{
		spiSqlite3Connect = NULL;
		m_strLastErrInfo.Format(_T("�� %s ʱδ���ְ汾��Ϣ"),(CString)bstrDbPath);
		return E_FAIL;
	}
	/// ������ݿ������������Ҫȡ�汾�����жϣ�ͨ�������������ݿ�
	if(!lDBVersion && VARIANT_TRUE == bCreateFlag && NULL == bstrPW)
	{
		/// ���ݿⴴ���Ƿ�
		spiSqlite3Connect = NULL;
#ifdef _DEBUG
		///�ж����ĸ�Ӧ�õ��ã�����Ƿ��񣬲�����
		TCHAR szModemName[MAX_PATH];
		::memset(szModemName,0,MAX_PATH*sizeof(TCHAR));
		::GetModuleFileName(NULL,szModemName,MAX_PATH*sizeof(TCHAR));
		CString strAppName(szModemName);
		if(-1 == strAppName.Find(TDHXKJ_HOSTSERVICE_NAME))
		{
			/// �������ģʽ������ʾ������������
			CString strMessage(_T(""));
			strMessage.Format(_T("���ݿ��ļ� %s �����쳣��\r\n���ܸ�ʽ���ƻ����Ƿ�ɾ���ؽ���"),(CString )bstrDbPath);
			if(::MessageBox(::GetForegroundWindow(),strMessage,_T("��ʾ"),
				MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				::DeleteFile(bstrDbPath);
				return CreateDefaultDB(bstrDbPath,bstrPW,ppVal);
			}
		}
#endif
		m_strLastErrInfo.Format(_T("�����ݿ�ʱִ�� %s ����"),strSqlString);
		return E_FAIL;
	}
	if(NULL != spiSqlite3Connect)
	{
		/// ��ѯ���ؽӿ�ָ��
		hRet = spiSqlite3Connect->QueryInterface(IID_ISqlite3Connect,(LPVOID *)ppVal);
		spiSqlite3Connect = NULL;
	}
	return hRet;
}

STDMETHODIMP CConnectHelper::get_LastErrorInfo(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!pVal)
		return ERROR_INVALID_PARAMETER;
	if(!m_strLastErrInfo.IsEmpty())
	{
		CComBSTR bstrErr(m_strLastErrInfo);
		bstrErr.CopyTo(pVal);

		return S_OK;
	}
	/// �޳�����Ϣ
	return E_FAIL;
}

STDMETHODIMP CConnectHelper::InitDBEnv(BSTR bstrDbPath)
{
	// TODO: �ڴ����ʵ�ִ���
	if(!bstrDbPath)
		return E_POINTER;
	CBaseFuncLib::GetAppDir(TDHXKJ_HOSTSERVICE_NAME);
	return S_OK;
}