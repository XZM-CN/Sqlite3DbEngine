// XMsXmlWrapper.cpp : CXMsXmlWrapper 的实现

#include "stdafx.h"
#include "XMsXmlWrapper.h"

#include "BaseFuncLib.h"

// CXMsXmlWrapper



STDMETHODIMP CXMsXmlWrapper::TestAllMsxmlDesignFormat(MsXmlTest eMsXml)
{
	CString strPath = CBaseFuncLib::GetAppConfigPath();
	switch (eMsXml)
	{
	case e_MsXml00:
		{
			strPath = strPath + _T("createXml.xml");
			m_pMsXmlBase->createXml(strPath.GetBuffer());
		}
		break;
	case e_MsXml01:
		{
			strPath = strPath + _T("createXmlEx.xml");
			m_pMsXmlBase->createXmlEx(strPath.GetBuffer());
		}
		break;
	case e_MsXml02:
		{
			strPath = strPath + _T("CreateDemo01.xml");
			m_pMsXmlBase->CreateDemo01(strPath.GetBuffer());
		}
		break;
	case e_MsXml03:
		{
			strPath = strPath + _T("CreateDemo02.xml");
			m_pMsXmlBase->CreateDemo02(strPath.GetBuffer());
		}
		break;
	case e_MsXml04:
		{
			strPath = strPath + _T("CreateDemo03.xml");
			m_pMsXmlBase->CreateDemo03(strPath.GetBuffer());
		}
		break;
	case e_MsXml05:
		{
			strPath = strPath + _T("CreateDemo04.xml");
			m_pMsXmlBase->CreateDemo04(strPath.GetBuffer());
		}
		break;
	case e_MsXml06:
		{
			strPath = strPath + _T("CreateDemo05.xml");
			m_pMsXmlBase->CreateDemo05(strPath.GetBuffer());
		}
		break;
	case e_MsXml07:
		{
			strPath = strPath + _T("CreateDemo06.xml");
			m_pMsXmlBase->CreateDemo06(strPath.GetBuffer());
		}
		break;
	case e_MsXml08:
		{
			strPath = strPath + _T("CreateDemo07.xml");
			m_pMsXmlBase->CreateDemo07(strPath.GetBuffer());
		}
		break;
	case e_MsXml09:
		{
			strPath = strPath + _T("CreateDemo08.xml");
			m_pMsXmlBase->CreateDemo08(strPath.GetBuffer());
		}
		break;
	case e_MsXml10:
		{
			strPath = strPath + _T("CreateDemo09.xml");
			m_pMsXmlBase->CreateDemo09(strPath.GetBuffer());
		}
		break;
	case e_MsXml11:
		{
			strPath = strPath + _T("OpenXml.xml");
			m_pMsXmlBase->OpenXml(strPath.GetBuffer());
		}
		break;
	default:
		break;
	}

	return S_OK;
}


STDMETHODIMP CXMsXmlWrapper::SystemCfg(void)
{
	// TODO: 在此添加实现代码
	CString strPath = CBaseFuncLib::GetAppConfigPath();
	strPath = strPath + _T("SystemCfg.xml");
	m_pMsXmlBase->SystemCfg(strPath.GetBuffer());
	return S_OK;
}


STDMETHODIMP CXMsXmlWrapper::GetSysCfg(BSTR filePath,
	BSTR* ServerMajorIP, LONG* ServerMajorPort,
	BSTR* ServerMinorIP, LONG* ServerMinorPort,
	BSTR* ServerSpareIP, LONG* ServerSparePort)
{
	// TODO: 在此添加实现代码
	CString strKeyName(filePath);
	TCHAR* _ServerMajorIP = NULL; LONG _ServerMajorPort = 0 ;
	TCHAR* _ServerMinorIP = NULL; LONG _ServerMinorPort = 0 ;
	TCHAR* _ServerSpareIP = NULL; LONG _ServerSparePort = 0 ;
	m_pMsXmlBase->OpenSysCfgXml(strKeyName.GetBuffer(),
		_ServerMajorIP, _ServerMajorPort,
		_ServerMinorIP, _ServerMinorPort,
		_ServerSpareIP, _ServerSparePort);
	return S_OK;
}
