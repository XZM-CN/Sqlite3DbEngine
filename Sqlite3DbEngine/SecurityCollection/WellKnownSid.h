//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/06/07  
// ><创建时间 ><  2017年:06月:07日   16时:16分:03秒  
// ><文件     ><  WellKnownSid.h  
// ><文件路径 ><  C:\迅雷下载\MySecurityTest\MySecurityTest  
// ><隶属工程><   $  $  
// ><当前用户 ><  xzm  
// ><作者     ><  $  $  
// ><出处     ><  
// ><         ><  1.  http://www.cppblog.com/weiym/archive/2013/08/25/202751.html?opt=admin
// ><         ><  2.  https://msdn.microsoft.com/zh-cn/library/f2c0w594(VS.80).aspx
// ><         ><  3.  
// ><         ><  4.  
// ><目的     ><  
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:06月:07日   16时:16分:03秒  
// ><原因     ><    
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