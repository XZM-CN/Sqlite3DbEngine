#include "stdafx.h"
#include "UserInfo.h"

#include "SidInfo.h"

CUserInfo::CUserInfo(PTOKEN_USER pUser)
	:m_pUser(pUser)
{
}

void CUserInfo::Print(std::wostream& os) const
{
	CSidInfo info(m_pUser->User.Sid);
	info.Print(os);
}