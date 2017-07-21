// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

// xzm_@_
//////////////////////////////////////////////////////////////////////////
#include <atlstr.h> // for error C2039: ��CString��: ���ǡ�ATL���ĳ�Ա
using namespace ATL; // for error C2504: ��CRegKey��: δ�������


//
// #import "*.dll"�ڱ��빤��ʱ�����������dll���ͷų�һ��*.tlh�ļ���
// ���tlh�ļ�������ATL��������dll��ͷ�ļ�
//////////////////////////////////////////////////////////////////////////
#ifndef WIN64
#ifdef _DEBUG
#import "..\\bin\\Debug\\SKFEncry.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#import "..\\bin\\Debug\\Sqlite3DbEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#import "..\\bin\\Debug\\JsonEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#import "..\\bin\\Debug\\LogEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
//#import "..\\bin\\Debug\\EncryEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#else
#import "..\\bin\\Release\\Sqlite3DbEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#endif
#else
#ifdef _DEBUG
#import "..\\..\\Release\\X64\\Sqlite3DbEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#else
#import "..\\..\\Release\\X64\\Sqlite3DbEngine.dll" no_namespace, raw_interfaces_only, raw_native_types, named_guids
#endif
#endif

/// ����STL������
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "HXBaseDef.h"
#include "BaseFuncLib.h"