#include "stdafx.h"
#include "IntegrityLevel.h"
using namespace std;

CIntegrityLevel::CIntegrityLevel(PTOKEN_MANDATORY_LABEL pIntegrity)
	:m_pIntegrity(pIntegrity)
{

}

//
// 如何判断用户的进程完整性级别？
//
// 该信息包含在Integrity  Level的SID里，通过GetTokenInformation， 第二个参数设置成TokenIntegrityLevel，大概代码如下， 详细可以参考后面下载的源代码。
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CIntegrityLevel::Print(std::wostream& os) const
{
	SID* pSid = (SID*)m_pIntegrity->Label.Sid;
	DWORD rid = pSid->SubAuthority[0];

	LPCTSTR lpszIntegrity = L"Unknown";
	switch (rid)
	{
	case SECURITY_MANDATORY_UNTRUSTED_RID:
	{
		lpszIntegrity = L"Untrusted";
		break;
	}
	case SECURITY_MANDATORY_LOW_RID:
	{
		lpszIntegrity = L"Low";
		break;
	}
	case SECURITY_MANDATORY_MEDIUM_RID:
	{
		lpszIntegrity = L"Medium";
		break;
	}

	case SECURITY_MANDATORY_MEDIUM_PLUS_RID:
	{
		lpszIntegrity = L"Medium +";
		break;
	}
	case SECURITY_MANDATORY_HIGH_RID:
	{
		lpszIntegrity = L"High";
		break;
	}
	case SECURITY_MANDATORY_SYSTEM_RID:
	{
		lpszIntegrity = L"System";
		break;
	}
	default:
	{
		lpszIntegrity = L"XXXXX";
	}
	}

	os << L"Integrity: " << lpszIntegrity << endl;
}
