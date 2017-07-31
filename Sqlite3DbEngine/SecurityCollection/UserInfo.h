#pragma once
#include <iostream>

class CUserInfo
{
public:
	CUserInfo(PTOKEN_USER pUser);

	void Print(std::wostream& os) const;

private:
	PTOKEN_USER m_pUser;
};
