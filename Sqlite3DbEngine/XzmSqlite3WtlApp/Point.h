#pragma once

#include <WTypes.h>

// �������д����Щ����
// CPoint pt;
// GetCursorPos(&pt);
// GetCursorPos����ʵ�������õ�������Ľṹ�壬CPointֻ�����ǹ�����һ��
/*
typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;
*/
//////////////////////////////////////////////////////////////////////////
//
// û�õļһ����
//
class CPoint : public tagPOINT
{
};