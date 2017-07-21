#pragma once

#include "HXErrDef.h"

/// ���ع���Ա�˺�
#define TDHXKJ_ACCOUNT_SYSADMIN		_T("sa")
#define TDHXKJ_ACCOUNT_ADMIN		_T("admin")
#define TDHXKJ_ACCOUNT_AUDITOR		_T("Auditor")

/// ���ش����������ַ
#define TDHXKJ_LOCALSERVER_ADDR		_T("127.0.0.1")

/// ������ʱ�ļ���չ��
#define	TDHXKJ_DOWNFILE_TEMPNAME	_T(".tmp")

/// ���������������
#define	TDHXKJ_HTTPMETHOD_POST		_T("POST")
#define	TDHXKJ_HTTPMETHOD_GET		_T("GET")

/// ����ÿ�ζ�ȡ���ݻ�������С,����Ϊ4K
#define NET_READ_MAXBUFFER			1024*4			
/// ����POST�����ļ���ʱ·��
#define NET_POST_TMPFILENAME		_T("pdt.tmp")

//API�������˿ڵ�ַ
#define API_WEB_PORT				INTERNET_DEFAULT_HTTP_PORT	

#define TEMP_EXT                    _T(".stf")		//���󷵻ص���ʱ�ļ�����

//////////////////////  �Զ������������Ϣ  //////////////////////////

#define WM_REQUEST_RECEIVED			WM_USER + 670	/// �첽������������Ѿ�����
#define WM_REQUEST_COMPLETED        WM_USER + 671   /// �첽������������Ѿ����
#define WM_CONNECT_CLOSED			WM_USER + 672	/// �첽���类�ر�

//���е���Ϣ�ڶ������������������ص��ļ���
#define WM_DOWNLOAD_TOTALSIZE		WM_USER + 700	/// �ļ��ܴ�С
#define WM_DOWNLOAD_PERCENT			WM_USER + 701	/// �Ѿ����صİٷֱȣ�100ʱ��ʾ������ɣ�0��ʾ��ʼ����
#define WM_DOWNLOAD_ERROR			WM_USER + 702	/// ���س�����һ�������Ǵ�����룬�����0����ʾֹͣ����
#define WM_DOWNLOAD_OK				WM_USER + 703	/// �������
#define WM_DOWNLOAD_DOWNSIZE		WM_USER + 704	///  �Ѿ����ش�С

/// ��������������
enum ENetReqID
{
	///////////////		����Ϊ�ڲ���Ҫ�������		///////////////
	ENETREQUEST_INVALID				= 0,	/// ��Ч����
	ENETREQUEST_UPLOAD_IMG			= 1,	/// �ϴ��ļ�

	ENETREQUEST_WEBPAGE_GET			= 5,	/// ��ȡ��ҳ����
	ENETREQUEST_DOWNLOAD_FILE		= 6,	/// �����ļ�
	ENETREQUEST_DOWNLOAD_IMG		= 7,	/// ����ͼƬ

	/// ���ܲ��Խӿ� 
	ENETREQUEST_EHELP_TEST = 10,	/// ���Խӿ�
};