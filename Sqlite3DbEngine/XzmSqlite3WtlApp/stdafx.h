// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER			0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE		0x0501
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif


// xzm_@_
//////////////////////////////////////////////////////////////////////////
#include <atlctrls.h >   // for CTreeViewCtrlEx
#include <atlframe.h >   // for COwnerDraw
#include <atlctrlx.h >   // for CCheckListViewCtrlImplTraits
#include <atlcrack.h>    // for REFLECTED_NOTIFY_CODE_HANDLER_EX
// WTL里的CString 坚决不能加，一加就特么出事儿了
//#include <atlmisc.h>
#include <atlddx.h>      // for DDX_CONTROL

#define  _WTL_USE_CSTRING 
#include <atlstr.h> // for error C2039: “CString”: 不是“ATL”的成员
using namespace ATL; // for error C2504: “CRegKey”: 未定义基类

#if _ATL_VER < 0x0700
#undef BEGIN_MSG_MAP
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x)
#endif

#include "HXBaseDef.h"

#ifndef WIN64
#ifdef _DEBUG
#import "..\\bin\\Debug\\Sqlite3DbEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#else
#import "..\\bin\\Release\\Sqlite3DbEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#endif
#else
#ifdef _DEBUG
#import "..\\..\\Release\\X64\\HXEng.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#else
#import "..\\..\\Release\\X64\\HXEng.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#endif
#endif