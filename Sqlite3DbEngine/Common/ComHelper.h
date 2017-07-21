#pragma once


class CComHelper
{
protected:
	/// ģ��������
	static STRINGHANDLE_MAP	m_mapInstance;

	/// �Ƿ���뱣��
	static SHORT		m_nCodeProtected;

public:
	CComHelper(void)
	{
	}

	/// ���ģ����ʱ·��
	static CString GetComTempPath(BOOL bCreateFlag = TRUE);

	/// ���ģ�鰲װ·��
	static CString GetAppInsPath();

	static HINSTANCE GetComHandle(const ATL::CString& strModelName);

	/**
	 * @brief �ͷ�ָ��ģ��
	 *
	 *
	 */
	static void FreeHandle(const ATL::CString& strModelName);

	/**
	 * @brief �ͷ�����ģ��
	 *
	 *
	 */
	static void FreeAllHandle();

	/// ��ô��뱣�����
	static SHORT GetSourceProtected();
};