// dllmain.h : 模块类的声明。

class CSKFEncryLibModule : public ATL::CAtlDllModuleT< CSKFEncryLibModule >
{
public :
	DECLARE_LIBID(LIBID_SKFEncryLibLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SKFENCRYLIB, "{8ACFA360-360C-490F-B65B-D614B5746086}")
};

extern class CSKFEncryLibModule _AtlModule;
