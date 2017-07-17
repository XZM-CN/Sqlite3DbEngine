/**
 * @file HXRegKey.cpp
 *
 * Copyright (c) 2016-?,天地和兴科技
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 注册表操作封装实现
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
// HXRegKey.cpp : 注册表封装实现

#include "stdafx.h"
#include "HXRegKey.h"
#include "BaseFuncLib.h"

CHXRegKey::CHXRegKey(const ATL::CString& strMainRegPath,\
		const ATL::CString& strChildName, HKEY hKey,REGSAM samDesired)
{
	m_bOpenFlag = FALSE;
	m_dwLastErr = 0;
	if(!strMainRegPath.IsEmpty())
		NewRegPath(strMainRegPath,strChildName,hKey,samDesired);
}

void CHXRegKey::Close()
{
	if(!m_bOpenFlag)
		return;
	CRegKey::Close();
	m_dwLastErr = 0;
	m_bOpenFlag = FALSE;
}

BOOL CHXRegKey::NewRegPath(const ATL::CString& strMainRegPath,\
		const ATL::CString& strChildName, HKEY hKey,REGSAM samDesired)
{
	Close();

	ATL::CString strRegPath;
	if(!strChildName.IsEmpty())
	{
		if(strMainRegPath.GetLength())
			strRegPath.Format(_T("%s\\%s"),strMainRegPath,strChildName);
		else
			strRegPath = strChildName;
	}
	else
		strRegPath = strMainRegPath;

	m_dwLastErr = Open(hKey,strRegPath,samDesired);
	if(ERROR_SUCCESS != m_dwLastErr)
	{
		if(ERROR_ACCESS_DENIED != m_dwLastErr)
		{
			/// 不存在，自动创建
			m_dwLastErr = Create(hKey,strRegPath,NULL,0,samDesired);
		}
	}
	if(ERROR_SUCCESS == m_dwLastErr)
	{
		m_bOpenFlag = TRUE;
	}
	else
	{
#ifdef APP_LOG_ENABLE
		CString strLogInfo(_T(""));
		strLogInfo.Format(_T("打开注册表%s失败,子节点%s，错误码%ld，权限%ld"),\
			strMainRegPath,strChildName,m_dwLastErr,samDesired);
		WRITELOGTOFILE(strLogInfo);
#endif
	}
	return m_bOpenFlag;
}

BOOL CHXRegKey::GetRegStringVal(const ATL::CString& strKeyName,
								  ATL::CString& strKeyValue)
{
	BOOL bGetFlag = FALSE;
	if(!m_bOpenFlag)
		return bGetFlag;
	DWORD dwSize = 0;
	m_dwLastErr = CRegKey::QueryStringValue(strKeyName,NULL,&dwSize);

	TCHAR *szBuf = new TCHAR[dwSize+1];
	if(NULL == szBuf)
		return bGetFlag;
	memset(szBuf,0,(dwSize+1)*sizeof(TCHAR));
	m_dwLastErr = CRegKey::QueryStringValue(strKeyName,szBuf,&dwSize);
	if(m_dwLastErr != ERROR_SUCCESS)
	{
		if(NULL != szBuf)
		{
			delete []szBuf;
			szBuf = NULL;
		}
		return bGetFlag;
	}
	bGetFlag = TRUE;
	strKeyValue = szBuf;
	if(NULL != szBuf)
	{
		delete []szBuf;
		szBuf = NULL;
	}

	if(-1 != strKeyName.Find(PRODUCT_COMMAN_RUNPATH) || -1 != strKeyName.Find(PRODUCT_COMMAN_INSPATH)
		 || -1 != strKeyName.Find(PRODUCT_COMMAN_DATAPATH))
	{
		///获取路径类型，自动加"\"
		if(!strKeyValue.IsEmpty())
		{
			if(0 != strKeyValue.Right(1).CompareNoCase(_T("\\")))
				strKeyValue+=_T("\\");
		}
		else
			strKeyValue=_T("\\");
	}

	return bGetFlag;
}

BOOL CHXRegKey::SetRegStringVal(const ATL::CString& strKeyName,
								  const ATL::CString& strKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetStringValue(strKeyName,strKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::GetRegMultiStringVal(const ATL::CString& strKeyName,CSTRING_VECTOR& vectKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	ULONG unLen = 0;
	m_dwLastErr = CRegKey::QueryMultiStringValue(strKeyName,NULL,&unLen);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	if(!unLen)
		return TRUE;/// 无数据
	TCHAR* pData = new TCHAR[unLen];
	if(NULL == pData)
		return FALSE;
	memset(pData,0,unLen*sizeof(TCHAR));
	m_dwLastErr = CRegKey::QueryMultiStringValue(strKeyName,pData,&unLen);
	if(m_dwLastErr != ERROR_SUCCESS)
	{
		delete []pData;
		pData = NULL;
		return FALSE;
	}
	CString strData(_T(""));
	ULONG nIndex = 0;
	for(;nIndex < unLen;nIndex++)
	{
		if('\0' == pData[nIndex])
		{
			if(strData.GetLength())
				vectKeyValue.push_back(strData);
			strData.Empty();
			continue;
		}
		strData += pData[nIndex];
	}
	delete []pData;
	pData = NULL;
	return TRUE;
}
	
BOOL CHXRegKey::SetRegMultiStringVal(const ATL::CString& strKeyName,CSTRING_VECTOR& vectKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	if(!vectKeyValue.size())
	{
		DeleteValue(strKeyName);
		return FALSE;
	}
	ULONG nDataLen = 0;
	ULONG nIndex = 0,nSize = (ULONG)vectKeyValue.size();
	for(nIndex = 0;nIndex < nSize;nIndex++)
		nDataLen += (vectKeyValue[nIndex].GetLength()+1);
	nDataLen++;
	TCHAR* pData = new TCHAR[nDataLen];
	if(NULL == pData)
		return FALSE;
	memset(pData,0,(nDataLen)*sizeof(TCHAR));
	TCHAR *pPtr = pData;
	for(nIndex = 0;nIndex < nSize;nIndex++)
	{
		CString strTmp = vectKeyValue[nIndex];
		_tcscpy_s(pPtr, strTmp.GetLength()+1, strTmp);
		pPtr += _tcslen(pPtr)+1;
	}
	*pPtr = 0;
	m_dwLastErr = ::RegSetValueEx (m_hKey, strKeyName, 0, REG_MULTI_SZ, (BYTE *)pData, nDataLen*sizeof(TCHAR));
	delete []pData;
	pData = NULL;
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::GetRegDwordVal(const ATL::CString& strKeyName,
								 DWORD& dwKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::QueryDWORDValue(strKeyName,dwKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::SetRegDwordVal(const ATL::CString& strKeyName,
								 DWORD dwKeyValue)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetDWORDValue(strKeyName,dwKeyValue);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::GetRegBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes)
{
	if(!m_bOpenFlag)
		return FALSE;
	ATLASSERT(ppValue && pnBytes);
	m_dwLastErr = CRegKey::QueryBinaryValue(strKeyName,NULL,pnBytes);
	if(0 == *pnBytes || m_dwLastErr != ERROR_SUCCESS)
	{
		*ppValue = NULL;
		return FALSE;
	}
	ULONG nLen = (*pnBytes)+1;
	*ppValue = new BYTE[nLen];
	memset(*ppValue,0,nLen*sizeof(BYTE));
	m_dwLastErr = CRegKey::QueryBinaryValue(strKeyName,*ppValue,pnBytes);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::SetRegBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetBinaryValue(strKeyName,Value,nBytes);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::GetRegGuidValue(const ATL::CString &strKeyName,GUID &Value)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::QueryGUIDValue(strKeyName,Value)==ERROR_SUCCESS;
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::SetRegGuidValue(const ATL::CString &strKeyName,GUID Value)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::SetGUIDValue(strKeyName,Value);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

BOOL CHXRegKey::DelKeyName(const ATL::CString& strKeyName)
{
	if(!m_bOpenFlag)
		return FALSE;
	m_dwLastErr = CRegKey::DeleteValue(strKeyName);
	if(m_dwLastErr != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}

ATL::CString CHXRegKey::GetAppRunPath()
{
	ATL::CString strAppPath;
	GetRegStringVal(PRODUCT_COMMAN_RUNPATH,strAppPath);
	return strAppPath;
}

BOOL CHXRegKey::SetAppRunPath(const ATL::CString &strAppPath)
{
	if(strAppPath.IsEmpty() || -1 == strAppPath.Find(_T("\\")))
	{
		ATLASSERT(0);
		return FALSE;
	}
	return SetRegStringVal(PRODUCT_COMMAN_RUNPATH,strAppPath);
}