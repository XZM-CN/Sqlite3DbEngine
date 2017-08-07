// ServiceControl.h : CServiceControl ������

#pragma once
#include "resource.h"       // ������



#include "DispatchModuleCenter_i.h"
#include "common.h"
#include "univpr0lib.h"
#include "ComHelper.h"
#include "CriticalSectionWrapper.h"


using namespace ATL;

template <class T>
class CProxy_IServiceControlEvents : public IConnectionPointImpl<T, &__uuidof( _IServiceControlEvents ), CComDynamicUnkArray>
{
	// ����: �������������������
public:
	HRESULT Fire_NewLoadModuleEvent(ULONG nProcID,ULONG nParentProcID,DATE dtTime,BSTR bstrModulePath)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[4];
				avarParams[3] = nProcID;
				avarParams[3].vt = VT_I2;
				avarParams[2] = nParentProcID;
				avarParams[2].vt = VT_I2;
				avarParams[1] = dtTime;
				avarParams[1].vt = VT_DATE;
				avarParams[0] = bstrModulePath;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 4, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}

	HRESULT Fire_NewInstallFileEvent(ULONG nParentProcID,BSTR bstrModulePath)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1] = nParentProcID;
				avarParams[1].vt = VT_I2;
				avarParams[0] = bstrModulePath;
				avarParams[0].vt = VT_BSTR;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};

typedef std::list	<BYTE*>		MEMORY_LIST;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CServiceControl

class ATL_NO_VTABLE CServiceControl :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CServiceControl, &CLSID_ServiceControl>,
	public IConnectionPointContainerImpl<CServiceControl>,
	public CProxy_IServiceControlEvents<CServiceControl>,
	public IDispatchImpl<IServiceControl, &IID_IServiceControl, &LIBID_DispatchModuleCenterLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
protected:

	/// ��������
	CString			m_strServiceName;
	/// ���һ�β�������
	CString			m_strLastErr;
	/// ���һ�δ�����
	ULONG			m_nLastErrCode;

	/// �Ƿ�Ϊ����
	VARIANT_BOOL	m_bDriverFlag;
	/// �Ƿ�֧�ְ�ȫģʽ����
	VARIANT_BOOL	m_bSafeBoot;

	/// �������
	HANDLE			m_hDriverDevice;

	/// �˺Ź������
	CComPtr <IAccountMgr>	m_spiAccountMgr;

	/// �����ײ���������
	volatile BOOL	m_bReadDriver;

	HANDLE			m_hReadThread;
	HANDLE			m_hsgReadEvent;
	/// �����¼�֪ͨ
	HANDLE			m_hDealMsgThread;
	HANDLE			m_hsgDealMsgEvent;

	CCriticalSectionWrapper m_sGlobalCriticalSection;

	/// ���������������ֹͣ
	BOOL ControlStatus(const CString& strServiceName,BOOL bStartFlag);
	/// ֹͣ����֧����������
	DWORD StopService( SC_HANDLE hSCM, SC_HANDLE hService, 
		BOOL fStopDependencies, DWORD dwTimeout );

	///��������ļ�·��
	CString GetDriverPath();

	/// ������
	HANDLE OpenSafeDriver(const CString& strDriverName,DWORD dwFlag,DWORD dwRight);

	static UINT ReadDriverThread(LPVOID pParam);

	static UINT DealMsgThread(LPVOID pParam);

public:
	CServiceControl()
		:m_strServiceName(_T(""))
		,m_strLastErr(_T(""))
		,m_nLastErrCode(0)
		,m_bDriverFlag(VARIANT_FALSE)
		,m_bSafeBoot(VARIANT_TRUE)
		,m_hDriverDevice(NULL)
		,m_bReadDriver(FALSE)
		,m_hReadThread(NULL)
		,m_hDealMsgThread(NULL)
		,m_spiAccountMgr(NULL)
		,m_hsgDealMsgEvent(NULL)
		,m_hsgReadEvent(NULL)
	{
	}

	MEMORY_LIST	m_MsgList;

DECLARE_REGISTRY_RESOURCEID(IDR_SERVICECONTROL)

BEGIN_COM_MAP(CServiceControl)
	COM_INTERFACE_ENTRY(IServiceControl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CServiceControl)
	CONNECTION_POINT_ENTRY(__uuidof(_IServiceControlEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		m_spiAccountMgr = NULL;
		m_hReadThread = NULL;
		m_hDealMsgThread = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
		m_strServiceName.Empty();
		m_strLastErr.Empty();

		m_sGlobalCriticalSection.Lock();
		MEMORY_LIST::iterator it = m_MsgList.begin();
		while(it != m_MsgList.end())
		{
			BYTE* pBuf = (*it);
			if(NULL != pBuf)
			{
				delete []pBuf;
				pBuf = NULL;
			}
			it++;
		}
		m_MsgList.clear();
		m_sGlobalCriticalSection.Unlock();

		m_hDealMsgThread = NULL;
		m_hReadThread = NULL;
		m_spiAccountMgr = NULL;

		CloseDriver();
	}

	void ReadDriverProc(void);

	void DealMsgProc(void);

public:

	STDMETHOD(GetServiceName)(VARIANT_BOOL* pbDriver, BSTR* pVal);
	STDMETHOD(PutServiceName)(VARIANT_BOOL bDriver, BSTR newVal);
	STDMETHOD(PutAccountMgr)(IDispatch* piAccountMgr);
	STDMETHOD(put_SafeBoot)(VARIANT_BOOL newVal);

	STDMETHOD(Add)(void);
	STDMETHOD(Start)(void);
	STDMETHOD(GetServiceStatus)(SHORT* pnStatus);
	STDMETHOD(Stop)(void);
	STDMETHOD(Del)(void);
	STDMETHOD(IsExist)(VARIANT_BOOL* pbFlag);
	STDMETHOD(OpenDriver)(BSTR bstrDriverName,VARIANT_BOOL bOverlapped);
	STDMETHOD(SendUsbCmdToDriver)(USHORT usCmd);
	STDMETHOD(SendControlToDevice)(ULONG unCmd, BYTE* pData, USHORT uDataLen);
	STDMETHOD(CloseDriver)(void);
	STDMETHOD(StartDriverMonitor)(void);
	STDMETHOD(StopDriverMonitor)(void);

	/**
	 * @brief ������һ�β����ĳ�����Ϣ
	 *
	 *
	 * @param pVal 
	 * @return 
	 */
	STDMETHOD(get_LastErrorInfo)(BSTR* pVal);
	STDMETHOD(get_LastErrorCode)(ULONG* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(ServiceControl), CServiceControl)
