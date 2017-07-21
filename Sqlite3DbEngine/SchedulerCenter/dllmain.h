// dllmain.h : 模块类的声明。

class CSchedulerCenterModule : public ATL::CAtlDllModuleT< CSchedulerCenterModule >
{
public :
	DECLARE_LIBID(LIBID_SchedulerCenterLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SCHEDULERCENTER, "{07C4738C-B21A-4A30-92A9-ED4079635AD3}")
};

extern class CSchedulerCenterModule _AtlModule;
