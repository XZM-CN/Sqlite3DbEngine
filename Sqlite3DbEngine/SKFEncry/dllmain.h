// dllmain.h : 模块类的声明。

class CSKFEncryModule : public ATL::CAtlDllModuleT< CSKFEncryModule >
{
public :
	DECLARE_LIBID(LIBID_SKFEncryLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SKFENCRY, "{9E29A2A8-DAD1-40E9-A7B0-B1862A7A55BA}")
};

extern class CSKFEncryModule _AtlModule;
