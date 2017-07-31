#include "stdafx.h"
#include "TokenInfo.h"
#include "GetTokenInfo.h"

#include "Base.h"

#include "SidInfo.h"
#include "UserInfo.h"
//#include "GroupsInfo.h"
#include "PrivilegeInfo.h"
#include "LogonSid.h"
#include "IntegrityLevel.h"
#include "StatisticsInfo.h"


using namespace std;



CTokenInfo::CTokenInfo(HANDLE hToken)
	:m_hToken(hToken)
{
}

void CTokenInfo::PrintUserInfo() const
{
	wcout << L"***** PrintUserInfo *******" << endl;
	wcout << L"*************************************************************************" << endl << endl;

	LocalMemory<PTOKEN_USER> user;

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenUser ���������� ���������Ƶ��û��ʻ���TOKEN_USER�ṹ
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenUser, user);

	CUserInfo info(user);
	std::wcout << info << endl << endl;
}

void CTokenInfo::PrintGroupsInfo() const
{
	wcout << L"***** PrintGroupsInfo *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	LocalMemory<PTOKEN_GROUPS> groups;

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenGroups ���������յ���������������������ʻ��� TOKEN_GROUPS�ṹ
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenGroups, groups);

	CGroupsInfo info(groups);
	std::wcout << info << endl << endl;
}

void CTokenInfo::PrintPrivilegeInfo() const
{
	wcout << L"***** PrintPrivilegeInfo *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	LocalMemory<PTOKEN_PRIVILEGES> privilages;

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenPrivileges ���������� ���������Ƶ���Ȩ��TOKEN_PRIVILEGES�ṹ
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenPrivileges, privilages);

	CPrivilegeInfo info(privilages);
	std::wcout << info << endl << endl;
}

void CTokenInfo::PrintSessionID() const
{
	wcout << L"***** PrintSessionID *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	DWORD dwSessionID(0xFFFFFFFF);
	DWORD dwRetLen(0);

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenSessionId ����������һ��DWORDֵ��ָʾ��������������ն˷���Ự��ʶ��
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	::GetTokenInformation(m_hToken, TokenSessionId, &dwSessionID, sizeof(DWORD), &dwRetLen);
	std::wcout << L"session id: " << dwSessionID << endl;
	std::wcout << endl << endl;

#ifdef DEBGU
	DWORD dwID(0);
	/**
	* @brief    ProcessIdToSessionId (Retrieves the Remote Desktop Services session associated with a specified process.)
	*
	* @Function ������ָ�����̹�����Զ���������Ự
	*
	* @param  DWORD dwProcessId ָ�����̱�ʶ����ʹ�� GetCurrentProcessId������������ǰ���̵Ľ��̱�ʶ��
	*
	* @param  DWORD *pSessionId ָ�����ָ�������������е�Զ���������Ự�ı�ʶ���ı�����ָ�롣
	*                           Ҫ������ǰ���ӵ�����̨�ĻỰ�ı�ʶ������ʹ��WTSGetActiveConsoleSessionId ����
	*
	* @return
	*
	* @Date 2017/06/30  11:45:02
	*/
	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief    GetCurrentProcessId
	*
	* @Function �õ���ǰ���̵�ID
	*
	* @param
	*
	* @return
	*
	* @Date 2017/06/30  11:45:11
	*/
	ProcessIdToSessionId(GetCurrentProcessId(), &dwID);
	assert(dwID == dwSessionID);
#endif
}

void CTokenInfo::PrintLogonSid() const
{
	wcout << L"***** PrintLogonSid *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	LocalMemory<PTOKEN_GROUPS> groups;

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenLogonSid ����������һ��ָ�����Ƶĵ�¼SID ��TOKEN_GROUPS�ṹ
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenLogonSid, groups);

	CLogonSid info(groups);
	std::wcout << info << endl << endl;
}

void CTokenInfo::PrintIntegriyLevel() const
{
	wcout << L"***** PrintIntegriy *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	LocalMemory<PTOKEN_MANDATORY_LABEL> label;

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenIntegrityLevel ����������һ��TOKEN_MANDATORY_LABEL�ṹ��ָ�������Ƶ������Լ���
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenIntegrityLevel, label);

	CIntegrityLevel info(label);
	std::wcout << info << endl << endl;
}

void CTokenInfo::PrintVirtualizeEnabled() const
{
	wcout << L"***** PrintVirtualizeEnabled *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	DWORD bEnabled(0xFFFFFFFF);
	DWORD dwRetLen(0);

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenVirtualizationEnabled ���Ϊ�����������⻯�������������շ���ֵ��DWORDֵ
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	::GetTokenInformation(m_hToken, TokenVirtualizationEnabled, &bEnabled, sizeof(DWORD), &dwRetLen);
	std::wcout << L"Virtualized: " << (bEnabled ? L"Yes" : L"No") << endl;
	std::wcout << endl << endl;
}

void CTokenInfo::PrintStatistics() const
{
	wcout << L"***** PrintStatistics *******" << endl << endl;
	wcout << L"*************************************************************************" << endl << endl;

	LocalMemory<PTOKEN_STATISTICS> label;

	//
	// TOKEN_INFORMATION_CLASS������
	//
	// TokenStatistics ���������հ�����������ͳ����Ϣ�� TOKEN_STATISTICS�ṹ
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenStatistics, label);

	CStatisticsInfo info(label);
	std::wcout << info << endl << endl;
}