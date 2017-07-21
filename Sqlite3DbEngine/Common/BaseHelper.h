#pragma once
#include "ComHelper.h"

class CBaseHelper : public CComHelper
{
protected:
	/// 模块名称
	static ATL::CString m_strModelName;

public:

	/**
	 * @brief  获得网络请求接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <INetRequest> GetNetRequest();

	/**
	 * @brief  获得账号管理接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <IAccountMgr> GetAccountMgr();

	/**
	 * @brief  获得服务控制接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <IServiceControl> GetServiceControl();


	static CComPtr <ISysEventLog> GetSysEventLog();
};
