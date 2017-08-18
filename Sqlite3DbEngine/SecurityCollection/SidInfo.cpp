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

// ��m_pSid��SID��ת�����ַ�����m_pSid���Զ����ƴ洢�ģ�û����ʾ
void CSidInfo::PrintSidString(std::wostream& os) const
{
	LPTSTR lpszSid = NULL;
	::ConvertSidToStringSid(m_pSid, &lpszSid);
	os << L"sid: " << lpszSid << endl;
	::LocalFree(lpszSid);
}

// �����m_pSid��SID����ص�
// 
// �˻�����
// ������
// �˻�����
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
	 * @Function          ������SID���ʻ��������Լ��ҵ���SID�ĵ�һ���������
	 *
	 * @param[ _In_opt_] LPCTSTR lpSystemName
	 *
	 * @param[  _In_   ] PSID lpSid      sid��security identifier��ȫ��ʶ����ֵ
	 *
	 * @param[_Out_opt_] LPTSTR lpName   �˻�����
	 *
	 * @param[ _Inout_ ] LPDWORD cchName �˻�����
	 *
	 * @param[_Out_opt_] LPTSTR lpReferencedDomainName  ������
	 *
	 * @param[ _Inout_ ] LPDWORD cchReferencedDomainName�򳤶�
	 *
	 * @param[  _Out_  ] PSID_NAME_USE peUse �˻�����
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
		// �û�SID
		lpszType = L"SidTypeUser";
		break;

	case SidTypeGroup:
		// ��SID
		lpszType = L"SidTypeGroup";
		break;

	case SidTypeDomain:
		// ��SID
		lpszType = L"SidTypeDomain";
		break;

	case SidTypeAlias:
		// ����SID
		lpszType = L"SidTypeAlias";
		break;

	case SidTypeWellKnownGroup:
		// ֪�������SID
		lpszType = L"SidTypeWellKnownGroup";
		break;

	case SidTypeDeletedAccount:
		// ��ɾ���ʻ���SID
		lpszType = L"SidTypeDeletedAccount";
		break;

	case SidTypeInvalid:
		// ��Ч��SID
		lpszType = L"SidTypeInvalid";
		break;

	case SidTypeUnknown:
		// δ֪���͵�SID
		lpszType = L"SidTypeUnknown";
		break;

	case SidTypeComputer:
		// �������SID
		lpszType = L"SidTypeComputer";
		break;

	case SidTypeLabel:
		// ǿ���������Ա�ǩSID
		lpszType = L"SidTypeLabel";
		break;

	default:
		lpszType = L"xxxx";
	}

	os << L"name: " << szName << endl;
	os << L"domain: " << szDomain << endl;
	os << L"type: " << lpszType << endl;
}