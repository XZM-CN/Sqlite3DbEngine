#pragma once


#include <AclAPI.h>

#include <iostream>
class CSidInfo
{
public:
	CSidInfo(PSID pSid);

	void Print(std::wostream& os) const;

	void PrintSidString(std::wostream& os) const;
	void PrintSidAccount(std::wostream& os) const;

private:
	PSID m_pSid;
};