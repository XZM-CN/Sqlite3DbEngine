
/**
* @file WeiNet.h
*
* Copyright (c) 2016-?,天地和兴科技
* All rights reserved.
*
* @ingroup 
*
* @brief 基于HTTP协议的封装类实现
*
*
*
* @cuo*
* @Date 2011-08-17
*
* @author WZH
*
* @history 
*
*/
#include "StdAfx.h"
#include <time.h>
#include "../ZLib/zlib.h"

#include "HXRegKey.h"
#include "BaseFuncLib.h"
#include "WinNet.h"

#ifndef GWL_USERDATA
#define GWL_USERDATA        (-21)
#endif

typedef std::map <CWinNet* ,CWinNet*>	WEIBONET_MAP;

/// 全局网络对象
WEIBONET_MAP	g_mapWeibNet;

extern HANDLE g_hInstance;

#pragma warning( disable : 4819 )

#pragma comment(lib,"Wininet.lib")

static CString g_strHeader = L"Accept: */*\r\nAccept-Language: zh-CN\r\nAccept-Charset: utf-8\r\nContent-Type:  application/x-www-form-urlencoded\r\n";

#ifndef WIN64
#ifdef _DEBUG
#pragma comment(lib, "..\\bin\\\\Debug\\ZLib.lib")
#else
#pragma comment(lib, "..\\bin\\\\Release\\ZLib.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "..\\bin\\\\Debug\\X64\\ZLib.lib")
#else
#pragma comment(lib, "..\\bin\\\\Release\\X64\\ZLib.lib")
#endif
#endif

//回调函数定义
void __stdcall InternetStatusCallback(HINTERNET hInternet,DWORD dwContext,
	DWORD dwInternetStatus,LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);

//回调函数实现
void __stdcall InternetStatusCallback(HINTERNET hInternet,DWORD dwContext,
	DWORD dwInternetStatus,LPVOID lpStatusInfo, DWORD dwStatusInfoLen)
{
	CWinNet *pHttpVisit=(CWinNet *)dwContext;//网络访问类
	ATLASSERT(pHttpVisit);
	if(NULL == pHttpVisit)
		return;
	/// 可能网络对象已经被删除
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(pHttpVisit);
	if(it == g_mapWeibNet.end())
		return;
	//调用类实现的处理函数
	it->first->DealWithCallBack(hInternet,dwInternetStatus,\
		lpStatusInfo,dwStatusInfoLen);
}

/////////////////////////     网络访问类实现     //////////////////////////////
CWinNet::CWinNet(HWND hParentWnd)
:m_hParentWnd(hParentWnd)
,m_hInternet(NULL)
,m_hConnect(NULL)
,m_hRequest(NULL)
,m_hConnectedEvent(NULL)
,m_hRequestOpenedEvent(NULL)
,m_hRequestCompleteEvent(NULL)
,m_hStopRequestEvent(NULL)
,m_hDownFile(NULL)
,m_hDownThread(NULL)
,m_hSelfWnd(NULL)
,m_dwConnectMode(0)
,m_dwInternetStatus(0)
,m_dwFileSize(0)
,m_dwDownloadSize(0)
,m_dwHttpStatus(0)
,m_uiPort(0)
,m_szPostBuff(NULL)
,m_bAllDone(FALSE)
,m_bNoCookie(FALSE)
,m_bNetConnect(FALSE)
,m_bSecureFlag(FALSE)
,m_bUseIE(TRUE)
,m_eServiceID(ENETREQUEST_INVALID)
{
	m_strUrl.Empty();
	m_strMethod.Empty();
	m_strSendHeader.Empty();
	m_strDownFilePath.Empty();
	m_strPostFilePath.Empty();
	m_strProxyServer.Empty();

	if(m_hParentWnd && !::IsWindow(m_hParentWnd))
		ATLASSERT(0);

	WNDCLASS wndclass;
	wndclass.style=0;
	wndclass.lpfnWndProc = AsyncWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = (HINSTANCE )g_hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	CString strClaseName=(_T(""));
	strClaseName.Format(_T("WeiBoNet_Wnd%s"),CBaseFuncLib::NumToStr((DWORD)this));

	wndclass.lpszClassName = strClaseName;
	::RegisterClass(&wndclass);

	m_hSelfWnd=::CreateWindow(strClaseName,strClaseName,\
		0,0,0,0,0,HWND_MESSAGE,0,(HINSTANCE )g_hInstance,0);
	BOOL bWinFlag = ::IsWindow(m_hSelfWnd);
	ATLASSERT(bWinFlag);
	if(!bWinFlag)
		return;
	::SetWindowLong(m_hSelfWnd, GWL_USERDATA,(LONG)this);
	g_mapWeibNet[this] = this;
}

void CWinNet::ResetMemberVariant()
{
	m_dwConnectMode=0;
	m_dwInternetStatus=0;
	m_dwHttpStatus = 0;
	m_dwFileSize=0;
	m_dwDownloadSize=0;
	m_sTrySendRequest=0;

	m_bAllDone = FALSE;
	m_bNetConnect = FALSE;

	CloseDownloadFile();
	CloseNetHandle(m_hRequest);
	CloseNetHandle(m_hConnect);

	CloseNormalHandle(m_hConnectedEvent);
	CloseNormalHandle(m_hRequestOpenedEvent);
	CloseNormalHandle(m_hRequestCompleteEvent);
	CloseNormalHandle(m_hStopRequestEvent);

	m_hDownThread=NULL;
	m_hDownFile = NULL;
	
	DelPostData();
	m_strUrl.Empty();
	m_strMethod.Empty();
	m_strDownFilePath.Empty();
	m_strRetHeader.Empty();
	m_strLocation.Empty();
	m_strServerFileName.Empty();
}

void CWinNet::DelPostData()
{
	if(NULL == m_szPostBuff)
		return;
	delete []m_szPostBuff;
	m_szPostBuff = NULL;
}

CWinNet::~CWinNet(void)
{
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(this);
	if(it != g_mapWeibNet.end())
		g_mapWeibNet.erase(it);

	ResetMemberVariant();

	CloseNetHandle(m_hInternet);
	m_strPostFilePath.Empty();
	if( NULL != m_hSelfWnd )
	{
		::DestroyWindow(m_hSelfWnd);
		m_hSelfWnd=NULL;
	}
}

void CWinNet::DealWithCallBack(HINTERNET hInternet,DWORD dwInternetStatus, 
				LPVOID lpStatusInfo, DWORD dwStatusInfoLen)
{
	m_dwInternetStatus=dwInternetStatus;
	switch(m_dwInternetStatus)
	{
		case INTERNET_STATUS_HANDLE_CREATED:	/// 句柄创建
			{
				INTERNET_ASYNC_RESULT *pRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
				if(NULL == m_hConnect)
				{
					m_hConnect = (HINTERNET)pRes->dwResult;
					m_bNetConnect = TRUE;
					SetWaitEvent(m_hConnectedEvent);
				}
				else if(m_bNetConnect && NULL == m_hRequest)
				{
					m_hRequest = (HINTERNET)pRes->dwResult;
					SetWaitEvent(m_hRequestOpenedEvent);
				}
			}
			break;
		case INTERNET_STATUS_REQUEST_SENT:		/// 请求已发送
			{
#ifdef _DEBUG
				DWORD *lpBytesSent = (DWORD*)lpStatusInfo;
				::Sleep(1);
#endif
			}
			break;
		case INTERNET_STATUS_RECEIVING_RESPONSE:	/// 正在接受反馈
			{
#ifdef _DEBUG
				::Sleep(1);
#endif
			}
			break;
		case INTERNET_STATUS_CONNECTING_TO_SERVER:	/// 正在连接到服务器
		case INTERNET_STATUS_CLOSING_CONNECTION:	/// 正在关闭连接
		case INTERNET_STATUS_HANDLE_CLOSING:		/// 句柄正在关闭
		case INTERNET_STATUS_INTERMEDIATE_RESPONSE:	/// 收到服务器反馈
		case INTERNET_STATUS_NAME_RESOLVED:			/// 域名已解析
		case INTERNET_STATUS_REDIRECT:				/// 重定向
		case INTERNET_STATUS_RESOLVING_NAME:		/// 正在解析域名
		case INTERNET_STATUS_SENDING_REQUEST:		/// 正在发送请求
		case INTERNET_STATUS_STATE_CHANGE:			/// 连接转换改变
		case INTERNET_STATUS_CONNECTED_TO_SERVER:	/// 已经连接到服务器
			break;//无需处理
		case INTERNET_STATUS_RESPONSE_RECEIVED:		/// 反馈已接受
			{
				DWORD *dwBytesReceived = (DWORD*)lpStatusInfo;
                if (*dwBytesReceived == 0)
                    m_bAllDone = TRUE;
			}
			break;
		case INTERNET_STATUS_REQUEST_COMPLETE :		/// 发生请求完成
		{
			// Used by the Win32 API function InternetConnect to indicate that it has created the new handle.
			// This lets the application call the Win32 function InternetCloseHandle from another thread if the connect is taking too long.
			// See the ActiveX SDK for more information about these functions. 
			BOOL bWinFlag = ::IsWindow(m_hSelfWnd);
			if(bWinFlag)
			{
				::SendMessage(m_hSelfWnd,WM_REQUEST_COMPLETED,\
					(WPARAM)lpStatusInfo,(LPARAM)this);
			}
			break;
		}
		case INTERNET_STATUS_CONNECTION_CLOSED:		/// 连接被关闭
		{
			// Successfully closed the connection to the server.
			// The lpvStatusInformation parameter is NULL.
			if(NULL != m_hStopRequestEvent)
				SetWaitEvent(m_hStopRequestEvent);/// 停止网络请求
			if(IsWindow(m_hParentWnd))
				::PostMessage(m_hParentWnd,WM_CONNECT_CLOSED,0,m_eServiceID);
			break;
		}
		default:
			break;//其它状态，不处理
	}
}

LRESULT CALLBACK CWinNet::AsyncWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	//窗口接收消息过程
	if (WM_REQUEST_COMPLETED == message)
	{
		LPVOID lpvStatusInformation=(LPVOID)wParam;
		CWinNet *pHttpVisit=(CWinNet *)lParam;
		if(NULL == lpvStatusInformation || NULL == pHttpVisit)
			return 0;
		WEIBONET_MAP::iterator it = g_mapWeibNet.find(pHttpVisit);
		if(it == g_mapWeibNet.end())
			return 0;
		if(NULL != it->first->m_hStopRequestEvent)
		{
			it->first->SetWaitEvent(pHttpVisit->m_hRequestCompleteEvent);//设置信号
			it->first->SetWaitEvent(pHttpVisit->m_hStopRequestEvent);
			return 1;
		}
		INTERNET_ASYNC_RESULT* res = LPINTERNET_ASYNC_RESULT(lpvStatusInformation);
		DWORD dwErrCode = res->dwError;
		HINTERNET hInet = (HINTERNET)(res->dwResult);
		if(ERROR_SUCCESS == dwErrCode)
		{
			it->first->SetWaitEvent(pHttpVisit->m_hRequestCompleteEvent);//设置信号
			it->first->SaveResponseInfo(THREAD_PRIORITY_ABOVE_NORMAL);
			return 1;
		}
		CString strErrInfo=pHttpVisit->ErrorInfo(_T("网络操作"),dwErrCode);
//		if(strErrInfo.IsEmpty() && 12017 != dwErrCode)
		if(strErrInfo.IsEmpty())
		{
			/// 无错误信息，继续尝试获取数据
			::Sleep(200);
			it->first->SaveResponseInfo(THREAD_PRIORITY_ABOVE_NORMAL);
			return 1;
		}
		it->first->SetWaitEvent(it->first->m_hRequestCompleteEvent);//设置信号
		it->first->SetError(dwErrCode,strErrInfo);
		if(ERROR_INTERNET_NAME_NOT_RESOLVED == dwErrCode 
			|| ERROR_INTERNET_TIMEOUT == dwErrCode
			)
		{
			/// 超时或服务器无法解析
			::Sleep(1);
		}
#ifdef APP_LOG_ENABLE
		//CString strLogInfo(_T(""));
		//strLogInfo.Format(_T("WeiNet::AsyncWindowProc 请求服务 %d 出错，错误码：%ld 错误信息：%s"),\
		//	pHttpVisit->GetCurServiceID(),dwErrCode,strErrInfo);
		//WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
		/// 出现错误，停止下载
		it->first->StopDownloadFile();
		if(pHttpVisit->m_hParentWnd && ::IsWindow(pHttpVisit->m_hParentWnd))
		{
			if(it->first->GetCurServiceID() < ENETREQUEST_EHELP_TEST)
				::PostMessage(pHttpVisit->m_hParentWnd,WM_DOWNLOAD_ERROR,(WPARAM )ERROR_HTTP_OPENREQUEST,(LPARAM )it->first->GetCurServiceID());
			else
				::PostMessage(pHttpVisit->m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )dwErrCode,(LPARAM )it->first->GetCurServiceID());
		}
		return 0;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

static char dummy_head[2] =
{
	0x8 + 0x7 * 0x10,
	(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
};

int CWinNet::HttpGzDecompress(const BYTE *zdata, const DWORD nzdata,const HANDLE hUnzipFile)
{
	int err = 0;
	/// 每次8K
	int nBuffSize = 8*1024;
	BYTE *data = new BYTE[nBuffSize];
	if(NULL == data)
		return -1;
	memset(data,0,nBuffSize);

	/* decompression stream */
	z_stream d_stream = {0}; 
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
	d_stream.next_in  = (BYTE *)zdata;
	d_stream.avail_in = nzdata;
	d_stream.next_out = data;
	if(inflateInit2(&d_stream, 47) != Z_OK) 
	{
		delete []data;
		data = NULL;
		return -1;
	}

	DWORD dwWriteLen = 0;
	while (d_stream.total_in < nzdata) 
	{
		d_stream.avail_in = d_stream.avail_out = nBuffSize;
		if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END)
		{
			int nDataLen = (int )strlen((const char *)data);
			if(nDataLen)
			{
				dwWriteLen = 0;
				WriteFile(hUnzipFile,data,nDataLen,&dwWriteLen,NULL);
				ATLASSERT(dwWriteLen);
			}
			break;
		}
		if(err != Z_OK )
		{
			if(err == Z_DATA_ERROR)
			{
				d_stream.next_in = (Bytef*) dummy_head;
				d_stream.avail_in = sizeof(dummy_head);
				if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK)
				{
					delete []data;
					data = NULL;
					inflateEnd(&d_stream);
					return -1;
				}
			}
			else 
			{
				delete []data;
				data = NULL;
				inflateEnd(&d_stream);
				return -1;
			}
		}
		WriteFile(hUnzipFile,data,nBuffSize,&dwWriteLen,NULL);
		ATLASSERT(dwWriteLen);

		memset(data,0,nBuffSize);
		d_stream.next_out = data;
	}
	if(NULL != data)
	{
		delete []data;
		data = NULL;
	}
	if(inflateEnd(&d_stream) != Z_OK) 
		return -1;
	return 0;
}

BOOL CWinNet::CloseNormalHandle(HANDLE &hHandle)
{
	BOOL bCloseFlag = FALSE;
	if (NULL == hHandle)
		return bCloseFlag;
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(this);
	if(it == g_mapWeibNet.end())
		return bCloseFlag;
	DWORD dwFlag = 0;
	BOOL bGetFlag = GetHandleInformation(hHandle,&dwFlag);
	if(bGetFlag)
	{
		dwFlag = dwFlag & HANDLE_FLAG_PROTECT_FROM_CLOSE;
		if(!dwFlag)
			::CloseHandle(hHandle);
	}
	hHandle=NULL;
	return bCloseFlag;
}

BOOL CWinNet::CloseNetHandle(HINTERNET &hHandle)
{
	if (NULL == hHandle)
		return FALSE;
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(this);
	if(it == g_mapWeibNet.end())
		return FALSE;
	BOOL bCloseFlag = ::InternetCloseHandle(hHandle);
	hHandle=NULL;
	if (bCloseFlag)
		return TRUE;
	DWORD dwErrCode=::GetLastError();
	if(!dwErrCode || m_eServiceID<ENETREQUEST_EHELP_TEST)
		return TRUE;
#ifdef APP_LOG_ENABLE
	CString strErrInfo=ErrorInfo(_T("InternetCloseHandle"),dwErrCode);
	if(!strErrInfo.IsEmpty())
		SetError(ERROR_HTTP_CLOSE,strErrInfo);
#endif
	return bCloseFlag;
}

BOOL CWinNet::OpenInternet(DWORD dwConnectMode)
{
	if(NULL != m_hInternet)
	{
		if(m_dwConnectMode==dwConnectMode)
			return TRUE;//已经打开
		if(INTERNET_FLAG_ASYNC==m_dwConnectMode)
			::InternetSetStatusCallback(m_hInternet,NULL);//注销回调函数
		CloseNetHandle(m_hInternet);
	}
	CString strAgent(_T(""));
	CHXRegKey regKey(_T("Software\\Microsoft\\Windows\\CurrentVersion"),_T("Internet Settings"),HKEY_CURRENT_USER);
	if(m_bUseIE || m_strUserAgent.IsEmpty())
	{
		regKey.GetRegStringVal(_T("User Agent"),strAgent);
		if(strAgent.IsEmpty())
			strAgent = _T("Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 6.0)");
	}
	else
		strAgent = m_strUserAgent;
/*
#ifdef APP_LOG_ENABLE
	CString strLogInfo(_T(""));
	strLogInfo.Format(_T("WeiNet::OpenInternet Object:%s UA:%s"),\
		CBaseFuncLib::NumToStr((DWORD)this),strAgent);
	WRITELOGTOFILE(strLogInfo);
#endif
*/
	DWORD dwProxy = -1;
	if(m_bUseIE)
		regKey.GetRegDwordVal(_T("ProxyEnable"),dwProxy);
	if(dwProxy > 0 || !m_strProxyServer.IsEmpty())
	{
		CString strProxyServer(_T(""));
		if(m_bUseIE)
			regKey.GetRegStringVal(_T("ProxyServer"),strProxyServer);
		else
			strProxyServer = m_strProxyServer;
/*
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("WeiNet::OpenInternet Object:%s ProxyServer:%s"),\
			CBaseFuncLib::NumToStr((DWORD)this),strProxyServer);
		WRITELOGTOFILE(strLogInfo);
#endif
*/
		m_hInternet = ::InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,strProxyServer,0,dwConnectMode);
	}
	else
	{
		m_hInternet = ::InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,0,dwConnectMode);
	}

	if(!m_hInternet)
	{
		DWORD dwErrCode=::GetLastError();
		if(dwErrCode)
		{
			CString strErrInfo=ErrorInfo(_T("InternetOpen"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_OPEN,strErrInfo);
		}
		ATLASSERT(0);
		return FALSE;
	}
	if(INTERNET_FLAG_ASYNC==dwConnectMode)
		::InternetSetStatusCallback(m_hInternet,(INTERNET_STATUS_CALLBACK)InternetStatusCallback);
	
	UINT nConnectTimeout=30*1000;
	::InternetSetOption(m_hInternet,INTERNET_OPTION_CONNECT_TIMEOUT,&nConnectTimeout,sizeof(UINT));
	return TRUE;
}

void CWinNet::SetSendHeader(short sType,BOOL bZipData,const CString& strAuthorization)
{
	m_strSendHeader.Empty();
	CString strHeader(g_strHeader);
	if(bZipData && 6 != sType)
		strHeader += _T("Accept-Encoding: gzip, deflate\r\n");
	switch(sType)
	{
	/// GET，POST
	case 1:
		m_strSendHeader.Format(_T("%s%s"),strHeader,strAuthorization);
		break;
	/// 内部接口，不区分
	case 5:
		m_strSendHeader = strHeader;
		break;
	/// 下载文件
	case 6:
		m_strSendHeader.Format(_T("Accept: */*\r\n%s%s"),strHeader,strAuthorization);
		break;
	default:
		m_strSendHeader = strAuthorization;
		break;
	}
}

DWORD CWinNet::GetDownFileSize()
{
	return m_dwFileSize;
}

DWORD CWinNet::GetDownloadedSize()
{
	return m_dwDownloadSize;
}

BOOL CWinNet::StopDownloadFile()
{
	if(NULL == m_hDownThread)
	{
		CloseNetHandle(m_hRequest);
		return TRUE;
	}
	CloseNormalHandle(m_hConnectedEvent);

	if(NULL == m_hRequest && NULL == m_hDownThread)
		return TRUE;

	//初始状态无信号
	if(NULL == m_hStopRequestEvent)
		m_hStopRequestEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	ResetEvent(m_hStopRequestEvent);

	CloseNetHandle(m_hRequest);
	/// 等待结束
	DWORD dwRet=WaitForSingleObject(m_hStopRequestEvent,INFINITE);
	ResetEvent(m_hStopRequestEvent);
	CloseNormalHandle(m_hStopRequestEvent);

	CloseNormalHandle(m_hDownThread);

	return TRUE;
}

BOOL CWinNet::IsIdle(BOOL bAllFlag)
{
	if(NULL != m_hRequest)
	{
		if(bAllFlag)
			return FALSE;
		if(NULL != m_hDownThread)
			return FALSE;
		/// 没有进入下载线程
		return TRUE;
	}

	CloseNormalHandle(m_hDownThread);
	return TRUE;
}

UINT CWinNet::ParserUrl(const CString& strURL,CString& strServerName,CString& strVirtualUrl)
{
	m_bSecureFlag = FALSE;
	UINT uiPort= 0;
	CString strUrl = strURL;
	if(!m_uiPort)
	{
		if(-1 != strUrl.Find(_T("s://")) || -1 != strUrl.Find(_T("S://")))
		{
			m_bSecureFlag = TRUE;
			uiPort = INTERNET_DEFAULT_HTTPS_PORT;
		}
		else
			uiPort = API_WEB_PORT;
	}
	else
	{
		if(-1 != strUrl.Find(_T("s://")) || -1 != strUrl.Find(_T("S://")))
			m_bSecureFlag = TRUE;
		uiPort = m_uiPort;/// 指定端口
	}

	int iFind=strUrl.Find(_T("://"));
	if(-1 != iFind)
	{
		//有头
		strServerName=strUrl.Left(iFind+3);
		strUrl.Delete(0,iFind+3);
		iFind=-1;
	}
	else
		strServerName=_T("http://");//缺省头
	iFind=strUrl.Find(_T('/'));
	if(-1 != iFind)
	{
		//有子路径
		CString strTemName=strUrl.Left(iFind);
		strUrl.Delete(0,iFind);//保留/
		strVirtualUrl=strUrl;
		iFind=-1;
		iFind=strTemName.Find(_T(':'));
		if(-1 != iFind)
		{
			//有端口号
			strServerName+=strTemName.Left(iFind);
			strTemName.Delete(0,iFind+1);
			iFind=-1;
			uiPort=CBaseFuncLib::StrToNum(strTemName);
		}
		else
			strServerName+=strTemName;
	}
	else
	{
		//没有子路径
		iFind=strUrl.Find(_T(':'));
		if(-1 != iFind)
		{
			//有端口号
			strServerName+=strUrl.Left(iFind);
			strUrl.Delete(0,iFind+1);
			iFind=-1;
			uiPort=CBaseFuncLib::StrToNum(strUrl);
		}
		else
			strServerName+=strUrl;
	}
	return uiPort;
}

BOOL CWinNet::RequestUrl(const CString& strUrl,ENetReqID eServiceID,
			DWORD dwConnectMode,const CString& strDownFilePath,
			const CString& strMethod)
{
	m_eServiceID = eServiceID;

	CString strServer(_T("")),strURL(_T(""));
	INTERNET_PORT port = ParserUrl(strUrl,strServer,strURL);
	return OpenURL(strServer,strURL,dwConnectMode,strDownFilePath,port,strMethod);
}

BOOL CWinNet::OpenURL(const CString& strServer, const CString& strUrl,
			DWORD dwConnectMode,const CString& strDownFilePath,
			INTERNET_PORT port,const CString& strMethod)
{
	if(NULL != m_hDownThread)
	{
		ATLASSERT(0);
		CString strErrInfo(_T("上一次网络操作还没有完成。"));
		SetError(ERROR_HTTP_CONNECT,strErrInfo);
		return FALSE;
	}
	ResetMemberVariant();//恢复成员变量初始状态
	m_strDownFilePath=strDownFilePath;//记录下载文件路径

	if(!OpenInternet(dwConnectMode) || NULL == m_hInternet)
		return FALSE;
	CloseNetHandle(m_hConnect);

	if(INTERNET_FLAG_ASYNC==dwConnectMode)
	{
		m_hConnectedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hRequestOpenedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		m_hRequestCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	CString strHost=strServer;
	int iFind=-1;
	iFind=strHost.Find(_T("://"));
	if(-1!=iFind)
		strHost.Delete(0,iFind+3);

	DWORD dwFlags = INTERNET_FLAG_NO_UI;
	if(m_bSecureFlag)
		dwFlags |= INTERNET_FLAG_SECURE;
	m_hConnect = ::InternetConnect(m_hInternet,strHost,port,NULL,  
		NULL,INTERNET_SERVICE_HTTP,dwFlags,(DWORD)this);
/*
#ifdef APP_LOG_ENABLE
	CString strLogInfo(_T(""));
	strLogInfo.Format(_T("WeiNet::OpenURL Object:%s Host:%s Port:%d"),\
		CBaseFuncLib::NumToStr((DWORD)this),strHost,port);
	WRITELOGTOFILE(strLogInfo);
#endif
*/
	if(NULL == m_hConnect)
	{
		DWORD dwErrCode = ::GetLastError();
		if(ERROR_IO_PENDING == dwErrCode)
		{
			//初始状态无信号
			WaitForSingleObject(m_hConnectedEvent, INFINITE);
			ResetEvent(m_hConnectedEvent);
		}

		if((INTERNET_FLAG_ASYNC==dwConnectMode &&  dwErrCode!= ERROR_IO_PENDING)
			|| INTERNET_FLAG_ASYNC!=dwConnectMode)
		{
			if(dwErrCode)
			{
				CString strErrInfo=ErrorInfo(_T("InternetConnect"),dwErrCode);
				if(!strErrInfo.IsEmpty())
					SetError(ERROR_HTTP_CONNECT,strErrInfo);
				else
					SetError(dwErrCode,_T("网络连接出错。"));//网络连接错误
				return FALSE;
			}
		}
	}
	m_strMethod=strMethod;
	m_strUrl=strUrl;
	m_dwConnectMode=dwConnectMode;
	return OpenRequest();
}

BOOL CWinNet::OpenRequest()
{
	if(NULL == m_hConnect)
		return FALSE;
	DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_KEEP_CONNECTION;//加入保存连接的标志
	if(m_bNoCookie)
		dwFlags |= INTERNET_FLAG_NO_COOKIES;/// 不自动设置cookie，程序设置cookie
	dwFlags |= INTERNET_FLAG_NO_CACHE_WRITE;
	if(m_bSecureFlag)
	{
		dwFlags |= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID
				|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);
	}
	if(ENETREQUEST_UPLOAD_IMG == m_eServiceID)
		dwFlags |= INTERNET_FLAG_NO_AUTO_REDIRECT;/// 禁止重定向

	if(ENETREQUEST_WEBPAGE_GET == m_eServiceID
		|| ENETREQUEST_DOWNLOAD_FILE == m_eServiceID)
		dwFlags |= INTERNET_FLAG_TRANSFER_BINARY;

	CString strUrl(m_strUrl);
	if(0 != m_strMethod.CompareNoCase(TDHXKJ_HTTPMETHOD_GET))
	{
		if(m_strPostFilePath.IsEmpty() || !CBaseFuncLib::IsPathExist(m_strPostFilePath))
		{
			int nFind = m_strUrl.Find(_T("?"));
			ATLASSERT(-1 != nFind);
			if(-1 != nFind)
			{
				strUrl = m_strUrl.Left(nFind);
				m_strUrl.Delete(0,nFind+1);
			}
			else
			{
				strUrl = m_strUrl;
				m_strUrl.Empty();
			}
		}
	}

	m_hRequest = ::HttpOpenRequest(m_hConnect,m_strMethod,strUrl,NULL, 
				   NULL,NULL,dwFlags,(DWORD)this);
	strUrl.Empty();

	if (NULL == m_hRequest)
    {
		DWORD dwErrCode = ::GetLastError();
		if(ERROR_IO_PENDING == dwErrCode)
		{		
			WaitForSingleObject(m_hRequestOpenedEvent, INFINITE);
			ResetEvent(m_hRequestOpenedEvent);
		}

		if((INTERNET_FLAG_ASYNC==m_dwConnectMode && dwErrCode != ERROR_IO_PENDING)
			|| INTERNET_FLAG_ASYNC!=m_dwConnectMode)
		{
			if(dwErrCode)
			{
				CString strErrInfo=ErrorInfo(_T("HttpOpenRequest"),dwErrCode);
				if(!strErrInfo.IsEmpty())
					SetError(ERROR_HTTP_OPENREQUEST,strErrInfo);
				else
					SetError(dwErrCode,_T("网络请求出错。"));//网络请求错误
				return FALSE;
			}
		}
    }
	return SendRequest();
}

BOOL CWinNet::SendRequest()
{
	if(NULL == m_hRequest)
	{
#ifdef APP_LOG_ENABLE
			WRITELOGTOFILE(CString(_T("CWinNet::SendRequest NULL HANDLE")));
#endif
			return FALSE;//句柄无效
	}
	if(m_sTrySendRequest)
	{
		/// 先结束本次请求
		::HttpEndRequest(m_hRequest,NULL,0,0);
	}
	else
	{
		UINT nConnectRetries = 3;
		UINT nConnectRetryTime = 1000;
		UINT nNormalOverTime = 30*1000;
		UINT nDataOverTime = 60*1000;

		if(ENETREQUEST_UPLOAD_IMG == m_eServiceID)
		{
			nNormalOverTime = 60*1000;
			nDataOverTime = 3*60*1000;
		}

		::InternetSetOption(m_hRequest,INTERNET_OPTION_SEND_TIMEOUT,\
			&nNormalOverTime,sizeof(UINT));
		::InternetSetOption(m_hRequest,INTERNET_OPTION_RECEIVE_TIMEOUT,\
			&nNormalOverTime,sizeof(UINT));
		::InternetSetOption(m_hRequest,INTERNET_OPTION_DATA_SEND_TIMEOUT,\
			&nDataOverTime,sizeof(UINT));
		::InternetSetOption(m_hRequest,INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,\
			&nDataOverTime,sizeof(UINT));
			
		::InternetSetOption(m_hRequest,INTERNET_OPTION_CONNECT_BACKOFF,\
			&nConnectRetryTime,sizeof(UINT));
		::InternetSetOption(m_hRequest,INTERNET_OPTION_CONNECT_RETRIES,\
			&nConnectRetries,sizeof(UINT));

		if(m_strDownFilePath.IsEmpty())
		{
			CString strTmpPath(_T(""));
			COleDateTime curTime = COleDateTime::GetCurrentTime();
			strTmpPath.Format(_T("%s%d"),CBaseFuncLib::GetTmpPath(),curTime.GetHour());
			CBaseFuncLib::CreatePath(strTmpPath);
			m_strDownFilePath.Format(_T("%s\\%d_%ld_%ld%s"),strTmpPath,\
				m_eServiceID,(DWORD)::GetCurrentProcessId(),::rand(),TEMP_EXT);
			/// 删除可能已经存在的文件
			::DeleteFile(m_strDownFilePath);
		}

		ReadyDownloadFile(FALSE,TRUE);

		if(!m_strSendHeader.GetLength())
			SetSendHeader(0,FALSE);
	}
/*
#ifdef APP_LOG_ENABLE
	CString strLogInfo(_T(""));
	strLogInfo.Format(_T("WeiNet::SendRequest Object:%s Method:%s"),\
		CBaseFuncLib::NumToStr((DWORD)this),m_strMethod);
	WRITELOGTOFILE(strLogInfo);
#endif
*/
	CString strRequestRange(_T(""));
	BOOL bDownloadFile=FALSE,bSendRequest=FALSE;
	CString strExt(m_strDownFilePath.Right(4));
	if(strExt.CompareNoCase(TEMP_EXT)!=0)
		bDownloadFile=TRUE;

	DWORD dwPostLen = 0;

	if(m_bSecureFlag)
	{
		DWORD dwFlags = 0; 
		DWORD dwBuffLen = sizeof(dwFlags); 
		InternetQueryOption (m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwBuffLen); 
		dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA; 
		InternetSetOption (m_hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags,sizeof (dwFlags));
	}

	if(bDownloadFile)
	{
		strRequestRange.Format(_T("Range:bytes=%s-"),CBaseFuncLib::NumToStr(m_dwDownloadSize));//文件下载支持断点续传
		bSendRequest=::HttpSendRequest(m_hRequest,NULL,\
			0,NULL,0);
	}
	else
	{
		if(0 != m_strMethod.CompareNoCase(TDHXKJ_HTTPMETHOD_GET))
		{
			DelPostData();
			if(!m_strPostFilePath.IsEmpty() && CBaseFuncLib::IsPathExist(m_strPostFilePath))
			{
				/// 考虑HttpSendRequestEx 配合 InternetWriteFile 和 HttpEndRequest发送大量数据
				/// ::HttpAddRequestHeaders(m_hRequest,m_strSendHeader,m_strSendHeader.GetLength(),HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);
				/// 读取POST数据，固定的文件名
				dwPostLen = CBaseFuncLib::ReadAllData(m_strPostFilePath,&m_szPostBuff);
				if(!dwPostLen)
				{
#ifdef APP_LOG_ENABLE
					ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(m_strPostFilePath);
					/// 跟踪失败
					CString strLogInfo(_T(""));
					strLogInfo.Format(_T("WeiNet::SendRequest 请求 %d 内存不足，大小：%ld PostUrl:%s"),\
						m_eServiceID,dwFileSize,m_strUrl);
					WRITELOGTOFILE(strLogInfo);
#endif
				}
			}
			else
			{
				dwPostLen = CBaseFuncLib::Us2ToChar(m_strUrl,(char **)&m_szPostBuff)-1;
/*
#ifdef APP_LOG_ENABLE
				if(ENETREQUEST_STATUSES_REPOST == m_eServiceID)
				{
					/// 跟踪转发微博失败
					CString strLogInfo(_T(""));
					strLogInfo.Format(_T("WeiNet::SendRequest Object:%s Post:%s"),\
						CBaseFuncLib::NumToStr((DWORD)this),m_strUrl);
					WRITELOGTOFILE(strLogInfo);
				}
#endif
*/
			}
			ATLASSERT(m_szPostBuff);
			if(NULL != m_szPostBuff)
			{
				bSendRequest = ::HttpSendRequest(m_hRequest,m_strSendHeader,m_strSendHeader.GetLength(),\
					(LPVOID)m_szPostBuff,dwPostLen);
				if(bSendRequest)
				{
					DelPostData();
				}
			}
			else
			{
				SetError(ERROR_HTTP_QUERYINFO,_T("内存不足"));//内存不足
				return FALSE;
			}
		}
		else
			bSendRequest=::HttpSendRequest(m_hRequest,m_strSendHeader,m_strSendHeader.GetLength(),NULL,0);
	}
	while (!bSendRequest)
	{
		DWORD dwErrCode=::GetLastError();
		if(INTERNET_FLAG_ASYNC==m_dwConnectMode)
		{
			if(ERROR_IO_PENDING==dwErrCode)
			{
				return TRUE;//异步时进入重载操作，很快返回
			}
		}
		else
		{
			if(ERROR_INTERNET_NAME_NOT_RESOLVED == dwErrCode 
				|| ERROR_INTERNET_TIMEOUT == dwErrCode )
			{
				/// 服务器无法解析或超时
				::Sleep(500);
			}
		}
		if(dwErrCode)
		{
			if(m_sTrySendRequest)
				dwErrCode = ERROR_INTERNET_CONNECTION_RESET;
			CString strErrInfo=ErrorInfo(_T("HttpSendRequest"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(dwErrCode,strErrInfo);
			else
				SetError(ERROR_HTTP_SENDREQUEST,_T("网络发送出错。"));//网络发送错误
			if(m_sTrySendRequest)
			{
#ifdef APP_LOG_ENABLE
				CString strFileName(m_strDownFilePath);
				int nFileFind = strFileName.ReverseFind(_T('\\'));
				strFileName = strFileName.Right(strFileName.GetLength()-nFileFind-1);
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("WeiNet::HttpSendRequest 服务%d，错误码：%ld 错误信息：%s，文件：%s"),\
					this->GetCurServiceID(),dwErrCode,strErrInfo,strFileName);
				WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif

				/// 发送错误通知
				if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
					::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )dwErrCode,(LPARAM )GetCurServiceID());
			}
		}
		DelPostData();
		return FALSE;
	}
	DelPostData();
	return bSendRequest;
}

CString CWinNet::ErrorInfo(const CString& strFunction,DWORD dwErrCode)
{
	CString strErrInfo(_T(""));
	if(dwErrCode==ERROR_INTERNET_OPERATION_CANCELLED)
		return strErrInfo;//操作被取消，不报错
	if(ERROR_INTERNET_TIMEOUT == dwErrCode)
	{
		strErrInfo = _T("12002:操作超时，请检查网络连接是否正常，稍后再试，谢谢！");
		return strErrInfo;
	}
	HINSTANCE hWinINetLibrary = NULL;
	hWinINetLibrary = ::LoadLibrary(_T("wininet.dll"));

	LPTSTR lpBuffer=NULL;
	if(NULL != hWinINetLibrary)
	{
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
			hWinINetLibrary, dwErrCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL);
	}
	if(NULL==lpBuffer)
	{
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, dwErrCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
			(LPTSTR) &lpBuffer, 0, NULL);
	}
	if(NULL!=lpBuffer)
	{
		CString strFileName(m_strDownFilePath);
		int nFileFind = strFileName.ReverseFind(_T('\\'));
		strFileName = strFileName.Right(strFileName.GetLength()-nFileFind-1);

		strErrInfo.Format(_T("%s错误码:%s 文件：%s\r\n错误信息：%s"),\
			strFunction,CBaseFuncLib::NumToStr(dwErrCode),strFileName,lpBuffer);
		LocalFree(lpBuffer);
	}
#ifndef _AFXDLL
	if(hWinINetLibrary)
	{
		::FreeLibrary(hWinINetLibrary);
		hWinINetLibrary = NULL;
	}
#endif
	return strErrInfo;
}

void CWinNet::GetFileType()
{
	if(NULL==m_hRequest)
		return;
	TCHAR szBuf[1024];
	memset(szBuf,0,1024*sizeof(TCHAR));
	DWORD dwSize = sizeof(szBuf);
	::HttpQueryInfo (m_hRequest,HTTP_QUERY_CONTENT_DISPOSITION,\
		&szBuf, &dwSize, NULL);
	if(!dwSize)
	{
#ifdef APP_LOG_ENABLE
		DWORD dwErrCode = ::GetLastError();
		if(dwErrCode)
		{
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("WeiNet::GetFileType 失败，错误码：%ld"),dwErrCode);
			WRITELOGTOFILE(strLogInfo);
			strLogInfo.Empty();
		}
#endif
		return;
	}
	m_strServerFileName.Empty();
	CString strFileName(szBuf);
	int nFind = strFileName.Find(_T("=\""));
	if(-1 != nFind)
		strFileName.Delete(0,nFind+2);
	nFind = strFileName.Find(_T("\""));
	if(-1 != nFind)
		strFileName = strFileName.Left(nFind);
	m_strServerFileName = CBaseFuncLib::PercentStringDecode(strFileName,TRUE);
	strFileName.Empty();
}

void CWinNet::QuerytLocation()
{
	if(NULL==m_hRequest)
		return;
	TCHAR szBuf[1024];
	memset(szBuf,0,1024*sizeof(TCHAR));
	DWORD dwSize = sizeof(szBuf);
	::HttpQueryInfo (m_hRequest,HTTP_QUERY_LOCATION,\
		&szBuf, &dwSize, NULL);
	if(!dwSize)
	{
		DWORD dwErrCode=::GetLastError();
		if(dwErrCode)
		{
			CString strErrInfo=ErrorInfo(_T("HttpQueryInfo QuerytLocation"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_STATUSCODE,strErrInfo);//网络错误
			else
				SetError(dwErrCode,_T("查询网络返回数据出错"));
		}
		return;
	}
	m_strLocation = szBuf;
}

DWORD CWinNet::GetHttpStatus(BOOL bCheckFlag)
{
	if(!bCheckFlag)
		return m_dwHttpStatus;
	DWORD dwStatusCode=0;
	if(NULL==m_hRequest)
		return dwStatusCode;
	DWORD dwSize = sizeof(dwStatusCode);
	::HttpQueryInfo (m_hRequest,HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,\
		&dwStatusCode, &dwSize, NULL);
	if(!dwSize)
	{
		DWORD dwErrCode=::GetLastError();
		if(dwErrCode)
		{
			CString strErrInfo = ErrorInfo(_T("HttpQueryInfo GetHttpStatus"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_STATUSCODE,strErrInfo);//网络错误
			else
				SetError(dwErrCode,_T("查询网络返回数据出错"));
		}
	}
	return dwStatusCode;
}

BOOL CWinNet::GetDownContent(DWORD &dwContentLength,BOOL bDownloadFile)
{
	dwContentLength=0;
	if(NULL==m_hRequest)
		return FALSE;
	DWORD dwSize = sizeof(dwContentLength);
	::HttpQueryInfo (m_hRequest,HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER ,\
		&dwContentLength, &dwSize, NULL);
	if(!dwSize)
	{
		DWORD dwErrCode=::GetLastError();
		if(12150 != dwErrCode && dwErrCode)
		{
			CString strErrInfo=ErrorInfo(_T("HttpQueryInfo GetDownContent"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_QUERYINFO,strErrInfo);
			else
				SetError(dwErrCode,_T("查询网络返回数据出错"));
			return FALSE;
		}
	}

	//不是所有的请求都可以返回长度
	return TRUE;
}

BOOL CWinNet::SaveHeaderInfo()
{
	if(NULL == m_hRequest)
		return FALSE;
	DWORD dwSize = 0;
	::HttpQueryInfo(m_hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, NULL, &dwSize,NULL);
	if(!dwSize)
	{
		DWORD dwErrCode = ::GetLastError();
		if(12150 == dwErrCode)
			return TRUE;
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("WeiNet::SaveHeaderInfo 失败，错误码：%ld"),dwErrCode);
		WRITELOGTOFILE(strLogInfo);
		strLogInfo.Empty();
#endif
		return FALSE;
	}
	TCHAR *lpHeaderInfo = NULL;

	int len = dwSize/sizeof(TCHAR)+1;
	lpHeaderInfo = new TCHAR[len];
	if(NULL == lpHeaderInfo)
	{
		SetError(ERROR_HTTP_QUERYINFO,_T("内存不足"));//内存不足
		return FALSE;
	}
	::memset(lpHeaderInfo,0,len*sizeof(TCHAR));
	::HttpQueryInfo (m_hRequest,HTTP_QUERY_RAW_HEADERS_CRLF,\
		(LPVOID)lpHeaderInfo, &dwSize, NULL);
	if(dwSize)
	{
		/// 获得请求返回的头信息
		m_strRetHeader = lpHeaderInfo;
	}
	else
	{
		DWORD dwErrCode=GetLastError();
		if(dwErrCode)
		{
			CString strErrInfo=ErrorInfo(_T("SaveHeaderInfo"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_QUERYINFO,strErrInfo);//直接报错误
			else
				SetError(dwErrCode,_T("查询网络返回数据出错"));
			return FALSE;
		}
	}
	delete []lpHeaderInfo;
	lpHeaderInfo=NULL;
	return TRUE;
}

BOOL CWinNet::SaveResponseInfo(DWORD dwThreadPri)
{
	if(NULL != m_hDownThread)
	{
		return TRUE;//已经在下载文件
	}
	CString strExt(m_strDownFilePath.Right(4));
	if(0 != strExt.CompareNoCase(TEMP_EXT))
		return DownFile(m_strDownFilePath,dwThreadPri);//下载文件

	DWORD dwThreadID = 0;
	m_hDownThread = CREATE_MYTHREAD(DownloadRequestThread,this,dwThreadID);
	if(NULL != m_hDownThread)
	{
		::SetThreadPriority(m_hDownThread,dwThreadPri);
		return TRUE;
	}
#ifdef APP_LOG_ENABLE
	DWORD dwErrCode = ::GetLastError();
	if(dwErrCode)
	{
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("WeiNet::SaveResponseInfo Object:%s CreateThread Failed，错误码：%ld"),\
			CBaseFuncLib::NumToStr((DWORD)this),dwErrCode);
		WRITELOGTOFILE(strLogInfo);
		strLogInfo.Empty();
	}
#endif
	return FALSE;
}

UINT CWinNet::DownloadRequestThread(LPVOID pParam)
{
	UINT uiRet=-1;
	CWinNet *pHttpVisit=(CWinNet *)pParam;
	ATLASSERT(pHttpVisit);

	uiRet=pHttpVisit->DownloadRequestProc();//执行下载普通数据过程
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(pHttpVisit);
	if(it == g_mapWeibNet.end())
		return 0;
	if(NULL != pHttpVisit->m_hStopRequestEvent)
		pHttpVisit->SetWaitEvent(pHttpVisit->m_hStopRequestEvent);

	pHttpVisit->CloseNetHandle(pHttpVisit->m_hRequest);

	pHttpVisit->CloseNormalHandle(pHttpVisit->m_hDownThread);/// 标记线程已经结束

	return uiRet;
}

UINT CWinNet::DownloadFileThread(LPVOID pParam)
{
	UINT uiRet=-1;
	CWinNet *pHttpVisit=(CWinNet *)pParam;
	ATLASSERT(pHttpVisit);

	uiRet = pHttpVisit->DownloadFileProc();/// 执行下载文件过程
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(pHttpVisit);
	if(it == g_mapWeibNet.end())
		return 0;
	if(NULL != pHttpVisit->m_hStopRequestEvent)
		pHttpVisit->SetWaitEvent(pHttpVisit->m_hStopRequestEvent);

	pHttpVisit->CloseNetHandle(pHttpVisit->m_hRequest);
	pHttpVisit->CloseNormalHandle(pHttpVisit->m_hDownThread);/// 标记线程已经结束
	return uiRet;
}

BOOL CWinNet::ReadyDownloadFile(BOOL bCreateFlag,BOOL bCloseFlag)
{
	CloseDownloadFile();
	m_dwDownloadSize = 0;
	if(CBaseFuncLib::IsPathExist(m_strDownFilePath))
	{
		//已经存在
		m_hDownFile = ::CreateFile(m_strDownFilePath,GENERIC_WRITE,0,NULL,\
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == m_hDownFile)
		{
			DWORD dwErrCode=::GetLastError();
			if(dwErrCode)
			{
				CString strErrInfo = ErrorInfo(_T("CreateFile"),dwErrCode);
				if(!strErrInfo.IsEmpty())
					SetError(ERROR_HTTP_OPENFILE,strErrInfo);
			}
			m_hDownFile = NULL;
			if(m_hParentWnd && ::IsWindow(m_hParentWnd))
			{
				::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
					(WPARAM )ERROR_HTTP_OPENFILE,(LPARAM )m_eServiceID);
			}
			return FALSE;
		}
		m_dwDownloadSize = GetFileSize (m_hDownFile,NULL) ;
		if (m_dwDownloadSize == 0xFFFFFFFF)
		{ 
			DWORD dwErrCode=::GetLastError();
			CloseDownloadFile();
			if(dwErrCode)
			{
				CString strErrInfo=ErrorInfo(_T("GetFileSize"),dwErrCode);
#ifdef APP_LOG_ENABLE
/*
				CString strLogInfo(_T(""));
				strLogInfo.Format(_T("WeiNet::ReadyDownloadFile Open Object:%s 错误码：%ld,错误描述：%s"),\
					CBaseFuncLib::NumToStr((DWORD)this),dwErrCode,strErrInfo);
				WRITELOGTOFILE(strLogInfo);
				strLogInfo.Empty();
*/
#endif				
				if(!strErrInfo.IsEmpty())
					SetError(ERROR_HTTP_GETFILESIZE,strErrInfo);
			}
			if(m_hParentWnd && ::IsWindow(m_hParentWnd))
			{
				::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
					(WPARAM )ERROR_HTTP_GETFILESIZE,(LPARAM )m_eServiceID);
			}
			return FALSE;
		}
	}
	else
	{
		if(bCreateFlag)
		{
			m_hDownFile = ::CreateFile(m_strDownFilePath,GENERIC_WRITE,0,NULL, \
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
			if (INVALID_HANDLE_VALUE == m_hDownFile)
			{
				DWORD dwErrCode=::GetLastError();
				if(dwErrCode)
				{
					CString strErrInfo=ErrorInfo(_T("CreateFile"),dwErrCode);
#ifdef APP_LOG_ENABLE
/*
					CString strLogInfo(_T(""));
					strLogInfo.Format(_T("WeiNet::ReadyDownloadFile Create Object:%s 错误码：%ld,错误描述：%s"),\
						CBaseFuncLib::NumToStr((DWORD)this),dwErrCode,strErrInfo);
					WRITELOGTOFILE(strLogInfo);
					strLogInfo.Empty();
*/
#endif								
					if(!strErrInfo.IsEmpty())
						SetError(ERROR_HTTP_OPENFILE,strErrInfo);
				}
				m_hDownFile = NULL;
				if(m_hParentWnd && ::IsWindow(m_hParentWnd))
				{
					::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
						(WPARAM )ERROR_HTTP_OPENFILE,(LPARAM )m_eServiceID);
				}
				return FALSE;
			}
		}
	}
	if(bCloseFlag && m_hDownFile)
		CloseDownloadFile();
	return TRUE;
}

BOOL CWinNet::DownFile(const CString& filename,DWORD dwThreadPri)
{
	if(NULL == m_hRequest)
		return FALSE;
	if(0!=filename.CompareNoCase(m_strDownFilePath))
	{
		m_strDownFilePath.Empty();
		m_strDownFilePath=filename;
	}
	DWORD dwThreadID = 0;
	m_hDownThread = CREATE_MYTHREAD(DownloadFileThread,this,dwThreadID);
	if(NULL != m_hDownThread)
		::SetThreadPriority(m_hDownThread,dwThreadPri);
	return m_hDownThread != NULL;
}

void CWinNet::SetError( long lErrCode, const CString& strMessage )
{
	if(lErrCode)
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("WeiNet::SetError 错误码：%ld,命令：%ld，URL：%s,描述：%s"),\
			lErrCode,m_eServiceID,m_strUrl,strMessage);
		WRITELOGTOFILE(strLogInfo);
		strLogInfo.Empty();
#endif
	}
	__super::SetError(lErrCode,strMessage);
}

void CWinNet::SetWaitEvent(HANDLE hEvent)
{
	if(NULL == hEvent)
		return;
	::SetEvent(hEvent);
}

UINT CWinNet::DownloadRequestProc()
{
	if(NULL == m_hRequest)
		return FALSE;
	if(ENETREQUEST_UPLOAD_IMG == m_eServiceID)
		QuerytLocation();
	m_dwHttpStatus = GetHttpStatus(TRUE);
	if(ENETREQUEST_UPLOAD_IMG == m_eServiceID && 302 == m_dwHttpStatus)
	{
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
	
		if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
			::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )0,(LPARAM )ENETREQUEST_UPLOAD_IMG);
		return TRUE;
	}

	if( m_dwHttpStatus != HTTP_STATUS_OK
		&& m_dwHttpStatus != 403
		&& m_dwHttpStatus != 401
		&& m_dwHttpStatus != 400)
	{
		CString strErrInfo(_T(""));
		if(m_dwHttpStatus)
		{
			strErrInfo.Format(_T("网络请求出错！状态码：%ld"),m_dwHttpStatus);
			SetError(ERROR_HTTP_STATUSCODE,strErrInfo);//网络错误
		}

#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("WeiNet::DownloadRequestProc 服务 %d 查询状态出错，信息：%s"),\
			this->GetCurServiceID(),strErrInfo);
		WRITELOGTOFILE2(strLogInfo,_T(""),LOGTYPE_NORMAL);
#endif
		if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
			::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )ERROR_HTTP_STATUSCODE,(LPARAM )GetCurServiceID());
		return FALSE;
	}

	DWORD dwContentLength = 0;
	if(!GetDownContent(dwContentLength))
		return FALSE;
	if(!SaveHeaderInfo())
		return FALSE;
	m_dwFileSize = dwContentLength;

	DWORD dwErrCode  = 0;
	if(!CBaseFuncLib::IsPathExist(m_strDownFilePath))
	{
		m_hDownFile = ::CreateFile(m_strDownFilePath,GENERIC_WRITE,0,NULL,\
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		m_hDownFile = ::CreateFile(m_strDownFilePath,GENERIC_WRITE,0,NULL,\
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	if(INVALID_HANDLE_VALUE == m_hDownFile)
	{
		/// 创建文件失败
		dwErrCode = ::GetLastError();
		CString strErrInfo = ErrorInfo(_T("CreateFile"),dwErrCode);
		if(!strErrInfo.IsEmpty())
		{
			this->SetError(dwErrCode,strErrInfo);
		}
		m_hDownFile = NULL;
		if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
			::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )dwErrCode,(LPARAM )GetCurServiceID());
		return FALSE;
	}
	ATLASSERT(m_hDownFile != INVALID_HANDLE_VALUE);
	m_dwDownloadSize=0;

	short sFormatFlag = 0;
	BYTE *lpBufferByte = NULL;
	DWORD dwDownloaded = 0;

#ifdef APP_LOG_ENABLE
#ifdef _DEBUG
	CString strLogInfo(_T(""));
	strLogInfo.Format(_T("WeiNet::DownloadRequestProc Object:%s:%s 大小:%s 开始下载"),\
		CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath,CBaseFuncLib::NumToStr(m_dwFileSize));
	WRITELOGTOFILE(strLogInfo);
#endif
#endif

	try
	{
		DWORD dwReadBuf = 0;
		do
		{
			DWORD dwSize = 0;//先获得有多少可用数据
			BOOL bReadFlag = FALSE;

			if(m_dwFileSize-m_dwDownloadSize < NET_READ_MAXBUFFER)
				dwSize = m_dwFileSize-m_dwDownloadSize;
			if(!dwSize)
				dwSize = NET_READ_MAXBUFFER;
			ATLASSERT(dwSize);
			if(NULL == lpBufferByte)
			{
				dwReadBuf = dwSize;
				lpBufferByte = new BYTE[dwReadBuf+1];
			}
			ATLASSERT(lpBufferByte);
			if(NULL == lpBufferByte)
			{
				CloseDownloadFile();
				CString strErrInfo = ErrorInfo(_T("内存不足"),ERROR_HTTP_INTERNETREADFILE);
				SetError(ERROR_HTTP_INTERNETREADFILE,strErrInfo);//直接报错误
				CloseNormalHandle(m_hDownThread);//标记线程已经结束

				if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
					::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )ERROR_HTTP_INTERNETREADFILE,(LPARAM )GetCurServiceID());
				return FALSE;
			}
TRY_READ:
			dwDownloaded = 0;
			::memset(lpBufferByte,0,(dwReadBuf+1)*sizeof(BYTE));
			if(INTERNET_FLAG_ASYNC == m_dwConnectMode && NULL != m_hRequestCompleteEvent)
				ResetEvent(m_hRequestCompleteEvent);
			bReadFlag=::InternetReadFile(m_hRequest,(LPVOID)lpBufferByte,dwSize,&dwDownloaded);
			if( bReadFlag && !dwDownloaded)
			{
				if(NULL != lpBufferByte)
				{
					delete []lpBufferByte;
					lpBufferByte=NULL;
				}
				break;//已经读取完成
			}
			if (!bReadFlag && !dwDownloaded)
			{
				dwErrCode=::GetLastError();
				if(!dwErrCode)
				{
					::Sleep(100);
					goto TRY_READ;/// 循环读取
				}
				else if(dwErrCode== ERROR_IO_PENDING)
				{
					DWORD dwRet=WaitForSingleObject(m_hRequestCompleteEvent,INFINITE);
					ResetEvent(m_hRequestCompleteEvent);
					dwDownloaded = dwSize;
				}
				else
				{
					CloseDownloadFile();
					if(NULL != m_hRequest)
					{
						CString strErrInfo = ErrorInfo(_T("InternetReadFile"),dwErrCode);
						if(!strErrInfo.IsEmpty())
							SetError(dwErrCode,strErrInfo);//直接报网络连接错误
						else
						{
#ifdef APP_LOG_ENABLE
							SetError(dwErrCode,_T("网络读取文件出错"));//网络读取文件错误
#endif
						}
					}
					if(NULL != lpBufferByte)
					{
						delete []lpBufferByte;
						lpBufferByte=NULL;
					}
					CloseNormalHandle(m_hDownThread);/// 标记线程已经结束

					if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
						::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )ERROR_HTTP_INTERNETREADFILE,(LPARAM )GetCurServiceID());
					return FALSE;
				}
			}
			if(NULL != m_hStopRequestEvent || NULL == m_hDownFile)
			{
				if(NULL != lpBufferByte)
				{
					delete []lpBufferByte;
					lpBufferByte=NULL;
				}
				CloseDownloadFile();
				CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
				//停止下载
				if(m_hParentWnd && ::IsWindow(m_hParentWnd))
				{
					if(!GetLastError() && NULL != m_hStopRequestEvent)
						SetError(ESOCIAL_ERROR_BASECODE,_T("网络操作被取消"));

					::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )ESOCIAL_ERROR_BASECODE,(LPARAM )GetCurServiceID());
				}
				return FALSE;
			}
			if(NULL != m_hDownFile && dwDownloaded)
			{
				lpBufferByte[dwReadBuf] = 0;
				if(0 == m_dwDownloadSize)
				{
					/// 第一个数据包，判断格式
					char cBuf[32];
					memset(cBuf,0,32*sizeof(char));
					memcpy(cBuf,lpBufferByte,32);
					CString strContent(cBuf);
					if(-1 != strContent.Find(_T("{\"")))
						sFormatFlag = 1;
					else if(-1 != strContent.Find(_T("<!DOCTYPE"))
					|| -1 != strContent.Find(_T("<html>")))
						sFormatFlag = 2;
					strContent.Empty();
				}
				DWORD dwBytesWritten = 0;
				if( !::WriteFile (m_hDownFile,lpBufferByte,dwDownloaded,&dwBytesWritten,NULL ) || dwBytesWritten != dwDownloaded)
				{
					dwErrCode=::GetLastError();
					/// 写文件失败报错！
					ATLASSERT(FALSE);
#ifdef APP_LOG_ENABLE
#ifdef _DEBUG
					CString strLogInfo(_T(""));
					strLogInfo.Format(_T("WeiNet::DownloadRequestProc Object:%s:%s 写文件失败"),\
						CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath);
					WRITELOGTOFILE(strLogInfo);
#endif
#endif
				}
			}

			/// 重置缓存
			::memset(lpBufferByte,0,(dwReadBuf+1)*sizeof(BYTE));
			m_dwDownloadSize += dwDownloaded;

			if(m_dwFileSize && m_dwDownloadSize>=m_dwFileSize)
				break;//已经下载完成
			::Sleep(50);
		} while ( dwDownloaded && !m_bAllDone );
	}
	catch ( ... )
	{
		SetError(ERROR_HTTP_INTERNETREADFILE,_T("网络请求处理结果时产生异常错误！"));
	}
	if(NULL != lpBufferByte)
	{
		delete []lpBufferByte;
		lpBufferByte = NULL;
	}
	CloseDownloadFile();

#ifdef APP_LOG_ENABLE
#ifdef _DEBUG
	strLogInfo.Empty();
	DWORD dwFileSize = (DWORD)CBaseFuncLib::GetFileSize(m_strDownFilePath);
	strLogInfo.Format(_T("WeiNet::DownloadRequestProc Object:%s:%s，大小:%s 下载结束"),\
		CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath,CBaseFuncLib::NumToStr(dwFileSize));
	WRITELOGTOFILE(strLogInfo);
#endif
#endif

	if(!sFormatFlag)
	{
		/// 需要解压
		BYTE *pData = NULL;
		ULONGLONG dwFileSize = CBaseFuncLib::GetFileSize(m_strDownFilePath);
		if(dwFileSize)
		{
			DWORD dwLen = CBaseFuncLib::ReadAllData(m_strDownFilePath,&pData);
			ATLASSERT(pData);
		}
		HANDLE hUnzipFile = NULL;
		if(NULL != pData)
		{
			::DeleteFile(m_strDownFilePath+TDHXKJ_DOWNFILE_TEMPNAME);
			::MoveFile(m_strDownFilePath,m_strDownFilePath+TDHXKJ_DOWNFILE_TEMPNAME);

			hUnzipFile = ::CreateFile(m_strDownFilePath,GENERIC_WRITE,FILE_SHARE_WRITE,\
				NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		}
		if(NULL != hUnzipFile && INVALID_HANDLE_VALUE != hUnzipFile)
		{
			int nUnzip = HttpGzDecompress(pData, (const DWORD)dwFileSize, hUnzipFile);
			if(0 != nUnzip)
			{
				CloseNormalHandle(hUnzipFile);
				::DeleteFile(m_strDownFilePath);
				::MoveFile(m_strDownFilePath+TDHXKJ_DOWNFILE_TEMPNAME,m_strDownFilePath);
#ifdef APP_LOG_ENABLE
#ifdef _DEBUG
				strLogInfo.Empty();
				strLogInfo.Format(_T("WeiNet::DownloadRequestProc Object:%s:%s 解压失败"),\
					CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath);
				WRITELOGTOFILE(strLogInfo);
#endif
#endif
			}
			else
			{
				CloseNormalHandle(hUnzipFile);
#ifndef _DEBUG
				::DeleteFile(m_strDownFilePath+TDHXKJ_DOWNFILE_TEMPNAME);
#endif
			}
		}
		else
		{
			::DeleteFile(m_strDownFilePath);
			::MoveFile(m_strDownFilePath+TDHXKJ_DOWNFILE_TEMPNAME,m_strDownFilePath);
		}
		if(NULL != pData)
		{
			delete []pData;
			pData = NULL;
		}
	}
	else if(2 == sFormatFlag)
	{
		CString strOldName(m_strDownFilePath);
		/// 修改扩展名
		m_strDownFilePath.Replace(TEMP_EXT,_T(".html"));
		/// 删除可能存在的临时文件
		if(CBaseFuncLib::IsPathExist(m_strDownFilePath))
		{
			::SetFileAttributes(m_strDownFilePath,FILE_ATTRIBUTE_NORMAL);
			::DeleteFile(m_strDownFilePath);
		}
		::MoveFile(strOldName,m_strDownFilePath);
	}

	CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
	
	if(NULL==m_hStopRequestEvent && INTERNET_FLAG_ASYNC==m_dwConnectMode && m_hParentWnd && ::IsWindow(m_hParentWnd))
		::PostMessage(m_hParentWnd,WM_REQUEST_COMPLETED,(WPARAM )0,(LPARAM )GetCurServiceID());
	return TRUE;
}

UINT CWinNet::DownloadFileProc()
{
	UINT uiRetFlag=0;
	if(NULL == m_hRequest)
	{
		if(m_hParentWnd && ::IsWindow(m_hParentWnd))
		{
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
				(WPARAM )ERROR_HTTP_OPENREQUEST,(LPARAM )m_eServiceID);
		}
		return uiRetFlag;
	}
	if(!ReadyDownloadFile())
	{
		CloseNetHandle(m_hRequest);
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
		return uiRetFlag;
	}
	m_dwHttpStatus = GetHttpStatus(TRUE);
	if(416 == m_dwHttpStatus)
	{
		//range超出范围，已经下载完成
		CloseDownloadFile();
		CloseNetHandle(m_hRequest);
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
		if(m_hParentWnd && ::IsWindow(m_hParentWnd) && INTERNET_FLAG_ASYNC==m_dwConnectMode)
		{
#ifdef APP_LOG_ENABLE
			//CString strLogInfo(_T(""));
			//strLogInfo.Format(_T("WeiNet::DownloadFileProc 下载文件 %s 时状态错误416，对象：%ld"),\
			//	m_strDownFilePath,CBaseFuncLib::NumToStr((DWORD)this));
			//WRITELOGTOFILE(strLogInfo);
#endif
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_PERCENT,(WPARAM )100,(LPARAM )m_eServiceID);
		}
		return uiRetFlag;
	}
	else if(m_dwHttpStatus != HTTP_STATUS_PARTIAL_CONTENT 
		&& m_dwHttpStatus != HTTP_STATUS_OK 
		&& m_dwHttpStatus != 0 )
	{
		if(m_dwHttpStatus)
		{
			CString strErrInfo=ErrorInfo(_T("DownloadFileProc"),m_dwHttpStatus);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_STATUSCODE,strErrInfo);//网络错误
#ifdef APP_LOG_ENABLE
			else
			{
				strErrInfo.Format(_T("下载时网络状态错误：%ld"),m_dwHttpStatus);
				SetError(m_dwHttpStatus,strErrInfo);
			}
#endif
		}

		CloseDownloadFile();
		CloseNetHandle(m_hRequest);
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束

		if(m_hParentWnd && ::IsWindow(m_hParentWnd))
		{
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
				(WPARAM )ERROR_HTTP_STATUSCODE,(LPARAM )m_eServiceID);
		}
		return uiRetFlag;
	}
	if(NULL != m_hStopRequestEvent)
	{
		CloseDownloadFile();
		CloseNetHandle(m_hRequest);
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
		if(m_hParentWnd && ::IsWindow(m_hParentWnd))
		{
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,NULL,(LPARAM )m_eServiceID);
		}
		return TRUE;
	}

	DWORD dwContentLength = 0;
	if(!GetDownContent(dwContentLength,TRUE))
	{
		CloseDownloadFile();
		CloseNetHandle(m_hRequest);
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
		if(m_hParentWnd && ::IsWindow(m_hParentWnd))
		{
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
				(WPARAM )ERROR_HTTP_QUERYINFO,(LPARAM )m_eServiceID);
		}
		return uiRetFlag;
	}
	if(dwContentLength >= NET_READ_MAXBUFFER)
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("WeiNet::DownloadFileProc Object:%s 文件 %s 大小 %s"),\
			CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath,CBaseFuncLib::NumToStr(dwContentLength));
		WRITELOGTOFILE(strLogInfo);
#endif
	}
	else
		m_dwDownloadSize = 0;/// 不能获取长度的，不支持断点续传
	m_dwFileSize = dwContentLength + m_dwDownloadSize;
	if(m_dwFileSize && m_hParentWnd && ::IsWindow(m_hParentWnd))
	{
		::PostMessage(m_hParentWnd,WM_DOWNLOAD_TOTALSIZE,\
			(WPARAM )m_dwFileSize,(LPARAM )m_eServiceID);
	}

	GetFileType();

	if(::SetFilePointer(m_hDownFile,m_dwDownloadSize,NULL,FILE_BEGIN) == 0xFFFFFFFF)
	{
		DWORD dwErrCode=GetLastError();
		CloseDownloadFile();
		CloseNetHandle(m_hRequest);
		CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
		if(dwErrCode)
		{
			CString strErrInfo=ErrorInfo(_T("SetFilePointer"),dwErrCode);
			if(!strErrInfo.IsEmpty())
				SetError(ERROR_HTTP_SEEKFILE,strErrInfo);
		}
		if(m_hParentWnd && ::IsWindow(m_hParentWnd))
		{
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
				(WPARAM )ERROR_HTTP_SEEKFILE,(LPARAM )m_eServiceID);
		}
		return uiRetFlag;
	}

	BOOL bTryFlag = FALSE;
	BYTE *lpBufferByte=NULL;
	DWORD percent=0,dwDownloaded=0;
/*
#ifdef APP_LOG_ENABLE
	strLogInfo.Empty();
	strLogInfo.Format(_T("WeiNet::DownloadFileProc Object:%s:下载大小:%s 总大小:%s 开始下载"),\
		CBaseFuncLib::NumToStr((DWORD)this),CBaseFuncLib::NumToStr(dwContentLength),CBaseFuncLib::NumToStr(m_dwFileSize));
	WRITELOGTOFILE(strLogInfo);
#endif
*/
	try
	{
		DWORD dwReadBuf = 0;
		do
		{
			DWORD dwSize=0;
			BOOL bReadFlag = TRUE;
			if(!m_dwFileSize)
				dwSize = 2*NET_READ_MAXBUFFER;
			else
			{
				if(m_dwFileSize-m_dwDownloadSize < 2*NET_READ_MAXBUFFER)
					dwSize = m_dwFileSize-m_dwDownloadSize;
				else
					dwSize = 2*NET_READ_MAXBUFFER;
			}
			ATLASSERT(dwSize);
			dwDownloaded=0;
			if(NULL == lpBufferByte)
			{
				dwReadBuf = dwSize;
				lpBufferByte = new BYTE[dwReadBuf+1];
			}
			if(NULL == lpBufferByte)
			{
				CString strErrInfo(_T("内存不足"));
				SetError(ERROR_HTTP_INTERNETREADFILE,strErrInfo);
				if(m_hParentWnd && ::IsWindow(m_hParentWnd))
				{
					::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
						(WPARAM)ERROR_HTTP_INTERNETREADFILE,(LPARAM )m_eServiceID);
				}
				break;/// 内存失败
			}
			if(INTERNET_FLAG_ASYNC == m_dwConnectMode && NULL != m_hRequestCompleteEvent)
				ResetEvent(m_hRequestCompleteEvent);
			::memset(lpBufferByte,0,(dwReadBuf+1)*sizeof(BYTE));
			bReadFlag=::InternetReadFile(m_hRequest,(LPVOID)lpBufferByte,dwSize,&dwDownloaded);
			if(bReadFlag && !dwDownloaded)
			{
				if(NULL != lpBufferByte)
				{
					delete []lpBufferByte;
					lpBufferByte=NULL;
				}
				break;//已经读取完成
			}

			if(!dwDownloaded)
			{
				DWORD dwErrCode=GetLastError();
				if(!dwErrCode || dwErrCode== ERROR_IO_PENDING)
				{
					//初始状态无信号
					DWORD dwRet=WaitForSingleObject(m_hRequestCompleteEvent,INFINITE);
					ResetEvent(m_hRequestCompleteEvent);
					if(strlen((const char *)lpBufferByte))
						dwDownloaded = dwSize;
				}
				else
				{
					if(NULL != lpBufferByte)
					{
						delete []lpBufferByte;
						lpBufferByte=NULL;
					}

					CloseDownloadFile();
					if(NULL != m_hRequest)
					{
						CString strErrInfo = ErrorInfo(_T("InternetReadFile"),dwErrCode);
						if(!strErrInfo.IsEmpty())
							SetError(dwErrCode,strErrInfo);
					}
					CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
					if(m_hParentWnd && ::IsWindow(m_hParentWnd))
					{
						::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
							(WPARAM)ERROR_HTTP_INTERNETREADFILE,(LPARAM )m_eServiceID);
					}
					return uiRetFlag;
				}
			}
			lpBufferByte[dwReadBuf] = 0;
			if(NULL != m_hStopRequestEvent || NULL == m_hDownFile)
			{
				//停止下载
				CloseDownloadFile();
				if(NULL != lpBufferByte)
				{
					delete []lpBufferByte;
					lpBufferByte=NULL;
				}
				CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
				if(m_hParentWnd && ::IsWindow(m_hParentWnd))
				{
					if(!GetLastError() && NULL != m_hStopRequestEvent)
						SetError(ESOCIAL_ERROR_BASECODE,_T("网络操作被取消"));
					::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,(WPARAM )ESOCIAL_ERROR_BASECODE,(LPARAM )m_eServiceID);
				}
				return uiRetFlag;
			}
			if(!m_dwDownloadSize)
			{
				lpBufferByte[dwReadBuf] = 0;
				/// 第一个数据包，判断格式
				char cBuf[32];
				memset(cBuf,0,32*sizeof(char));
				memcpy(cBuf,lpBufferByte,32);
				CString strContent(cBuf);
				if(-1 != strContent.Find(_T("<?xml")))
					bTryFlag = TRUE;
				strContent.Empty();
			}
			DWORD cbWritten=0;
			if (FALSE==::WriteFile(m_hDownFile,lpBufferByte,dwDownloaded,&cbWritten,NULL))
			{
				DWORD dwErrCode=::GetLastError();
				CloseDownloadFile();
				if(NULL != lpBufferByte)
				{
					delete []lpBufferByte;
					lpBufferByte=NULL;
				}
				if(dwErrCode)
				{
					CString strErrInfo=ErrorInfo(_T("WriteFile"),dwErrCode);
					if(!strErrInfo.IsEmpty())
						SetError(ERROR_HTTP_WRITEFILE,strErrInfo);
					else
						SetError(dwErrCode,_T("网络返回写文件出错。"));//网络返回写文件错误
				}
				CloseNormalHandle(m_hDownThread);/// 标记线程已经结束
				if(m_hParentWnd && ::IsWindow(m_hParentWnd))
				{
					::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
						(WPARAM)ERROR_HTTP_WRITEFILE,(LPARAM )m_eServiceID);
				}
				return uiRetFlag;
			}
			::memset(lpBufferByte,0,(dwReadBuf+1)*sizeof(BYTE));
			m_dwDownloadSize += cbWritten;
			ATLASSERT(cbWritten == dwDownloaded);
			if(m_hParentWnd && ::IsWindow(m_hParentWnd))
			{
				if(m_dwFileSize && 100*m_dwDownloadSize/m_dwFileSize > percent)
				{
					percent = (DWORD)100*m_dwDownloadSize/m_dwFileSize;
					if(100 != percent)
					{
						/// 发送进度通知
						::PostMessage(m_hParentWnd,WM_DOWNLOAD_PERCENT,\
							(WPARAM )percent,(LPARAM )m_eServiceID);
					}
				}
				else if(!m_dwFileSize)
					::PostMessage(m_hParentWnd,WM_DOWNLOAD_DOWNSIZE,\
							(WPARAM )m_dwDownloadSize,(LPARAM )m_eServiceID);
			}
			if(m_dwFileSize && m_dwDownloadSize >= m_dwFileSize)
				break;//已经下载完成
			::Sleep(50);
		} while( dwDownloaded && !m_bAllDone );
	}
	catch ( ... )
	{
//		MessageBox(m_hParentWnd,_T("下载文件时产生异常错误！"),_T("LAN_APPLICATION_NAME"),MB_OK);
	}

	if(NULL != lpBufferByte)
	{
		delete []lpBufferByte;
		lpBufferByte = NULL;
	}

	CloseDownloadFile();
	CloseNetHandle(m_hRequest);
	CloseNormalHandle(m_hDownThread);/// 标记线程已经结束

	uiRetFlag=1;
	if(m_hParentWnd && ::IsWindow(m_hParentWnd))
	{
		if(!m_dwFileSize && m_dwDownloadSize)
		{
#ifdef APP_LOG_ENABLE
			CString strLogInfo(_T(""));
			strLogInfo.Format(_T("WeiNet::DownloadFileProc Object:%s:文件%s 下载大小:%s 未知总大小"),\
				CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath,CBaseFuncLib::NumToStr(m_dwDownloadSize));
			WRITELOGTOFILE(strLogInfo);
#endif
			percent = 100;
		}
		if(bTryFlag)
		{
			/// 下载文件，必须重试
			::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
				(WPARAM)ERROR_HTTP_TRYDOWNLOAD,(LPARAM )m_eServiceID);
		}
		else
		{
			if((m_dwDownloadSize==m_dwFileSize || m_dwDownloadSize==dwContentLength || !m_dwFileSize)
				&& INTERNET_FLAG_ASYNC==m_dwConnectMode)
			{
				::PostMessage(m_hParentWnd,WM_DOWNLOAD_PERCENT,(WPARAM )percent,(LPARAM )m_eServiceID);
			}
			else
			{
				if(INTERNET_FLAG_ASYNC==m_dwConnectMode)
				{
					//发送通知，下载不完整
					::PostMessage(m_hParentWnd,WM_DOWNLOAD_ERROR,\
						(WPARAM)ERROR_HTTP_INTERNETREADFILE,(LPARAM )m_eServiceID);
#ifdef APP_LOG_ENABLE
					CString strLogInfo(_T(""));
					strLogInfo.Format(_T("WeiNet::DownloadFileProc Object:%s::%s 大小:%s 实际下载：%s"),\
						CBaseFuncLib::NumToStr((DWORD)this),m_strDownFilePath,\
						CBaseFuncLib::NumToStr(m_dwDownloadSize),CBaseFuncLib::NumToStr(m_dwFileSize));
					WRITELOGTOFILE(strLogInfo);
#endif
				}
			}
		}
	}
	return uiRetFlag;
}

void CWinNet::CloseDownloadFile()
{
	WEIBONET_MAP::iterator it = g_mapWeibNet.find(this);
	if(it == g_mapWeibNet.end())
		return;
	if(NULL==it->second->m_hDownFile || INVALID_HANDLE_VALUE == it->second->m_hDownFile)
		return;
	CloseNormalHandle(it->second->m_hDownFile);
}

BOOL CWinNet::StopHttpVisit(BOOL bStopFlag)
{
	if(NULL == m_hInternet || NULL == m_hConnect)
		return FALSE;
	StopDownloadFile();
	CloseDownloadFile();
	m_strDownFilePath.Empty();
	m_strRetHeader.Empty();
	CloseNetHandle(m_hConnect);
	if(!bStopFlag)
	{
		return TRUE;//不关闭网络环境
	}
	m_strSendHeader.Empty();
	/// 关闭网络句柄m_hInternet
	if(INTERNET_FLAG_ASYNC==m_dwConnectMode)
		::InternetSetStatusCallback(m_hInternet, NULL);//注销回调函数
	CloseNetHandle(m_hInternet);
	return TRUE;
}