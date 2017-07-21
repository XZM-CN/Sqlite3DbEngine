// dllmain.h : 模块类的声明。

class CDispatchModuleCenterModule : public ATL::CAtlDllModuleT< CDispatchModuleCenterModule >
{
public :
	DECLARE_LIBID(LIBID_DispatchModuleCenterLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DISPATCHMODULECENTER, "{46D50C0F-3D72-4893-80A2-5CABF28C3FD1}")
};

extern class CDispatchModuleCenterModule _AtlModule;
