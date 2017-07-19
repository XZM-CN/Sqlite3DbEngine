#pragma once

#include <WTypes.h>

// 这个类重写的有些多余
// CPoint pt;
// GetCursorPos(&pt);
// GetCursorPos函数实际上是用的是下面的结构体，CPoint只不过是过度了一下
/*
typedef struct tagPOINT
{
	LONG  x;
	LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;
*/
//////////////////////////////////////////////////////////////////////////
//
// 没用的家伙，哈哈
//
class CPoint : public tagPOINT
{
};