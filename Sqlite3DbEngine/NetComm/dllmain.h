// dllmain.h : 模块类的声明。

class CNetCommModule : public ATL::CAtlDllModuleT< CNetCommModule >
{
public :
	DECLARE_LIBID(LIBID_NetCommLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NETCOMM, "{E083C075-354D-495A-8A96-B13B9E45F668}")
};

extern class CNetCommModule _AtlModule;
