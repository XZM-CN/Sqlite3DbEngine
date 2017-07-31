// dllmain.h : 模块类的声明。

class CSignalToDriveceModularModule : public ATL::CAtlDllModuleT< CSignalToDriveceModularModule >
{
public :
	DECLARE_LIBID(LIBID_SignalToDriveceModularLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIGNALTODRIVECEMODULAR, "{C60F0CD2-B23C-41C4-A28E-1F624E083D52}")
};

extern class CSignalToDriveceModularModule _AtlModule;
