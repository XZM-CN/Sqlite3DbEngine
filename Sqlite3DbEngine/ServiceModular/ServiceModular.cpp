// ServiceModular.cpp : WinMain ��ʵ��


#include "stdafx.h"
#include "resource.h"
#include "ServiceModular_i.h"


#include <stdio.h>

class CServiceModularModule : public ATL::CAtlServiceModuleT< CServiceModularModule, IDS_SERVICENAME >
	{
public :
	DECLARE_LIBID(LIBID_ServiceModularLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SERVICEMODULAR, "{3ACD424D-D8F8-4884-A9E3-EDFD991128F9}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��İ�ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� null ��ȫ˵������

		return S_OK;
	}
	};

CServiceModularModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

