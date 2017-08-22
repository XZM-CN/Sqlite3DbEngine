//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/21  
// ><创建时间 ><  2017年:08月:21日   15时:52分:56秒  
// ><文件     ><  SchedulerBase.h  
// ><文件路径 ><  g:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\SchedulerCenter  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  调度中心--调度器的基类
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:08月:21日   15时:52分:56秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

/// 获得类工厂创建函数定义
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

