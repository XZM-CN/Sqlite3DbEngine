//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/06/07  
// ><����ʱ�� ><  2017��:06��:07��   16ʱ:16��:03��  
// ><�ļ�     ><  WellKnownSid.h  
// ><�ļ�·�� ><  C:\Ѹ������\MySecurityTest\MySecurityTest  
// ><��������><   $  $  
// ><��ǰ�û� ><  xzm  
// ><����     ><  $  $  
// ><����     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  https://msdn.microsoft.com/zh-cn/library/f2c0w594(VS.80).aspx
// ><         ><  3.  
// ><         ><  4.  
// ><Ŀ��     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:06��:07��   16ʱ:16��:03��  
// ><ԭ��     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

struct WellKnownSid : SID
{
public:
	WellKnownSid(BYTE authority,
		DWORD firstSubAuthority,
		DWORD secondSubAuthority = 0)
	{
		::ZeroMemory(this, 
			sizeof (WellKnownSid));

		Revision = SID_REVISION;
		SubAuthorityCount = (0 != secondSubAuthority ? 2 : 1);
		IdentifierAuthority.Value[5] = authority;
		SubAuthority[0] = firstSubAuthority;
		SubAuthority[1] = secondSubAuthority;
	}

	BYTE GetAuthority() const
	{
		return IdentifierAuthority.Value[5];
	}

	DWORD GetFirstSubAuthority() const
	{
		return SubAuthority[0];
	}

	DWORD GetSecondSubAuthority() const
	{
		return SubAuthority[1];
	}

	static WellKnownSid Everyone()
	{
		return WellKnownSid(WorldAuthority,
			SECURITY_WORLD_RID);
	}

	static WellKnownSid Administrators()
	{
		return WellKnownSid(NtAuthority,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS);
	}

	static WellKnownSid Users()
	{
		return WellKnownSid(NtAuthority,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_USERS);
	}

	enum WellKnownAuthorities
	{
		NullAuthority,
		WorldAuthority,
		LocalAuthority,
		CreatorAuthority,
		NonUniqueAuthority,
		NtAuthority,
		MandatoryLabelAuthority = 16
	};

private:
	DWORD m_secondSubAuthority;
};