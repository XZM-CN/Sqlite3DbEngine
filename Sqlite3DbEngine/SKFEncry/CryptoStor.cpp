// CryptoStor.cpp : CCryptoStor 的实现

#include "stdafx.h"
#include "CryptoStor.h"
#include "BaseFuncLib.h"

// CCryptoStor

STDMETHODIMP CCryptoStor::EnumDisk(BSTR* pbstrDisks)
{
	// TODO: 在此添加实现代码
	if(NULL == pbstrDisks)
		return E_POINTER;
	ATLASSERT(m_pCCHelper);
	if(NULL == m_pCCHelper)
		return E_FAIL;
	CString strDisks = m_pCCHelper->EnumDisk();
	CComBSTR bstrVal(strDisks);
	bstrVal.CopyTo(pbstrDisks);
	bstrVal.Empty();
	strDisks.Empty();	
	return S_OK;
}

STDMETHODIMP CCryptoStor::PutCurDisk(BSTR cDisk,DWORD* dwCapcity)
{
	// TODO: 在此添加实现代码
	if(0 == cDisk || NULL == dwCapcity)
		return E_POINTER;
	CComBSTR bstrDisk;
	bstrDisk.Append(cDisk);
	if(!bstrDisk.Length())
		return E_FAIL;
	(*dwCapcity) = m_pCCHelper->OpenDev(bstrDisk.m_str[0],INTERFACEFILE);
	bstrDisk.Empty();
	return S_OK;
}

STDMETHODIMP CCryptoStor::GetCurDisk(BSTR* pcDisk)
{
	// TODO: 在此添加实现代码
	if(NULL == pcDisk)
		return E_POINTER;
	CComBSTR bstrDisk;
	bstrDisk.Append(m_pCCHelper->GetCurDisk());
	bstrDisk.CopyTo(pcDisk);
	bstrDisk.Empty();
	return S_OK;
}

BOOL CCryptoStor::GetAllFileInfo(DWORD _dwBegin,DWORD& _pdwRealBegin)
{
	if(NULL == m_pCCHelper)
		return FALSE;
	DWORD dwBeginAddr = 0;
	BYTE pBuf[MAX_SECTOR_SIZE] = {0};
	DWORD dwBeginTemp = 0;
	int iRet = m_pCCHelper->ReadDiskData(_dwBegin,MAX_SECTOR_SIZE,pBuf);
	if(SDR_OK != iRet)
		return FALSE;

	STRUCT_FILEINFO* pFileInfo = (STRUCT_FILEINFO*)pBuf;
	if(pFileInfo->m_cFileBeigSign[0] == TDHXKJ_CC_FILEBEGIN
		&& pFileInfo->m_cFileBeigSign[1] == TDHXKJ_CC_FILEBEGIN
		&& pFileInfo->m_cFileBeigSign[2] == TDHXKJ_CC_FILEBEGIN
		&& pFileInfo->m_cFileBeigSign[3] == TDHXKJ_CC_FILEBEGIN)
	{
		DWORD32 dwFileLen = pFileInfo->m_dwFileLength;
		if(dwFileLen % MAX_SECTOR_SIZE)
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
		dwBeginTemp += 1;///加一个文件系统扇区

		if(pFileInfo->m_cFileDeleteSign[0] != TDHXKJ_CC_FILEDELETE
			&& pFileInfo->m_cFileDeleteSign[1] != TDHXKJ_CC_FILEDELETE
			&& pFileInfo->m_cFileDeleteSign[2] != TDHXKJ_CC_FILEDELETE
			&& pFileInfo->m_cFileDeleteSign[3] != TDHXKJ_CC_FILEDELETE)
		{
			STRUCT_FILEINFO* pFile = new STRUCT_FILEINFO;
			memcpy(pFile,pFileInfo,sizeof(STRUCT_FILEINFO));
			pFile->m_dwFileBegin = _pdwRealBegin;//原结构体中开始地址在加入磁盘整理后不再准确

			m_vectFileList.push_back(pFile);
		}

		_pdwRealBegin += dwBeginTemp;
		if(!GetAllFileInfo(_pdwRealBegin,_pdwRealBegin))
			return FALSE;
	}
	return TRUE;
}

STDMETHODIMP CCryptoStor::CloseDisk(void)
{
	DelReadInfo();
	if(NULL != m_pCCHelper)
		m_pCCHelper->FreeComm();
	return S_OK;
}

void CCryptoStor::DelReadInfo()
{
	/// 释放读取文件信息
	for (DWORD i = 0 ; i <m_vectFileList.size() ; i ++)
	{
		if(m_vectFileList[i] != NULL)
		{
			delete (m_vectFileList[i]);
			m_vectFileList[i] = NULL;
		}
	}
	m_vectFileList.clear();
}

STDMETHODIMP CCryptoStor::GetFileCount(DWORD* dwFileCount)
{
	// TODO: 在此添加实现代码
	if(NULL == dwFileCount)
		return E_POINTER;
	DWORD dwRealBegin = 0;
	GetAllFileInfo(0,dwRealBegin);
	/// 获得文件个数
	(*dwFileCount) = (DWORD)m_vectFileList.size();
	return S_OK;
}

STDMETHODIMP CCryptoStor::GetDiskStatus(SHORT* psStatus)
{
	// TODO: 在此添加实现代码
	if(NULL == psStatus)
		return E_POINTER;
	DWORD dwRealBegin = 0;

	return S_OK;
}

STDMETHODIMP CCryptoStor::GetFileInfo(LONG dwFileIndex,BSTR* pbstrFileName,DWORD* dwBegin,DWORD* dwFileSize)
{
	// TODO: 在此添加实现代码
	if(NULL == pbstrFileName || NULL == dwBegin || NULL == dwFileSize)
		return E_POINTER;
	if(dwFileIndex < 0 || dwFileIndex >= (signed )m_vectFileList.size())
		return E_INVALIDARG;
	if(NULL == m_pCCHelper)
		return E_POINTER;
	/// 获得文件信息
	(*dwBegin) = m_vectFileList[dwFileIndex]->m_dwFileBegin;
	(*dwFileSize) = m_vectFileList[dwFileIndex]->m_dwFileLength;

	CComBSTR bstrVal(m_vectFileList[dwFileIndex]->m_cFileName);
	bstrVal.CopyTo(pbstrFileName);
	bstrVal.Empty();
	return S_OK;
}

STDMETHODIMP CCryptoStor::ReadOnlyFile(BSTR bstrFileName,BSTR bstrPathFile)
{
	// TODO: 在此添加实现代码
	if(NULL == m_pCCHelper)
		return E_POINTER;
	HRESULT hRet(E_FAIL);
	DWORD dwFileCount = 0;
	GetFileCount(&dwFileCount);
	for(DWORD nIndex = 0;nIndex < dwFileCount;nIndex++)
	{
		CString strFileName(m_vectFileList[nIndex]->m_cFileName);
		if(0 != strFileName.CompareNoCase(bstrFileName))
			continue;
		/// 找到，读取指定文件
		hRet = ReadOutFile(m_vectFileList[nIndex]->m_dwFileBegin,\
			m_vectFileList[nIndex]->m_dwFileLength,bstrFileName,bstrPathFile);
		break;
	}
	return hRet;
}

STDMETHODIMP CCryptoStor::ReadOutFile(DWORD dwBegin,DWORD dwFileLength,BSTR bstrFileName,BSTR bstrPathFile)
{
	// TODO: 在此添加实现代码
	if(NULL == m_pCCHelper)
		return E_POINTER;
	try
	{
		DWORD dwLen = m_pCCHelper->ReadFileOut(dwBegin,dwFileLength,bstrFileName,bstrPathFile);
		if(!dwLen)
			return E_FAIL;
	}
	catch ( ... )
	{
	}
	return S_OK;
}

STDMETHODIMP CCryptoStor::AddWriteFile(BSTR bstrPathFile)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrPathFile)
		return E_POINTER;
	/// 文件不存在或大小为0的文件，拒绝写
	if(!CBaseFuncLib::GetFileSize(bstrPathFile))
		return E_FAIL;
	/// 添加到列表
	m_WriteFiles.push_back(bstrPathFile);
	return S_OK;
}

STDMETHODIMP CCryptoStor::ClearAllFile(void)
{
	// TODO: 在此添加实现代码
	/// 清除所有写的文件
	if(NULL == m_pCCHelper)
		return E_POINTER;
	DWORD dwBeginAddr = 0;
	bool bRet = m_pCCHelper->ClearAllFile(dwBeginAddr);
	if(!bRet)
		return E_FAIL;
	this->DelReadInfo();
	return S_OK;
}

STDMETHODIMP CCryptoStor::WriteInFile(BSTR bstrPathFile,BSTR bstrFileName,VARIANT_BOOL bAddFlag)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrPathFile)
		return E_POINTER;
	if(NULL == m_pCCHelper)
		return E_POINTER;
	/// 写单个文件
	DWORD dwFileSize = CBaseFuncLib::GetFileSize(bstrPathFile);
	if(!dwFileSize)
		return E_FAIL;/// 空文件
	CString strFileName(_T(""));
	if(NULL != bstrFileName)
		strFileName = bstrFileName;
	else
	{
		CString strPathFile(bstrPathFile);
		strFileName = strPathFile;
		int nFind = strPathFile.ReverseFind(_T('\\'));
		if(-1 != nFind)
			strFileName.Delete(0,nFind+1);
	}
	BOOL bExistFlag = FALSE;
	if(VARIANT_FALSE == bAddFlag)
	{
		/// 判断是否存在
		DWORD dwRealBegin = 0;
		if(!m_vectFileList.size())
			GetAllFileInfo(0,dwRealBegin);
		dwRealBegin = 0;
		for(int nIndex = 0;nIndex < (signed)m_vectFileList.size();nIndex++)
		{
			CString strTem(m_vectFileList[nIndex]->m_cFileName);
			if(0 == strFileName.CompareNoCase(strTem))
			{
				dwRealBegin = m_vectFileList[nIndex]->m_dwFileBegin;
				bExistFlag = TRUE;
				break;
			}
		}
		if(TRUE == bExistFlag)
		{
			/// 先删除，再写
			bool bRetFlag = m_pCCHelper->DelFile(dwRealBegin);
			if(bRetFlag)
				this->DelReadInfo();
		}
	}

	DWORD dwBegin = 0;
	DWORD dwRealBegin = 0;
	bool bGetFlag = m_pCCHelper->GetFirstWriteAddr(dwBegin,dwRealBegin);
	if(!bGetFlag)
		dwRealBegin = 0;
	DWORD dwNextBegin = m_pCCHelper->WriteFileIn(dwRealBegin,bstrPathFile,strFileName);
	if(dwNextBegin == dwRealBegin)
	{
		/// 获取错误信息
		return E_FAIL;/// 写失败
	}
	this->DelReadInfo();
	return S_OK;
}

STDMETHODIMP CCryptoStor::DelFile(BSTR bstrFileName)
{
	// TODO: 在此添加实现代码
	if(NULL == bstrFileName)
		return E_POINTER;
	if(NULL == m_pCCHelper)
		return E_POINTER;
	BOOL bExistFlag = FALSE;
	CString strFileName(bstrFileName);
	DWORD dwRealBegin = 0;
	if(!m_vectFileList.size())
		GetAllFileInfo(0,dwRealBegin);
	dwRealBegin = 0;
	for(int nIndex = 0;nIndex < (signed)m_vectFileList.size();nIndex++)
	{
		CString strTem(m_vectFileList[nIndex]->m_cFileName);
		if(0 == strFileName.CompareNoCase(strTem))
		{
			dwRealBegin = m_vectFileList[nIndex]->m_dwFileBegin;
			bExistFlag = TRUE;
			break;
		}
	}
	if(!bExistFlag)
		return E_FAIL;
	bool bRetFlag = m_pCCHelper->DelFile(dwRealBegin);
	if(bRetFlag)
		this->DelReadInfo();
	return S_OK;
}

STDMETHODIMP CCryptoStor::WriteAllFile(VARIANT_BOOL bBeginFirst)
{
	// TODO: 在此添加实现代码
	if(NULL == m_pCCHelper)
		return E_POINTER;
	DWORD dwSize = (DWORD)m_WriteFiles.size();
	if(!dwSize)
		return E_FAIL;
	/// 从最开始位置覆盖写
	DWORD dwBegin = 0;
	DWORD dwRealBegin = 0;
	if(VARIANT_FALSE == bBeginFirst)
	{
		/// 获取开始写位置
		bool bGetFlag = m_pCCHelper->GetFirstWriteAddr(dwBegin,dwRealBegin);
		if(!bGetFlag)
			dwRealBegin = 0;
	}

	/// 循环写
	for(DWORD dwIndex = 0;dwIndex < dwSize;dwIndex++)
	{
		dwRealBegin = m_pCCHelper->WriteFileIn(dwRealBegin,m_WriteFiles[dwIndex]);
		dwRealBegin++;
	}

	m_WriteFiles.clear();
	this->DelReadInfo();
	return S_OK;
}

STDMETHODIMP CCryptoStor::get_LastErrCode(LONG* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal || NULL == m_pCCHelper)
		return E_POINTER;
	(*pVal) = m_pCCHelper->GetLastErrCode();
	return S_OK;
}

STDMETHODIMP CCryptoStor::get_LastErrInfo(BSTR* pVal)
{
	// TODO: 在此添加实现代码
	if(NULL == pVal)
		return E_POINTER;
	CString strLastErr;
	if(NULL == m_pCCHelper)
		strLastErr = _T("初始化错误");
	else
	{
		strLastErr = m_pCCHelper->GetLastErrInfo();
		if(strLastErr.IsEmpty())
		{
			LONG nErrCode = m_pCCHelper->GetLastErrCode();
			if(nErrCode)
			{
				LPVOID lpMsgBuf = NULL;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,nErrCode,0, // Default language
					(LPTSTR)&lpMsgBuf,0,NULL );
				strLastErr.Format(_T("%s"),lpMsgBuf);
				LocalFree(lpMsgBuf);
				lpMsgBuf = NULL;
			}
		}
	}
	CComBSTR bstrVal(strLastErr);
	bstrVal.CopyTo(pVal);
	bstrVal.Empty();
	strLastErr.Empty();
	return S_OK;
}

STDMETHODIMP CCryptoStor::DiskClean()
{
	// TODO: 在此添加实现代码
	if(NULL == m_pCCHelper)
		return E_POINTER;
	/// 磁盘清理
	m_pCCHelper->DiskClean();
	return S_OK;
}