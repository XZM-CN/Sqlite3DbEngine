// TestInterface.cpp : CTestInterface 的实现

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
* @Function 这里包含2个概念 一个是运行程序的账号是管理员账号， 另外一个是当前运行的环境是管理员环境。
*           我们下面的Am_I_In_Admin_Group(TRUE)相当于Windows API IsUserAnAdmin()
*
* @param
*
* @return   如果bCheckAdminMode是TRUE， 则除了检测Admin账号外，还检测是真的运行在Admin环境， 否则只是检测Admin账号。
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
	* @Function 该结构表示的顶级权威的安全标识符（SID）
	*
	* typedef struct _SID_IDENTIFIER_AUTHORITY {
	*     BYTE Value[6];
	* } SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;
	*
	* @Date xzm_@_2017/06/30  13:40:11
	*/
	SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;

	/**
	* @brief    OpenThreadToken（The OpenThreadToken function opens the access token associated with a thread）
	*
	* @Function OpenThreadToken函数打开与线程相关联的访问令牌
	*
	* @param    HANDLE ThreadHandle 线程的句柄，其访问令牌被打开
	*
	* @param    DWORD DesiredAccess 指定一个访问掩码，指定访问令牌的请求类型
	*
	* @param    BOOL OpenAsSelf     如果要针对进程级安全上下文进行访问检查，则为TRUE 。
	*                               FALSE if the access check is to be made against the current security context of the thread calling the OpenThreadToken function.
	*
	* @param    PHANDLE TokenHandle 指向接收新打开的访问令牌的句柄的变量的指针
	*
	* @return   如果函数成功，返回值不为零。如果函数失败，返回值为零。要获取扩展错误信息，请调用 GetLastError。
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
			* @Function 该OpenProcessToken函数打开与进程相关联的访问令牌
			*
			* @param   HANDLE ThreadHandle 访问令牌被打开的进程的句柄。该进程必须具有PROCESS_QUERY_INFORMATION访问权限
			*
			* @param   DWORD DesiredAccess 指定一个访问掩码，指定访问令牌的请求类型。将这些请求的访问类型与令牌的自由访问控制列表（DACL）进行比较，以确定哪些访问被授予或拒绝。
			*                              https://msdn.microsoft.com/en-us/library/windows/desktop/aa374905(v=vs.85).aspx
			*
			* @param   PHANDLE TokenHandle 指向一个句柄的指针，用于在函数返回时识别新打开的访问令牌。
			*
			* @return   BOOL 如果函数成功，返回值不为零。如果函数失败，返回值为零。要获取扩展错误信息，请调用 GetLastError。
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
	* @Function          GetTokenInformation                            函数检索指定类型信息的访问令牌。调用进程必须具有获取信息的适当访问权限
	*
	* @param[  _In_   ]  HANDLE                  TokenHandle            检索信息的访问令牌的句柄
	*
	* @param[  _In_   ]  TOKEN_INFORMATION_CLASS TokenInformationClass  指定来自TOKEN_INFORMATION_CLASS枚举类型的值， 以识别函数检索的信息类型
	*
	* @param[_Out_opt_]  LPVOID                  TokenInformation       指向缓冲区的指针，该函数填充所请求的信息。放入此缓冲区的结构取决于由TokenInformationClass参数指定的信息类型
	*
	* @param[  _In_   ]  DWORD                   TokenInformationLength 指定由TokenInformation参数指向的缓冲区的大小（以字节为单位）。如果TokenInformation为NULL，则此参数必须为零
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
	* @Function          该函数分配和初始化一个SID，一共可以分配初始化8中子授权的SID
	*
	* @param[  _In_   ]  PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority 指向SID_IDENTIFIER_AUTHORITY结构的指针 。此结构提供了在SID中设置的顶级标识符权限值
	*
	* @param[  _In_   ]  BYTE  nSubAuthorityCount 指定要在SID中放置的子权限的数量。此参数还标识了多少次授权参数具有有意义的值。此参数必须包含1到8的值
	*
	* @param[  _In_   ]  DWORD dwSubAuthority0    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority1    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority2    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority3    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority4    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority5    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority6    Sub权限值放在SID中
	*
	* @param[  _In_   ]  DWORD dwSubAuthority7    Sub权限值放在SID中
	*
	* @param[  _Out_  ]  PSID  *pSid              指向接收指向分配和初始化的SID结构的指针的变量的指针
	*
	* @return            如果函数成功，返回值不为零。如果函数失败，返回值为零。要获取扩展错误信息，请调用 GetLastError。
	*
	* @Date 2017/06/30  13:40:11
	*/
	if (!::AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psidAdmin))
		return (FALSE);

	fAdmin = FALSE;

	for (dwGroup = 0; dwGroup < ptg->GroupCount; dwGroup++)
	{
		/**
		* @brief             EqualSid）（The EqualSid function tests two security identifier (SID) values for equality. Two SIDs must match exactly to be considered equal.）
		*
		* @Function          EqualSid功能测试两个安全标识符是否相等（SID）的值。两个SID必须完全匹配以被认为是相等的
		*
		* @param[  _In_   ]  PSID pSid1 指向要比较的第一个SID结构的指针 。这种结构被认为是有效的
		*
		* @param[  _In_   ]  PSID pSid2 指向第二个SID结构进行比较。这种结构被认为是有效的
		*
		* @return            如果 SID结构相等，则返回值不为零。如果SID结构不相等，返回值为零。要获取扩展错误信息，请调用 GetLastError。如果任一SID结构无效，则返回值未定义。
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
* @Function          如何提升权限？注意只有原来是Disable的权限才可以提成Enable， 如果原来就没有这个权限， 是提不上去的。
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
	* @brief             LookupPrivilegeValue（The LookupPrivilegeValue function retrieves the locally unique identifier (LUID) used on a specified system to locally represent the specified privilege name.）
	*
	* @Function          启用权限或禁用所有权限。（Enable the privilege or disable all privileges.）
	*
	* @param[_Out_opt_]  LPCTSTR lpSystemName 指向以null结尾的字符串的指针，该字符串指定检索特权名称的系统的名称。如果指定了空字符串，则该函数将尝试在本地系统上查找特权名称
	*
	* @param[  _In_   ]  LPCTSTR lpName       指向空终止字符串的指针，该字符串指定Winnt.h头文件中定义的特权名称。例如，该参数可以指定常量SE_SECURITY_NAME或其相应的字符串“SeSecurityPrivilege”
	*
	* @param[  _Out_  ]  PLUID   lpLuid       指向一个变量的指针，该变量接收由lpSystemName参数指定的系统上已知权限的LUID 
	*
	* @return            如果函数成功，函数将返回非零值。如果函数失败，则返回零。要获取扩展错误信息，请调用 GetLastError。
	*
	* 启用和禁用C ++中的特权 https://msdn.microsoft.com/en-us/library/windows/desktop/aa446619(v=vs.85).aspx
	*
	* 特权常数               https://msdn.microsoft.com/en-us/library/windows/desktop/bb530716(v=vs.85).aspx
	*
	* @Date 2017/06/30  13:40:11
	*/
	BOOL bRet = ::LookupPrivilegeValue(NULL, lpszPrivilegeName, &tkp.Privileges[0].Luid);
	if (!bRet) return FALSE;

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	/**
	* @brief             AdjustTokenPrivileges（The AdjustTokenPrivileges function enables or disables privileges in the specified access token. Enabling or disabling privileges in an access token requires TOKEN_ADJUST_PRIVILEGES access.）
	*
	* @Function          
	*
	* @param[  _In_   ]  HANDLE            TokenHandle 访问令牌的句柄，其中包含要修改的权限。句柄必须有TOKEN_ADJUST_PRIVILEGES访问令牌。如果PreviousState参数不为NULL，则该句柄还必须具有TOKEN_QUERY访问权限。
	*
	* @param[  _In_   ]  BOOL              DisableAllPrivileges 指定该功能是否禁用所有令牌的权限。如果此值为TRUE，该函数将禁用所有权限，并忽略NewState参数。如果为FALSE，则该函数将根据NewState参数指向的信息修改权限。
	*
	* @param[_Out_opt_]  PTOKEN_PRIVILEGES NewState 指向一个TOKEN_PRIVILEGES结构的指针， 它指定一个特权数组及其属性。如果DisableAllPrivileges参数为FALSE，则 AdjustTokenPrivileges 函数启用，禁用或删除令牌的这些权限。下表描述了基于特权属性的AdjustTokenPrivileges函数采取的操作。
	*
	* @param[  _In_   ]  DWORD             BufferLength 指定由PreviousState参数指向的缓冲区的大小（以字节为单位）。如果PreviousState参数为NULL，则此参数可以为零
	*
	* @param[  _Out_  ]  PTOKEN_PRIVILEGES PreviousState 指向缓冲区的指针，该函数使用包含函数修改的任何特权的先前状态的TOKEN_PRIVILEGES结构填充。也就是说，如果该功能已经修改了特权，则该特权及其先前的状态将包含在由PreviousState引用的TOKEN_PRIVILEGES结构中。如果TOKEN_PRIVILEGES的PrivilegeCount成员为零，则此功能不会更改任何权限。此参数可以为NULL
	*
	* @param[  _Out_  ]  PDWORD            ReturnLength 指向一个变量的指针，该变量接收由LastState参数指向的缓冲区所需的大小（以字节为单位）。此参数可以是NULL如果PreviousState为NULL
	*
	* @return            如果函数成功，返回值不为零。要确定该函数是否调整了所有指定的权限，请调用 GetLastError，该函数在函数成功时返回以下值之一
	*
	* @Date 2017/06/30  13:40:11
	*/
	bRet = ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
	if (ERROR_SUCCESS == bRet) {
		wcout << L"该函数调整所有指定的权限。" << endl << endl;
	}
	else if (ERROR_NOT_ALL_ASSIGNED == bRet) {
		wcout << L"该标记没有在NewState参数中指定的一个或多个特权。即使没有调整任何权限，该功能也可能会成功。该PreviousState参数指示进行调整的权限。" << endl << endl;
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
	* @Function          检索顶级窗口的句柄，其类名和窗口名称与指定的字符串相匹配。此功能不搜索子窗口。此功能不执行区分大小写的搜索
	*
	* @param[_Out_opt_]  LPCTSTR lpClassName  
	*
	* @param[_Out_opt_]  LPCTSTR lpWindowName 窗口名称（窗口的标题）。如果此参数为NULL，则所有窗口名称都匹配
	*
	* @return            如果函数成功，则返回值是具有指定类名和窗口名称的窗口的句柄。如果函数失败，返回值为NULL。要获取扩展错误信息，请调用GetLastError。
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
		* @Function          找出某个窗口的创建者（线程或进程），返回创建者的标志符
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
		* @Function          函数用来打开一个已存在的进程对象，并返回进程的句柄
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
			* @Function          终止指定进程及其所有的线程
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
			std::wcout << L"杀死程序管理器progman.exe，"<< std::endl;
			std::wcout << L"progman.exe是从Windows 3.0延续下来的“程序管理器”，是Windows 3.X的外壳，相当于现在的Explorer.exe。"<< std::endl<< std::endl;
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
	* @param[  _In_   ]  HANDLE               ExistingTokenHandle 主要或假冒令牌的句柄。令牌也可以是限制令牌。句柄必须具有对令牌的TOKEN_DUPLICATE访问权限
	*
	* @param[  _In_   ]  DWORD                Flags指定附加权限选项。该参数可以为零或以下值的组合
	*
	* @param[  _In_   ]  DWORD                DisableSidCount 指定SidsToDisable数组中的条目数
	*
	* @param[ _In_opt_]  PSID_AND_ATTRIBUTES  SidsToDisable 指向SID_AND_ATTRIBUTES结构数组的指针，该 结构在受限制的令牌中指定拒绝唯一的SID。系统使用拒绝唯一SID来拒绝对安全对象的访问。没有拒绝的SID不允许访问。
	*
	* @param[  _In_   ]  DWORD                DeletePrivilegeCount 指定PrivilegesToDelete数组中的条目数
	*
	* @param[ _In_opt_]  PLUID_AND_ATTRIBUTES PrivilegesToDelete 指向LUID_AND_ATTRIBUTES结构数组的指针，该数组 指定在限制令牌中删除的权限
	*
	* @param[  _In_   ]  DWORD                RestrictedSidCount 指定SidsToRestrict数组中的条目数
	*
	* @param[ _In_opt_]  PSID_AND_ATTRIBUTES  SidsToRestrict指向SID_AND_ATTRIBUTES结构数组的指针，该 结构指定新令牌的限制SID的列表。如果现有令牌是受限制的令牌，则新令牌的限制SID列表是该阵列与现有令牌的限制SID列表的交集。不执行检查以删除放置在SidsToRestrict参数上的重复SID 。重复的SID允许限制令牌在限制SID列表中具有冗余信息。
	*
	* @param[  _Out_  ]  PHANDLE              NewTokenHandle 指向接收新限制令牌的句柄的变量的指针。该句柄具有与ExistingTokenHandle相同的访问权限。新的令牌与现有的令牌是相同的类型，主要或模拟。NewTokenHandle中返回的句柄可以重复。
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
	* @param[  _In_opt_   ]  HANDLE                hToken 表示用户的主令牌的句柄。句柄必须具有TOKEN_QUERY，TOKEN_DUPLICATE和TOKEN_ASSIGN_PRIMARY访问权限
	*
	* @param[  _In_opt_   ]  LPCTSTR               lpApplicationName 要执行的模块的名称。该模块可以是基于Windows的应用程序
	*
	* @param[  _Inout_opt_]  LPTSTR                lpCommandLine 要执行的命令行。该字符串的最大长度为32K个字符
	*
	* @param[  _In_opt_   ]  LPSECURITY_ATTRIBUTES lpProcessAttributes 指向SECURITY_ATTRIBUTES结构的指针，该 结构指定新进程对象的安全描述符，并确定子进程是否可以继承返回的进程的句柄
	*
	* @param[  _In_opt_   ]  LPSECURITY_ATTRIBUTES lpThreadAttributes 指向SECURITY_ATTRIBUTES结构的指针，该结构指定新线程对象的安全描述符，并确定子进程是否可以继承线程返回的句柄
	*
	* @param[  _In_       ]  BOOL                  bInheritHandles 如果此参数为TRUE，则调用进程中的每个可继承句柄都由新进程继承。如果参数为FALSE，则不会继承手柄
	*
	* @param[  _In_       ]  DWORD                 dwCreationFlags 控制优先级的标志和进程的创建。有关值的列表，请参阅 过程创建标志
	*
	* @param[  _In_opt_   ]  LPVOID                lpEnvironment 指向新进程的环境块的指针。如果此参数为NULL，则新进程将使用调用进程的环境
	*
	* @param[  _In_opt_   ]  LPCTSTR               lpCurrentDirectory 进程当前目录的完整路径。字符串也可以指定一个UNC路径
	*
	* @param[  _In_       ]  LPSTARTUPINFO         lpStartupInfo 指向STARTUPINFO或STARTUPINFOEX结构的指针 。
	*
	* @param[  _Out_      ]  LPPROCESS_INFORMATION lpProcessInformation指向一个PROCESS_INFORMATION结构的指针， 用于接收关于新进程的标识信息。
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
	std::wcout << L"文件资源管理器Explorer.exe被创建"<< std::endl;

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
			* @brief    imbue函数是指对象引用,表示输出时,使用的区域语言对象
			*
			* @Function 遇到从文本文件里面读取中文，出现乱码的问题时，用此函数即可
			*
			* @param    const locale& _Loc  对象引用,表示输出时,使用的区域语言对象
			*
			* @return   locale   之前的使用的区域语言
			*
			* @Date 2017/06/30  11:20:55
			*/
			std::wcout.imbue(std::locale("chs"));


			CXHandle token;

			/**
			* @brief    OpenProcessToken
			*
			* @Function 该OpenProcessToken函数打开与进程相关联的访问令牌
			*
			* @param   HANDLE ThreadHandle 访问令牌被打开的进程的句柄。该进程必须具有PROCESS_QUERY_INFORMATION访问权限
			*
			* @param   DWORD DesiredAccess 指定一个访问掩码，指定访问令牌的请求类型。将这些请求的访问类型与令牌的自由访问控制列表（DACL）进行比较，以确定哪些访问被授予或拒绝。
			*                              https://msdn.microsoft.com/en-us/library/windows/desktop/aa374905(v=vs.85).aspx
			*
			*
			*
			* @param   PHANDLE TokenHandle 指向一个句柄的指针，用于在函数返回时识别新打开的访问令牌。
			*
			* @return   BOOL 如果函数成功，返回值不为零。如果函数失败，返回值为零。要获取扩展错误信息，请调用 GetLastError。
			*
			* @Date 2017/06/30  11:22:34
			*/
			//////////////////////////////////////////////////////////////////////////
			/**
			* @brief    GetCurrentProcess(Retrieves a pseudo handle for the current process.)
			*
			* @Function 检索当前进程的伪句柄。
			*
			* @param
			*
			* @return   返回值是当前进程的伪句柄。
			*
			* @Date 2017/06/30  11:52:38
			*/
			::OpenProcessToken(GetCurrentProcess(),
				// TOKEN_ADJUST_DEFAULT   | // 需要更改访问令牌的默认所有者，主组或DACL。
				// TOKEN_ADJUST_GROUPS    | // 需要调整访问令牌中组的属性。
				TOKEN_ADJUST_PRIVILEGES   | // 启用或禁用访问令牌中的权限。
				// TOKEN_ADJUST_SESSIONID | // 需要调整访问令牌的会话ID。需要SE_TCB_NAME权限。
				// TOKEN_ASSIGN_PRIMARY   | // 将主令牌附加到进程必需。还需要SE_ASSIGNPRIMARYTOKEN_NAME特权才能完成此任务。
				// TOKEN_DUPLICATE        | // 需要复制访问令牌。
				// TOKEN_EXECUTE          | // 结合STANDARD_RIGHTS_EXECUTE和TOKEN_IMPERSONATE。
				// TOKEN_IMPERSONATE      | // 必须将一个模拟访问令牌附加到进程。
				TOKEN_QUERY             //| // 需要查询访问令牌。
				// TOKEN_QUERY_SOURCE     | // 需要查询访问令牌的来源。
				// TOKEN_READ             | // 组合STANDARD_RIGHTS_READ和TOKEN_QUERY。
				// TOKEN_WRITE            | // 组合STANDARD_RIGHTS_WRITE，TOKEN_ADJUST_PRIVILEGES，TOKEN_ADJUST_GROUPS和TOKEN_ADJUST_DEFAULT。
				// TOKEN_ALL_ACCESS       | // 结合令牌的所有可能的访问权限。
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
				//关机，别打开
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