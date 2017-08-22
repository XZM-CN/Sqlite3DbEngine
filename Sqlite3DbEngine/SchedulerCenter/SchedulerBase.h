//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/08/21  
// ><����ʱ�� ><  2017��:08��:21��   15ʱ:52��:56��  
// ><�ļ�     ><  SchedulerBase.h  
// ><�ļ�·�� ><  g:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\SchedulerCenter  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  ��������--�������Ļ���
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:08��:21��   15ʱ:52��:56��  
// ><ԭ��     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

/// ����๤��������������
typedef HRESULT ( __stdcall LPGetClassObject)( REFCLSID, REFIID, LPVOID* );


#define MODULARPATH_    _T("DecryptEngine.dll")
#define MODULARPATH_    _T("DispatchModuleCenter.dll")
#define MODULARPATH_    _T(".dll")
#define MODULARPATH_    _T(".dll")
#define MODULARPATH_    _T(".dll")
#define MODULARPATH_MSXML    _T("MsXmlEngine.dll")
#define MODULARPATH_    _T(".dll")

class CSchedulerBase
{
public:
	CSchedulerBase(void);
	CSchedulerBase(CString strModulerName);
	virtual ~CSchedulerBase(void);

public:
	virtual HINSTANCE CreateInstance( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv );
	virtual void      CreateInterface( HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv );
	virtual CComPtr<IDispatch>  GetPtr( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid);
	virtual CString  GetPtrPath( LPCTSTR lpszFilename);
	virtual CString GetModulePath(HANDLE hModule = NULL);
protected:
	CString m_strModulerName;
};

