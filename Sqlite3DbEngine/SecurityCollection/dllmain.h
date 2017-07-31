// dllmain.h : 模块类的声明。

class CSecurityCollectionModule : public ATL::CAtlDllModuleT< CSecurityCollectionModule >
{
public :
	DECLARE_LIBID(LIBID_SecurityCollectionLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SECURITYCOLLECTION, "{C365D8AE-C5EA-4028-80E5-C05546EA10C8}")
};

extern class CSecurityCollectionModule _AtlModule;
