#pragma once
#include <tchar.h>
/// 引入STL标准库对象
#include <map>

using namespace std;

#include "SKF.h"
#include "ComHelper.h"
#include "CriticalSectionWrapper.h "

#define TDHXKJ_SKF_AUTHKEYLEN	16


class CSKFBase
{
protected:

	/// 动态库句柄
	static HANDLE m_hLib;

	/// 记录载入次数
	static int	m_nLoadRef;

	/// 同步锁
	static CCriticalSectionWrapper m_sGlobalCriticalSection;

	/// TF安全卡ID
	CString		m_strTFID;

	/// 设备句柄
	HANDLE		m_hDev;

	/// 当前卡对应的盘符
	WCHAR		m_cCurDisk;
	/// 卡集合
	CSTRING_MAP	m_mapCard;

	/// 标记是否锁定设备
	bool	m_bLockDev;

	/// 设备状态
	int		m_nDeviceState;

	/// 最近操作错误码
	int		m_nLastErrCode;

	/// 最近操作错误描述
	CString	m_strLastErrInfo;

	/// 获得设备ID
	virtual CString GetDID(HANDLE hDev) = 0;

public:

	CSKFBase(void);

	virtual ~CSKFBase(void){};

	virtual bool InitSKF() = 0;
	/// 释放资源
	virtual void UnInit();

	/// 获得当前盘符
	virtual WCHAR GetCurDisk()
	{
		return m_cCurDisk;
	}

	/// 获得最近失败错误码
	virtual int GetLastErrCode();

	/// 获得最近失败描述
	static CString GetLastErrInfo(LONG nErrCode);

	/// 重置错误信息
	void ResetErrInfo();

	///获得插入的卡数量
	virtual SHORT GetDeviceCount(CString& strCIDS) = 0;

	/// 获得指定盘符的设备ID
	virtual CString GetDeviceID(WCHAR cDisk = 0) = 0;

	/// 关闭当前设备连接
	virtual bool CloseCurCard() = 0;

	/// 设置当前盘符的设备
	virtual bool PutCurDisk(WCHAR cDisk) = 0;

	/// 连接设备
	virtual HANDLE ConnectCard(WCHAR cDisk) = 0;

	/// 锁定设备
	virtual bool LockTFDev(VARIANT_BOOL bLockFlag) = 0;

	/// 获得设备状态
	virtual SHORT GetStatus() = 0;
};
