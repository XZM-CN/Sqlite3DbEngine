// TestInterface.cpp : CTestInterface ��ʵ��

#include "stdafx.h"
#include "TestInterface.h"

#include <stdio.h>
#include <tchar.h>

#include <Windows.h>


#include <malloc.h>
#include "WellKnownSid.h"
#include "XHandle.h"
#include "TokenInfo.h "

#include <iostream>
using namespace std;

/**
* @brief    Am_I_In_Admin_Group
*
* @Function �������2������ һ�������г�����˺��ǹ���Ա�˺ţ� ����һ���ǵ�ǰ���еĻ����ǹ���Ա������
*           ���������Am_I_In_Admin_Group(TRUE)�൱��Windows API IsUserAnAdmin()
*
* @param
*
* @return   ���bCheckAdminMode��TRUE�� ����˼��Admin�˺��⣬����������������Admin������ ����ֻ�Ǽ��Admin�˺š�
*
* @Date 2017/06/30  13:27:01
*/
BOOL CTestInterface::Am_I_In_Admin_Group(BOOL bCheckAdminMode /*= FALSE*/)
{
	BOOL   fAdmin;
	HANDLE  hThread;
	TOKEN_GROUPS *ptg = NULL;
	DWORD  cbTokenGroups;
	DWORD  dwGroup;
	PSID   psidAdmin;

	/**
	* @brief    The structure represents the top-level authority of a security identifier (SID).
	*
	* @Function �ýṹ��ʾ�Ķ���Ȩ���İ�ȫ��ʶ����SID��
	*
	* typedef struct _SID_IDENTIFIER_AUTHORITY {
	*     BYTE Value[6];
	* } SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;
	*
	* @Date xzm_@_2017/06/30  13:40:11
	*/
	SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;

	/**
	* @brief    OpenThreadToken��The OpenThreadToken function opens the access token associated with a thread��
	*
	* @Function OpenThreadToken���������߳�������ķ�������
	*
	* @param    HANDLE ThreadHandle �̵߳ľ������������Ʊ���
	*
	* @param    DWORD DesiredAccess ָ��һ���������룬ָ���������Ƶ���������
	*
	* @param    BOOL OpenAsSelf     ���Ҫ��Խ��̼���ȫ�����Ľ��з��ʼ�飬��ΪTRUE ��
	*                               FALSE if the access check is to be made against the current security context of the thread calling the OpenThreadToken function.
	*
	* @param    PHANDLE TokenHandle ָ������´򿪵ķ������Ƶľ���ı�����ָ��
	*
	* @return   ��������ɹ�������ֵ��Ϊ�㡣�������ʧ�ܣ�����ֵΪ�㡣Ҫ��ȡ��չ������Ϣ������� GetLastError��
	*
	* @Date 2017/06/30  13:31:41
	*/
	if (!::OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &hThread))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			/**
			* @brief    OpenProcessToken
			*
			* @Function ��OpenProcessToken�����������������ķ�������
			*
			* @param   HANDLE ThreadHandle �������Ʊ��򿪵Ľ��̵ľ�����ý��̱������PROCESS_QUERY_INFORMATION����Ȩ��
			*
			* @param   DWORD DesiredAccess ָ��һ���������룬ָ���������Ƶ��������͡�����Щ����ķ������������Ƶ����ɷ��ʿ����б�DACL�����бȽϣ���ȷ����Щ���ʱ������ܾ���
			*                              https://msdn.microsoft.com/en-us/library/windows/desktop/aa374905(v=vs.85).aspx
			*
			* @param   PHANDLE TokenHandle ָ��һ�������ָ�룬�����ں�������ʱʶ���´򿪵ķ������ơ�
			*
			* @return   BOOL ��������ɹ�������ֵ��Ϊ�㡣�������ʧ�ܣ�����ֵΪ�㡣Ҫ��ȡ��չ������Ϣ������� GetLastError��
			*
			* @Date 2017/06/30  11:22:34
			*/
			if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hThread))
				return (FALSE);
		}
		else
			return (FALSE);
	}

	/**
	* @brief             GetTokenInformation(The GetTokenInformation function retrieves a specified type of information about an access token. The calling process must have appropriate access rights to obtain the information.)
	*
	* @Function          GetTokenInformation                            ��������ָ��������Ϣ�ķ������ơ����ý��̱�����л�ȡ��Ϣ���ʵ�����Ȩ��
	*
	* @param[  _In_   ]  HANDLE                  TokenHandle            ������Ϣ�ķ������Ƶľ��
	*
	* @param[  _In_   ]  TOKEN_INFORMATION_CLASS TokenInformationClass  ָ������TOKEN_INFORMATION_CLASSö�����͵�ֵ�� ��ʶ������������Ϣ����
	*
	* @param[_Out_opt_]  LPVOID                  TokenInformation       ָ�򻺳�����ָ�룬�ú���������������Ϣ������˻������Ľṹȡ������TokenInformationClass����ָ������Ϣ����
	*
	* @param[  _In_   ]  DWORD                   TokenInformationLength ָ����TokenInformation����ָ��Ļ������Ĵ�С�����ֽ�Ϊ��λ�������TokenInformationΪNULL����˲�������Ϊ��
	*
	* @param[  _Out_  ]  ReturnLength
	*
	* @return
	*
	* @Date 2017/06/30  13:40:11
	*/
	if (::GetTokenInformation(hThread, TokenGroups, NULL, 0, &cbTokenGroups))
		return (FALSE);


	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		return (FALSE);


	if (!(ptg = (TOKEN_GROUPS*)_alloca(cbTokenGroups)))
		return (FALSE);


	if (!::GetTokenInformation(hThread, TokenGroups, ptg, cbTokenGroups, &cbTokenGroups))
		return (FALSE);

	/**
	* @brief             AllocateAndInitializeSid(The AllocateAndInitializeSid function allocates and initializes a security identifier (SID) with up to eight subauthorities.)
	*
	* @Function          �ú�������ͳ�ʼ��һ��SID��һ�����Է����ʼ��8������Ȩ��SID
	*
	* @param[  _In_   ]  PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority ָ��SID_IDENTIFIER_AUTHORITY�ṹ��ָ�� ���˽ṹ�ṩ����SID�����õĶ�����ʶ��Ȩ��ֵ
	*
	* @param[  _In_   ]  BYTE  nSubAuthorityCount ָ��Ҫ��SID�з��õ���Ȩ�޵��������˲�������ʶ�˶��ٴ���Ȩ���������������ֵ���˲����������1��8��ֵ
	*
	* @param[  _In_   ]  DWORD dwSubAuthority0    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority1    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority2    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority3    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority4    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority5    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority6    SubȨ��ֵ����SID��
	*
	* @param[  _In_   ]  DWORD dwSubAuthority7    SubȨ��ֵ����SID��
	*
	* @param[  _Out_  ]  PSID  *pSid              ָ�����ָ�����ͳ�ʼ����SID�ṹ��ָ��ı�����ָ��
	*
	* @return            ��������ɹ�������ֵ��Ϊ�㡣�������ʧ�ܣ�����ֵΪ�㡣Ҫ��ȡ��չ������Ϣ������� GetLastError��
	*
	* @Date 2017/06/30  13:40:11
	*/
	if (!::AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psidAdmin))
		return (FALSE);

	fAdmin = FALSE;

	for (dwGroup = 0; dwGroup < ptg->GroupCount; dwGroup++)
	{
		/**
		* @brief             EqualSid����The EqualSid function tests two security identifier (SID) values for equality. Two SIDs must match exactly to be considered equal.��
		*
		* @Function          EqualSid���ܲ���������ȫ��ʶ���Ƿ���ȣ�SID����ֵ������SID������ȫƥ���Ա���Ϊ����ȵ�
		*
		* @param[  _In_   ]  PSID pSid1 ָ��Ҫ�Ƚϵĵ�һ��SID�ṹ��ָ�� �����ֽṹ����Ϊ����Ч��
		*
		* @param[  _In_   ]  PSID pSid2 ָ��ڶ���SID�ṹ���бȽϡ����ֽṹ����Ϊ����Ч��
		*
		* @return            ��� SID�ṹ��ȣ��򷵻�ֵ��Ϊ�㡣���SID�ṹ����ȣ�����ֵΪ�㡣Ҫ��ȡ��չ������Ϣ������� GetLastError�������һSID�ṹ��Ч���򷵻�ֵδ���塣
		*
		* @Date 2017/06/30  13:40:11
		*/
		if (::EqualSid(ptg->Groups[dwGroup].Sid, psidAdmin))
		{
			if (bCheckAdminMode)
			{
				if ((ptg->Groups[dwGroup].Attributes) & SE_GROUP_ENABLED)
				{
					fAdmin = TRUE;
				}
			}
			else
			{
				fAdmin = TRUE;
			}
			break;
		}
	}

	::FreeSid(psidAdmin);

	return (fAdmin);
}

/**
* @brief             EnablePrivilege
*
* @Function          �������Ȩ�ޣ�ע��ֻ��ԭ����Disable��Ȩ�޲ſ������Enable�� ���ԭ����û�����Ȩ�ޣ� ���᲻��ȥ�ġ�
*
* @param[  _In_   ]  HANDLE hToken
*
* @param[  _In_   ]  LPCTSTR lpszPrivilegeName
*
* @return            
*
* @Date 2017/06/30  13:40:11
*/
BOOL CTestInterface::EnablePrivilege(HANDLE hToken, LPCTSTR lpszPrivilegeName)
{
	TOKEN_PRIVILEGES tkp = { 0 };

	/**
	* @brief             LookupPrivilegeValue��The LookupPrivilegeValue function retrieves the locally unique identifier (LUID) used on a specified system to locally represent the specified privilege name.��
	*
	* @Function          ����Ȩ�޻��������Ȩ�ޡ���Enable the privilege or disable all privileges.��
	*
	* @param[_Out_opt_]  LPCTSTR lpSystemName ָ����null��β���ַ�����ָ�룬���ַ���ָ��������Ȩ���Ƶ�ϵͳ�����ơ����ָ���˿��ַ�������ú����������ڱ���ϵͳ�ϲ�����Ȩ����
	*
	* @param[  _In_   ]  LPCTSTR lpName       ָ�����ֹ�ַ�����ָ�룬���ַ���ָ��Winnt.hͷ�ļ��ж������Ȩ���ơ����磬�ò�������ָ������SE_SECURITY_NAME������Ӧ���ַ�����SeSecurityPrivilege��
	*
	* @param[  _Out_  ]  PLUID   lpLuid       ָ��һ��������ָ�룬�ñ���������lpSystemName����ָ����ϵͳ����֪Ȩ�޵�LUID 
	*
	* @return            ��������ɹ������������ط���ֵ���������ʧ�ܣ��򷵻��㡣Ҫ��ȡ��չ������Ϣ������� GetLastError��
	*
	* ���úͽ���C ++�е���Ȩ https://msdn.microsoft.com/en-us/library/windows/desktop/aa446619(v=vs.85).aspx
	*
	* ��Ȩ����               https://msdn.microsoft.com/en-us/library/windows/desktop/bb530716(v=vs.85).aspx
	*
	* @Date 2017/06/30  13:40:11
	*/
	BOOL bRet = ::LookupPrivilegeValue(NULL, lpszPrivilegeName, &tkp.Privileges[0].Luid);
	if (!bRet) return FALSE;

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	/**
	* @brief             AdjustTokenPrivileges��The AdjustTokenPrivileges function enables or disables privileges in the specified access token. Enabling or disabling privileges in an access token requires TOKEN_ADJUST_PRIVILEGES access.��
	*
	* @Function          
	*
	* @param[  _In_   ]  HANDLE            TokenHandle �������Ƶľ�������а���Ҫ�޸ĵ�Ȩ�ޡ����������TOKEN_ADJUST_PRIVILEGES�������ơ����PreviousState������ΪNULL����þ�����������TOKEN_QUERY����Ȩ�ޡ�
	*
	* @param[  _In_   ]  BOOL              DisableAllPrivileges ָ���ù����Ƿ�����������Ƶ�Ȩ�ޡ������ֵΪTRUE���ú�������������Ȩ�ޣ�������NewState���������ΪFALSE����ú���������NewState����ָ�����Ϣ�޸�Ȩ�ޡ�
	*
	* @param[_Out_opt_]  PTOKEN_PRIVILEGES NewState ָ��һ��TOKEN_PRIVILEGES�ṹ��ָ�룬 ��ָ��һ����Ȩ���鼰�����ԡ����DisableAllPrivileges����ΪFALSE���� AdjustTokenPrivileges �������ã����û�ɾ�����Ƶ���ЩȨ�ޡ��±������˻�����Ȩ���Ե�AdjustTokenPrivileges������ȡ�Ĳ�����
	*
	* @param[  _In_   ]  DWORD             BufferLength ָ����PreviousState����ָ��Ļ������Ĵ�С�����ֽ�Ϊ��λ�������PreviousState����ΪNULL����˲�������Ϊ��
	*
	* @param[  _Out_  ]  PTOKEN_PRIVILEGES PreviousState ָ�򻺳�����ָ�룬�ú���ʹ�ð��������޸ĵ��κ���Ȩ����ǰ״̬��TOKEN_PRIVILEGES�ṹ��䡣Ҳ����˵������ù����Ѿ��޸�����Ȩ�������Ȩ������ǰ��״̬����������PreviousState���õ�TOKEN_PRIVILEGES�ṹ�С����TOKEN_PRIVILEGES��PrivilegeCount��ԱΪ�㣬��˹��ܲ�������κ�Ȩ�ޡ��˲�������ΪNULL
	*
	* @param[  _Out_  ]  PDWORD            ReturnLength ָ��һ��������ָ�룬�ñ���������LastState����ָ��Ļ���������Ĵ�С�����ֽ�Ϊ��λ�����˲���������NULL���PreviousStateΪNULL
	*
	* @return            ��������ɹ�������ֵ��Ϊ�㡣Ҫȷ���ú����Ƿ����������ָ����Ȩ�ޣ������ GetLastError���ú����ں����ɹ�ʱ��������ֵ֮һ
	*
	* @Date 2017/06/30  13:40:11
	*/
	bRet = ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
	if (ERROR_SUCCESS == bRet) {
		wcout << L"�ú�����������ָ����Ȩ�ޡ�" << endl << endl;
	}
	else if (ERROR_NOT_ALL_ASSIGNED == bRet) {
		wcout << L"�ñ��û����NewState������ָ����һ��������Ȩ����ʹû�е����κ�Ȩ�ޣ��ù���Ҳ���ܻ�ɹ�����PreviousState����ָʾ���е�����Ȩ�ޡ�" << endl << endl;
	}

	return bRet;
}

//can not close the computer now
VOID CTestInterface::TestRestrictToken()
{
	/**
	* @brief             FindWindow(Retrieves a handle to the top-level window whose class name and window name match the specified strings.
	*                               This function does not search child windows. This function does not perform a case-sensitive search.
	*                               To search child windows, beginning with a specified child window, use the FindWindowEx function.)
	*
	* @Function          �����������ڵľ�����������ʹ���������ָ�����ַ�����ƥ�䡣�˹��ܲ������Ӵ��ڡ��˹��ܲ�ִ�����ִ�Сд������
	*
	* @param[_Out_opt_]  LPCTSTR lpClassName  
	*
	* @param[_Out_opt_]  LPCTSTR lpWindowName �������ƣ����ڵı��⣩������˲���ΪNULL�������д������ƶ�ƥ��
	*
	* @return            ��������ɹ����򷵻�ֵ�Ǿ���ָ�������ʹ������ƵĴ��ڵľ�����������ʧ�ܣ�����ֵΪNULL��Ҫ��ȡ��չ������Ϣ�������GetLastError��
	*
	* @Date 2017/06/30  13:40:11
	*/
	HWND hWndPragma = ::FindWindow(_T("Progman"), _T("Program Manager"));
	if (hWndPragma != NULL)
	{
		DWORD dwProcessId(0);

		/**
		* @brief             GetWindowThreadProcessId
		*
		* @Function          �ҳ�ĳ�����ڵĴ����ߣ��̻߳���̣������ش����ߵı�־��
		*
		* @param[  _In_   ]  
		*
		* @param[_Out_opt_]  
		*
		* @return            
		*
		* @Date 2017/06/30  13:40:11
		*/
		GetWindowThreadProcessId(hWndPragma, &dwProcessId);

		/**
		* @brief             OpenProcess
		*
		* @Function          ����������һ���Ѵ��ڵĽ��̶��󣬲����ؽ��̵ľ��
		*
		* @param[  _In_   ]  
		*
		* @param[  _In_   ]  
		*
		* @param[  _In_   ]  
		*
		* @return            
		*
		* @Date 2017/06/30  13:40:11
		*/
		CXHandle h = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
		if (h != NULL)
		{
			/**
			* @brief             TerminateProcess
			*
			* @Function          ��ָֹ�����̼������е��߳�
			*
			* @param[  _In_   ]  
			*
			* @param[  [  _In_   ]  
			*
			* @return            
			*
			* @Date 2017/06/30  13:40:11
			*/
			TerminateProcess(h, 0);
			std::wcout << L"ɱ�����������progman.exe��"<< std::endl;
			std::wcout << L"progman.exe�Ǵ�Windows 3.0���������ġ����������������Windows 3.X����ǣ��൱�����ڵ�Explorer.exe��"<< std::endl<< std::endl;
		}
	}

	CXHandle processToken;
	::OpenProcessToken(::GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_QUERY, processToken.ref());


	LUID shutdownPrivilege = { 0 };
	::LookupPrivilegeValue(0, SE_SHUTDOWN_NAME, &shutdownPrivilege);

	LUID_AND_ATTRIBUTES privilegesToDelete[] =
	{
		shutdownPrivilege, 0
	};

	CXHandle restrictedToken;

	/**
	* @brief             CreateRestrictedToken(The CreateRestrictedToken function creates a new access token that is a restricted version of an existing access token. The restricted token can have disabled security identifiers (SIDs), deleted privileges, and a list of restricting SIDs. For more information, see Restricted Tokens.)
	*
	* @Function          
	*
	* @param[  _In_   ]  HANDLE               ExistingTokenHandle ��Ҫ���ð���Ƶľ��������Ҳ�������������ơ����������ж����Ƶ�TOKEN_DUPLICATE����Ȩ��
	*
	* @param[  _In_   ]  DWORD                Flagsָ������Ȩ��ѡ��ò�������Ϊ�������ֵ�����
	*
	* @param[  _In_   ]  DWORD                DisableSidCount ָ��SidsToDisable�����е���Ŀ��
	*
	* @param[ _In_opt_]  PSID_AND_ATTRIBUTES  SidsToDisable ָ��SID_AND_ATTRIBUTES�ṹ�����ָ�룬�� �ṹ�������Ƶ�������ָ���ܾ�Ψһ��SID��ϵͳʹ�þܾ�ΨһSID���ܾ��԰�ȫ����ķ��ʡ�û�оܾ���SID��������ʡ�
	*
	* @param[  _In_   ]  DWORD                DeletePrivilegeCount ָ��PrivilegesToDelete�����е���Ŀ��
	*
	* @param[ _In_opt_]  PLUID_AND_ATTRIBUTES PrivilegesToDelete ָ��LUID_AND_ATTRIBUTES�ṹ�����ָ�룬������ ָ��������������ɾ����Ȩ��
	*
	* @param[  _In_   ]  DWORD                RestrictedSidCount ָ��SidsToRestrict�����е���Ŀ��
	*
	* @param[ _In_opt_]  PSID_AND_ATTRIBUTES  SidsToRestrictָ��SID_AND_ATTRIBUTES�ṹ�����ָ�룬�� �ṹָ�������Ƶ�����SID���б�������������������Ƶ����ƣ��������Ƶ�����SID�б��Ǹ��������������Ƶ�����SID�б�Ľ�������ִ�м����ɾ��������SidsToRestrict�����ϵ��ظ�SID ���ظ���SID������������������SID�б��о���������Ϣ��
	*
	* @param[  _Out_  ]  PHANDLE              NewTokenHandle ָ��������������Ƶľ���ı�����ָ�롣�þ��������ExistingTokenHandle��ͬ�ķ���Ȩ�ޡ��µ����������е���������ͬ�����ͣ���Ҫ��ģ�⡣NewTokenHandle�з��صľ�������ظ���
	*
	*
	* @return            
	*
	* @Date 2017/06/30  13:40:11
	*/
	::CreateRestrictedToken(processToken,
		0, // flags DISABLE_MAX_PRIVILEGE|SANDBOX_INERT|LUA_TOKEN|WRITE_RESTRICTED
		0,
		NULL,
		_countof(privilegesToDelete),
		privilegesToDelete,
		0, // number of SIDs to restrict,
		0, // no SIDs to restrict,
		restrictedToken.ref());

	STARTUPINFO startupInfo = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION processInfo = { 0 };

	/**
	* @brief             CreateProcessAsUser(Creates a new process and its primary thread. The new process runs in the security context of the user represented by the specified token.)
	*
	* @Function          
	*
	* @param[  _In_opt_   ]  HANDLE                hToken ��ʾ�û��������Ƶľ��������������TOKEN_QUERY��TOKEN_DUPLICATE��TOKEN_ASSIGN_PRIMARY����Ȩ��
	*
	* @param[  _In_opt_   ]  LPCTSTR               lpApplicationName Ҫִ�е�ģ������ơ���ģ������ǻ���Windows��Ӧ�ó���
	*
	* @param[  _Inout_opt_]  LPTSTR                lpCommandLine Ҫִ�е������С����ַ�������󳤶�Ϊ32K���ַ�
	*
	* @param[  _In_opt_   ]  LPSECURITY_ATTRIBUTES lpProcessAttributes ָ��SECURITY_ATTRIBUTES�ṹ��ָ�룬�� �ṹָ���½��̶���İ�ȫ����������ȷ���ӽ����Ƿ���Լ̳з��صĽ��̵ľ��
	*
	* @param[  _In_opt_   ]  LPSECURITY_ATTRIBUTES lpThreadAttributes ָ��SECURITY_ATTRIBUTES�ṹ��ָ�룬�ýṹָ�����̶߳���İ�ȫ����������ȷ���ӽ����Ƿ���Լ̳��̷߳��صľ��
	*
	* @param[  _In_       ]  BOOL                  bInheritHandles ����˲���ΪTRUE������ý����е�ÿ���ɼ̳о�������½��̼̳С��������ΪFALSE���򲻻�̳��ֱ�
	*
	* @param[  _In_       ]  DWORD                 dwCreationFlags �������ȼ��ı�־�ͽ��̵Ĵ������й�ֵ���б������ ���̴�����־
	*
	* @param[  _In_opt_   ]  LPVOID                lpEnvironment ָ���½��̵Ļ������ָ�롣����˲���ΪNULL�����½��̽�ʹ�õ��ý��̵Ļ���
	*
	* @param[  _In_opt_   ]  LPCTSTR               lpCurrentDirectory ���̵�ǰĿ¼������·�����ַ���Ҳ����ָ��һ��UNC·��
	*
	* @param[  _In_       ]  LPSTARTUPINFO         lpStartupInfo ָ��STARTUPINFO��STARTUPINFOEX�ṹ��ָ�� ��
	*
	* @param[  _Out_      ]  LPPROCESS_INFORMATION lpProcessInformationָ��һ��PROCESS_INFORMATION�ṹ��ָ�룬 ���ڽ��չ����½��̵ı�ʶ��Ϣ��
	*
	* @return            
	*
	* @Date 2017/06/30  13:40:11
	*/
	::CreateProcessAsUser(restrictedToken,
		L"C:\\Windows\\Explorer.exe",
		0, // cmd line
		0, // process attributes
		0, // thread attributes
		FALSE, // don't inherit handles
		0, // flags
		0, // inherit environment
		0, // inherit current directory
		&startupInfo,
		&processInfo);
	std::wcout << L"�ļ���Դ������Explorer.exe������"<< std::endl;

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}

// CTestInterface



STDMETHODIMP CTestInterface::Test(SecurityCollectionTest eJt)
{
	switch (eJt)
	{
	case e_SC00:
		{
			/**
			* @brief    imbue������ָ��������,��ʾ���ʱ,ʹ�õ��������Զ���
			*
			* @Function �������ı��ļ������ȡ���ģ��������������ʱ���ô˺�������
			*
			* @param    const locale& _Loc  ��������,��ʾ���ʱ,ʹ�õ��������Զ���
			*
			* @return   locale   ֮ǰ��ʹ�õ���������
			*
			* @Date 2017/06/30  11:20:55
			*/
			std::wcout.imbue(std::locale("chs"));


			CXHandle token;

			/**
			* @brief    OpenProcessToken
			*
			* @Function ��OpenProcessToken�����������������ķ�������
			*
			* @param   HANDLE ThreadHandle �������Ʊ��򿪵Ľ��̵ľ�����ý��̱������PROCESS_QUERY_INFORMATION����Ȩ��
			*
			* @param   DWORD DesiredAccess ָ��һ���������룬ָ���������Ƶ��������͡�����Щ����ķ������������Ƶ����ɷ��ʿ����б�DACL�����бȽϣ���ȷ����Щ���ʱ������ܾ���
			*                              https://msdn.microsoft.com/en-us/library/windows/desktop/aa374905(v=vs.85).aspx
			*
			*
			*
			* @param   PHANDLE TokenHandle ָ��һ�������ָ�룬�����ں�������ʱʶ���´򿪵ķ������ơ�
			*
			* @return   BOOL ��������ɹ�������ֵ��Ϊ�㡣�������ʧ�ܣ�����ֵΪ�㡣Ҫ��ȡ��չ������Ϣ������� GetLastError��
			*
			* @Date 2017/06/30  11:22:34
			*/
			//////////////////////////////////////////////////////////////////////////
			/**
			* @brief    GetCurrentProcess(Retrieves a pseudo handle for the current process.)
			*
			* @Function ������ǰ���̵�α�����
			*
			* @param
			*
			* @return   ����ֵ�ǵ�ǰ���̵�α�����
			*
			* @Date 2017/06/30  11:52:38
			*/
			::OpenProcessToken(GetCurrentProcess(),
				// TOKEN_ADJUST_DEFAULT   | // ��Ҫ���ķ������Ƶ�Ĭ�������ߣ������DACL��
				// TOKEN_ADJUST_GROUPS    | // ��Ҫ��������������������ԡ�
				TOKEN_ADJUST_PRIVILEGES   | // ���û���÷��������е�Ȩ�ޡ�
				// TOKEN_ADJUST_SESSIONID | // ��Ҫ�����������ƵĻỰID����ҪSE_TCB_NAMEȨ�ޡ�
				// TOKEN_ASSIGN_PRIMARY   | // �������Ƹ��ӵ����̱��衣����ҪSE_ASSIGNPRIMARYTOKEN_NAME��Ȩ������ɴ�����
				// TOKEN_DUPLICATE        | // ��Ҫ���Ʒ������ơ�
				// TOKEN_EXECUTE          | // ���STANDARD_RIGHTS_EXECUTE��TOKEN_IMPERSONATE��
				// TOKEN_IMPERSONATE      | // ���뽫һ��ģ��������Ƹ��ӵ����̡�
				TOKEN_QUERY             //| // ��Ҫ��ѯ�������ơ�
				// TOKEN_QUERY_SOURCE     | // ��Ҫ��ѯ�������Ƶ���Դ��
				// TOKEN_READ             | // ���STANDARD_RIGHTS_READ��TOKEN_QUERY��
				// TOKEN_WRITE            | // ���STANDARD_RIGHTS_WRITE��TOKEN_ADJUST_PRIVILEGES��TOKEN_ADJUST_GROUPS��TOKEN_ADJUST_DEFAULT��
				// TOKEN_ALL_ACCESS       | // ������Ƶ����п��ܵķ���Ȩ�ޡ�
				,
				token.ref());

			if (token.IsNull())
				return S_FALSE;

			CTokenInfo tokenInfo(token);
			tokenInfo.PrintUserInfo();
			tokenInfo.PrintGroupsInfo();
			tokenInfo.PrintPrivilegeInfo();
			tokenInfo.PrintSessionID();
			tokenInfo.PrintLogonSid();
			tokenInfo.PrintIntegriyLevel();
			tokenInfo.PrintVirtualizeEnabled();
			tokenInfo.PrintStatistics();

			BOOL bAdmin = FALSE;

			bAdmin = Am_I_In_Admin_Group();
			std::wcout << L"Am_I_In_Admin_Group: " << bAdmin << std::endl;

			bAdmin = Am_I_In_Admin_Group(TRUE);
			std::wcout << L"Am_I_In_Admin_Group(Running): " << bAdmin << std::endl << std::endl;

			// ::ShellExecute(0, L"runas", L"C:\\Windows\\Notepad.exe", 0, 0, SW_SHOWNORMAL);

			if(TRUE)
			{
				EnablePrivilege(token, SE_SHUTDOWN_NAME);
				//�ػ������
				//::ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
			}

			if(TRUE)
			{
				TestRestrictToken();
			}
		}
		break;
	case e_SC01:
		break;
	case e_SC02:
		break;
	case e_SC03:
		break;
	case e_SC04:
		break;
	case e_SC05:
		break;
	case e_SC06:
		break;
	case e_SC07:
		break;
	case e_SC08:
		break;
	case e_SC09:
		{
			break;
		}
	case e_SC10:
		{
			break;
		}
	case e_SC11:
		{
			break;
		}
	case e_SC12:
		{
			break;
		}
	case e_SC13:
		{
			break;
		}
	case e_SC14:
		{
			break;
		}
	case e_SC15:
		{
			break;
		}
	case e_SC16:
		{
			break;
		}
	case e_SC17:
		{
			break;
		}
	case e_SC18:
		{
			break;
		}
	case e_SC19:
		{
			break;
		}
	case e_SC20:
		{
			break;
		}
	default:
		break;
	}
	return S_OK;
}