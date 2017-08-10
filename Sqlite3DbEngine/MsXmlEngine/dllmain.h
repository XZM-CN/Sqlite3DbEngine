// dllmain.h : 模块类的声明。

class CMsXmlEngineModule : public ATL::CAtlDllModuleT< CMsXmlEngineModule >
{
public :
	DECLARE_LIBID(LIBID_MsXmlEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MSXMLENGINE, "{94B51C66-9D08-4648-A6CE-791F2148F9F4}")
};

extern class CMsXmlEngineModule _AtlModule;
