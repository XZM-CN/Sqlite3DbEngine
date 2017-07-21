// dllmain.h : 模块类的声明。

class CDecryptEngineModule : public ATL::CAtlDllModuleT< CDecryptEngineModule >
{
public :
	DECLARE_LIBID(LIBID_DecryptEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DECRYPTENGINE, "{517F390E-3B65-48B9-87A9-C7C6806BCD71}")
};

extern class CDecryptEngineModule _AtlModule;
