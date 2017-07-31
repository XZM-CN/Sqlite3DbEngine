#pragma once


#include <AclAPI.h>

#include <iostream>

class CSidInfo;
class CUserInfo;
class CGroupsInfo;
class CPrivilegeInfo;
class CLogonSid;
class CIntegrityLevel;
class CStatisticsInfo;

//std::wostream& wostream(std::wostream, const CSidInfo& sid);
std::wostream& operator << (std::wostream& os, const CSidInfo& sid);
std::wostream& operator << (std::wostream& os, const CUserInfo& user);
std::wostream& operator << (std::wostream& os, const CGroupsInfo& groups);
std::wostream& operator << (std::wostream& os, const CPrivilegeInfo& privileges);
std::wostream& operator << (std::wostream& os, const CLogonSid& groups);
std::wostream& operator << (std::wostream& os, const CIntegrityLevel& integrity);
std::wostream& operator << (std::wostream& os, const LUID& id);
std::wostream& operator << (std::wostream& os, const CStatisticsInfo& pSt);