// dllmain.h : 模块类的声明。

class CSignalToServiceModularModule : public ATL::CAtlDllModuleT< CSignalToServiceModularModule >
{
public :
	DECLARE_LIBID(LIBID_SignalToServiceModularLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIGNALTOSERVICEMODULAR, "{E0EB26D3-F8BF-4790-8DFE-6396231C2905}")
};

extern class CSignalToServiceModularModule _AtlModule;
