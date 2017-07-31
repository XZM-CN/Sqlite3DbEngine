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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenUser 缓冲区接收 到包含令牌的用户帐户的TOKEN_USER结构
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenGroups 缓冲区接收到包含与令牌相关联的组帐户的 TOKEN_GROUPS结构
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenPrivileges 缓冲区接收 到包含令牌的特权的TOKEN_PRIVILEGES结构
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenSessionId 缓冲区接收一个DWORD值，指示与令牌相关联的终端服务会话标识符
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
	* @Function 检索与指定进程关联的远程桌面服务会话
	*
	* @param  DWORD dwProcessId 指定进程标识符。使用 GetCurrentProcessId函数来检索当前进程的进程标识符
	*
	* @param  DWORD *pSessionId 指向接收指定进程正在运行的远程桌面服务会话的标识符的变量的指针。
	*                           要检索当前连接到控制台的会话的标识符，请使用WTSGetActiveConsoleSessionId 函数
	*
	* @return
	*
	* @Date 2017/06/30  11:45:02
	*/
	//////////////////////////////////////////////////////////////////////////
	/**
	* @brief    GetCurrentProcessId
	*
	* @Function 得到当前进程的ID
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenLogonSid 缓冲区接收一个指定令牌的登录SID 的TOKEN_GROUPS结构
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenIntegrityLevel 缓冲区接收一个TOKEN_MANDATORY_LABEL结构，指定该令牌的完整性级别
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenVirtualizationEnabled 如果为令牌启用虚拟化，缓冲区将接收非零值的DWORD值
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
	// TOKEN_INFORMATION_CLASS的意义
	//
	// TokenStatistics 缓冲区接收包含各种令牌统计信息的 TOKEN_STATISTICS结构
	//
	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa379626(v=vs.85).aspx
	//
	//////////////////////////////////////////////////////////////////////////
	GetTokenInformation(m_hToken, TokenStatistics, label);

	CStatisticsInfo info(label);
	std::wcout << info << endl << endl;
}