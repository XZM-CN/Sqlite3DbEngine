// dllmain.h : 模块类的声明。

class CLogEngineModule : public ATL::CAtlDllModuleT< CLogEngineModule >
{
public :
	DECLARE_LIBID(LIBID_LogEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LOGENGINE, "{C470180B-DDF2-4E0C-A523-DB5714ED7847}")
};

extern class CLogEngineModule _AtlModule;
