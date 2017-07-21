
/**
* @file WinNet.h
*
* Copyright (c) 2013-?,ZORROSOFT
* All rights reserved.
*
* @ingroup WeiXinTong
*
* @brief 基于HTTP协议的封装类文件头
*
*
*
* @version 
*
* @Date 2011-10-07
*
* @author 汪佐怀
*
* @history 
*
*/
//  WinNet.h: interface for the CWinNet class.
//

#pragma once

#include <wininet.h>
#include "HXNetDef.h"

/**
 * @ingroup WeiXinTong
 *
 * @brief 网络访问类，同时支持同步、异步方式
 *
 *
 * @Date 2011-09-17
 *
 * @author WZH
 *
 * @Function List 
 *
 *
 *
*/
class CWinNet 
	: public CErrorBase
{
public:
	CWinNet(HWND hParentWnd=NULL);
	~CWinNet(void);

	//发送请求，同步网络访问时调用，异步网络时自动调用
	BOOL SendRequest();

	//发生请求后缓存得到的反馈信息，同步网络访问时手工调用，异步网络时自动调用
	BOOL SaveResponseInfo(DWORD dwThreadPri);

	//获得当前Http状态
	DWORD GetHttpStatus(BOOL bCheckFlag = FALSE);

	//判断本对象当前是否处于网络访问中
	BOOL IsIdle(BOOL bAllFlag = TRUE);

	//处理异步反馈消息
	void DealWithCallBack(HINTERNET hInternet,DWORD dwInternetStatus, 
				LPVOID lpStatusInfo, DWORD dwStatusInfoLen);

	/**
	 * @brief 请求指定URL服务，缺省为同步方式，缺省端口,异步dwConnectMode传INTERNET_FLAG_ASYNC
	 *
	 *
	 * @param strUrl 网络URL全路径
	 * @param dwConnectMode 连接类型
	 * @param ENetReqID 后台服务请求ID
	 * @param strDownFilePath 下载文件路径
	 * @param strMethod 请求方法
	 * @return 返回是否成功
	 */
	BOOL RequestUrl(const CString& strUrl,ENetReqID eServiceID,
			DWORD dwConnectMode=0,const CString& strDownFilePath=_T(""),
			const CString& strMethod=TDHXKJ_HTTPMETHOD_GET);

	//下载文件，需要先调用OpenURL
	BOOL DownFile(const CString& filename,DWORD dwThreadPri = THREAD_PRIORITY_NORMAL);

	/// 设置POST文件路径
	void SetPostFilePath(const CString& strPostFilePath)
	{
		m_strPostFilePath = strPostFilePath;
	}

	ENetReqID GetCurServiceID()
	{
		return m_eServiceID;
	}

	DWORD GetConnectMode()
	{
		return m_dwConnectMode;
	}

	CString GetMethod()
	{
		return m_strMethod;
	}

	CString GetLocation()
	{
		return m_strLocation;
	}

	CString GetServerFileName()
	{
		return m_strServerFileName;
	}

	//关闭下载文件
	void CloseDownloadFile();

	//获得下载文件大小
	DWORD GetDownFileSize();

	//获得已经下载大小
	DWORD GetDownloadedSize();

	//停止下载文件
	BOOL StopDownloadFile();

	//获得网络访问返回到头信息
	CString GetReturnHeader()
	{ 
		return m_strRetHeader;
	}

	/**
	 * @brief 设置发送头信息
	 *
	 *
	 * @param strAuthorization 头内容
	 */
	void SetSendHeader(short sType = 0,BOOL bZipData = TRUE,
		const CString& strAuthorization = _T("Content-Type: application/x-www-form-urlencoded"));

	/// 获得网络访问后得到的临时文件全路径
	CString GetDownFilePath()
	{ 
		return m_strDownFilePath;
	}
	/// 获取请求URL
	CString GetRequestUrl()
	{
		return m_strUrl;
	}

	/// 获得是否使用IE设置
	BOOL GetUseIE()
	{
		return m_bUseIE;
	}
	/// 设置是否使用IE设置
	void SettUseIE(BOOL bUseIE)
	{
		m_bUseIE = bUseIE;
	}
	/// 获得代理服务器
	CString GetProxyServer()
	{
		return m_strProxyServer;
	}
	/// 设置代理信息
	void SetProxyServer(const CString& strProxyServer)
	{
		m_strProxyServer.Empty();
		m_strProxyServer = strProxyServer;
	}
	/// 获得UserAgent
	CString GetUserAgent()
	{
		return m_strUserAgent;
	}
	/// 设置代理信息
	void SetUserAgent(const CString& strUserAgent)
	{
		m_strUserAgent.Empty();
		m_strUserAgent = strUserAgent;
	}

	//停止网络访问，释放时必须调用以释放资源
	BOOL StopHttpVisit(BOOL bStopFlag=TRUE);

	//获得自身窗体句柄
	HWND GetWndHandle()
	{ 
		return m_hSelfWnd; 
	}

	/// 设置父窗口句柄
	void SetParentWnd(HWND hParentWnd)
	{
		m_hParentWnd = hParentWnd;
	}
	/// 设置COOKIE
	void SetNoCookie(BOOL bCookieFlag)
	{
		m_bNoCookie = bCookieFlag;
	}
	/// 设置端口
	void SetPort(UINT uiPort)
	{
		m_uiPort = uiPort;
	}

	//下载文件过程
	UINT DownloadFileProc();

	//下载请求返回数据过程
	UINT DownloadRequestProc();

	//获得下载线程句柄
	HANDLE GetDownloadThread()
	{
		return m_hDownThread;
	}

	/**
	 * @brief 设置等待事件句柄
	 *
	 *
	 * @param hEvent 
	 */
	void SetWaitEvent(HANDLE hEvent);

	virtual void SetError( long lErrCode, const CString& strMessage );

	/**
	 * @brief 解压数据到文件中
	 *
	 *
	 * @param zdata 
	 * @param nzdata 
	 * @param hUnzipFile 
	 * @return 
	 */
	static int HttpGzDecompress(const BYTE *zdata,const DWORD nzdata,const HANDLE hUnzipFile);

protected:

	/// 打开网络连接
	BOOL OpenInternet(DWORD dwConnectMode);

	/// 打开请求
	BOOL OpenRequest();

	/// 关闭端口
	BOOL CloseNetHandle(HINTERNET &hHandle);

	/// 关闭事件句柄
	BOOL CloseNormalHandle(HANDLE &hHandle);

	/// 保存头信息
	BOOL SaveHeaderInfo();

	/// 获得下载长度
	BOOL GetDownContent(DWORD &dwContentLength,BOOL bDownloadFile = FALSE);

	/// 生成错误信息
	CString ErrorInfo(const CString& strFunction,DWORD dwErrCode);

	/// 获得位置信息
	void QuerytLocation();

	/// 获得下载文件类型
	void GetFileType();

	/**
	 * @brief 解析URL，分解出相关数据
	 *
	 *
	 * @param strURL 原URL
	 * @param strServerName HOST NAME
	 * @param strVirtualUrl 子路径
	 * @return 端口号
	 */
	UINT ParserUrl(const CString& strURL,CString& strServerName,CString& strVirtualUrl);

	/**
	 * @brief 打开URL，缺省为同步方式，缺省端口,异步dwConnectMode传INTERNET_FLAG_ASYNC
	 *
	 *
	 * @param strServer 服务器地址
	 * @param strUrl    URL子路径
	 * @param dwConnectMode 连接类型
	 * @param strDownFilePath 下载文件路径
	 * @param port 端口
	 * @param strMethod 请求方法
	 * @return 
	 */
	BOOL OpenURL(const CString& strServer, const CString& strUrl,
			DWORD dwConnectMode=0,const CString& strDownFilePath=_T(""),
			INTERNET_PORT port=INTERNET_DEFAULT_HTTP_PORT,
			const CString& strMethod=TDHXKJ_HTTPMETHOD_GET);

	//准备下载文件
	BOOL ReadyDownloadFile(BOOL bCreateFlag=TRUE,BOOL bCloseFlag=FALSE);

	//重置成员变量初始值
	void ResetMemberVariant();

	/**
	 * @brief 删除POST数据
	 *
	 *
	 */
	void DelPostData();

	//异步回调处理
	static LRESULT CALLBACK AsyncWindowProc( HWND hWnd,UINT message,
		WPARAM wParam, LPARAM lParam );

	//下载文件线程，发送下载进度
	static UINT DownloadFileThread(LPVOID pParam);

	//下载网络请求数据线程，不发送进度通知
	static UINT DownloadRequestThread(LPVOID pParam);

protected:
	HINTERNET m_hInternet;			/// 网络句柄
    HINTERNET m_hConnect;			/// 连接句柄
    HINTERNET m_hRequest;			/// 请求句柄

	HANDLE	m_hDownThread;			/// 下载文件线程
	HANDLE	m_hDownFile;			/// 下载文件句柄

	HANDLE	m_hConnectedEvent;		/// 连接信号
	HANDLE	m_hRequestOpenedEvent;	/// 请求打开
	HANDLE	m_hRequestCompleteEvent;/// 请求完成
	HANDLE	m_hStopRequestEvent;	/// 停止信号

	BOOL	m_bAllDone;				/// 是否都已经完成
	BOOL	m_bNetConnect;			/// 是否处于网络连接状态
	BOOL	m_bNoCookie;			/// 是否保留COOKIE
	BOOL	m_bSecureFlag;			/// 是否为安全协议
	BOOL	m_bUseIE;			/// 使用IE代理

	HWND m_hSelfWnd;				/// 本窗体句柄
	HWND m_hParentWnd;				/// 父窗体句柄

	CString m_strMethod;			/// 网络访问方法
	CString m_strUrl;				/// 网络访问路径
	CString m_strSendHeader;		/// 发送头信息
	CString m_strRetHeader;			/// 返回头信息
	CString m_strDownFilePath;		/// 下载文件路径
	CString m_strPostFilePath;		/// POST数据包路径
	CString	m_strProxyServer;		/// 代理服务器
	CString	m_strUserAgent;			/// 连接UserAgent
	CString m_strLocation;			/// LOCATION
	CString m_strServerFileName;	/// 服务器上的文件名

	DWORD m_dwConnectMode;			/// 网络连接模式，支持同步和异步访问
	DWORD m_dwInternetStatus;		/// 当前网络反馈状态
	DWORD m_dwFileSize;				/// 下载文件的大小
	DWORD m_dwDownloadSize;			/// 已经下载文件的大小
	DWORD m_dwHttpStatus;			/// HTTP状态码

	UINT m_uiPort;					/// 指定端口

	short m_sTrySendRequest;		/// 发送请求尝试次数
	BYTE	*m_szPostBuff;			/// POST缓存数据

	ENetReqID	m_eServiceID;		/// 后台服务ID
};
