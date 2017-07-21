#pragma once
#include "ComHelper.h"


class CSKFHelper
	: public CComHelper
{
protected:
	/// 模块名称
	static ATL::CString m_strModelName;

public:

	/**
	 * @brief  获得安全U卡接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <ISafeCard> GetSafeCard();

	/**
	 * @brief  获得安全U卡接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <ICryptoStor> GetCryptoStor();

	/**
	 * @brief  获得安全服务接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <ISafeService> GetSafeService();

	/**
	 * @brief  获得WMI设备接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <IWMIDevice> GetWMIDevice();

	/**
	 * @brief  获得USB设备接口
	 *
	 *
	 * @return 
	 */
	static CComPtr <IUSBDevice> GetUSBDevice();
};
