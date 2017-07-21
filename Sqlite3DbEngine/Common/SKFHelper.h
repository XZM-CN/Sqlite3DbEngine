#pragma once
#include "ComHelper.h"


class CSKFHelper
	: public CComHelper
{
protected:
	/// ģ������
	static ATL::CString m_strModelName;

public:

	/**
	 * @brief  ��ð�ȫU���ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <ISafeCard> GetSafeCard();

	/**
	 * @brief  ��ð�ȫU���ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <ICryptoStor> GetCryptoStor();

	/**
	 * @brief  ��ð�ȫ����ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <ISafeService> GetSafeService();

	/**
	 * @brief  ���WMI�豸�ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <IWMIDevice> GetWMIDevice();

	/**
	 * @brief  ���USB�豸�ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <IUSBDevice> GetUSBDevice();
};
