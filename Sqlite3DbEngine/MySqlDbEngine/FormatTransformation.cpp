// FormatTransformation.cpp : CFormatTransformation 的实现

#include "stdafx.h"
#include "FormatTransformation.h"


// CFormatTransformation

#include "comutil.h"
using namespace _com_util;
#include "OLEIDL.h"
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "oledlg.lib")

STDMETHODIMP CFormatTransformation::BSTR2charPtr01(BSTR bstr)
{
	// TODO: 在此添加实现代码
	char *ptr = NULL;
	//ptr = _com_util::ConvertBSTRToString(bstr);
	return S_OK;
}
