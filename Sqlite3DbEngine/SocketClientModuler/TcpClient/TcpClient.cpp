#include <iostream>
#include <windows.h>
#include <PROCESS.H>

using namespace std;
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


unsigned _stdcall thread1(void* ptr)
{
	EnterCriticalSection(&gSection);
	cout<<"thread 1"<<endl;
	Sleep(2000);
	PStSafePtr pSafePtr=(PStSafePtr)ptr;
	LeaveCriticalSection(&gSection);
	if (pSafePtr)
	{
		//�ڻ�ȡ��ͬ��������ǰ��Ҫ�Ƚ����ٽ�������֤ͬʱֻ��һ���̷߳���
		//EnterCriticalSection(&(pSafePtr->cs));
		auto_lock m(&pSafePtr->cs);
		Sleep(10);
		delete pSafePtr->pInt;
		pSafePtr->pInt=0;
		cout<<"delete ptr!"<<endl;
		//���߳������˳��ٽ���
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
		//�ڻ�ȡ��ͬ��������ǰ����Ҫ�����ٽ�������֤ͬһʱ��ֻ��һ���̷߳���
		//EnterCriticalSection(&(pSafePtr->cs));
		// ������ �����ٽ��� 
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
		//����������˳��ٽ���
		//LeaveCriticalSection(&(pSafePtr->cs));
	}
	return 0;
}

int main()
{
	StSafePtr st;
	//��ʼ���ٽ���
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
	//������Դ
	DeleteCriticalSection(&st.cs);
	DeleteCriticalSection(&gSection);
	return 0;
}