// HostService.cpp : WinMain ��ʵ��


#include "stdafx.h"
#include "resource.h"
#include "HostService_i.h"


#include <stdio.h>

class CHostServiceModule : public ATL::CAtlServiceModuleT< CHostServiceModule, IDS_SERVICENAME >
	{
public :
	DECLARE_LIBID(LIBID_HostServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HOSTSERVICE, "{E623D696-680F-4268-9897-B834859E4C47}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��İ�ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķ� null ��ȫ˵������

		return S_OK;
	}
	};

CHostServiceModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

