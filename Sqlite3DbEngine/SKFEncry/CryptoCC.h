#pragma once
#include "SdCryptoStor.h"

/// ��д����
const int MAX_SECTOR_SIZE	= 512;
const int MAX_BUF_SIZE		= 1024;
const int MAX_READ_SIZE		= MAX_BUF_SIZE*MAX_BUF_SIZE*10;
const int MAX_WRITE_SIZE	= MAX_BUF_SIZE*MAX_BUF_SIZE*10;

const int MAX_CLEAN_BUF_SIZE = MAX_BUF_SIZE*MAX_BUF_SIZE*10;

/// �ļ���ʼ���
#define TDHXKJ_CC_FILEBEGIN		0xFB
/// �ļ�ɾ�����
#define TDHXKJ_CC_FILEDELETE	0xFD

typedef struct FILE_INFO
{
	unsigned char m_cFileBeigSign[4];	/// �ļ���ʼ��־FB FB FB FB
	char m_cFileName[256];				/// �ļ�����
	DWORD32 m_dwFileLength;				/// �ļ�����
	DWORD   m_dwFileBegin;				/// �ļ���ʼ����
	unsigned char m_cFileDeleteSign[4]; /// �ļ�ɾ����־FD FD FD FD
	char m_cFileInfo[32];				/// �ļ���Ϣ��Ԥ��
}STRUCT_FILEINFO;

///˽�����ļ���Ϣ�б�
typedef std::vector<STRUCT_FILEINFO* > FILEINFO_VECTOR;

class CCryptoCC
{
protected:
	/// �豸���
	HDEV	m_hDev;

	/// ��ǰ�����̷�
	WCHAR	m_cCurDisk;

	/// ������
	LONG	m_nLastErrCode;

	/// ��ǰ��������
	CString m_strLastErrInfo;

public:
	CCryptoCC(void);

	/// ��ʼ��ͨ�Žӿ�
	void InitComm(PCHAR szFileName = NULL);
	/// �ͷŶ˿�
	void FreeComm();

	/// ö�ٴ���
	CString EnumDisk();
	/// ������
	DWORD OpenDev(WCHAR cDisk = 0,PCHAR szFileName = NULL);

	/// ��ȡ����
	DWORD GetCapcity();

	/// ��õ�ǰ�̷�
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

	/// ����������
	int ReadDiskData(ULONG uLba, DWORD dwBytesLength, UCHAR *pBuf);

	///�����ļ�
	DWORD ReadFileOut(DWORD dwBegin,DWORD dwFileLength,\
		LPTSTR strFileName,LPTSTR strPathFile);

	/// �����ļ����Ϊɾ��
	bool ClearAllFile(DWORD& dwBeginAddr);

	/// �ҵ�һ��δ���ɾ�����ļ�
	DWORD GetFirstFileAddrAfterDelFile(DWORD _dwBeign,DWORD& _pdwRealBegin);

	/// �ҵ�һ�����ɾ�����ļ�
	DWORD GetFirstDeleteFileBeginAddr(DWORD _dwBeign,DWORD& _pdwRealBegin);

	/// �ҿ�������׷д�ļ�
	bool GetFirstWriteAddr(DWORD _dwBeign,DWORD& _pdwRealBegin);

	/// ɾ��ָ��λ�õ��ļ�
	bool DelFile(DWORD dwFileBegin);

	/// ָ��λ��д�ļ�
	DWORD WriteFileIn(DWORD dwRealBegin,const CString& strPathFile,const CString& strFName = _T(""));

	/// ��������
	bool DiskClean();
};