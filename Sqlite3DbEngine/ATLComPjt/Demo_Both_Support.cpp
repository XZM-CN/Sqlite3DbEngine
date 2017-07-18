// Demo_Both_Support.cpp : CDemo_Both_Support µÄÊµÏÖ

#include "stdafx.h"
#include "Demo_Both_Support.h"


// CDemo_Both_Support

STDMETHODIMP CDemo_Both_Support::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_IDemo_Both_Support
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
