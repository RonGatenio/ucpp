#include <windows.h>
#include "Mutex_internal.h"

bool _cdecl MUTEX_Create(HMUTEX* phMutex)
{
	bool bRet = false;
	PMUTEX ptMutex = NULL;

	ptMutex = std_malloc(sizeof(*ptMutex));
	if (NULL == ptMutex)
	{
		goto lblCleanup;
	}

	*phMutex = (HMUTEX)ptMutex;
	ptMutex = NULL;
	bRet = true;

lblCleanup:
	if (NULL != ptMutex)
	{
		MUTEX_Destroy((HMUTEX)ptMutex);
		ptMutex = NULL;
	}

	return bRet;
}

void _cdecl MUTEX_Destroy(HMUTEX hMutex)
{
	if (NULL == hMutex)
	{
		return;
	}

	std_free(hMutex);
}

bool _cdecl MUTEX_Lock(HMUTEX hMutex)
{
	bool bRet = false;
	PMUTEX ptMutex = (PMUTEX)hMutex;

	if (NULL == ptMutex)
	{
		goto lblCleanup;
	}
	
	while(0 != InterlockedCompareExchange(&ptMutex->lSpinLock, 1, 0))
	{}
		
	bRet = true;

lblCleanup:
	return bRet;
}

bool _cdecl MUTEX_UnLock(HMUTEX hMutex)
{
	bool bRet = false;
	PMUTEX ptMutex = (PMUTEX)hMutex;

	if (NULL == ptMutex)
	{
		goto lblCleanup;
	}

	InterlockedExchange(&ptMutex->lSpinLock, 0);

	bRet = true;

lblCleanup:
	return bRet;
}
