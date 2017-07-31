#pragma once
#include <iostream>
class CTokenInfo
{
public:
	CTokenInfo(HANDLE hToken);

	void PrintUserInfo() const;
	void PrintGroupsInfo() const;
	void PrintPrivilegeInfo() const;
	void PrintSessionID() const;
	void PrintLogonSid() const;
	void PrintIntegriyLevel() const;
	void PrintVirtualizeEnabled() const;
	void PrintStatistics() const;

private:
	HANDLE m_hToken;// ∑√Œ ¡Ó≈∆µƒæ‰±˙
};