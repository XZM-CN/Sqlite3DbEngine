// SoftEncry.h : CSoftEncry ������

#pragma once
#include "resource.h"       // ������



#include "SKFEncry_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CSoftEncry

class ATL_NO_VTABLE CSoftEncry :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CSoftEncry, &CLSID_SoftEncry>,
	public IDispatchImpl<ISoftEncry, &IID_ISoftEncry, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CSoftEncry()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_SOFTENCRY)

	BEGIN_COM_MAP(CSoftEncry)
		COM_INTERFACE_ENTRY(ISoftEncry)
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
	STDMETHOD(AesEcbEnc)(BYTE *szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesEcbDes)(BYTE *szIn,ULONG nInLen,BYTE *szKey,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesCbcEnc)(BYTE *szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** ppBuf);
	STDMETHOD(AesCbcDes)(BYTE *szIn,ULONG nInLen,BYTE *szKey,BYTE *szIV,ULONG* pnOutLen,BYTE** ppBuf);

	STDMETHOD(ReleaseBuf)(BYTE* pBuf);
};

OBJECT_ENTRY_AUTO(__uuidof(SoftEncry), CSoftEncry)
