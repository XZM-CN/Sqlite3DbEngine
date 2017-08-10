#pragma once


//��һ��CRITICAL_SECTION �ṹ�����Ҫͬ�������ݹ�������
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