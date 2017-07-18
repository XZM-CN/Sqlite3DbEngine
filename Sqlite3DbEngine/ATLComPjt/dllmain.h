// dllmain.h : 模块类的声明。

class CATLComPjtModule : public ATL::CAtlDllModuleT< CATLComPjtModule >
{
public :
	DECLARE_LIBID(LIBID_ATLComPjtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLCOMPJT, "{E0238D1D-0333-474C-BC46-8F358EE2A077}")
};

extern class CATLComPjtModule _AtlModule;
