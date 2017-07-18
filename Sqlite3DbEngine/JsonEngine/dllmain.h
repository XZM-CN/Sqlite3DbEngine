// dllmain.h : 模块类的声明。

class CJsonEngineModule : public ATL::CAtlDllModuleT< CJsonEngineModule >
{
public :
	DECLARE_LIBID(LIBID_JsonEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_JSONENGINE, "{8BCD3338-A4F6-4BAA-88F6-3EA14BCFF100}")
};

extern class CJsonEngineModule _AtlModule;
