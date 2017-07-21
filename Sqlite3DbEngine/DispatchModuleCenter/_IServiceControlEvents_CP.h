#pragma once

using namespace ATL;

template <class T>
class CProxy_IServiceControlEvents : public IConnectionPointImpl<T, &__uuidof( _IServiceControlEvents ), CComDynamicUnkArray>
{
	// 警告: 此类可以由向导重新生成
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
