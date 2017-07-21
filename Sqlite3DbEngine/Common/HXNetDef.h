#pragma once

#include "HXErrDef.h"

/// 本地管理员账号
#define TDHXKJ_ACCOUNT_SYSADMIN		_T("sa")
#define TDHXKJ_ACCOUNT_ADMIN		_T("admin")
#define TDHXKJ_ACCOUNT_AUDITOR		_T("Auditor")

/// 本地代理服务器地址
#define TDHXKJ_LOCALSERVER_ADDR		_T("127.0.0.1")

/// 下载临时文件扩展名
#define	TDHXKJ_DOWNFILE_TEMPNAME	_T(".tmp")

/// 定义网络操作方法
#define	TDHXKJ_HTTPMETHOD_POST		_T("POST")
#define	TDHXKJ_HTTPMETHOD_GET		_T("GET")

/// 网络每次读取数据缓冲区大小,定义为4K
#define NET_READ_MAXBUFFER			1024*4			
/// 网络POST数据文件临时路径
#define NET_POST_TMPFILENAME		_T("pdt.tmp")

//API服务器端口地址
#define API_WEB_PORT				INTERNET_DEFAULT_HTTP_PORT	

#define TEMP_EXT                    _T(".stf")		//请求返回到临时文件类型

//////////////////////  自定义网络操作消息  //////////////////////////

#define WM_REQUEST_RECEIVED			WM_USER + 670	/// 异步网络访问请求已经接收
#define WM_REQUEST_COMPLETED        WM_USER + 671   /// 异步网络访问请求已经完成
#define WM_CONNECT_CLOSED			WM_USER + 672	/// 异步网络被关闭

//所有的消息第二个参数都是正在下载的文件名
#define WM_DOWNLOAD_TOTALSIZE		WM_USER + 700	/// 文件总大小
#define WM_DOWNLOAD_PERCENT			WM_USER + 701	/// 已经下载的百分比，100时表示下载完成，0表示开始下载
#define WM_DOWNLOAD_ERROR			WM_USER + 702	/// 下载出错，第一个参数是错误号码，如果是0，表示停止下载
#define WM_DOWNLOAD_OK				WM_USER + 703	/// 下载完成
#define WM_DOWNLOAD_DOWNSIZE		WM_USER + 704	///  已经下载大小

/// 所有网络请求定义
enum ENetReqID
{
	///////////////		以下为内部需要定义服务		///////////////
	ENETREQUEST_INVALID				= 0,	/// 无效功能
	ENETREQUEST_UPLOAD_IMG			= 1,	/// 上传文件

	ENETREQUEST_WEBPAGE_GET			= 5,	/// 获取网页数据
	ENETREQUEST_DOWNLOAD_FILE		= 6,	/// 下载文件
	ENETREQUEST_DOWNLOAD_IMG		= 7,	/// 下载图片

	/// 功能测试接口 
	ENETREQUEST_EHELP_TEST = 10,	/// 测试接口
};