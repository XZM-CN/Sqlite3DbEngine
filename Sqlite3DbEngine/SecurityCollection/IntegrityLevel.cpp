#include "stdafx.h"
#include "IntegrityLevel.h"
using namespace std;

CIntegrityLevel::CIntegrityLevel(PTOKEN_MANDATORY_LABEL pIntegrity)
	:m_pIntegrity(pIntegrity)
{

}

//
// ����ж��û��Ľ��������Լ���
//
// ����Ϣ������Integrity  Level��SID�ͨ��GetTokenInformation�� �ڶ����������ó�TokenIntegrityLevel����Ŵ������£� ��ϸ���Բο��������ص�Դ���롣
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
