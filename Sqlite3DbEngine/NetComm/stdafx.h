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


/// ����STL������
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "HXBaseDef.h"