#pragma once
#include <iostream>


class CGroupsInfo
{
public:
	CGroupsInfo(PTOKEN_GROUPS pGroups);

	void Print(std::wostream& os) const;
	void PrintOneGroup(std::wostream& os, SID_AND_ATTRIBUTES* pItem) const;
	void PrintGroupFlag(std::wostream& os, DWORD dwFalg) const;

private:
	PTOKEN_GROUPS m_pGroups;
};