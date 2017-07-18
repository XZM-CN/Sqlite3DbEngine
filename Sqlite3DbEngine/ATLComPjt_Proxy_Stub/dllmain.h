// dllmain.h : 模块类的声明。

class CATLComPjt_Proxy_StubModule : public ATL::CAtlDllModuleT< CATLComPjt_Proxy_StubModule >
{
public :
	DECLARE_LIBID(LIBID_ATLComPjt_Proxy_StubLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLCOMPJT_PROXY_STUB, "{96826349-386F-4574-8697-2DC7BCCEA11F}")
};

extern class CATLComPjt_Proxy_StubModule _AtlModule;
