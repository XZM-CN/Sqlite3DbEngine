// dllmain.h : 模块类的声明。

class CSqlite3DbEngineModule : public ATL::CAtlDllModuleT< CSqlite3DbEngineModule >
{
public :
	DECLARE_LIBID(LIBID_Sqlite3DbEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SQLITE3DBENGINE, "{2AF40B30-F1B8-42DD-A6A2-777967AC3EEF}")
};

extern class CSqlite3DbEngineModule _AtlModule;
