// dllmain.h : 模块类的声明。

class CEncryEngineModule : public ATL::CAtlDllModuleT< CEncryEngineModule >
{
public :
	DECLARE_LIBID(LIBID_EncryEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ENCRYENGINE, "{589FC864-AEC3-45AC-9D73-A95C06B92DAB}")
};

extern class CEncryEngineModule _AtlModule;
