//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/08/18  
// ><����ʱ�� ><  2017��:08��:18��   09ʱ:57��:12��  
// ><�ļ�     ><  AesLogic.h  
// ><�ļ�·�� ><  G:\ProgramKing\Sqlite3DbEngine\Sqlite3DbEngine\EncryEngine  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  http://blog.csdn.net/charleslei/article/details/48710293
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  
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
// ><�޸����� ><  2017��:08��:18��   09ʱ:57��:12��  
// ><ԭ��     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
// AesLogic.h : CAesLogic ������

#pragma once
#include "resource.h"       // ������



#include "EncryEngine_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
	// ECBģʽ���������뱾ģʽ��Electronic codebook��
	STDMETHOD(AesEcbEnc)(BYTE *szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesEcbDes)(BYTE *szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** ppBuf);

	// CBCģʽ������������ӣ�Cipher-block chaining��
	STDMETHOD(AesCbcEnc)(BYTE *szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesCbcDes)(BYTE *szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** ppBuf);

	// CFBģʽ(���ķ���:Cipher feedback)
	STDMETHOD(AesCfbEnc)();
	STDMETHOD(AesCfbDes)();

	// OFBģʽ�����������Output feedback��
	STDMETHOD(AesOfbEnc)();
	STDMETHOD(AesOfbDes)();

	// 
	STDMETHOD(ReleaseBuf)(BYTE* pBuf);
	STDMETHOD(Test)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(AesLogic), CAesLogic)
