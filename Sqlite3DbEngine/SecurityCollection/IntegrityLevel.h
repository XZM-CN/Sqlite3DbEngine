#pragma once
#include<iostream>

class CIntegrityLevel
{
public:
	CIntegrityLevel(PTOKEN_MANDATORY_LABEL pIntegrity);

	void Print(std::wostream& os) const;

private:
	PTOKEN_MANDATORY_LABEL m_pIntegrity;
};