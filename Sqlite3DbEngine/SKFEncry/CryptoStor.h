// CryptoStor.h : CCryptoStor 的声明

#pragma once
#include "resource.h"       // 主符号



#include "SKFEncry_i.h"

#include "CryptoCC.h"

template <class T>
class CProxy_ICryptoStorEvents : public IConnectionPointImpl<T, &__uuidof( _ICryptoStorEvents ), CComDynamicUnkArray>
{
	// 警告: 此类可以由向导重新生成
public:
};



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CCryptoStor

class ATL_NO_VTABLE CCryptoStor :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCryptoStor, &CLSID_CryptoStor>,
	public IConnectionPointContainerImpl<CCryptoStor>,
	public CProxy_ICryptoStorEvents<CCryptoStor>,
	public IDispatchImpl<ICryptoStor, &IID_ICryptoStor, &LIBID_SKFEncryLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// 国芯私密接口
	CCryptoCC*		m_pCCHelper;

	/// 写文件列表
	CSTRING_VECTOR	m_WriteFiles;

	/// 文件列表
	FILEINFO_VECTOR	m_vectFileList;

public:
	CCryptoStor()
		:m_pCCHelper(NULL)
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_CRYPTOSTOR)

	BEGIN_COM_MAP(CCryptoStor)
		COM_INTERFACE_ENTRY(ICryptoStor)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CCryptoStor)
		CONNECTION_POINT_ENTRY(__uuidof(_ICryptoStorEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_pCCHelper = new CCryptoCC();
		if(NULL != m_pCCHelper)
			m_pCCHelper->InitComm(INTERFACEFILE);
		return S_OK;
	}

	void FinalRelease()
	{
		m_WriteFiles.clear();

		DelReadInfo();

		if(NULL != m_pCCHelper)
		{
			m_pCCHelper->FreeComm();
			delete m_pCCHelper;
			m_pCCHelper = NULL;
		}
	}

	void DelReadInfo();

	/// 获取所有文件信息
	BOOL GetAllFileInfo(DWORD _dwBegin,DWORD& _pdwRealBegin);

public:

	STDMETHOD(EnumDisk)(BSTR* pbstrDisks);
	STDMETHOD(PutCurDisk)(BSTR cDisk,DWORD* dwCapcity);
	STDMETHOD(GetCurDisk)(BSTR* pcDisk);

	STDMETHOD(GetFileCount)(DWORD* dwFileCount);
	STDMETHOD(GetFileInfo)(LONG dwFileIndex,BSTR* pbstrFileName,DWORD* dwBegin,DWORD* dwFileSize);
	STDMETHOD(ReadOutFile)(DWORD dwBegin,DWORD dwFileLength,BSTR bstrFileName,BSTR bstrPathFile);
	STDMETHOD(ReadOnlyFile)(BSTR bstrFileName,BSTR bstrPathFile);
	STDMETHOD(WriteInFile)(BSTR bstrPathFile,BSTR bstrFileName,VARIANT_BOOL bAddFlag);
	STDMETHOD(AddWriteFile)(BSTR bstrPathFile);

	STDMETHOD(GetDiskStatus)(SHORT* psStatus);
	STDMETHOD(WriteAllFile)(VARIANT_BOOL bBeginFirst);
	STDMETHOD(DelFile)(BSTR bstrFileName);

	STDMETHOD(ClearAllFile)(void);
	STDMETHOD(DiskClean)(void);
	STDMETHOD(CloseDisk)(void);

	STDMETHOD(get_LastErrCode)(LONG* pVal);
	STDMETHOD(get_LastErrInfo)(BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(CryptoStor), CCryptoStor)
