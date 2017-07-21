// ServiceControl.cpp : CServiceControl ��ʵ��

#include "stdafx.h"
#include "ServiceControl.h"
#include "BaseFuncLib.h"
#include "SqliteHelper.h"
#include "SKFHelper.h"
#include "HXRegKey.h"
#include "PathNtDosMapper.h"
#include "ProcessMrg.h"
#include "shlobj.h"

static BOOL	g_bSetIO = FALSE;

#ifndef WIN64
/// ���64λ�ض�������
PVOID				g_OldValue = NULL;

lpWow64DisableWow64FsRedirection	g_lpDisabelWow64	= NULL;
lpWow64RevertWow64FsRedirection		g_EnabelWow64		= NULL;
#endif

// CServiceControl

STDMETHODIMP CServiceControl::PutAccountMgr(IDispatch* piAccountMgr)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == piAccountMgr)
		return E_FAIL;
	m_spiAccountMgr = NULL;
	HRESULT hRet = piAccountMgr->QueryInterface(IID_IAccountMgr,(LPVOID *)&m_spiAccountMgr);
	ATLASSERT(m_spiAccountMgr);
	return hRet;
}

STDMETHODIMP CServiceControl::GetServiceName(VARIANT_BOOL* pbDriver, BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pbDriver || NULL == pVal)
		return E_POINTER;
	(*pbDriver) = m_bDriverFlag;
	CComBSTR bstrVal(m_strServiceName);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CServiceControl::PutServiceName(VARIANT_BOOL bDriver, BSTR newVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == newVal)
		return E_POINTER;
	if(0 != m_strServiceName.CompareNoCase(newVal) && VARIANT_TRUE == m_bDriverFlag
		&& 0 == m_strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
		this->CloseDriver();
	m_strServiceName.Empty();
	m_strServiceName = newVal;
	m_bDriverFlag = bDriver;
	return S_OK;
}

CString CServiceControl::GetDriverPath()
{
	/// ����֧�ְ�ȫģʽ�������У�ֻ�ܷŵ�ϵͳ����Ŀ¼
	CString strDriverPath(_T(""));
	strDriverPath = CBaseFuncLib::GetSpecialFolderPath(CSIDL_SYSTEM);
	strDriverPath += _T("drivers\\");
	strDriverPath += TDHXKJ_SAFEDRIVER;
	return strDriverPath;
}

STDMETHODIMP CServiceControl::put_SafeBoot(VARIANT_BOOL newVal)
{
	m_bSafeBoot = newVal;
	return S_OK;
}

STDMETHODIMP CServiceControl::Add(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);		
	GetVersionEx((OSVERSIONINFO *)&osvi);
	if(VARIANT_TRUE == m_bDriverFlag)
	{
		SC_HANDLE hSCM = ::OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCM)
		{
			m_nLastErrCode = ::GetLastError();
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("��ӷ���%s�򿪿�����ʧ�ܣ������룺%d"),\
				m_strServiceName,m_nLastErrCode);
			WRITELOGTOFILE(strLogInfo);
#endif
			return E_FAIL;
		}
		/// ����ϵͳ���Ѿ�֧��BOOTģʽ����ʹ��
		DWORD dwStartFlag = SERVICE_BOOT_START;
		CString strDriverPath = GetDriverPath();
		SC_HANDLE hService = ::CreateService(
			hSCM, TDHXKJ_SAFEDRIVER_NAME, TDHXKJ_SAFEDRIVER_NAME,
			SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,dwStartFlag, SERVICE_ERROR_IGNORE,
			strDriverPath,NULL, 0, NULL, NULL, NULL);
		if(hService == NULL)
		{
			m_nLastErrCode = ::GetLastError();
			if(m_nLastErrCode != ERROR_SERVICE_EXISTS && m_nLastErrCode != ERROR_SERVICE_MARKED_FOR_DELETE)
			{
#ifdef APP_LOG_ENABLE
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("��������%sʧ�ܣ������룺%d"),\
					strDriverPath,m_nLastErrCode);
				WRITELOGTOFILE(strLogInfo);
#endif
			}
			::CloseServiceHandle(hSCM);
			hSCM = NULL;
			return E_FAIL;
		}
		else
		{
			CloseServiceHandle(hService);
			hService = NULL;
			if(VARIANT_TRUE == m_bSafeBoot)
			{
				/// ���ð�ȫģʽ������Ŀ
				CHXRegKey RegKey(_T("SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal"),TDHXKJ_SAFEDRIVER_NAME,HKEY_LOCAL_MACHINE);
				RegKey.SetRegStringVal(_T(""),_T("Driver"));
				RegKey.Close();
				RegKey.NewRegPath(_T("SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network"),TDHXKJ_SAFEDRIVER_NAME,HKEY_LOCAL_MACHINE);
				RegKey.SetRegStringVal(_T(""),_T("Driver"));
				RegKey.Close();

			}
			CString strImagePath;
			CHXRegKey RegKey(_T("SYSTEM\\CurrentControlSet\\Services"),TDHXKJ_SAFEDRIVER_NAME,HKEY_LOCAL_MACHINE);
			if(VARIANT_TRUE == m_bSafeBoot && osvi.dwMajorVersion > 5)
				strImagePath = _T("\\SystemRoot\\System32\\drivers\\");
			else
				strImagePath = _T("System32\\drivers\\");
			strImagePath += TDHXKJ_SAFEDRIVER_NAME;
			strImagePath += _T(".sys");
			RegKey.SetRegStringVal(_T("ImagePath"),strImagePath);
		}
		::CloseServiceHandle(hSCM);
		hSCM = NULL;
		return S_OK;
	}
	CString strRunExe(_T(""));
	strRunExe.Format(_T("%s%s.exe"),CComHelper::GetAppInsPath(),m_strServiceName);
	CBaseFuncLib::RunExe(strRunExe,_T("/Service"),TRUE,FALSE);
	strRunExe.Empty();
	if(VARIANT_TRUE == m_bSafeBoot)
	{
		/// ���ð�ȫģʽ������Ŀ
		CHXRegKey RegKey(_T("SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Minimal"),TDHXKJ_HOSTSERVICE_NAME,HKEY_LOCAL_MACHINE);
		RegKey.SetRegStringVal(_T(""),_T("Service"));
		RegKey.Close();
		RegKey.NewRegPath(_T("SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Network"),TDHXKJ_HOSTSERVICE_NAME,HKEY_LOCAL_MACHINE);
		RegKey.SetRegStringVal(_T(""),_T("Service"));
		RegKey.Close();
	}
	return S_OK;
}

STDMETHODIMP CServiceControl::Del(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = 0;
	ULONG unRight = 0;
	if(NULL != m_spiAccountMgr)
		m_spiAccountMgr->get_Right(&unRight);
	if(USEAUTHTYPE_UNLOAD != (unRight & USEAUTHTYPE_UNLOAD))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}
	/// ɾ��ǰ��ֹͣ
	SHORT nStatus = 0;
	this->GetServiceStatus(&nStatus);
	if(SERVICE_RUNNING == nStatus)
		this->Stop();

	if(VARIANT_TRUE == m_bDriverFlag)
	{
		SC_HANDLE hSCM = ::OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
		if (NULL == hSCM)
			return E_FAIL;
		SC_HANDLE hService = ::OpenService((SC_HANDLE)hSCM, m_strServiceName, SERVICE_ALL_ACCESS);
		if(NULL == hService)
		{
			::CloseServiceHandle(hSCM);
			hSCM = NULL;
			m_nLastErrCode = ::GetLastError();
			if(ERROR_SERVICE_DOES_NOT_EXIST == m_nLastErrCode)
				return S_OK;/// �����ڣ�ɾ�����
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("ɾ�������%sʧ�ܣ������룺%ld"),\
				m_strServiceName,m_nLastErrCode);
			WRITELOGTOFILE(strLogInfo);
#endif
			return E_FAIL;
		}
		BOOL bRet = ::DeleteService(hService);
		m_nLastErrCode = ::GetLastError();
		CloseServiceHandle(hService);
		hService = NULL;
		::CloseServiceHandle(hSCM);
		hSCM = NULL;
		if(0 == m_strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
			g_bSetIO = FALSE;
		if(!bRet)
		{
			if(ERROR_SERVICE_MARKED_FOR_DELETE != m_nLastErrCode)
			{
#ifdef APP_LOG_ENABLE
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("ɾ������%sʧ�ܣ������룺%d"),\
					m_strServiceName,m_nLastErrCode);
				WRITELOGTOFILE(strLogInfo);
#endif
			}
			return E_FAIL;
		}
		return S_OK;
	}
	CString strRunExe(_T(""));
	strRunExe.Format(_T("%s%s.exe"),CComHelper::GetAppInsPath(),m_strServiceName);
	CBaseFuncLib::RunExe(strRunExe,_T("/UnregServer"),TRUE,FALSE);
	strRunExe.Empty();
	return S_OK;
}

DWORD CServiceControl::StopService( SC_HANDLE hSCM, SC_HANDLE hService, 
	BOOL fStopDependencies, DWORD dwTimeout ) 
{
   SERVICE_STATUS ss;
   DWORD dwStartTime = GetTickCount();

   // Make sure the service is not already stopped
   if ( !QueryServiceStatus( hService, &ss ) )
      return GetLastError();

   if ( ss.dwCurrentState == SERVICE_STOPPED ) 
      return ERROR_SUCCESS;

   // If a stop is pending, just wait for it
   while ( ss.dwCurrentState == SERVICE_STOP_PENDING ) 
   {
      Sleep( ss.dwWaitHint );
      if ( !QueryServiceStatus( hService, &ss ) )
         return GetLastError();

      if ( ss.dwCurrentState == SERVICE_STOPPED )
         return ERROR_SUCCESS;

      if ( GetTickCount() - dwStartTime > dwTimeout )
         return ERROR_TIMEOUT;
   }

   // If the service is running, dependencies must be stopped first
   if ( fStopDependencies ) 
   {
      DWORD i = 0;
      DWORD dwBytesNeeded = 0;
      DWORD dwCount = 0;

      LPENUM_SERVICE_STATUS   lpDependencies = NULL;
      ENUM_SERVICE_STATUS     ess;
      SC_HANDLE               hDepService = NULL;

      // Pass a zero-length buffer to get the required buffer size
      if ( EnumDependentServices( hService, SERVICE_ACTIVE, 
         lpDependencies, 0, &dwBytesNeeded, &dwCount ) ) 
	  {
         // If the Enum call succeeds, then there are no dependent
         // services so do nothing
      } 
	  else 
	  {
         if ( GetLastError() != ERROR_MORE_DATA )
            return GetLastError(); // Unexpected error

         // Allocate a buffer for the dependencies
         lpDependencies = (LPENUM_SERVICE_STATUS) HeapAlloc( 
               GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded );

         if ( !lpDependencies )
            return GetLastError();

         __try 
		 {
            // Enumerate the dependencies
            if ( !EnumDependentServices( hService, SERVICE_ACTIVE, 
                  lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                  &dwCount ) )
               return GetLastError();

            for ( i = 0; i < dwCount; i++ ) 
			{
               ess = *(lpDependencies + i);
               // Open the service
               hDepService = ::OpenService( hSCM, ess.lpServiceName, 
                     SERVICE_STOP | SERVICE_QUERY_STATUS );
               if ( !hDepService )
                  return GetLastError();

               __try 
			   {
                  // Send a stop code
                  if ( !ControlService( hDepService, SERVICE_CONTROL_STOP,&ss ) )
                     return GetLastError();

                  // Wait for the service to stop
                  while ( ss.dwCurrentState != SERVICE_STOPPED ) 
				  {
                     Sleep( ss.dwWaitHint );
                     if ( !QueryServiceStatus( hDepService, &ss ) )
                        return GetLastError();
                     if ( ss.dwCurrentState == SERVICE_STOPPED )
                        break;
                     if ( GetTickCount() - dwStartTime > dwTimeout )
                        return ERROR_TIMEOUT;
                  }
               } __finally 
			   {
                  // Always release the service handle
                  CloseServiceHandle( hDepService );
               }
            }
         } __finally 
		 {
            // Always free the enumeration buffer
            HeapFree( GetProcessHeap(), 0, lpDependencies );
         }
      } 
   }
   // Send a stop code to the main service
   if ( !ControlService( hService, SERVICE_CONTROL_STOP, &ss ) )
      return GetLastError();

   // Wait for the service to stop
   while ( ss.dwCurrentState != SERVICE_STOPPED ) 
   {
      Sleep( ss.dwWaitHint );
      if ( !QueryServiceStatus( hService, &ss ) )
         return GetLastError();
      if ( ss.dwCurrentState == SERVICE_STOPPED )
         break;
      if ( GetTickCount() - dwStartTime > dwTimeout )
         return ERROR_TIMEOUT;
   }

   // Return success
   return ERROR_SUCCESS;
}

STDMETHODIMP CServiceControl::get_LastErrorInfo(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_FAIL;
	CComBSTR bstrVal(m_strLastErr);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CServiceControl::get_LastErrorCode(ULONG* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pVal)
		return E_FAIL;
	(*pVal) = m_nLastErrCode;
	return S_OK;
}

BOOL CServiceControl::ControlStatus(const CString& strServiceName,BOOL bStartFlag)
{
	BOOL bFlag = FALSE;
	SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_CONNECT);
	if (NULL == scm)
		return bFlag;
	SC_HANDLE svc = ::OpenService(scm,strServiceName,SERVICE_ALL_ACCESS);
	if (NULL == svc)
	{
		m_nLastErrCode = ::GetLastError();
		::CloseServiceHandle(scm);
		scm = NULL;

		if(0 == strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
		{
			CHXRegKey regKey(TDHXKJ_HOSTSERVICE_REGPATH,TDHXKJ_CONFIG_NODENAME,HKEY_LOCAL_MACHINE);
			regKey.SetRegDwordVal(TDHXKJ_LOCKUI_DRIVERERR,m_nLastErrCode);
		}

		if(ERROR_SERVICE_DOES_NOT_EXIST != m_nLastErrCode)
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("���Ʒ���״̬��%sʧ�ܣ������룺%ld"),\
				strServiceName,m_nLastErrCode);
			WRITELOGTOFILE(strLogInfo);
#endif
		}
		return bFlag;
	}
	SERVICE_STATUS ServiceStatus;
	if(bStartFlag)
	{
		bFlag = ::QueryServiceStatus(svc,&ServiceStatus);
		if(ServiceStatus.dwCurrentState == SERVICE_STOPPED || ServiceStatus.dwCurrentState == SERVICE_PAUSED)
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			if(0 == strServiceName.CompareNoCase(TDHXKJ_HOSTSERVICE_NAME))
				strLogInfo = _T("��������������ȫ��������");
			else if(0 == strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
				strLogInfo = _T("��������������ȫ��������");
			else
				strLogInfo.Format(_T("������������ %s"),strServiceName);
			WRITELOGTOFILE(strLogInfo);
#endif
			/// ��������Service
			bFlag = ::StartService(svc,0,NULL);
			if(!bFlag)
			{
				m_nLastErrCode = ::GetLastError();

				LPVOID lpMsgBuf = NULL;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,m_nLastErrCode,0,(LPTSTR)&lpMsgBuf,0,NULL );
				m_strLastErr = (LPTSTR)lpMsgBuf;
				LocalFree(lpMsgBuf);
				lpMsgBuf = NULL;

				if(0 == strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
				{
					CHXRegKey regKey(TDHXKJ_HOSTSERVICE_REGPATH,TDHXKJ_CONFIG_NODENAME,HKEY_LOCAL_MACHINE);
					regKey.SetRegDwordVal(TDHXKJ_LOCKUI_DRIVERERR,m_nLastErrCode);
				}

				if(ERROR_SERVICE_ALREADY_RUNNING != m_nLastErrCode && ERROR_NOT_SAFEBOOT_SERVICE != m_nLastErrCode 
					&& ERROR_SERVICE_DEPENDENCY_FAIL != m_nLastErrCode)
				{
#ifdef APP_LOG_ENABLE
					CString strLogInfo(_T(""));
					strLogInfo.Format(_T("���� %s ʧ��,�����룺%ld��ԭ��%s"),\
						strServiceName,m_nLastErrCode,m_strLastErr);
					WRITELOGTOFILE(strLogInfo);
#endif
				}
			}
			else
			{
				int iIndex = 0;
				/// �ȴ��������
				bFlag = ::QueryServiceStatus(svc,&ServiceStatus);
				while(bFlag && ServiceStatus.dwCurrentState != SERVICE_RUNNING)
				{
					::Sleep(100);
					bFlag = ::QueryServiceStatus(svc,&ServiceStatus);
					iIndex++;
					if(iIndex > 50)
						break;
				}
			}
		}
		else
		{
			bFlag = FALSE;
			/// ���ñ�Ǵ���
			if(ServiceStatus.dwCurrentState == SERVICE_STOP_PENDING)
			{
				m_nLastErrCode = ERROR_SERVICE_MARKED_FOR_DELETE;
				CHXRegKey regKey(TDHXKJ_HOSTSERVICE_REGPATH,TDHXKJ_CONFIG_NODENAME,HKEY_LOCAL_MACHINE);
				regKey.SetRegDwordVal(TDHXKJ_LOCKUI_DRIVERERR,m_nLastErrCode);
			}
		}
	}
	else
	{
		bFlag = ::QueryServiceStatus(svc,&ServiceStatus);
		if (bFlag && (ServiceStatus.dwCurrentState == SERVICE_RUNNING || ServiceStatus.dwCurrentState == SERVICE_START_PENDING))
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			if(0 == strServiceName.CompareNoCase(TDHXKJ_HOSTSERVICE_NAME))
				strLogInfo = _T("����ֹͣ������ȫ��������");
			else if(0 == strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
				strLogInfo = _T("����ֹͣ������ȫ��������");
			else
				strLogInfo.Format(_T("����ֹͣ���� %s"),strServiceName);
			WRITELOGTOFILE(strLogInfo);
#endif
			m_nLastErrCode = StopService(scm,svc,FALSE,1000*5);
			if(ERROR_SUCCESS == m_nLastErrCode)
				bFlag = TRUE;
		}
	}

	::CloseServiceHandle(svc);
	svc = NULL;
	::CloseServiceHandle(scm);
	scm = NULL;
	return bFlag;
}

STDMETHODIMP CServiceControl::Start(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	HRESULT hRet(E_FAIL);
	SHORT nServiceStatus = 0;
	this->GetServiceStatus(&nServiceStatus);
	if(SERVICE_RUNNING == nServiceStatus || SERVICE_START_PENDING == nServiceStatus)
		return S_OK;/// �Ѿ���ʼ����
	BOOL bRetFlag = ControlStatus(m_strServiceName,TRUE);
	if(!bRetFlag)
		return E_FAIL;

	if(VARIANT_TRUE == m_bDriverFlag && FALSE == g_bSetIO &&
		0 == m_strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
	{
		/// ����IO���ƽ���
		CComPtr <ISafeService> spiSafeService = CSKFHelper::GetSafeService();
		if(NULL == spiSafeService)
			return S_OK;
		SET_DI_CONTROL sdc = {0};
		GetModuleFileName(NULL, sdc.szPath, sizeof(sdc.szPath));
		spiSafeService->FileMD5(CComBSTR(sdc.szPath),sdc.digest);

		/// ���ݴ�����Ϣ
		DWORD dwErrCode = m_nLastErrCode;
		CString strErrInfo = m_strLastErr;
		/// �ж������������豸Ψһ�ɿصĽ��̲���
		hRet = SendControlToDevice(UvProt_SetDIControl,(BYTE *)&sdc,sizeof(sdc));
#ifdef APP_LOG_ENABLE
		if(FAILED(hRet))
		{
			WRITELOGTOFILE(_T("��������IO����ʧ�ܡ�"));
		}
#endif
		m_strLastErr.Empty();
		m_nLastErrCode = dwErrCode;
		m_strLastErr = strErrInfo;
	}
	return S_OK;
}

STDMETHODIMP CServiceControl::Stop(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	ULONG unRight = 0;
	if(NULL != m_spiAccountMgr)
		m_spiAccountMgr->get_Right(&unRight);
	if(USEAUTHTYPE_SELFSAFE != (unRight & USEAUTHTYPE_SELFSAFE))
	{
		m_strLastErr = _T("û��Ȩ��ִ�д˲�����");
		return E_FAIL;/// û�в���Ȩ��
	}
	if(VARIANT_TRUE == m_bDriverFlag 
		&& 0 == m_strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
	{
		g_bSetIO = FALSE;
		CloseDriver();/// ֹͣ����ǰ���ȹر������豸����
	}
	SHORT nServiceStatus = 0;
	this->GetServiceStatus(&nServiceStatus);
	if(SERVICE_STOPPED == nServiceStatus || SERVICE_STOP_PENDING == nServiceStatus)
		return S_OK;/// ��ʼֹͣ��׼��ֹͣ
	BOOL bRetFlag = ControlStatus(m_strServiceName,FALSE);
	if(!bRetFlag)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CServiceControl::GetServiceStatus(SHORT* pnStatus)
{
	// TODO: �ڴ����ʵ�ִ���
	HRESULT hRet(E_FAIL);
	SC_HANDLE scm,svc;

	(*pnStatus) = 0;
	scm = ::OpenSCManager(NULL,NULL,SERVICE_QUERY_STATUS);
	if(NULL == scm)
		return hRet;
	svc = ::OpenService(scm,m_strServiceName,SERVICE_QUERY_STATUS);
	if (NULL == svc)
	{
		if(ERROR_SERVICE_DOES_NOT_EXIST != ::GetLastError())
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("��ѯ����״̬��%sʧ�ܣ������룺%ld"),\
				m_strServiceName,::GetLastError());
			WRITELOGTOFILE(strLogInfo);
#endif
		}
		return hRet;
	}
	SERVICE_STATUS ServiceStatus;
	BOOL bRetFlag = ::QueryServiceStatus(svc,&ServiceStatus);
	if(bRetFlag)
	{
		(*pnStatus) = (SHORT)ServiceStatus.dwCurrentState;
		hRet = S_OK;
	}
	::CloseServiceHandle(svc);
	svc = NULL;
	::CloseServiceHandle(scm);
	scm = NULL;
	return hRet;
}

STDMETHODIMP CServiceControl::IsExist(VARIANT_BOOL* pbFlag)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pbFlag)
		return E_POINTER;
	(*pbFlag) = VARIANT_FALSE;

	HRESULT hRet(S_OK);
	SC_HANDLE scm,svc;
	scm = ::OpenSCManager(NULL,NULL,SERVICE_QUERY_STATUS);
	if(NULL == scm)
		return E_FAIL;
	svc = ::OpenService(scm,m_strServiceName,SERVICE_QUERY_STATUS);
	if (NULL == svc)
	{
		::CloseServiceHandle(scm);
		return hRet;
	}
	(*pbFlag) = VARIANT_TRUE;
	::CloseServiceHandle(svc);
	svc = NULL;
	::CloseServiceHandle(scm);
	scm = NULL;
	return S_OK;
}

/// IsDebuggerPresent ����Ƿ����ģʽ�����ⱻ����
HANDLE CServiceControl::OpenSafeDriver(const CString& strDriverName,\
	DWORD dwFlag,DWORD dwRight)
{
	CString strDevicePath(_T(""));
	/// windows 2000 ��ʼ��Global·��
	strDevicePath.Format(_T("\\\\.\\Global\\%s"),strDriverName);
	HANDLE hDevice = ::CreateFile( strDevicePath, dwRight,\
		0,NULL,OPEN_EXISTING, dwFlag, NULL);
	CHXRegKey regKey(TDHXKJ_HOSTSERVICE_REGPATH,TDHXKJ_CONFIG_NODENAME,HKEY_LOCAL_MACHINE);
	if( NULL == hDevice || hDevice == INVALID_HANDLE_VALUE )
	{
		m_nLastErrCode = ::GetLastError();
		regKey.SetRegDwordVal(TDHXKJ_LOCKUI_DRIVERERR,m_nLastErrCode);

#ifdef APP_LOG_ENABLE
		CString strLogText;
		strLogText.Format(_T("������ʧ�ܣ�������:%ld"),m_nLastErrCode);
		WRITELOGTOFILE(strLogText);
		strLogText.Empty();
#endif
	}
	else
		regKey.DelKeyName(TDHXKJ_LOCKUI_DRIVERERR);

	regKey.Close();
	return hDevice;
}

STDMETHODIMP CServiceControl::OpenDriver(BSTR bstrDriverName,VARIANT_BOOL bOverlapped)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	CString strDriverName(_T(""));
	if(NULL != bstrDriverName)
		strDriverName = bstrDriverName;
	else
		strDriverName = TDHXKJ_SAFEDRIVER_NAME;
	HANDLE hDevice = OpenSafeDriver(strDriverName,\
		VARIANT_TRUE == bOverlapped ? FILE_FLAG_OVERLAPPED : FILE_ATTRIBUTE_NORMAL,GENERIC_READ|GENERIC_WRITE);
	if ( hDevice == INVALID_HANDLE_VALUE )
		return E_FAIL;
	m_hDriverDevice = hDevice;/// ��¼�豸���
	return S_OK;
}

STDMETHODIMP CServiceControl::SendUsbCmdToDriver(USHORT usCmd)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	HRESULT hRet = SendControlToDevice(UvProt_SetUDiskAccess,(BYTE *)&usCmd,sizeof(USHORT));
	return hRet;
}

STDMETHODIMP CServiceControl::SendControlToDevice(ULONG unCmd, BYTE* pData, USHORT uDataLen)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL == pData || uDataLen < 1)
		return E_POINTER;
	if(NULL == m_hDriverDevice)
	{
		/// ����豸�Ƿ�����
		m_bDriverFlag = VARIANT_TRUE;
		m_strServiceName = TDHXKJ_SAFEDRIVER_NAME;
		VARIANT_BOOL bExistFlag = VARIANT_FALSE;
		this->IsExist(&bExistFlag);
		if(VARIANT_FALSE == bExistFlag)
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("�豸�������� %ld ʧ�ܣ�������δ��װ"),\
				unCmd);
			WRITELOGTOFILE(strLogInfo);
#endif
			return S_OK;
		}
		/// ������������
		SHORT nStatus = 0;
		this->GetServiceStatus(&nStatus);
		if(SERVICE_RUNNING != nStatus && SERVICE_START_PENDING != nStatus)
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("�豸�������� %ld ʧ�ܣ�����״̬����״̬��%ld"),\
				unCmd,nStatus);
			WRITELOGTOFILE(strLogInfo);
#endif
			return S_OK;
		}
		int nIndex = 100;
		while(SERVICE_START_PENDING == nStatus && nIndex > 0)
		{
			::Sleep(100);
			nIndex--;
			this->GetServiceStatus(&nStatus);
		}
		HANDLE hDevice = this->OpenSafeDriver(m_strServiceName,FILE_ATTRIBUTE_NORMAL,GENERIC_WRITE);
		if ( hDevice != INVALID_HANDLE_VALUE && NULL != hDevice)
			m_hDriverDevice = hDevice;
	}
	if(NULL == m_hDriverDevice)
	{
		return E_FAIL;
	}
	DWORD dwBytesReturned = 0;
	BOOL bResult = ::DeviceIoControl(m_hDriverDevice,unCmd,pData,\
		uDataLen,NULL,0,&dwBytesReturned,NULL);
	if( !bResult )
	{
		m_nLastErrCode = GetLastError();
		if(UvProt_SetDIControl != unCmd)
		{
#ifdef APP_LOG_ENABLE
			//// д��־
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("�豸ָ�� %ld ����ʧ�ܣ������룺%ld"),\
				unCmd,m_nLastErrCode);
			WRITELOGTOFILE(strLogInfo);
#endif
			return E_FAIL;
		}
	}
	else
	{
		if(UvProt_SetDIControl == unCmd && FALSE == g_bSetIO && 0 == m_strServiceName.CompareNoCase(TDHXKJ_SAFEDRIVER_NAME))
			g_bSetIO = TRUE;
	}
	return S_OK;
}

STDMETHODIMP CServiceControl::CloseDriver(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(NULL != m_hDriverDevice)
	{
		::CloseHandle(m_hDriverDevice);
		m_hDriverDevice = NULL;
	}
	return S_OK;
}

UINT CServiceControl::DealMsgThread(LPVOID pParam)
{
	CServiceControl* pThis = (CServiceControl* )pParam;
	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#ifndef WIN64
	/// ֧��64λϵͳ
	HMODULE	hKernel	= NULL;
	if(CBaseFuncLib::IsWin64(hKernel))
	{
		g_lpDisabelWow64 = (lpWow64DisableWow64FsRedirection)GetProcAddress(\
			hKernel,"Wow64DisableWow64FsRedirection"); 
		g_EnabelWow64 = (lpWow64RevertWow64FsRedirection)GetProcAddress(\
			hKernel,"Wow64RevertWow64FsRedirection");
	}
	if(NULL != g_lpDisabelWow64)
		g_lpDisabelWow64(&g_OldValue);
	hKernel = NULL;
#endif
	pThis->DealMsgProc();
#ifndef WIN64
	if(NULL != g_EnabelWow64 && NULL != g_OldValue)
		g_EnabelWow64(g_OldValue);
	g_EnabelWow64 = NULL;
	g_lpDisabelWow64 = NULL;
#endif
	::CoUninitialize();
	return 0;
}

void CServiceControl::DealMsgProc(void)
{
	DWORD numMsgs = 0;
	BYTE *buf = NULL;
	m_hsgDealMsgEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL );

	while (m_bReadDriver && NULL != m_hsgDealMsgEvent) 
	{
		m_sGlobalCriticalSection.Lock();
		MEMORY_LIST::iterator it = m_MsgList.begin();
		if(it != m_MsgList.end())
		{
			buf = (*it);
			/// ժ��
			m_MsgList.erase(it);
		}
		m_sGlobalCriticalSection.Unlock();

		BYTE *pStartBuf = buf;
		if(NULL != buf)
		{
			DWORD dwVal = 0;
			CHXRegKey regKey(TDHXKJ_HOSTSERVICE_REGPATH,TDHXKJ_CONFIG_NODENAME,HKEY_LOCAL_MACHINE);
			DWORD dwSafeGuard = 0;
			regKey.GetRegDwordVal(TDHXKJ_CONFIG_SAFEGUARD,dwSafeGuard);

			// ������Ϣ���ʼ������
			numMsgs = *((DWORD*) buf);
#ifdef APP_LOG_ENABLE
			//CString strLogInfo(_T(""));
			//strLogInfo.Format(_T("����֪ͨ���ݸ�����%ld"),numMsgs);
			//WRITELOGTOFILE(strLogInfo);
#endif
			/// ƫ���ڴ�
			buf = buf + sizeof(DWORD);
			for (;numMsgs > 0; numMsgs--, buf += HSG_MSG_BUF_SIZE) 
			{
				SHORT nFlag = 0;
				PHSG_MSG_LOG pMsg = (PHSG_MSG_LOG)buf;
				///������־����m_LogFlag��õ�ǰ��ȫ����
				/*CString strstr;
				strstr.Format(_T("��������ȡ����־���ͣ�%d\n"),pMsg->m_LogFlag);
				OutputDebugString(strstr);*/

				switch(pMsg->m_LogFlag)
				{
				case LOG_DENY_RUN_NON_WHITE:
				case LOG_ALLOW_RUN_NON_WHITE:
					regKey.GetRegDwordVal(TDHXKJ_CONFIG_RUNCONTROL,dwVal);
					nFlag = (SHORT)dwVal;
					break;
				case LOG_RUN_WHITE:
					/// ����Ҫ����
					break;
				case LOG_VIOLATE_PROTECT_REG:
				case LOG_VIOLATE_PROTECT_DIR:
				case LOG_TERMINATE_PROTECT_EXE:
					regKey.GetRegDwordVal(TDHXKJ_CONFIG_SAFEFLAG,dwVal);
					nFlag = (SHORT)dwVal;
					break;
				case LOG_VIOLATE_USB_ACCESS:
					regKey.GetRegDwordVal(TDHXKJ_CONFIG_UCONTROL,dwVal);
					nFlag = (SHORT)dwVal;
					break;
				default:
					break;
				}
				FILETIME fileTime;
				fileTime.dwHighDateTime = pMsg->m_MsgTime.HighPart;
				fileTime.dwLowDateTime = pMsg->m_MsgTime.LowPart;
				SYSTEMTIME sysTime;
				COleDateTime LogTime = COleDateTime::GetCurrentTime();
				BOOL bTransFlag = ::FileTimeToSystemTime(&fileTime,&sysTime);
				if(bTransFlag)
				{
					LogTime = COleDateTime(sysTime);
				}
				if(pMsg->m_LogFlag >= LOG_DENY_RUN_NON_WHITE)
				{
					if(LOG_INSTALL_DISKFILE == pMsg->m_LogFlag)
					{
						CString strCurPath = CBaseFuncLib::GetModulePath();
						CString strRunPath(pMsg->m_Target);
						strRunPath.MakeLower();
						/// ��ʱĿ¼�ĳ���֪ͨ
						if(-1 == strRunPath.Find(_T("\\temp\\")) && -1 == strRunPath.Find(_T("hxsafeapp.exe")))
						{
#ifdef APP_LOG_ENABLE
//							WRITELOGTOFILE(pMsg->m_Target);
#endif
							/// ��װ�ļ�֪ͨ
							/*CString strstr;
							strstr.Format(_T("��װX::����ID��%d,ģ�飺%s\n"),pMsg->m_Pid,pMsg->m_Target);
							OutputDebugString(strstr);*/
							Fire_NewInstallFileEvent(pMsg->m_Pid,CComBSTR(pMsg->m_Target));
						}
					}
					else
					{
						if(dwSafeGuard > 0)
						{
							/// д���ݿ���־
							ULONG unLogID = CDbHelper::WriteWarnLog((EHXSafeLogType )pMsg->m_LogFlag,LogTime.m_dt,nFlag,pMsg->m_Target,pMsg->m_Parent);
							if(LOG_DENY_RUN_NON_WHITE == pMsg->m_LogFlag ||
								LOG_ALLOW_RUN_NON_WHITE == pMsg->m_LogFlag ||
								LOG_VIOLATE_USB_ACCESS == pMsg->m_LogFlag)
							{
								//
								/*CString strstr2;
								strstr2.Format(_T("X::���̣�%s,ģ�飺%s\n"),pMsg->m_Parent,pMsg->m_Target);
								OutputDebugString(strstr2);*/
								/// ֪ͨ�������Ԥ�������¼�
								HANDLE hAlertEvent = ::OpenEvent(EVENT_MODIFY_STATE,FALSE,TDHXKJ_SERVICEEVENT_ALERT);
								if(NULL == hAlertEvent || INVALID_HANDLE_VALUE == hAlertEvent)
								{
									DWORD dwLastErr = ::GetLastError();
#ifdef APP_LOG_ENABLE
									if(ERROR_FILE_NOT_FOUND != dwLastErr)
									{
										CString strLogInfo(_T(""));
										strLogInfo.Format(_T("�������¼��˿�ʧ��,�����룺%ld��"),\
											dwLastErr);
										WRITELOGTOFILE(strLogInfo);
									}
#endif
								}
								else
								{
									/// ֪ͨ���������Ԥ����Ϣ
									regKey.SetRegDwordVal(TDHXKJ_ALERT_LOGTYPE,pMsg->m_LogFlag);
									regKey.SetRegDwordVal(TDHXKJ_ALERT_LOGID,unLogID);
									::SetEvent(hAlertEvent);

									CloseHandle(hAlertEvent);
									hAlertEvent = NULL;
									CString strstr;
									strstr.Format(_T("����Ԥ���¼�������־��ţ�%d"),unLogID);
									OutputDebugString(strstr);
								}
							}
						}
					}
				}
				else
				{
					/// д�ļ���־
					if(LOG_RUN_ERR_TOHOSTSERVER != pMsg->m_LogFlag)
					{
						CPathNtDosMapper PathMap;
						CString strText(pMsg->m_Target);
						int nFind = strText.Find(_T('.'));
						if(-1 != nFind)
						{
							int nFind1 = strText.ReverseFind(_T('.'));
							if(nFind1 != nFind)
								strText = strText.Left(nFind1+4);
							else
								strText = strText.Left(nFind+4);
						}
						CString strWindowPath = CBaseFuncLib::GetSpecialFolderPath(CSIDL_WINDOWS);
						if(-1 != strText.Find(_T("\\SystemRoot\\")))
							strText.Replace(_T("\\SystemRoot\\"),strWindowPath);
						strText = PathMap.NtToDosEx(strText);
						if(0 == strText.Left(1).CompareNoCase(_T("\\")))
							strText.Insert(0,strWindowPath.Left(2));
						if(!CBaseFuncLib::IsPathExist(strText))
						{
							nFind = strText.Find(_T("\\??\\"));
							if(-1 != nFind)
								strText.Delete(0,nFind+4);
							nFind = strText.Find(_T('.'));
							if(-1 != nFind)
								strText = strText.Left(nFind+4);
						}
						if(CBaseFuncLib::IsPathExist(strText))
						{
							if(dwSafeGuard > 0)
							{
								/// ��ȡ������ID
								DWORD dwParentID = CProcessMrg::GetParentPID(pMsg->m_Pid);
								/// ֻ������ڵ�ģ��
								Fire_NewLoadModuleEvent(pMsg->m_Pid,dwParentID,LogTime.m_dt,CComBSTR(strText));
							}
						}
#ifdef _DEBUG
						else
						{
							CString strLogInfo(_T(""));
							strLogInfo.Format(_T("����·������:%s ����ID=%ld,·�� %s"),strText,pMsg->m_Pid,strText);
							WRITELOGTOFILE(strLogInfo);
							strLogInfo.Empty();
						}
#endif
					}
#ifdef APP_LOG_ENABLE
					else
					{
						WRITELOGTOFILE((CString(_T("������")))+pMsg->m_Target);
					}
#endif
				}
			}
			/// �ͷ��ڴ�
			delete []pStartBuf;
			pStartBuf = NULL;
			buf = NULL;
		}

		BOOL bWaitFlag = TRUE;
		m_sGlobalCriticalSection.Lock();
		it = m_MsgList.begin();
		if(it != m_MsgList.end())
			bWaitFlag = FALSE;
		m_sGlobalCriticalSection.Unlock();
		if(bWaitFlag)
		{
			::ResetEvent(m_hsgDealMsgEvent);
			/// ����Ϣ��Ҫ�����ȴ�֪ͨ��ʵ����ʱ
			DWORD dwRet = WaitForSingleObject(m_hsgDealMsgEvent,INFINITE)-WAIT_OBJECT_0;
			switch(dwRet)
			{
			case 0:
				break;
			default:
				break;
			}
		}
		if(!m_bReadDriver)
			break;
		/// �жϴ���ʣ��ռ䣬���������Զ���������
		ULARGE_INTEGER TotalBytes,FreeAv,FreeBytes;
		memset(&TotalBytes,0,sizeof(ULARGE_INTEGER));
		CString strModulePath = CBaseFuncLib::GetModulePath(NULL);
		GetDiskFreeSpaceEx(strModulePath.Left(2),&FreeAv,&TotalBytes,&FreeBytes);
#ifndef _DEBUG
		if(0 == FreeAv.HighPart && FreeAv.LowPart/(1024*1024) < 100)
#endif
		{
			/// �û����ÿռ�С��100MB
			CComPtr <ILogService> spiLogService = CDbHelper::GetLogService();
			if(NULL != spiLogService)
			{
				/// ɾ��Ӧ����־�Ͳ�����־
				spiLogService->Del(SAFELOGTYPE_UNWHITEFILERUN,90);
				spiLogService->Del(SAFELOGTYPE_OPERATION,90);
				spiLogService = NULL;

				/// ɾ����־��������
				WIN32_FIND_DATA fd;
				HANDLE hFind = FindFirstFile(strModulePath+_T("Temp\\*.bak"), &fd);
				while (hFind != INVALID_HANDLE_VALUE)
				{
					if (FILE_ATTRIBUTE_DIRECTORY != (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						ATL::CString strFile(_T(""));
						strFile.Format(_T("%sTemp\\%s"),strModulePath,fd.cFileName);
						::SetFileAttributes(strFile,FILE_ATTRIBUTE_NORMAL);
						::DeleteFile(strFile);
						strFile.Empty();
					}
					if (!FindNextFile (hFind, &fd))
						break;
				}
				if(hFind != INVALID_HANDLE_VALUE)
				{
					FindClose (hFind);
					hFind = NULL;
				}
#ifdef APP_LOG_ENABLE
				WRITELOGTOFILE(_T("ִ��������־�������"));
#endif
			}
		}
	}

	CloseHandle(m_hsgDealMsgEvent);
	m_hsgDealMsgEvent = NULL;

	m_hDealMsgThread = NULL;
}

void CServiceControl::ReadDriverProc(void)
{
	HANDLE hLogDevice = this->OpenSafeDriver(TDHXKJ_SAFEDRIVER_NAME,FILE_ATTRIBUTE_NORMAL,GENERIC_READ);
	if( NULL == hLogDevice || hLogDevice == INVALID_HANDLE_VALUE)
	{
		/// �������豸ʧ��
		hLogDevice = NULL;
		m_hReadThread = NULL;
		return;
	}
	BOOL rtn = FALSE;

	/// ֻ�ܷ���ջ�ڴ棬NEW���ڴ�����
	BYTE hsgReadBuffer[DRIVER_IOBUFFLEN+HSG_MSG_BUF_SIZE];
	memset(hsgReadBuffer,0,DRIVER_IOBUFFLEN+HSG_MSG_BUF_SIZE);

	m_hsgReadEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	if(NULL != m_hsgReadEvent)
		m_bReadDriver = TRUE;

	try
	{
		while (m_bReadDriver) 
		{
			DWORD dwLastErr = 0;
			DWORD outLen = -1;
			/*OVERLAPPED ovlp;

			ZeroMemory(&ovlp, sizeof(OVERLAPPED));
			ovlp.hEvent = m_hsgReadEvent;*/
			memset(hsgReadBuffer,0,DRIVER_IOBUFFLEN+HSG_MSG_BUF_SIZE);

			rtn = ::DeviceIoControl(hLogDevice,IOCTL_MSG_READ_FROM_HSG,NULL,0,\
				hsgReadBuffer,sizeof(hsgReadBuffer),&outLen,NULL);
			// may have been closed
			if (!m_bReadDriver)
				break;
			if(!rtn)
				dwLastErr = GetLastError();
			bool isIoPending = ( dwLastErr == ERROR_IO_PENDING);

			if (!rtn && !isIoPending) 
			{
				m_bReadDriver = FALSE;
#ifdef APP_LOG_ENABLE
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("���������¼��˿�ʧ��,�����룺%ld��"),dwLastErr);
				WRITELOGTOFILE(strLogInfo);
#endif
				break;
			}
			/*rtn = GetOverlappedResult( hLogDevice, &ovlp, &outLen, isIoPending );*/
			if (outLen <= 0) 
			{
				/// ��ȡ����ʧ��
#ifdef APP_LOG_ENABLE
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("��ȡ����֪ͨ�����쳣,����ֵ��%ld"),rtn);
				WRITELOGTOFILE(strLogInfo);
#endif
				continue;
			}

			/// �����ڴ棬����ʹ��
			BYTE *pMsgBuf = new BYTE[outLen+1];
			if(NULL == pMsgBuf)
				continue;
			
			memset(pMsgBuf,0,outLen+1);
			memcpy(pMsgBuf,hsgReadBuffer,outLen);
			/// �����ڴ��б���
			m_sGlobalCriticalSection.Lock();
			m_MsgList.push_back(pMsgBuf);
			m_sGlobalCriticalSection.Unlock();
			if(NULL == m_hDealMsgThread)
			{
				DWORD dwThreadID = 0;
				m_hDealMsgThread = CREATE_MYTHREAD(DealMsgThread,this,dwThreadID);
			}
			else
			{
				if(NULL != m_hsgDealMsgEvent)
				{
					::SetEvent(m_hsgDealMsgEvent);
				}
			}
		}
	}
	catch(...)
	{
	}

	CloseHandle(m_hsgReadEvent);
	m_hsgReadEvent = NULL;

	::CloseHandle(hLogDevice);
	hLogDevice = NULL;

	m_hReadThread = NULL;
	m_bReadDriver = FALSE;
}

UINT CServiceControl::ReadDriverThread(LPVOID pParam)
{
	CServiceControl* pThis = (CServiceControl* )pParam;
#ifndef WIN64
	/// ֧��64λϵͳ
	HMODULE	hKernel	= NULL;
	if(CBaseFuncLib::IsWin64(hKernel))
	{
		g_lpDisabelWow64 = (lpWow64DisableWow64FsRedirection)GetProcAddress(\
			hKernel,"Wow64DisableWow64FsRedirection"); 
		g_EnabelWow64 = (lpWow64RevertWow64FsRedirection)GetProcAddress(\
			hKernel,"Wow64RevertWow64FsRedirection");
	}
	if(NULL != g_lpDisabelWow64)
		g_lpDisabelWow64(&g_OldValue);
	hKernel = NULL;
#endif
	pThis->ReadDriverProc();
#ifndef WIN64
	if(NULL != g_EnabelWow64 && NULL != g_OldValue)
		g_EnabelWow64(g_OldValue);
	g_EnabelWow64 = NULL;
	g_lpDisabelWow64 = NULL;
#endif
	return 0;
}

STDMETHODIMP CServiceControl::StartDriverMonitor(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	if(NULL != m_hReadThread)
		return S_OK;/// �Ѿ�����
	DWORD dwThreadID = 0;
	m_hReadThread = CREATE_MYTHREAD(ReadDriverThread,this,dwThreadID);
	return S_OK;
}

STDMETHODIMP CServiceControl::StopDriverMonitor(void)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strLastErr.Empty();
	m_nLastErrCode = ERROR_SUCCESS;
	if(NULL == m_hReadThread)
		return S_FALSE;
	if(NULL != m_hsgReadEvent)
	{
		/// �����¼���״̬֪ͨ
		m_bReadDriver = FALSE;
		::SetEvent(m_hsgReadEvent);
	}
	if(NULL != m_hsgDealMsgEvent)
	{
		::SetEvent(m_hsgDealMsgEvent);
	}

	/// ��Ҫ�ȴ��ײ��˳�
	SHORT nIndex = 100;
	while((NULL != m_hReadThread || NULL != m_hDealMsgThread) && nIndex > 0)
	{
		::Sleep(100);
		nIndex--;
	}

	return S_OK;
}