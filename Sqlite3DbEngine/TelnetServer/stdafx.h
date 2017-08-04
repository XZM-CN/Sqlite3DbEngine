// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <atlbase.h>

/// 定义使用ATL的CString
#define _CSTRING_NS	ATL
//#define __ATLDLGS_H__


#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>


// xzm_@_
//////////////////////////////////////////////////////////////////////////
#include <atlctrls.h >   // for CTreeViewCtrlEx
#include <atlframe.h >   // for COwnerDraw
#include <atlctrlx.h >   // for CCheckListViewCtrlImplTraits
#include <atlcrack.h>    // for REFLECTED_NOTIFY_CODE_HANDLER_EX
// WTL里的CString 坚决不能加，一加就特么出事儿了
// #include <atlmisc.h>
#include <atlddx.h>      // for DDX_CONTROL


#include <atlstr.h> // for error C2039: “CString”: 不是“ATL”的成员
using namespace ATL; // for error C2504: “CRegKey”: 未定义基类


#include <winsock.h>
#pragma comment (lib,"ws2_32.lib")


//#include "HXBaseDef.h"
#include <fcntl.h > // for _O_TEXT _open_osfhandle
#include <iostream>
#include <io.h> // for _open_osfhandle
using namespace std;

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


