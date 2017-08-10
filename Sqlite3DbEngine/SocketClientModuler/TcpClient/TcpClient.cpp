#include <iostream>
#include <windows.h>
#include <PROCESS.H>

using namespace std;
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


unsigned _stdcall thread1(void* ptr)
{
	EnterCriticalSection(&gSection);
	cout<<"thread 1"<<endl;
	Sleep(2000);
	PStSafePtr pSafePtr=(PStSafePtr)ptr;
	LeaveCriticalSection(&gSection);
	if (pSafePtr)
	{
		//在获取被同步的数据前，要先进入临界区，保证同时只有一个线程访问
		//EnterCriticalSection(&(pSafePtr->cs));
		auto_lock m(&pSafePtr->cs);
		Sleep(10);
		delete pSafePtr->pInt;
		pSafePtr->pInt=0;
		cout<<"delete ptr!"<<endl;
		//多线程任务退出临界区
		//LeaveCriticalSection(&(pSafePtr->cs));
	}
	return 0;
}

unsigned _stdcall thread2(void* ptr)
{
	EnterCriticalSection(&gSection);
	cout<<"therad 2"<<endl;
	Sleep(2000);
	PStSafePtr pSafePtr=(PStSafePtr)ptr;
	LeaveCriticalSection(&gSection);
	if (pSafePtr)
	{
		//在获取被同步的数据前，先要进入临界区，保证同一时刻只有一个线程访问
		//EnterCriticalSection(&(pSafePtr->cs));
		// 利用类 进入临界区 
		auto_lock m(&pSafePtr->cs);
		int* p =pSafePtr->pInt;
		if(p)
		{
			*p=100;
			cout<<*p<<endl;
		}
		else
		{
			cout<<" p is null.."<<endl;
		}
		//任务结束，退出临界区
		//LeaveCriticalSection(&(pSafePtr->cs));
	}
	return 0;
}

int main()
{
	StSafePtr st;
	//初始化临界区
	InitializeCriticalSection(&st.cs);
	InitializeCriticalSection(&gSection);
	st.pInt=new int (10);
	HANDLE h1=(HANDLE)_beginthreadex(0,0,thread1,&st,0,0);
	HANDLE h2=(HANDLE)_beginthreadex(0,0,thread2,&st,0,0);
	::WaitForSingleObject(h1,INFINITE);
	CloseHandle(h1);
	::WaitForSingleObject(h2,INFINITE);
	CloseHandle(h2);
	delete st.pInt;
	//清理资源
	DeleteCriticalSection(&st.cs);
	DeleteCriticalSection(&gSection);
	return 0;
}