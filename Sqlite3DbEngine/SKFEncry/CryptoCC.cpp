#include "StdAfx.h"
#include "CryptoCC.h"
#include "BaseFuncLib.h"

#ifndef WIN64
#pragma comment(lib, "..\\bin\\Release\\SdCryptoStor.lib")
#else
#pragma comment(lib, "..\\bin\\Release\\X64\\SdCryptoStor.lib")
#endif

CCryptoCC::CCryptoCC(void)
	:m_hDev(NULL)
	,m_cCurDisk(0)
	,m_nLastErrCode(0)
	,m_strLastErrInfo(_T(""))
{
}

void CCryptoCC::InitComm(PCHAR szFileName)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	/// 初始化接口文件
	BYTE btHeader[7] = {0x4d,0x50,0x33,0x5f,0x56,0x30,0x31};
	BYTE btRecv[7] = {0xb2,0xaf,0xcc,0xa0,0xa9,0xcf,0xce};
	if(NULL == szFileName)
		sd_CommInit(btHeader, btRecv, INTERFACEFILE);
	else
		sd_CommInit(btHeader, btRecv, szFileName);
}

void CCryptoCC::FreeComm()
{
	m_cCurDisk = 0;
	if(NULL != m_hDev)
	{
		/// 是否端口
		sd_CloseDevice(m_hDev);
		m_hDev = NULL;
	}
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();
}

CString CCryptoCC::EnumDisk()
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	CString strDisks(_T(""));
	DWORD ulLength = 0;
	char* bufName = NULL;
	try
	{
		m_nLastErrCode = sd_EnumDevice(&bufName,&ulLength);
	}
	catch ( ... )
	{
	}
	if(SDR_OK != m_nLastErrCode)
	{
		if(NULL != bufName)
		{
			sd_Free(bufName);
			bufName = NULL;
		}
		return strDisks;
	}
	for(DWORD nIndex = 0;nIndex < ulLength;nIndex++)
	{
		if (bufName[nIndex] == 0 || bufName[nIndex] == NULL)
			continue;
		if(!strDisks.IsEmpty())
			strDisks += _T(";");
		strDisks += bufName[nIndex];
#ifdef _DEBUG
		HDEV hDev = NULL;
		if(sd_OpenDevice(&(bufName[nIndex]),&hDev)==SDR_OK)
		{
			DWORD dwCapcity = 0;
			m_nLastErrCode = sd_GetCapcity(hDev,&dwCapcity);
			sd_CloseDevice(hDev);
			if(m_nLastErrCode != SDR_OK||dwCapcity == 0)
			{
				/// 没有获取到容量
				continue;
			}
		}
#endif
	}
	if(NULL != bufName)
	{
		sd_Free(bufName);
		bufName = NULL;
	}
	return strDisks;
}

DWORD CCryptoCC::OpenDev(WCHAR cDisk,PCHAR szFileName)
{
	DWORD dwCapcity = 0;

	/// 初始化接口文件，必须有设备才行
	InitComm(szFileName);

	HDEV hDev = NULL;
	if(m_cCurDisk != cDisk || NULL == m_hDev)
	{
		if(NULL != m_hDev)
		{
			/// 关闭前面打开的设备
			sd_CloseDevice(m_hDev);
			m_hDev = NULL;
		}
		m_cCurDisk = 0;
		int nRet = sd_OpenDevice((const char *)&cDisk,&hDev);
		if(SDR_OK != nRet)
		{
			m_nLastErrCode = nRet;
			if(ERROR_ACCESS_DENIED != m_nLastErrCode)
			{
#ifdef APP_LOG_ENABLE
				/// 写日志
				CString strLog(_T(""));
				strLog.Format(_T("私密区访问失败，返回值:%ld"),nRet);
				WRITELOGTOFILE(strLog);
#endif
			}
			/// 获取失败信息
			return dwCapcity;
		}
	}

	if(m_cCurDisk == 0 && m_cCurDisk != cDisk)
	{
		m_hDev = hDev;
		m_cCurDisk = cDisk;/// 设为当前设备
	}
	if(!dwCapcity && NULL != m_hDev)
	{
		/// 获取容量
		sd_GetCapcity(m_hDev,&dwCapcity);
	}
	return dwCapcity;
}

DWORD CCryptoCC::ReadFileOut(DWORD dwBegin,DWORD dwFileLength,\
	LPTSTR strFileName,LPTSTR strPathFile)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	DWORD dwRetLen = 0;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return dwRetLen;///还没有打开设备
	}
	HANDLE hFileOpen = CreateFile(strPathFile,GENERIC_WRITE,FILE_SHARE_WRITE,\
		NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFileOpen == INVALID_HANDLE_VALUE)
	{
		m_nLastErrCode = ::GetLastError();
		return dwRetLen;
	}

	DWORD dwSectorCount = 0;
	if(dwFileLength % MAX_SECTOR_SIZE)
	{
		dwSectorCount = dwFileLength/MAX_SECTOR_SIZE + 1;
	}
	else
	{
		dwSectorCount = dwFileLength/MAX_SECTOR_SIZE;
	}
	DWORD dwReadBeginAddr = dwBegin + 1;
	if(dwFileLength < MAX_SECTOR_SIZE)
	{
		//文件长度小于一个扇区的
		BYTE pBuf[MAX_SECTOR_SIZE] = {0};
		sd_ReadData(m_hDev,dwReadBeginAddr,MAX_SECTOR_SIZE,pBuf);
		::WriteFile(hFileOpen,pBuf,dwFileLength,&dwRetLen,NULL);
		ATLASSERT(dwRetLen);
	}
	else
	{
		BYTE *pReadBuf = NULL;
		pReadBuf = new BYTE[MAX_READ_SIZE];
		if(NULL == pReadBuf)
		{
			::CloseHandle(hFileOpen);
			hFileOpen = NULL;
			return dwRetLen;/// 内存分配失败
		}
		if (dwSectorCount*MAX_SECTOR_SIZE > MAX_READ_SIZE)//文件长度大于最大读出缓冲的
		{
			DWORD32 dwLengthCount = 0;
			while ((dwFileLength-MAX_READ_SIZE) >= dwLengthCount)//文件剩余长度大于读出缓冲区大小则循环继续
			{
				memset(pReadBuf,0x00,MAX_READ_SIZE);
				sd_ReadData(m_hDev,dwReadBeginAddr,MAX_READ_SIZE,pReadBuf);
				DWORD dwWriteLen = 0;
				::WriteFile(hFileOpen,pReadBuf,MAX_READ_SIZE,&dwWriteLen,NULL);
				dwRetLen += dwWriteLen;
				dwLengthCount += MAX_READ_SIZE;
				dwReadBeginAddr += (MAX_READ_SIZE/MAX_SECTOR_SIZE);
				if(!dwWriteLen)
					break;/// 写失败					
			}
			DWORD dwLeftLength = dwFileLength-dwLengthCount;
			if(dwLeftLength > 0)//文件读取长度小于文件实际总长度
			{
				if(dwLeftLength%MAX_SECTOR_SIZE)
				{
					dwSectorCount = dwLeftLength/MAX_SECTOR_SIZE + 1;
				}
				else
				{
					dwSectorCount = dwLeftLength/MAX_SECTOR_SIZE;
				}

				memset(pReadBuf,0x00,MAX_READ_SIZE);
				sd_ReadData(m_hDev,dwReadBeginAddr,dwSectorCount*MAX_SECTOR_SIZE,pReadBuf);
				DWORD dwWriteLen = 0;
				::WriteFile(hFileOpen,pReadBuf,dwLeftLength,&dwWriteLen,NULL);
				dwRetLen += dwWriteLen;
			}
		}
		else
		{
			memset(pReadBuf,0x00,MAX_READ_SIZE);
			sd_ReadData(m_hDev,dwReadBeginAddr,dwFileLength,pReadBuf);
			/// 写文件长度的内容
			DWORD dwWriteLen = 0;
			::WriteFile(hFileOpen,pReadBuf,dwFileLength,&dwWriteLen,NULL);
			dwRetLen += dwWriteLen;
		}	
		delete []pReadBuf;
		pReadBuf = NULL;
	}

	::CloseHandle(hFileOpen);
	hFileOpen = NULL;
	return dwRetLen;
}

int CCryptoCC::ReadDiskData(ULONG uLba, DWORD dwBytesLength, UCHAR *pBuf)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return SDR_PARAM_ERROR;
	}
	return sd_ReadData(m_hDev,uLba,dwBytesLength,pBuf);
}

DWORD CCryptoCC::GetFirstDeleteFileBeginAddr(DWORD _dwBeign,DWORD& _pdwRealBegin)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	DWORD dwRet = 0;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return dwRet;
	}
	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	int iRet = -1;
	DWORD dwBeginTemp = 0;
	iRet = sd_ReadData(m_hDev,_dwBeign,MAX_SECTOR_SIZE,pBuf);
	if(iRet != SDR_OK)
	{
		m_nLastErrCode = iRet;
		return dwRet;
	}
	STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
	if(pFileInfo->m_cFileBeigSign[0] == TDHXKJ_CC_FILEBEGIN
		&&pFileInfo->m_cFileBeigSign[1] == TDHXKJ_CC_FILEBEGIN
		&&pFileInfo->m_cFileBeigSign[2] == TDHXKJ_CC_FILEBEGIN
		&&pFileInfo->m_cFileBeigSign[3] == TDHXKJ_CC_FILEBEGIN)
	{

		if(pFileInfo->m_cFileDeleteSign[0] == TDHXKJ_CC_FILEDELETE
			&&pFileInfo->m_cFileDeleteSign[1] == TDHXKJ_CC_FILEDELETE
			&&pFileInfo->m_cFileDeleteSign[2] == TDHXKJ_CC_FILEDELETE
			&&pFileInfo->m_cFileDeleteSign[3] == TDHXKJ_CC_FILEDELETE)
		{
			/// 找第一个标记删除的文件
			_pdwRealBegin = _dwBeign;
			dwRet = 1;
		}
		else
		{
			DWORD32 dwFileLen = pFileInfo->m_dwFileLength;
			if(dwFileLen%MAX_SECTOR_SIZE)
			{
				dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE)+1;//不足一个扇区按一个扇区计算
			}
			else
			{
				if(dwFileLen < MAX_SECTOR_SIZE)
				{
					dwBeginTemp = 1;
				}
				else
				{
					dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE);
				}
			}
			dwBeginTemp++;//加一个文件系统扇区
			_pdwRealBegin += dwBeginTemp;
			dwRet = GetFirstDeleteFileBeginAddr(_pdwRealBegin,_pdwRealBegin);
		}
	}
	return dwRet;
}

DWORD CCryptoCC::GetFirstFileAddrAfterDelFile(DWORD _dwBeign,DWORD& _pdwRealBegin)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	DWORD dwRet = 0;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return dwRet;
	}
	int iRet = -1;
	DWORD dwBeginTemp = 0;
	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	iRet = sd_ReadData(m_hDev,_dwBeign,MAX_SECTOR_SIZE,pBuf);
	if(iRet != SDR_OK)
	{
		m_nLastErrCode = iRet;
		return dwRet;
	}
	STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
	if(pFileInfo->m_cFileBeigSign[0] == TDHXKJ_CC_FILEBEGIN
		&&pFileInfo->m_cFileBeigSign[1] == TDHXKJ_CC_FILEBEGIN
		&&pFileInfo->m_cFileBeigSign[2] == TDHXKJ_CC_FILEBEGIN
		&&pFileInfo->m_cFileBeigSign[3] == TDHXKJ_CC_FILEBEGIN)
	{
		if(pFileInfo->m_cFileDeleteSign[0] != TDHXKJ_CC_FILEDELETE
			&&pFileInfo->m_cFileDeleteSign[1] != TDHXKJ_CC_FILEDELETE
			&&pFileInfo->m_cFileDeleteSign[2] != TDHXKJ_CC_FILEDELETE
			&&pFileInfo->m_cFileDeleteSign[3] != TDHXKJ_CC_FILEDELETE)
		{
			/// 找到第一个未删除的文件
			_pdwRealBegin = _dwBeign;
			dwRet = 1;
		}
		else
		{
			DWORD32 dwFileLen = pFileInfo->m_dwFileLength;
			if(dwFileLen%MAX_SECTOR_SIZE)
			{
				dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE)+1;//不足一个扇区按一个扇区计算
			}
			else
			{
				if(dwFileLen < MAX_SECTOR_SIZE)
				{
					dwBeginTemp = 1;
				}
				else
				{
					dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE);
				}
			}
			dwBeginTemp ++;//加一个文件系统扇区,
			_pdwRealBegin += dwBeginTemp;
			dwRet = GetFirstFileAddrAfterDelFile(_pdwRealBegin,_pdwRealBegin);		
		}	
	}
	return dwRet;
}

bool CCryptoCC::GetFirstWriteAddr(DWORD _dwBeign,DWORD& _pdwRealBegin)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	int iRet = -1;
	bool bBeginAddr = false;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return bBeginAddr;
	}

	DWORD dwBeginTemp = 0;
	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	iRet = sd_ReadData(m_hDev,_dwBeign,MAX_SECTOR_SIZE,pBuf);
	if(iRet == SDR_OK)
	{
		STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
		if(pFileInfo->m_cFileBeigSign[0] == TDHXKJ_CC_FILEBEGIN
			&&pFileInfo->m_cFileBeigSign[1] == TDHXKJ_CC_FILEBEGIN
			&&pFileInfo->m_cFileBeigSign[2] == TDHXKJ_CC_FILEBEGIN
			&&pFileInfo->m_cFileBeigSign[3] == TDHXKJ_CC_FILEBEGIN)
		{
			DWORD32 dwFileLen = pFileInfo->m_dwFileLength;
			if(dwFileLen%MAX_SECTOR_SIZE)
			{
				dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE)+1;//不足一个扇区按一个扇区计算
			}
			else
			{
				if(dwFileLen < MAX_SECTOR_SIZE)
				{
					dwBeginTemp = 1;
				}
				else
				{
					dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE);
				}
			}
			//加一个文件系统扇区
			dwBeginTemp ++;
			_pdwRealBegin += dwBeginTemp;
			GetFirstWriteAddr(_pdwRealBegin,_pdwRealBegin);
			bBeginAddr = true;
		}
	}
	return bBeginAddr;
}


bool CCryptoCC::ClearAllFile(DWORD& dwBeginAddr)
{
	bool bRet = false;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return bRet;///还没有打开设备
	}
	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	BYTE pWriteBuf[MAX_SECTOR_SIZE]= {0};
	int iRet = sd_ReadData(m_hDev,dwBeginAddr,MAX_SECTOR_SIZE,pBuf);
	if(SDR_OK != iRet)
	{
		m_nLastErrCode = iRet;
		return true;///无文件了 
	}
	STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
	if(TDHXKJ_CC_FILEBEGIN == pFileInfo->m_cFileBeigSign[0] &&
		TDHXKJ_CC_FILEBEGIN == pFileInfo->m_cFileBeigSign[1] &&
		TDHXKJ_CC_FILEBEGIN == pFileInfo->m_cFileBeigSign[2] &&
		TDHXKJ_CC_FILEBEGIN == pFileInfo->m_cFileBeigSign[3] &&
		TDHXKJ_CC_FILEDELETE != pFileInfo->m_cFileDeleteSign[0] &&
		TDHXKJ_CC_FILEDELETE != pFileInfo->m_cFileDeleteSign[1] &&
		TDHXKJ_CC_FILEDELETE != pFileInfo->m_cFileDeleteSign[2] &&
		TDHXKJ_CC_FILEDELETE != pFileInfo->m_cFileDeleteSign[3])
	{
		/// 标记文件为删除
		memset(pFileInfo->m_cFileDeleteSign,0,sizeof(pFileInfo->m_cFileDeleteSign));
		memset(pFileInfo->m_cFileInfo,0,sizeof(pFileInfo->m_cFileInfo));
		pFileInfo->m_cFileDeleteSign[0] = TDHXKJ_CC_FILEDELETE;
		pFileInfo->m_cFileDeleteSign[1] = TDHXKJ_CC_FILEDELETE;
		pFileInfo->m_cFileDeleteSign[2] = TDHXKJ_CC_FILEDELETE;
		pFileInfo->m_cFileDeleteSign[3] = TDHXKJ_CC_FILEDELETE;
		memcpy(pWriteBuf,pFileInfo,MAX_SECTOR_SIZE);
		iRet  = sd_WriteData(m_hDev,dwBeginAddr,MAX_SECTOR_SIZE,pWriteBuf);
	}

	DWORD32 dwFileLen = pFileInfo->m_dwFileLength;
	if(!dwFileLen)
	{
		bRet = true;;/// 无文件了
		return bRet;
	}
	DWORD dwBeginTemp = 0;
	if(dwFileLen%MAX_SECTOR_SIZE)
	{
		dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE)+1;//不足一个扇区按一个扇区计算
	}
	else
	{
		if(dwFileLen < MAX_SECTOR_SIZE)
		{
			dwBeginTemp = 1;
		}
		else
		{
			dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE);
		}
	}
	dwBeginTemp ++;//加一个文件系统扇区
	dwBeginAddr += dwBeginTemp;

	/// 递归查询下一个未标记删除的文件
	bRet = ClearAllFile(dwBeginAddr);
	if(iRet)
		bRet = true;
	return bRet;
}

DWORD CCryptoCC::GetCapcity()
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	DWORD dwCapcity = 0;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return dwCapcity;///还没有打开设备
	}
	m_nLastErrCode = sd_GetCapcity(m_hDev,&dwCapcity);
	return dwCapcity;
}

DWORD CCryptoCC::WriteFileIn(DWORD dwRealBegin,const CString& strPathFile,const CString& strFName)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	DWORD dwNextBegin = dwRealBegin;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return dwNextBegin;
	}
	DWORD m = 0;
	HANDLE hFileOpen = ::CreateFile(strPathFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
	if(NULL == hFileOpen)
	{
		m_nLastErrCode = ::GetLastError();
		return dwNextBegin;/// 打开文件失败
	}
	DWORD dwCapcity = GetCapcity();
	DWORD dwHeighSize = 0,dwSizeM = 0;
	DWORD dwFileSize = ::GetFileSize(hFileOpen,&dwHeighSize);
	dwSizeM = (DWORD)(dwFileSize/MAX_SECTOR_SIZE);
	if(dwFileSize%MAX_SECTOR_SIZE)
		dwSizeM++;
	if(dwSizeM >= dwCapcity)
	{
		::CloseHandle(hFileOpen);
		hFileOpen = NULL;
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("文件太大");
		return dwNextBegin;
	}
	STRUCT_FILEINFO strInfo;
	strInfo.m_cFileBeigSign[0] = TDHXKJ_CC_FILEBEGIN;
	strInfo.m_cFileBeigSign[1] = TDHXKJ_CC_FILEBEGIN;
	strInfo.m_cFileBeigSign[2] = TDHXKJ_CC_FILEBEGIN;
	strInfo.m_cFileBeigSign[3] = TDHXKJ_CC_FILEBEGIN;
	strInfo.m_dwFileBegin = dwRealBegin;
	DWORD dwNeed = dwSizeM;
	/// 计算需要写的扇区数量
	if((dwRealBegin + dwNeed) > dwCapcity)
	{
		/// 需要碎片整理
		ATLASSERT(FALSE);
		::CloseHandle(hFileOpen);
		hFileOpen = NULL;
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("需要碎片整理");
		return dwNextBegin;
	}
	memset(strInfo.m_cFileName,0,256);
	strInfo.m_dwFileLength = dwFileSize;
	CString strFileName(strFName);
	if(strFileName.IsEmpty())
	{
		strFileName = strPathFile;
		int nFind = strPathFile.ReverseFind(_T('\\'));
		if(-1 != nFind)
			strFileName.Delete(0,nFind+1);
	}
	char *szFile = NULL;
	int nFileLen = CBaseFuncLib::Us2ToChar(strFileName,&szFile);
	if(NULL != szFile)
	{
		strcpy_s(strInfo.m_cFileName,256,szFile);
		delete []szFile;
		szFile = NULL;
	}

	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	memset(pBuf,0,MAX_SECTOR_SIZE);
	memcpy(pBuf,&strInfo,sizeof(strInfo));
	int iRet  = sd_WriteData(m_hDev,dwRealBegin,MAX_SECTOR_SIZE,pBuf);
	if(iRet != SDR_OK)
	{
		::CloseHandle(hFileOpen);
		hFileOpen = NULL;
		m_nLastErrCode = iRet;
		return dwNextBegin;
	}
	dwRealBegin++;

	//申请大容量内存读取文件
	BYTE* pWriteBuf = new BYTE[MAX_WRITE_SIZE];
	if(pWriteBuf == NULL)
	{
		/// 分配内存失败
		::CloseHandle(hFileOpen);
		hFileOpen = NULL;
		m_strLastErrInfo = _T("内存不足");
		return dwNextBegin;
	}
	memset(pWriteBuf,0x00,MAX_WRITE_SIZE);

	DWORD dwReadLength = 0;
	::ReadFile(hFileOpen,pWriteBuf,MAX_WRITE_SIZE,&dwReadLength,NULL);
	while(dwReadLength)
	{
		if(dwReadLength == MAX_WRITE_SIZE)
		{
			iRet = sd_WriteData(m_hDev,dwRealBegin,MAX_WRITE_SIZE,pWriteBuf);
			if(iRet == SDR_OK)
			{
				/// 扇区数
				dwNextBegin += MAX_WRITE_SIZE/MAX_SECTOR_SIZE;
			}
			else
			{
				m_nLastErrCode = iRet;
				break;
			}
		}
		else if(dwReadLength > 0)
		{
			int iSector = 0;
			if(dwReadLength%MAX_SECTOR_SIZE)
			{
				iSector = dwReadLength/MAX_SECTOR_SIZE + 1;
			}
			else
			{
				if(dwReadLength < MAX_SECTOR_SIZE)
				{
					iSector = 1;
				}
				else
				{
					iSector = dwReadLength/MAX_SECTOR_SIZE;
				}
			}
			iRet = sd_WriteData(m_hDev,dwRealBegin,iSector*MAX_SECTOR_SIZE,pWriteBuf);
			if(SDR_OK == iRet)
				dwNextBegin += iSector;
			else
			{
				m_nLastErrCode = iRet;
				break;
			}
		}

		/// 再次读取，恢复参数
		dwReadLength = 0;
		/// 重置数据区
		memset(pWriteBuf,0x00,MAX_WRITE_SIZE);
		::ReadFile(hFileOpen,pWriteBuf,MAX_WRITE_SIZE,&dwReadLength,NULL);
	}

	if(pWriteBuf != NULL)
	{
		delete []pWriteBuf;
		pWriteBuf = NULL;
	}

	::CloseHandle(hFileOpen);
	hFileOpen = NULL;
	return dwNextBegin;
}

bool CCryptoCC::DelFile(DWORD dwFileBegin)
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	bool bRetFlag = false;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return bRetFlag;
	}
	BYTE pBuf[512]= {0};
	BYTE pWriteBuf[512]= {0};
	int iRet = sd_ReadData(m_hDev,dwFileBegin,MAX_SECTOR_SIZE,pBuf);
	if(iRet == SDR_OK)
	{
		STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
		if(pFileInfo->m_cFileBeigSign[0] == 0xFB
			&&pFileInfo->m_cFileBeigSign[1] == 0xFB
			&&pFileInfo->m_cFileBeigSign[2] == 0xFB
			&&pFileInfo->m_cFileBeigSign[3] == 0xFB)
		{
			pFileInfo->m_cFileDeleteSign[0] = 0xFD;
			pFileInfo->m_cFileDeleteSign[1] = 0xFD;
			pFileInfo->m_cFileDeleteSign[2] = 0xFD;
			pFileInfo->m_cFileDeleteSign[3] = 0xFD;
			memcpy(pWriteBuf,pFileInfo,MAX_SECTOR_SIZE);
			iRet  = sd_WriteData(m_hDev,dwFileBegin,MAX_SECTOR_SIZE,pWriteBuf);
			if(iRet == SDR_OK)
			{
				bRetFlag = true;
			}
			else
				m_nLastErrCode = iRet;
		}
	}
	return bRetFlag;
}

bool CCryptoCC::DiskClean()
{
	m_nLastErrCode = 0;
	m_strLastErrInfo.Empty();

	bool bCleanFlag = false;
	if(NULL == m_hDev)
	{
		m_nLastErrCode = SDR_PARAM_ERROR;
		m_strLastErrInfo = _T("还没有打开端口");
		return bCleanFlag;
	}
	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	int iRet = -1;
	DWORD dwFirstDelFileAddr = 0;
	DWORD dwCapcity = 0;
	iRet = sd_GetCapcity(m_hDev,&dwCapcity);
	if(dwCapcity == 0)
	{
		m_nLastErrCode = iRet;
		return bCleanFlag;
	}
	BYTE* btFileBuf = new BYTE[MAX_CLEAN_BUF_SIZE];
	if(btFileBuf == NULL)
	{
		m_strLastErrInfo = _T("内存不足");
		return bCleanFlag;
	}
	/// 找标记删除的文件
	if(!GetFirstDeleteFileBeginAddr(0,dwFirstDelFileAddr))
	{
		/// 无文件需要整理
		delete []btFileBuf;
		btFileBuf = NULL;
		return true;
	}
	DWORD dwBeginTemp = 0;
	DWORD dwFirstFileAddrAftDel = 0;
	DWORD dwNextFileAddrAftDel = 0;
	DWORD dwCurrentFileAddrTemp = 0; //记录当前移动文件的开始，以备移位后删除该文件

	dwFirstFileAddrAftDel = dwFirstDelFileAddr;
	/// 找标记删除后的未标记删除的文件
	while(GetFirstFileAddrAfterDelFile(dwFirstFileAddrAftDel,dwFirstFileAddrAftDel))
	{
		dwCurrentFileAddrTemp = dwFirstFileAddrAftDel;
		iRet = sd_ReadData(m_hDev,dwFirstFileAddrAftDel,MAX_SECTOR_SIZE,pBuf);
		if(iRet != SDR_OK)
			continue;
		STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
		if(pFileInfo->m_cFileBeigSign[0] != TDHXKJ_CC_FILEBEGIN
			|| pFileInfo->m_cFileBeigSign[1] != TDHXKJ_CC_FILEBEGIN
			|| pFileInfo->m_cFileBeigSign[2] != TDHXKJ_CC_FILEBEGIN
			|| pFileInfo->m_cFileBeigSign[3] != TDHXKJ_CC_FILEBEGIN)
			continue;/// 找到一个文件
		DWORD32 dwFileLen = pFileInfo->m_dwFileLength;
		if(dwFileLen%MAX_SECTOR_SIZE)
		{
			dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE)+1;//不足一个扇区按一个扇区计算
		}
		else
		{
			if(dwFileLen < MAX_SECTOR_SIZE)
			{
				dwBeginTemp = 1;
			}
			else
			{
				dwBeginTemp = (dwFileLen/MAX_SECTOR_SIZE);
			}
		}
		dwBeginTemp++;//加一个文件系统扇区

		if(dwFileLen <= MAX_CLEAN_BUF_SIZE)//小于10M直接申请空间全部读取之后再写入
		{
			memset(btFileBuf,0,MAX_CLEAN_BUF_SIZE);
			iRet = sd_ReadData(m_hDev,dwFirstFileAddrAftDel,dwBeginTemp*MAX_SECTOR_SIZE,btFileBuf);
			if(iRet == SDR_OK)
			{
				//读出来去除文件开始标志
				pFileInfo->m_cFileBeigSign[0] = 0x00;
				pFileInfo->m_cFileBeigSign[1] = 0x00;
				pFileInfo->m_cFileBeigSign[2] = 0x00;
				pFileInfo->m_cFileBeigSign[3] = 0x00;
				pFileInfo->m_dwFileBegin = 0;
				memset(pFileInfo->m_cFileName,0,sizeof(pFileInfo->m_cFileName));
				memset(pFileInfo->m_cFileInfo,0,sizeof(pFileInfo->m_cFileInfo));
				memset(pFileInfo->m_cFileDeleteSign,0,sizeof(pFileInfo->m_cFileDeleteSign));
				sd_WriteData(m_hDev,dwCurrentFileAddrTemp,MAX_SECTOR_SIZE,(UCHAR*)pFileInfo); 

				iRet = sd_WriteData(m_hDev,dwFirstDelFileAddr,dwBeginTemp*MAX_SECTOR_SIZE,btFileBuf);
				if(iRet == SDR_OK)
				{
					dwFirstDelFileAddr += dwBeginTemp;
					dwFirstFileAddrAftDel += dwBeginTemp;
					bCleanFlag = true;
				}
			}
		}
		else//大于10M申请10M的空间，循环读取写入
		{
			DWORD iLoop = 0;
			int iSectorCount = 0;
			dwFileLen += MAX_SECTOR_SIZE;//总长度加上文件系统的长度
			while ((dwFileLen - iLoop*MAX_CLEAN_BUF_SIZE) >= MAX_CLEAN_BUF_SIZE)
			{
				memset(btFileBuf,0,MAX_CLEAN_BUF_SIZE);
				iRet = sd_ReadData(m_hDev,dwFirstFileAddrAftDel,MAX_CLEAN_BUF_SIZE,btFileBuf);
				if(iRet == SDR_OK)
				{
					if(0 == iLoop)
					{
						//读出来去除文件开始标志
						pFileInfo->m_cFileBeigSign[0] = 0x00;
						pFileInfo->m_cFileBeigSign[1] = 0x00;
						pFileInfo->m_cFileBeigSign[2] = 0x00;
						pFileInfo->m_cFileBeigSign[3] = 0x00;
						pFileInfo->m_dwFileBegin = 0;
						memset(pFileInfo->m_cFileName,0,sizeof(pFileInfo->m_cFileName));
						memset(pFileInfo->m_cFileInfo,0,sizeof(pFileInfo->m_cFileInfo));
						memset(pFileInfo->m_cFileDeleteSign,0,sizeof(pFileInfo->m_cFileDeleteSign));
						sd_WriteData(m_hDev,dwCurrentFileAddrTemp,MAX_SECTOR_SIZE,(UCHAR*)pFileInfo);
						bCleanFlag = true;
					}

					iRet = sd_WriteData(m_hDev,dwFirstDelFileAddr,MAX_CLEAN_BUF_SIZE,btFileBuf);
					dwFirstFileAddrAftDel += MAX_CLEAN_BUF_SIZE/MAX_SECTOR_SIZE;
					dwFirstDelFileAddr += MAX_CLEAN_BUF_SIZE/MAX_SECTOR_SIZE;
					iLoop ++;
					bCleanFlag = true;
				}
			}
			DWORD dwLeftLen = dwFileLen-iLoop*MAX_CLEAN_BUF_SIZE;
			if(dwLeftLen > 0)
			{
				if(dwLeftLen%MAX_SECTOR_SIZE)
				{
					iSectorCount = dwLeftLen/MAX_SECTOR_SIZE + 1;
				}
				else
				{
					iSectorCount = dwLeftLen/MAX_SECTOR_SIZE;
				}
				memset(btFileBuf,0,MAX_CLEAN_BUF_SIZE);
				iRet = sd_ReadData(m_hDev,dwFirstFileAddrAftDel,iSectorCount*MAX_SECTOR_SIZE,btFileBuf);
				if(iRet == SDR_OK)
				{
					iRet = sd_WriteData(m_hDev,dwFirstDelFileAddr,iSectorCount*MAX_SECTOR_SIZE,btFileBuf);
					bCleanFlag = true;
				}
				dwFirstFileAddrAftDel += iSectorCount;
				dwFirstDelFileAddr += iSectorCount;
			}
		}
	}

	if(btFileBuf != NULL)
	{
		delete []btFileBuf;
		btFileBuf = NULL;
	}
	return bCleanFlag;
}