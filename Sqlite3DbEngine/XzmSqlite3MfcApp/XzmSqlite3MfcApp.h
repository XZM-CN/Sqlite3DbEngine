
// XzmSqlite3MfcApp.h : XzmSqlite3MfcApp Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CXzmSqlite3MfcAppApp:
// �йش����ʵ�֣������ XzmSqlite3MfcApp.cpp
//

class CXzmSqlite3MfcAppApp : public CWinAppEx
{
public:
	CXzmSqlite3MfcAppApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CXzmSqlite3MfcAppApp theApp;
