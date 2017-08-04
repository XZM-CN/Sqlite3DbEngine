// HostService.cpp : WinMain 的实现


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
		// TODO : 调用 CoInitializeSecurity 并为服务提供适当的安全设置
		// 建议 - PKT 级别的身份验证、
		// RPC_C_IMP_LEVEL_IDENTIFY 的模拟级别
		// 以及适当的非 null 安全说明符。

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

