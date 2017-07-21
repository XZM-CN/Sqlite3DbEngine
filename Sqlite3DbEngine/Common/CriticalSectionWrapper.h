#pragma once


class CCriticalSectionWrapper
{
public:
	CCriticalSectionWrapper();
	~CCriticalSectionWrapper();

	void Lock();
	void Unlock();
protected:

	CRITICAL_SECTION m_criticalSection;
};