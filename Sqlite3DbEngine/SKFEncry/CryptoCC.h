#pragma once
#include "SdCryptoStor.h"

/// 读写数据
const int MAX_SECTOR_SIZE	= 512;
const int MAX_BUF_SIZE		= 1024;
const int MAX_READ_SIZE		= MAX_BUF_SIZE*MAX_BUF_SIZE*10;
const int MAX_WRITE_SIZE	= MAX_BUF_SIZE*MAX_BUF_SIZE*10;

const int MAX_CLEAN_BUF_SIZE = MAX_BUF_SIZE*MAX_BUF_SIZE*10;

/// 文件开始标记
#define TDHXKJ_CC_FILEBEGIN		0xFB
/// 文件删除标记
#define TDHXKJ_CC_FILEDELETE	0xFD

typedef struct FILE_INFO
{
	unsigned char m_cFileBeigSign[4];	/// 文件开始标志FB FB FB FB
	char m_cFileName[256];				/// 文件名称
	DWORD32 m_dwFileLength;				/// 文件长度
	DWORD   m_dwFileBegin;				/// 文件开始扇区
	unsigned char m_cFileDeleteSign[4]; /// 文件删除标志FD FD FD FD
	char m_cFileInfo[32];				/// 文件信息，预留
}STRUCT_FILEINFO;

///私密区文件信息列表
typedef std::vector<STRUCT_FILEINFO* > FILEINFO_VECTOR;

class CCryptoCC
{
protected:
	/// 设备句柄
	HDEV	m_hDev;

	/// 当前操作盘符
	WCHAR	m_cCurDisk;

	/// 错误码
	LONG	m_nLastErrCode;

	/// 当前错误描述
	CString m_strLastErrInfo;

public:
	CCryptoCC(void);

	/// 初始化通信接口
	void InitComm(PCHAR szFileName = NULL);
	/// 释放端口
	void FreeComm();

	/// 枚举磁盘
	CString EnumDisk();
	/// 打开连接
	DWORD OpenDev(WCHAR cDisk = 0,PCHAR szFileName = NULL);

	/// 获取容量
	DWORD GetCapcity();

	/// 获得当前盘符
	WCHAR GetCurDisk()
	{
		return m_cCurDisk;
	}

	CString GetLastErrInfo()
	{
		return m_strLastErrInfo;
	}

	LONG GetLastErrCode()
	{
		return m_nLastErrCode;
	}

	/// 读磁盘数据
	int ReadDiskData(ULONG uLba, DWORD dwBytesLength, UCHAR *pBuf);

	///读出文件
	DWORD ReadFileOut(DWORD dwBegin,DWORD dwFileLength,\
		LPTSTR strFileName,LPTSTR strPathFile);

	/// 所有文件标记为删除
	bool ClearAllFile(DWORD& dwBeginAddr);

	/// 找第一个未标记删除的文件
	DWORD GetFirstFileAddrAfterDelFile(DWORD _dwBeign,DWORD& _pdwRealBegin);

	/// 找第一个标记删除的文件
	DWORD GetFirstDeleteFileBeginAddr(DWORD _dwBeign,DWORD& _pdwRealBegin);

	/// 找空扇区以追写文件
	bool GetFirstWriteAddr(DWORD _dwBeign,DWORD& _pdwRealBegin);

	/// 删除指定位置的文件
	bool DelFile(DWORD dwFileBegin);

	/// 指定位置写文件
	DWORD WriteFileIn(DWORD dwRealBegin,const CString& strPathFile,const CString& strFName = _T(""));

	/// 磁盘清理
	bool DiskClean();
};