#pragma once


//用一个CRITICAL_SECTION 结构体和需要同步的数据关联起来
typedef struct tagSafePtr
{
	int* pInt;
	CRITICAL_SECTION cs;
}StSafePtr,*PStSafePtr;

CRITICAL_SECTION gSection;

class auto_lock
{
public :
	auto_lock(PCRITICAL_SECTION pcs)
	{
		_pcs=pcs;
		EnterCriticalSection(_pcs);
	}
	~auto_lock()
	{
		LeaveCriticalSection(_pcs);
	}
private:
	PCRITICAL_SECTION _pcs;
};