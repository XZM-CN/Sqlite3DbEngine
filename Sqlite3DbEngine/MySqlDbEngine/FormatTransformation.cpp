// FormatTransformation.cpp : CFormatTransformation ��ʵ��

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
	// TODO: �ڴ����ʵ�ִ���
	char *ptr = NULL;
	//ptr = _com_util::ConvertBSTRToString(bstr);
	return S_OK;
}
