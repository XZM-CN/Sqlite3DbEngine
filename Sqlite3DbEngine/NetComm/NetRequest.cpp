// NetRequest.cpp : CNetRequest 的实现

#include "stdafx.h"
#include "NetRequest.h"
#include <ATLComTime.h>
#include "BaseFuncLib.h"

// CNetRequest
STDMETHODIMP CNetRequest::get_MsgWnd(LONGLONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = (LONGLONG )m_hMsgWnd;
	return S_OK;
}

STDMETHODIMP CNetRequest::put_MsgWnd(LONGLONG newVal)
{
	// TODO: Add your implementation code here
	if(!newVal)
	{
		m_hMsgWnd = NULL;
		return S_OK;
	}
	BOOL bWin = ::IsWindow((HWND)newVal);
	if(!bWin)
	{
		ATLASSERT(0);
		return E_FAIL;
	}
	if(m_hMsgWnd != (HWND)newVal)
	{
		m_hMsgWnd = (HWND)newVal;
		if(NULL != m_pWeiboNet)
			m_pWeiboNet->SetParentWnd(m_hMsgWnd);
	}
	return S_OK;
}

STDMETHODIMP CNetRequest::get_SyncConnect(VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_bSyncConnect;
	return S_OK;
}

STDMETHODIMP CNetRequest::put_SyncConnect(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
	m_bSyncConnect = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::put_Port(LONG newVal)
{
	m_nPort = (UINT )newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::put_NoCookie(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
	m_bNoCookie = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_Method(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strMethod);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CNetRequest::put_Method(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
		return E_POINTER;
	m_strMethod = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_Cookie(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strCookie);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CNetRequest::put_Cookie(BSTR newVal)
{
	// TODO: Add your implementation code here
	m_strCookie.Empty();
	if(NULL == newVal)
		return S_OK;
	m_strCookie = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_PostFile(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strPostFile);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CNetRequest::put_PostFile(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		m_strPostFile.Empty();
		return S_OK;
	}
	if(!CBaseFuncLib::IsPathExist(newVal))
		return E_FAIL;
	m_strPostFile = newVal;
	if(NULL != m_pWeiboNet)
		m_pWeiboNet->SetPostFilePath(m_strPostFile);
	return S_OK;
}

STDMETHODIMP CNetRequest::get_DownFile(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	if(NULL != m_pWeiboNet)
		m_strDownFile = m_pWeiboNet->GetDownFilePath();
	CComBSTR bstrVal(m_strDownFile);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();

	return S_OK;
}

STDMETHODIMP CNetRequest::put_DownFile(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		m_strDownFile.Empty();
		return S_OK;
	}
	m_strDownFile = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_PrxoyServer(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	if(NULL != m_pWeiboNet)
		m_strProxyServer = m_pWeiboNet->GetProxyServer();
	CComBSTR bstrVal(m_strProxyServer);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();

	return S_OK;
}

STDMETHODIMP CNetRequest::put_PrxoyServer(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		m_strProxyServer.Empty();
		return S_OK;
	}
	m_strProxyServer = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_Referer(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strHttpReferer);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();

	return S_OK;
}

STDMETHODIMP CNetRequest::put_Referer(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		m_strHttpReferer.Empty();
		return S_OK;
	}
	m_strHttpReferer = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_Origin(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CComBSTR bstrVal(m_strOrigin);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();

	return S_OK;
}

STDMETHODIMP CNetRequest::put_Origin(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		m_strOrigin.Empty();
		return S_OK;
	}
	m_strOrigin = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_UserAgent(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	if(NULL != m_pWeiboNet)
		m_strUserAgent = m_pWeiboNet->GetProxyServer();
	CComBSTR bstrVal(m_strUserAgent);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();

	return S_OK;
}

STDMETHODIMP CNetRequest::put_UserAgent(BSTR newVal)
{
	// TODO: Add your implementation code here
	if(NULL == newVal)
	{
		m_strUserAgent.Empty();
		return S_OK;
	}
	m_strUserAgent = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_UseIE(VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_bUseIE;
	return S_OK;
}

STDMETHODIMP CNetRequest::put_UseIE(VARIANT_BOOL newVal)
{
	// TODO: Add your implementation code here
	m_bUseIE = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_RequestUrl(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	CString strUrl(m_strUrl);
	if(strUrl.IsEmpty() && NULL != m_pWeiboNet)
		strUrl = m_pWeiboNet->GetRequestUrl();
	CComBSTR bstrVal(strUrl);
	bstrVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CNetRequest::get_Location(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal || NULL == m_pWeiboNet)
		return E_POINTER;
	CComBSTR bstrVal(m_pWeiboNet->GetLocation());
	bstrVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CNetRequest::get_ServerFileName(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal || NULL == m_pWeiboNet)
		return E_POINTER;
	CComBSTR bstrVal(m_pWeiboNet->GetServerFileName());
	bstrVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CNetRequest::get_ServiceID(SHORT* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = m_sServiceID;
	return S_OK;
}

STDMETHODIMP CNetRequest::put_ServiceID(SHORT newVal)
{
	// TODO: Add your implementation code here
	m_sServiceID = newVal;
	return S_OK;
}

STDMETHODIMP CNetRequest::SendHeader(SHORT sType, VARIANT_BOOL bZipFlag, BSTR bstrAuth)
{
	// TODO: Add your implementation code here
	/// 缓存
	m_bZipFlag = TRUE;
	if(VARIANT_FALSE == bZipFlag)
		m_bZipFlag = FALSE;
	m_sType = sType;
	if(NULL != bstrAuth)
		m_strAuth = bstrAuth;
	else
		m_strAuth.Empty();
	return S_OK;
}

STDMETHODIMP CNetRequest::Request(BSTR bstrUrl)
{
	// TODO: Add your implementation code here
	if(NULL == bstrUrl)
		return E_POINTER;
	CString strUrl(bstrUrl);
	if(strUrl.IsEmpty())
		return E_INVALIDARG;
	m_strUrl.Empty();
	m_strUrl = strUrl;
	CString strHeader(m_strAuth);
	if(!m_strHttpReferer.IsEmpty())
	{
		/// 添加Referer
		if(!strHeader.IsEmpty())
			strHeader += _T("\r\n");
		CString strTem(_T(""));
		strTem.Format(_T("Referer: %s"),m_strHttpReferer);
		strHeader += strTem;
	}
	if(!m_strOrigin.IsEmpty())
	{
		/// 添加Origin
		if(!strHeader.IsEmpty())
			strHeader += _T("\r\n");
		CString strTem(_T(""));
		strTem.Format(_T("Origin: %s"),m_strOrigin);
		strHeader += strTem;
	}

	if(!m_strCookie.IsEmpty())
	{
		/// 添加COOKIE
		if(!strHeader.IsEmpty())
			strHeader += _T("\r\n");
		CString strTem(_T(""));
		strTem.Format(_T("Cookie: %s"),m_strCookie);
		strHeader += strTem;
	}

	if(NULL == m_pWeiboNet)
	{
		m_pWeiboNet = new CWinNet(m_hMsgWnd);
		m_pWeiboNet->SetPostFilePath(m_strPostFile);
		if(VARIANT_TRUE == m_bUseIE)
			m_pWeiboNet->SettUseIE(TRUE);
		else
			m_pWeiboNet->SettUseIE(FALSE);
		m_pWeiboNet->SetProxyServer(m_strProxyServer);
		m_pWeiboNet->SetUserAgent(m_strUserAgent);
		m_pWeiboNet->SetSendHeader(m_sType,m_bZipFlag,strHeader);
	}
	else
	{
		if(!m_pWeiboNet->IsIdle())
		{
			m_pWeiboNet->StopHttpVisit(FALSE);/// 停止上一个请求
			m_pWeiboNet->SetPostFilePath(m_strPostFile);
			m_pWeiboNet->SetParentWnd(m_hMsgWnd);
			if(VARIANT_TRUE == m_bUseIE)
				m_pWeiboNet->SettUseIE(TRUE);
			else
				m_pWeiboNet->SettUseIE(FALSE);
			m_pWeiboNet->SetProxyServer(m_strProxyServer);
			m_pWeiboNet->SetUserAgent(m_strUserAgent);
			m_pWeiboNet->SetSendHeader(m_sType,m_bZipFlag,strHeader);
		}
	}
	ATLASSERT(m_pWeiboNet);
	if(NULL == m_pWeiboNet)
		return E_OUTOFMEMORY;
	DWORD dwConnectMode = 0;
	if(VARIANT_FALSE == m_bSyncConnect)
		dwConnectMode = INTERNET_FLAG_ASYNC;
	if(VARIANT_TRUE == m_bNoCookie)
		m_pWeiboNet->SetNoCookie(TRUE);
	else
		m_pWeiboNet->SetNoCookie(FALSE);
	if(m_nPort)
		m_pWeiboNet->SetPort(m_nPort);/// 指定端口

	if(m_strMethod.IsEmpty())
		m_strMethod = _T("GET");
	BOOL bRetFlag = FALSE;
	bRetFlag = m_pWeiboNet->RequestUrl(strUrl,(ENetReqID)m_sServiceID,dwConnectMode,m_strDownFile,m_strMethod);
	if(!bRetFlag)
		return E_FAIL;
	if(VARIANT_TRUE == m_bSyncConnect && !m_bDownFile)
	{
		bRetFlag = m_pWeiboNet->SaveResponseInfo(THREAD_PRIORITY_BELOW_NORMAL);
		if(bRetFlag)
		{
			HANDLE hDownloadHandle = m_pWeiboNet->GetDownloadThread();
			if(NULL != hDownloadHandle)
				WaitForSingleObject(hDownloadHandle,INFINITE);
		}
		else
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("HttpRequest::Request请求%s失败，错误码：%ld，原因：%s"),\
				strUrl,m_pWeiboNet->GetLastError(),m_pWeiboNet->GetLastMessage());
			WRITELOGTOFILE(strLogInfo);
#endif
		}
	}
	return S_OK;
}

STDMETHODIMP CNetRequest::DownloadFile(BSTR bstrUrl,VARIANT_BOOL bHaveName)
{
	if(NULL == bstrUrl)
		return E_POINTER;
	CString strFileUrl(bstrUrl);
	if(strFileUrl.IsEmpty())
		return E_INVALIDARG;
	CString strUserAgent(_T(""));
	CHXRegKey regKey(COMPANY_REGPATH,TDHXKJ_APPLICATION_NAME);
	regKey.GetRegStringVal(_T("UserAgent"),strUserAgent);
	put_UserAgent(CComBSTR(strUserAgent));
	strUserAgent.Empty();
	SendHeader(6,VARIANT_FALSE,NULL);

	m_strUrl.Empty();
	m_strUrl = strFileUrl;
	CString strServerName(_T("")),strVirtualUrl(_T(""));
	int iFind=-1;
	iFind=strFileUrl.ReverseFind(_T('/'));
	ATLASSERT(iFind!=-1);
	CString strFileName(_T(""));
	if(VARIANT_FALSE == bHaveName || m_strDownFile.IsEmpty())
	{
		strFileName = strFileUrl.Right(strFileUrl.GetLength()-iFind-1);
		if(VARIANT_FALSE == bHaveName)
		{
			iFind=strFileName.Find(_T('.'));
			ATLASSERT(-1 != iFind);//保证是下载文件
		}
		iFind=strFileUrl.ReverseFind(_T('='));
		if(-1 != iFind)
		{
			//文件名加上时间标记，避免重名
			COleDateTime curTime=COleDateTime::GetCurrentTime();
			strFileName.Format(_T("%ld_%02d%02d%02d%02d%02d%s"),(DWORD)this,curTime.GetMonth(),\
				curTime.GetDay(),curTime.GetHour(),curTime.GetMinute(),curTime.GetSecond(),\
				strFileUrl.Right(strFileUrl.GetLength()-iFind-1));
		}
	}
	CString strTemPath(_T("")),strFilePath(m_strDownFile);
	if(strFilePath.IsEmpty() || VARIANT_FALSE == bHaveName)
	{
		if(strFilePath.IsEmpty())
			strFilePath=CBaseFuncLib::GetTmpPath();
		int iIndex=0;
		strTemPath.Format(_T("%s%s"),strFilePath,strFileName);
		while(CBaseFuncLib::IsPathExist(strTemPath))
		{
			strTemPath.Empty();
			strTemPath.Format(_T("%s%d%s"),strFilePath,iIndex,strFileName);
			iIndex++;
		}
		m_strDownFile.Empty();
		m_strDownFile = strTemPath;
	}
	strFilePath.Empty();
	m_bDownFile = TRUE;
	HRESULT hRet = Request(bstrUrl);
	if(SUCCEEDED(hRet))
	{
		if(VARIANT_TRUE == m_bSyncConnect)
		{
			m_pWeiboNet->DownFile(m_strDownFile,THREAD_PRIORITY_BELOW_NORMAL);
			HANDLE hDownloadHandle = m_pWeiboNet->GetDownloadThread();
			if(NULL != hDownloadHandle)
				WaitForSingleObject(hDownloadHandle,INFINITE);
		}
	}
	m_bDownFile = FALSE;
	return hRet;
}

STDMETHODIMP CNetRequest::Stop(VARIANT_BOOL bReleaseConnect)
{
	// TODO: Add your implementation code here
	m_strUrl.Empty();
	if(NULL == m_pWeiboNet)
		return S_FALSE;
	BOOL bStopFlag = FALSE;
	if(VARIANT_TRUE == bReleaseConnect)
		bStopFlag = TRUE;
	m_pWeiboNet->StopHttpVisit(bStopFlag);
	m_strPostFile.Empty();
	m_strDownFile.Empty();
	m_strMethod.Empty();
	m_strAuth.Empty();
	if(VARIANT_TRUE == bReleaseConnect)
	{
		m_hMsgWnd = NULL;
		m_bUseIE = VARIANT_TRUE;
		m_strProxyServer.Empty();
		m_strUserAgent.Empty();
		delete m_pWeiboNet;
		m_pWeiboNet = NULL;
	}
	return S_OK;
}

STDMETHODIMP CNetRequest::get_ErrCode(LONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = 0;
	if(NULL == m_pWeiboNet)
		return S_FALSE;
	(*pVal) = m_pWeiboNet->GetLastError();
	return S_OK;
}

STDMETHODIMP CNetRequest::get_LastErrInfo(BSTR* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	if(NULL == m_pWeiboNet)
		return S_FALSE;
	CComBSTR bstrErr(m_pWeiboNet->GetLastMessage());
	bstrErr.CopyTo(pVal);
	bstrErr.Empty();
	return S_OK;
}

STDMETHODIMP CNetRequest::get_Busy(VARIANT_BOOL* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = VARIANT_FALSE;
	if(NULL == m_pWeiboNet)
		return S_OK;
	if(!m_pWeiboNet->IsIdle())
		(*pVal) = VARIANT_TRUE;
	return S_OK;
}

STDMETHODIMP CNetRequest::get_HttpStatus(LONG* pVal)
{
	// TODO: Add your implementation code here
	if(NULL == pVal)
		return E_POINTER;
	(*pVal) = 200;
	if(NULL == m_pWeiboNet)
		return S_OK;
	(*pVal) = (LONG )m_pWeiboNet->GetHttpStatus();
	return S_OK;
}

