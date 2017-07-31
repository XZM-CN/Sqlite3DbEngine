#pragma once
#include <iostream>

class CPrivilegeInfo
{
public:
	CPrivilegeInfo(PTOKEN_PRIVILEGES pPrivileges);

	void Print(std::wostream& os) const;
	void PrintOnePrivilege(std::wostream& os, LUID_AND_ATTRIBUTES* pItem) const;

private:
	PTOKEN_PRIVILEGES m_pPrivileges;
};