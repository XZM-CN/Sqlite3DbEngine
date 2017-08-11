// dllmain.h : 模块类的声明。

class CMySqlDbEngineModule : public ATL::CAtlDllModuleT< CMySqlDbEngineModule >
{
public :
	DECLARE_LIBID(LIBID_MySqlDbEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MYSQLDBENGINE, "{0817E8D1-BFB8-495D-B5B2-A875687E2CC7}")
};

extern class CMySqlDbEngineModule _AtlModule;
