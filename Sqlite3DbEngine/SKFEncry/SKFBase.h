#pragma once
#include <tchar.h>
/// ����STL��׼�����
#include <map>

using namespace std;

#include "SKF.h"
#include "ComHelper.h"
#include "CriticalSectionWrapper.h "

#define TDHXKJ_SKF_AUTHKEYLEN	16


class CSKFBase
{
protected:

	/// ��̬����
	static HANDLE m_hLib;

	/// ��¼�������
	static int	m_nLoadRef;

	/// ͬ����
	static CCriticalSectionWrapper m_sGlobalCriticalSection;

	/// TF��ȫ��ID
	CString		m_strTFID;

	/// �豸���
	HANDLE		m_hDev;

	/// ��ǰ����Ӧ���̷�
	WCHAR		m_cCurDisk;
	/// ������
	CSTRING_MAP	m_mapCard;

	/// ����Ƿ������豸
	bool	m_bLockDev;

	/// �豸״̬
	int		m_nDeviceState;

	/// �������������
	int		m_nLastErrCode;

	/// ���������������
	CString	m_strLastErrInfo;

	/// ����豸ID
	virtual CString GetDID(HANDLE hDev) = 0;

public:

	CSKFBase(void);

	virtual ~CSKFBase(void){};

	virtual bool InitSKF() = 0;
	/// �ͷ���Դ
	virtual void UnInit();

	/// ��õ�ǰ�̷�
	virtual WCHAR GetCurDisk()
	{
		return m_cCurDisk;
	}

	/// ������ʧ�ܴ�����
	virtual int GetLastErrCode();

	/// ������ʧ������
	static CString GetLastErrInfo(LONG nErrCode);

	/// ���ô�����Ϣ
	void ResetErrInfo();

	///��ò���Ŀ�����
	virtual SHORT GetDeviceCount(CString& strCIDS) = 0;

	/// ���ָ���̷����豸ID
	virtual CString GetDeviceID(WCHAR cDisk = 0) = 0;

	/// �رյ�ǰ�豸����
	virtual bool CloseCurCard() = 0;

	/// ���õ�ǰ�̷����豸
	virtual bool PutCurDisk(WCHAR cDisk) = 0;

	/// �����豸
	virtual HANDLE ConnectCard(WCHAR cDisk) = 0;

	/// �����豸
	virtual bool LockTFDev(VARIANT_BOOL bLockFlag) = 0;

	/// ����豸״̬
	virtual SHORT GetStatus() = 0;
};
