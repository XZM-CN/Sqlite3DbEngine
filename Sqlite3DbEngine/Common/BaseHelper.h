#pragma once
#include "ComHelper.h"

class CBaseHelper : public CComHelper
{
protected:
	/// ģ������
	static ATL::CString m_strModelName;

public:

	/**
	 * @brief  �����������ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <INetRequest> GetNetRequest();

	/**
	 * @brief  ����˺Ź���ӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <IAccountMgr> GetAccountMgr();

	/**
	 * @brief  ��÷�����ƽӿ�
	 *
	 *
	 * @return 
	 */
	static CComPtr <IServiceControl> GetServiceControl();


	static CComPtr <ISysEventLog> GetSysEventLog();
};
