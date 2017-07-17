/**
 * @file HXRegKey.h
 *
 * Copyright (c) 2016-?,天地和兴科技
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 注册表封装
 *
 *
 *
 * @version 
 *
 * @date 2016-01-18
 *
 * @author WZH
 *
 * @history 
 *
 */
// HXRegKey.h : 注册表操作头文件定义
//
//////////////////////////////////////////////////////////////////////

#pragma once

/// 定义产品在注册表中注册的运行路径键名称
#define PRODUCT_COMMAN_RUNPATH		_T("RunPath")
/// 定义产品在注册表中注册的主版本
#define PRODUCT_COMMAN_MAINVERNAME	_T("MainVer")
/// 定义产品在注册表中注册的次版本
#define PRODUCT_COMMAN_MINIVERNAME	_T("MiniVer")
/// 定义产品在注册表中注册的编译版本
#define PRODUCT_COMMAN_BUILDVERNAME	_T("BuildVer")
/// 定义产品在注册表中注册的安装路径
#define PRODUCT_COMMAN_INSPATH		_T("InsPath")
/// 定义产品在注册表中注册的安装盘
#define PRODUCT_COMMAN_INSDISK		_T("InstallDisk")
/// 定义产品在注册表中注册的设备ID
#define PRODUCT_COMMAN_DEVICEID		_T("DeviceID")

/// 定义检测升级版本的版本号
#define PRODUCT_COMMAN_UPDATEVER	_T("UpdateVer")
/// 升级URL
#define	PRODUCT_COMMAN_UPDATEURL	_T("UpdateUrl")
/// 升级日期
#define	PRODUCT_COMMAN_UPDATEDATE	_T("UpdateDate")
/// 升级描述
#define	PRODUCT_COMMAN_UPDATEDESC	_T("UpdateDesc")
/// 升级文件Sha码
#define	PRODUCT_COMMAN_UPDATESHA	_T("UpdateSHA")

/// 定义程序主窗口句柄名称
#define PRODUCT_COMMAN_MAINWND		_T("MainWnd")

/// 定义当前登录用户序号
#define PRODUCT_COMMAN_CURLOGIN		_T("CurLogin")
/// 定义数据保存节点名称
#define PRODUCT_COMMAN_DATAPATH		_T("DataPath")

/// 定义系统日志写入数据库完成
#define PRODUCT_WRITE_DB_COMPLETE	_T("WriteDBComplete")

#include <atlbase.h>

class CHXRegKey : public CRegKey
{
public:
	CHXRegKey(const ATL::CString& strMainRegPath = COMPANY_REGPATH,\
		const ATL::CString& strChildName = PRODUCT_REGNODENAME, \
		HKEY hKey = HKEY_CURRENT_USER,REGSAM samDesired = KEY_ALL_ACCESS);

	virtual ~CHXRegKey(void)
	{
		/// 销毁时自动关闭，避免句柄泄露
		Close();
	}

	/// 判断是否打开了注册表
	BOOL IsOpen()
	{
		return m_bOpenFlag;
	}

	/**
	 * @brief 设置新路径，重用类实例
	 *
	 *
	 * @param strMainRegPath 注册表主位置
	 * @param strChildName 注册表子目录名称
	 * @param bCurrentUser 是否为用户注册表，否则为系统注册表
	 * @return 
	 */
	BOOL NewRegPath(const ATL::CString& strMainRegPath = COMPANY_REGPATH,\
		const ATL::CString& strChildName = PRODUCT_REGNODENAME,\
		HKEY hKey = HKEY_CURRENT_USER,REGSAM samDesired = KEY_ALL_ACCESS);

	/**
	 * @brief 根据键名称获得字符串类型键值
	 *
	 *
	 * @param strKeyName  键名
	 * @param strKeyValue 键值
	 * @return 是否成功获取
	 */
	BOOL GetRegStringVal(const ATL::CString& strKeyName,ATL::CString& strKeyValue);
	
	/**
	 * @brief 设置注册表字符串类型值
	 *
	 *
	 * @param strKeyName  键名
	 * @param strKeyValue 键值
	 * @return 是否成功设置
	 */
	BOOL SetRegStringVal(const ATL::CString& strKeyName,const ATL::CString& strKeyValue);

	/**
	 * @brief 根据键名称获得字符串类型键值
	 *
	 *
	 * @param strKeyName  键名
	 * @param vectKeyValue 多个键值
	 * @return 是否成功获取
	 */
	BOOL GetRegMultiStringVal(const ATL::CString& strKeyName,CSTRING_VECTOR& vectKeyValue);
	
	/**
	 * @brief 设置注册表字符串类型值
	 *
	 *
	 * @param strKeyName  键名
	 * @param strKeyValue 键值
	 * @return 是否成功设置
	 */
	BOOL SetRegMultiStringVal(const ATL::CString& strKeyName,CSTRING_VECTOR& vectKeyValue);

	/**
	 * @brief 获得注册表DWORD类型值
	 *
	 *
	 * @param strKeyName 键名
	 * @param dwKeyValue 键值
	 * @return 是否获取成功
	 */
	BOOL GetRegDwordVal(const ATL::CString& strKeyName,DWORD& dwKeyValue);

	/**
	 * @brief 设置注册表DWORD类型值
	 *
	 *
	 * @param strKeyName 键名
	 * @param dwKeyValue 键值
	 * @return 设置是否成功
	 */
	BOOL SetRegDwordVal(const ATL::CString& strKeyName,DWORD dwKeyValue);

	/**
	 * @brief 获得注册表二进制值
	 *
	 *
	 * @param strKeyName 键名
	 * @param ppValue	键值
	 * @param pnBytes    返回长度
	 * @return 是否获取成功
	 */
	BOOL GetRegBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes);

	/**
	 * @brief 设置二进制值
	 *
	 *
	 * @param strKeyName 键名
	 * @param Value		 键值
	 * @param pnBytes    返回长度
	 * @return 是否设置成功
	 */
	BOOL SetRegBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes);

	/**
	 * @brief 获得GUID类型注册表值
	 *
	 *
	 * @param strKeyName 键名
	 * @param Value		 键值
	 * @return 返回是否获取成功
	 */
	BOOL GetRegGuidValue(const ATL::CString &strKeyName,GUID &Value);

	/**
	 * @brief 设置GUID类型注册表值
	 *
	 *
	 * @param strKeyName 键名
	 * @param Value		 键值
	 * @return 返回是否设置成功
	 */
	BOOL SetRegGuidValue(const ATL::CString &strKeyName,GUID Value);

	/**
	 * @brief 删除指定的键值
	 *
	 *
	 * @param strKeyName 键名
	 * @return 
	 */
	BOOL DelKeyName(const ATL::CString& strKeyName);

	/**
	 * @brief 获得主程序所在路径
	 *
	 *
	 * @return 返回运行路径
	 */
	ATL::CString GetAppRunPath();

	/**
	 * @brief 设置主程序运行路径
	 *
	 *
	 * @param strAppPath 应用程序运行路径
	 * @return 返回是否设置成功
	 */
	BOOL SetAppRunPath(const ATL::CString &strAppPath);

	DWORD GetLastErr()
	{
		return m_dwLastErr;
	}

	/**
	 * @brief 关闭注册表句柄
	 *
	 *
	 */
	virtual void Close();

protected:

	/// 错误码
	DWORD	m_dwLastErr;

	/// 是否打开了注册表
	BOOL	m_bOpenFlag;
};
