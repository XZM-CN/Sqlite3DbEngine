#include "stdafx.h"
#include "PrivilegeInfo.h"
using namespace std;

CPrivilegeInfo::CPrivilegeInfo(PTOKEN_PRIVILEGES pPrivileges)
	: m_pPrivileges(pPrivileges)
{

}

void CPrivilegeInfo::Print(std::wostream& os) const
{
	DWORD nCount = m_pPrivileges->PrivilegeCount;
	for (DWORD i = 0; i < nCount; ++i)
	{
		LUID_AND_ATTRIBUTES* pItem = const_cast<LUID_AND_ATTRIBUTES*>(&(m_pPrivileges->Privileges[i]));

		PrintOnePrivilege(os, pItem);
		os << endl;
	}
}
void CPrivilegeInfo::PrintOnePrivilege(std::wostream& os, LUID_AND_ATTRIBUTES* pItem) const
{
	DWORD dwName(256);
	TCHAR szName[256] = { 0 };
	LookupPrivilegeName(NULL, &pItem->Luid, szName, &dwName);
	os << L"Privilege Name: " << szName << endl;

	DWORD dwFlag(pItem->Attributes);

	os << L"Flag :" << dwFlag << endl;
	if (dwFlag & SE_PRIVILEGE_ENABLED_BY_DEFAULT)
	{
		os << L" Default Enabled";
	}
	else if (dwFlag & SE_PRIVILEGE_ENABLED)
	{
		os << L" Enabled";
	}
	else
	{
		os << L" Disabled";
	}

	os << endl;
}
