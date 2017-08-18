//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/08/18  
// ><创建时间 ><  2017年:08月:18日   09时:57分:12秒  
// ><文件     ><  AesLogic.h  
// ><文件路径 ><  G:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\EncryEngine  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  http://blog.csdn.net/charleslei/article/details/48710293
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  
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
// ><修改日期 ><  2017年:08月:18日   09时:57分:12秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
// AesLogic.h : CAesLogic 的声明

#pragma once
#include "resource.h"       // 主符号



#include "EncryEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CAesLogic

class ATL_NO_VTABLE CAesLogic :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAesLogic, &CLSID_AesLogic>,
	public IDispatchImpl<IAesLogic, &IID_IAesLogic, &LIBID_EncryEngineLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CAesLogic()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SOFTENCRY)

	BEGIN_COM_MAP(CAesLogic)
		COM_INTERFACE_ENTRY(IAesLogic)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// ECB模式（电子密码本模式：Electronic codebook）
	STDMETHOD(AesEcbEnc)(BYTE *szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesEcbDes)(BYTE *szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** ppBuf);

	// CBC模式（密码分组链接：Cipher-block chaining）
	STDMETHOD(AesCbcEnc)(BYTE *szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesCbcDes)(BYTE *szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** ppBuf);

	// CFB模式(密文反馈:Cipher feedback)
	STDMETHOD(AesCfbEnc)();
	STDMETHOD(AesCfbDes)();

	// OFB模式（输出反馈：Output feedback）
	STDMETHOD(AesOfbEnc)();
	STDMETHOD(AesOfbDes)();

	// 
	STDMETHOD(ReleaseBuf)(BYTE* pBuf);
	STDMETHOD(Test)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(AesLogic), CAesLogic)
