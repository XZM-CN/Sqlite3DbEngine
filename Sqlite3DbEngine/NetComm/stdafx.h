// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

// xzm_@_
//////////////////////////////////////////////////////////////////////////
#include <atlstr.h> // for error C2039: “CString”: 不是“ATL”的成员
using namespace ATL; // for error C2504: “CRegKey”: 未定义基类


/// 包含STL库声明
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "HXBaseDef.h"