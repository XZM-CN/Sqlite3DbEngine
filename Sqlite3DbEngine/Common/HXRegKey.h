/**
 * @file HXRegKey.h
 *
 * Copyright (c) 2016-?,��غ��˿Ƽ�
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief ע����װ
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
// HXRegKey.h : ע������ͷ�ļ�����
//
//////////////////////////////////////////////////////////////////////

#pragma once

/// �����Ʒ��ע�����ע�������·��������
#define PRODUCT_COMMAN_RUNPATH		_T("RunPath")
/// �����Ʒ��ע�����ע������汾
#define PRODUCT_COMMAN_MAINVERNAME	_T("MainVer")
/// �����Ʒ��ע�����ע��Ĵΰ汾
#define PRODUCT_COMMAN_MINIVERNAME	_T("MiniVer")
/// �����Ʒ��ע�����ע��ı���汾
#define PRODUCT_COMMAN_BUILDVERNAME	_T("BuildVer")
/// �����Ʒ��ע�����ע��İ�װ·��
#define PRODUCT_COMMAN_INSPATH		_T("InsPath")
/// �����Ʒ��ע�����ע��İ�װ��
#define PRODUCT_COMMAN_INSDISK		_T("InstallDisk")
/// �����Ʒ��ע�����ע����豸ID
#define PRODUCT_COMMAN_DEVICEID		_T("DeviceID")

/// �����������汾�İ汾��
#define PRODUCT_COMMAN_UPDATEVER	_T("UpdateVer")
/// ����URL
#define	PRODUCT_COMMAN_UPDATEURL	_T("UpdateUrl")
/// ��������
#define	PRODUCT_COMMAN_UPDATEDATE	_T("UpdateDate")
/// ��������
#define	PRODUCT_COMMAN_UPDATEDESC	_T("UpdateDesc")
/// �����ļ�Sha��
#define	PRODUCT_COMMAN_UPDATESHA	_T("UpdateSHA")

/// ������������ھ������
#define PRODUCT_COMMAN_MAINWND		_T("MainWnd")

/// ���嵱ǰ��¼�û����
#define PRODUCT_COMMAN_CURLOGIN		_T("CurLogin")
/// �������ݱ���ڵ�����
#define PRODUCT_COMMAN_DATAPATH		_T("DataPath")

/// ����ϵͳ��־д�����ݿ����
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
		/// ����ʱ�Զ��رգ�������й¶
		Close();
	}

	/// �ж��Ƿ����ע���
	BOOL IsOpen()
	{
		return m_bOpenFlag;
	}

	/**
	 * @brief ������·����������ʵ��
	 *
	 *
	 * @param strMainRegPath ע�����λ��
	 * @param strChildName ע�����Ŀ¼����
	 * @param bCurrentUser �Ƿ�Ϊ�û�ע�������Ϊϵͳע���
	 * @return 
	 */
	BOOL NewRegPath(const ATL::CString& strMainRegPath = COMPANY_REGPATH,\
		const ATL::CString& strChildName = PRODUCT_REGNODENAME,\
		HKEY hKey = HKEY_CURRENT_USER,REGSAM samDesired = KEY_ALL_ACCESS);

	/**
	 * @brief ���ݼ����ƻ���ַ������ͼ�ֵ
	 *
	 *
	 * @param strKeyName  ����
	 * @param strKeyValue ��ֵ
	 * @return �Ƿ�ɹ���ȡ
	 */
	BOOL GetRegStringVal(const ATL::CString& strKeyName,ATL::CString& strKeyValue);
	
	/**
	 * @brief ����ע����ַ�������ֵ
	 *
	 *
	 * @param strKeyName  ����
	 * @param strKeyValue ��ֵ
	 * @return �Ƿ�ɹ�����
	 */
	BOOL SetRegStringVal(const ATL::CString& strKeyName,const ATL::CString& strKeyValue);

	/**
	 * @brief ���ݼ����ƻ���ַ������ͼ�ֵ
	 *
	 *
	 * @param strKeyName  ����
	 * @param vectKeyValue �����ֵ
	 * @return �Ƿ�ɹ���ȡ
	 */
	BOOL GetRegMultiStringVal(const ATL::CString& strKeyName,CSTRING_VECTOR& vectKeyValue);
	
	/**
	 * @brief ����ע����ַ�������ֵ
	 *
	 *
	 * @param strKeyName  ����
	 * @param strKeyValue ��ֵ
	 * @return �Ƿ�ɹ�����
	 */
	BOOL SetRegMultiStringVal(const ATL::CString& strKeyName,CSTRING_VECTOR& vectKeyValue);

	/**
	 * @brief ���ע���DWORD����ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @param dwKeyValue ��ֵ
	 * @return �Ƿ��ȡ�ɹ�
	 */
	BOOL GetRegDwordVal(const ATL::CString& strKeyName,DWORD& dwKeyValue);

	/**
	 * @brief ����ע���DWORD����ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @param dwKeyValue ��ֵ
	 * @return �����Ƿ�ɹ�
	 */
	BOOL SetRegDwordVal(const ATL::CString& strKeyName,DWORD dwKeyValue);

	/**
	 * @brief ���ע��������ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @param ppValue	��ֵ
	 * @param pnBytes    ���س���
	 * @return �Ƿ��ȡ�ɹ�
	 */
	BOOL GetRegBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes);

	/**
	 * @brief ���ö�����ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @param Value		 ��ֵ
	 * @param pnBytes    ���س���
	 * @return �Ƿ����óɹ�
	 */
	BOOL SetRegBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes);

	/**
	 * @brief ���GUID����ע���ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @param Value		 ��ֵ
	 * @return �����Ƿ��ȡ�ɹ�
	 */
	BOOL GetRegGuidValue(const ATL::CString &strKeyName,GUID &Value);

	/**
	 * @brief ����GUID����ע���ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @param Value		 ��ֵ
	 * @return �����Ƿ����óɹ�
	 */
	BOOL SetRegGuidValue(const ATL::CString &strKeyName,GUID Value);

	/**
	 * @brief ɾ��ָ���ļ�ֵ
	 *
	 *
	 * @param strKeyName ����
	 * @return 
	 */
	BOOL DelKeyName(const ATL::CString& strKeyName);

	/**
	 * @brief �������������·��
	 *
	 *
	 * @return ��������·��
	 */
	ATL::CString GetAppRunPath();

	/**
	 * @brief ��������������·��
	 *
	 *
	 * @param strAppPath Ӧ�ó�������·��
	 * @return �����Ƿ����óɹ�
	 */
	BOOL SetAppRunPath(const ATL::CString &strAppPath);

	DWORD GetLastErr()
	{
		return m_dwLastErr;
	}

	/**
	 * @brief �ر�ע�����
	 *
	 *
	 */
	virtual void Close();

protected:

	/// ������
	DWORD	m_dwLastErr;

	/// �Ƿ����ע���
	BOOL	m_bOpenFlag;
};
