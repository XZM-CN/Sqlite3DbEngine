#include "stdafx.h"
#include "GroupsInfo.h"
#include "SidInfo.h"

#include "Base.h"
using namespace std;

CGroupsInfo::CGroupsInfo(PTOKEN_GROUPS pGroups)
	:m_pGroups(pGroups)
{

}

void CGroupsInfo::Print(std::wostream& os) const
{
	DWORD nCount = m_pGroups->GroupCount;

	for (DWORD i = 0; i < nCount; ++i)
	{
		SID_AND_ATTRIBUTES* pItem = &(m_pGroups->Groups)[i];

		PrintOneGroup(os, pItem);

		os << endl;
	}
}

void CGroupsInfo::PrintGroupFlag(std::wostream& os, DWORD dwFalg) const
{
	os << L"flag: ";

	if (dwFalg & SE_GROUP_ENABLED)
	{
		//os << L"SE_GROUP_ENABLED ";
	}

	if (dwFalg & SE_GROUP_ENABLED_BY_DEFAULT)
	{
		//os << L"SE_GROUP_ENABLED_BY_DEFAULT ";
	}

	if (dwFalg & SE_GROUP_INTEGRITY)
	{
		//os << L"SE_GROUP_INTEGRITY ";
		os << L"Integrity";
	}

	if (dwFalg & SE_GROUP_INTEGRITY_ENABLED)
	{
		//os << L"SE_GROUP_INTEGRITY_ENABLED ";
	}
	if (dwFalg & SE_GROUP_LOGON_ID)
	{
		//os << L"SE_GROUP_LOGON_ID ";
	}

	if (dwFalg & SE_GROUP_MANDATORY)
	{
		//os << L"SE_GROUP_MANDATORY ";
		os << L"Mandatory";
	}

	if (dwFalg & SE_GROUP_OWNER)
	{
		//os << L"SE_GROUP_OWNER ";
	}

	if (dwFalg & SE_GROUP_RESOURCE)
	{
		//os << L"SE_GROUP_RESOURCE ";
	}

	if (dwFalg & SE_GROUP_USE_FOR_DENY_ONLY)
	{
		//os << L"SE_GROUP_USE_FOR_DENY_ONLY ";
		os << L"Deny";
	}

	os << endl;
}

void CGroupsInfo::PrintOneGroup(std::wostream& os, SID_AND_ATTRIBUTES* pItem) const
{
	CSidInfo sid(pItem->Sid);
	os << sid;

	PrintGroupFlag(os, pItem->Attributes);
}
