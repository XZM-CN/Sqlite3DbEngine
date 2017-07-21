
/**
* @file WinNet.h
*
* Copyright (c) 2013-?,ZORROSOFT
* All rights reserved.
*
* @ingroup WeiXinTong
*
* @brief ����HTTPЭ��ķ�װ���ļ�ͷ
*
*
*
* @version 
*
* @Date 2011-10-07
*
* @author ������
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
 * @brief ��������࣬ͬʱ֧��ͬ�����첽��ʽ
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

	//��������ͬ���������ʱ���ã��첽����ʱ�Զ�����
	BOOL SendRequest();

	//��������󻺴�õ��ķ�����Ϣ��ͬ���������ʱ�ֹ����ã��첽����ʱ�Զ�����
	BOOL SaveResponseInfo(DWORD dwThreadPri);

	//��õ�ǰHttp״̬
	DWORD GetHttpStatus(BOOL bCheckFlag = FALSE);

	//�жϱ�����ǰ�Ƿ������������
	BOOL IsIdle(BOOL bAllFlag = TRUE);

	//�����첽������Ϣ
	void DealWithCallBack(HINTERNET hInternet,DWORD dwInternetStatus, 
				LPVOID lpStatusInfo, DWORD dwStatusInfoLen);

	/**
	 * @brief ����ָ��URL����ȱʡΪͬ����ʽ��ȱʡ�˿�,�첽dwConnectMode��INTERNET_FLAG_ASYNC
	 *
	 *
	 * @param strUrl ����URLȫ·��
	 * @param dwConnectMode ��������
	 * @param ENetReqID ��̨��������ID
	 * @param strDownFilePath �����ļ�·��
	 * @param strMethod ���󷽷�
	 * @return �����Ƿ�ɹ�
	 */
	BOOL RequestUrl(const CString& strUrl,ENetReqID eServiceID,
			DWORD dwConnectMode=0,const CString& strDownFilePath=_T(""),
			const CString& strMethod=TDHXKJ_HTTPMETHOD_GET);

	//�����ļ�����Ҫ�ȵ���OpenURL
	BOOL DownFile(const CString& filename,DWORD dwThreadPri = THREAD_PRIORITY_NORMAL);

	/// ����POST�ļ�·��
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

	//�ر������ļ�
	void CloseDownloadFile();

	//��������ļ���С
	DWORD GetDownFileSize();

	//����Ѿ����ش�С
	DWORD GetDownloadedSize();

	//ֹͣ�����ļ�
	BOOL StopDownloadFile();

	//���������ʷ��ص�ͷ��Ϣ
	CString GetReturnHeader()
	{ 
		return m_strRetHeader;
	}

	/**
	 * @brief ���÷���ͷ��Ϣ
	 *
	 *
	 * @param strAuthorization ͷ����
	 */
	void SetSendHeader(short sType = 0,BOOL bZipData = TRUE,
		const CString& strAuthorization = _T("Content-Type: application/x-www-form-urlencoded"));

	/// ���������ʺ�õ�����ʱ�ļ�ȫ·��
	CString GetDownFilePath()
	{ 
		return m_strDownFilePath;
	}
	/// ��ȡ����URL
	CString GetRequestUrl()
	{
		return m_strUrl;
	}

	/// ����Ƿ�ʹ��IE����
	BOOL GetUseIE()
	{
		return m_bUseIE;
	}
	/// �����Ƿ�ʹ��IE����
	void SettUseIE(BOOL bUseIE)
	{
		m_bUseIE = bUseIE;
	}
	/// ��ô��������
	CString GetProxyServer()
	{
		return m_strProxyServer;
	}
	/// ���ô�����Ϣ
	void SetProxyServer(const CString& strProxyServer)
	{
		m_strProxyServer.Empty();
		m_strProxyServer = strProxyServer;
	}
	/// ���UserAgent
	CString GetUserAgent()
	{
		return m_strUserAgent;
	}
	/// ���ô�����Ϣ
	void SetUserAgent(const CString& strUserAgent)
	{
		m_strUserAgent.Empty();
		m_strUserAgent = strUserAgent;
	}

	//ֹͣ������ʣ��ͷ�ʱ����������ͷ���Դ
	BOOL StopHttpVisit(BOOL bStopFlag=TRUE);

	//�����������
	HWND GetWndHandle()
	{ 
		return m_hSelfWnd; 
	}

	/// ���ø����ھ��
	void SetParentWnd(HWND hParentWnd)
	{
		m_hParentWnd = hParentWnd;
	}
	/// ����COOKIE
	void SetNoCookie(BOOL bCookieFlag)
	{
		m_bNoCookie = bCookieFlag;
	}
	/// ���ö˿�
	void SetPort(UINT uiPort)
	{
		m_uiPort = uiPort;
	}

	//�����ļ�����
	UINT DownloadFileProc();

	//�������󷵻����ݹ���
	UINT DownloadRequestProc();

	//��������߳̾��
	HANDLE GetDownloadThread()
	{
		return m_hDownThread;
	}

	/**
	 * @brief ���õȴ��¼����
	 *
	 *
	 * @param hEvent 
	 */
	void SetWaitEvent(HANDLE hEvent);

	virtual void SetError( long lErrCode, const CString& strMessage );

	/**
	 * @brief ��ѹ���ݵ��ļ���
	 *
	 *
	 * @param zdata 
	 * @param nzdata 
	 * @param hUnzipFile 
	 * @return 
	 */
	static int HttpGzDecompress(const BYTE *zdata,const DWORD nzdata,const HANDLE hUnzipFile);

protected:

	/// ����������
	BOOL OpenInternet(DWORD dwConnectMode);

	/// ������
	BOOL OpenRequest();

	/// �رն˿�
	BOOL CloseNetHandle(HINTERNET &hHandle);

	/// �ر��¼����
	BOOL CloseNormalHandle(HANDLE &hHandle);

	/// ����ͷ��Ϣ
	BOOL SaveHeaderInfo();

	/// ������س���
	BOOL GetDownContent(DWORD &dwContentLength,BOOL bDownloadFile = FALSE);

	/// ���ɴ�����Ϣ
	CString ErrorInfo(const CString& strFunction,DWORD dwErrCode);

	/// ���λ����Ϣ
	void QuerytLocation();

	/// ��������ļ�����
	void GetFileType();

	/**
	 * @brief ����URL���ֽ���������
	 *
	 *
	 * @param strURL ԭURL
	 * @param strServerName HOST NAME
	 * @param strVirtualUrl ��·��
	 * @return �˿ں�
	 */
	UINT ParserUrl(const CString& strURL,CString& strServerName,CString& strVirtualUrl);

	/**
	 * @brief ��URL��ȱʡΪͬ����ʽ��ȱʡ�˿�,�첽dwConnectMode��INTERNET_FLAG_ASYNC
	 *
	 *
	 * @param strServer ��������ַ
	 * @param strUrl    URL��·��
	 * @param dwConnectMode ��������
	 * @param strDownFilePath �����ļ�·��
	 * @param port �˿�
	 * @param strMethod ���󷽷�
	 * @return 
	 */
	BOOL OpenURL(const CString& strServer, const CString& strUrl,
			DWORD dwConnectMode=0,const CString& strDownFilePath=_T(""),
			INTERNET_PORT port=INTERNET_DEFAULT_HTTP_PORT,
			const CString& strMethod=TDHXKJ_HTTPMETHOD_GET);

	//׼�������ļ�
	BOOL ReadyDownloadFile(BOOL bCreateFlag=TRUE,BOOL bCloseFlag=FALSE);

	//���ó�Ա������ʼֵ
	void ResetMemberVariant();

	/**
	 * @brief ɾ��POST����
	 *
	 *
	 */
	void DelPostData();

	//�첽�ص�����
	static LRESULT CALLBACK AsyncWindowProc( HWND hWnd,UINT message,
		WPARAM wParam, LPARAM lParam );

	//�����ļ��̣߳��������ؽ���
	static UINT DownloadFileThread(LPVOID pParam);

	//�����������������̣߳������ͽ���֪ͨ
	static UINT DownloadRequestThread(LPVOID pParam);

protected:
	HINTERNET m_hInternet;			/// ������
    HINTERNET m_hConnect;			/// ���Ӿ��
    HINTERNET m_hRequest;			/// ������

	HANDLE	m_hDownThread;			/// �����ļ��߳�
	HANDLE	m_hDownFile;			/// �����ļ����

	HANDLE	m_hConnectedEvent;		/// �����ź�
	HANDLE	m_hRequestOpenedEvent;	/// �����
	HANDLE	m_hRequestCompleteEvent;/// �������
	HANDLE	m_hStopRequestEvent;	/// ֹͣ�ź�

	BOOL	m_bAllDone;				/// �Ƿ��Ѿ����
	BOOL	m_bNetConnect;			/// �Ƿ�����������״̬
	BOOL	m_bNoCookie;			/// �Ƿ���COOKIE
	BOOL	m_bSecureFlag;			/// �Ƿ�Ϊ��ȫЭ��
	BOOL	m_bUseIE;			/// ʹ��IE����

	HWND m_hSelfWnd;				/// ��������
	HWND m_hParentWnd;				/// ��������

	CString m_strMethod;			/// ������ʷ���
	CString m_strUrl;				/// �������·��
	CString m_strSendHeader;		/// ����ͷ��Ϣ
	CString m_strRetHeader;			/// ����ͷ��Ϣ
	CString m_strDownFilePath;		/// �����ļ�·��
	CString m_strPostFilePath;		/// POST���ݰ�·��
	CString	m_strProxyServer;		/// ���������
	CString	m_strUserAgent;			/// ����UserAgent
	CString m_strLocation;			/// LOCATION
	CString m_strServerFileName;	/// �������ϵ��ļ���

	DWORD m_dwConnectMode;			/// ��������ģʽ��֧��ͬ�����첽����
	DWORD m_dwInternetStatus;		/// ��ǰ���練��״̬
	DWORD m_dwFileSize;				/// �����ļ��Ĵ�С
	DWORD m_dwDownloadSize;			/// �Ѿ������ļ��Ĵ�С
	DWORD m_dwHttpStatus;			/// HTTP״̬��

	UINT m_uiPort;					/// ָ���˿�

	short m_sTrySendRequest;		/// ���������Դ���
	BYTE	*m_szPostBuff;			/// POST��������

	ENetReqID	m_eServiceID;		/// ��̨����ID
};
