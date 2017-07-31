#pragma once
#include "GroupsInfo.h"

class CLogonSid : public CGroupsInfo
{
public:
	CLogonSid(PTOKEN_GROUPS groups);
};