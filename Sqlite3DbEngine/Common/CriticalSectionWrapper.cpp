#include "stdafx.h"
#include "CriticalSectionWrapper.h"


CCriticalSectionWrapper::CCriticalSectionWrapper()
{
	InitializeCriticalSection(&m_criticalSection);
}

CCriticalSectionWrapper::~CCriticalSectionWrapper()
{
	DeleteCriticalSection(&m_criticalSection);
}

void CCriticalSectionWrapper::Lock()
{
	EnterCriticalSection(&m_criticalSection);
}

void CCriticalSectionWrapper::Unlock()
{
	LeaveCriticalSection(&m_criticalSection);
}

