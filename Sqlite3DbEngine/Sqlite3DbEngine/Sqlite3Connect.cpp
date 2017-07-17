// Sqlite3Connect.cpp : CSqlite3Connect 的实现

#include "stdafx.h"
#include "Sqlite3Connect.h"


// CSqlite3Connect



STDMETHODIMP CSqlite3Connect::Open(BSTR bstrDbFile, BSTR bstrPW, VARIANT_BOOL bCreateFlag)
{
	// TODO: 在此添加实现代码
	ATLASSERT(!m_pSqlite3Wrapper);
	m_pSqlite3Wrapper = new CSqlite3Wrapper();

	CString strReadDB(bstrDbFile);
	BOOL bOpenFlag = m_pSqlite3Wrapper->Open(strReadDB,bstrPW);
	return S_OK;
}
