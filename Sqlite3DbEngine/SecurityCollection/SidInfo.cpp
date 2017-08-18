#include "stdafx.h"
#include "SidInfo.h"

#include <Sddl.h> // for ConvertSidToStringSid
using namespace std;

CSidInfo::CSidInfo(PSID pSid)
	:m_pSid(pSid)
{
}

void CSidInfo::Print(std::wostream& os) const
{
	PrintSidString(os);
	PrintSidAccount(os);
}

// 将m_pSid（SID）转化成字符串，m_pSid是以二进制存储的，没法显示
void CSidInfo::PrintSidString(std::wostream& os) const
{
	LPTSTR lpszSid = NULL;
	::ConvertSidToStringSid(m_pSid, &lpszSid);
	os << L"sid: " << lpszSid << endl;
	::LocalFree(lpszSid);
}

// 输出与m_pSid（SID）相关的
// 
// 账户名称
// 域名称
// 账户类型
// 
void CSidInfo::PrintSidAccount(std::wostream& os) const
{
	DWORD dwLen = 256;
	TCHAR szName[256] = { 0 };
	TCHAR szDomain[256] = { 0 };
	SID_NAME_USE eType;

	/**
	 * @brief             LookupAccountSid(The LookupAccountSid function accepts a security identifier (SID) as input.
	 *                                     It retrieves the name of the account for this SID and the name of the first
	  *                                    domain on which this SID is found.)
	 *
	 * @Function          检索此SID的帐户的名称以及找到此SID的第一个域的名称
	 *
	 * @param[ _In_opt_] LPCTSTR lpSystemName
	 *
	 * @param[  _In_   ] PSID lpSid      sid（security identifier安全标识符）值
	 *
	 * @param[_Out_opt_] LPTSTR lpName   账户名称
	 *
	 * @param[ _Inout_ ] LPDWORD cchName 账户长度
	 *
	 * @param[_Out_opt_] LPTSTR lpReferencedDomainName  域名称
	 *
	 * @param[ _Inout_ ] LPDWORD cchReferencedDomainName域长度
	 *
	 * @param[  _Out_  ] PSID_NAME_USE peUse 账户类型
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	::LookupAccountSid(NULL, m_pSid, szName, &dwLen, szDomain, &dwLen, &eType);

	LPWSTR lpszType = L"NULL";
	switch (eType)
	{
	case SidTypeUser:
		// 用户SID
		lpszType = L"SidTypeUser";
		break;

	case SidTypeGroup:
		// 组SID
		lpszType = L"SidTypeGroup";
		break;

	case SidTypeDomain:
		// 域SID
		lpszType = L"SidTypeDomain";
		break;

	case SidTypeAlias:
		// 别名SID
		lpszType = L"SidTypeAlias";
		break;

	case SidTypeWellKnownGroup:
		// 知名团体的SID
		lpszType = L"SidTypeWellKnownGroup";
		break;

	case SidTypeDeletedAccount:
		// 已删除帐户的SID
		lpszType = L"SidTypeDeletedAccount";
		break;

	case SidTypeInvalid:
		// 无效的SID
		lpszType = L"SidTypeInvalid";
		break;

	case SidTypeUnknown:
		// 未知类型的SID
		lpszType = L"SidTypeUnknown";
		break;

	case SidTypeComputer:
		// 计算机的SID
		lpszType = L"SidTypeComputer";
		break;

	case SidTypeLabel:
		// 强制性完整性标签SID
		lpszType = L"SidTypeLabel";
		break;

	default:
		lpszType = L"xxxx";
	}

	os << L"name: " << szName << endl;
	os << L"domain: " << szDomain << endl;
	os << L"type: " << lpszType << endl;
}