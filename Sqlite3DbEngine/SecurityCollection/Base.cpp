#include "Base.h"
using namespace std;


#include "SidInfo.h"
#include "UserInfo.h"
//#include "GroupsInfo.h"
#include "PrivilegeInfo.h"
#include "LogonSid.h"
#include "IntegrityLevel.h"
#include "StatisticsInfo.h"

std::wostream& operator << (std::wostream& os, const CSidInfo& sid)
{
	sid.Print(os);
	return os;
}

std::wostream& operator << (std::wostream& os, const CUserInfo& user)
{
	user.Print(os);
	return os;
}

std::wostream& operator << (std::wostream& os, const CGroupsInfo& groups)
{
	groups.Print(os);
	return os;
}

std::wostream& operator << (std::wostream& os, const CPrivilegeInfo& privileges)
{
	privileges.Print(os);
	return os;
}

std::wostream& operator << (std::wostream& os, const CLogonSid& groups)
{
	groups.Print(os);
	return os;
}

std::wostream& operator << (std::wostream& os, const CIntegrityLevel& integrity)
{
	integrity.Print(os);
	return os;
}

std::wostream& operator << (std::wostream& os, const LUID& id)
{
	os << hex << showbase;

	if (id.HighPart != 0)
	{
		os << id.HighPart << " ";
	}

	os << hex << id.LowPart;

	os << oct << noshowbase;

	return os;
}

std::wostream& operator << (std::wostream& os, const CStatisticsInfo& t)
{
	t.Print(os);
	return os;
}

