#include "PerContextData_internal.h"

_Use_decl_annotations_
context_id_t
_cdecl
pcd_GetContextId(
	VOID
)
{
	return (context_id_t)PsGetCurrentThreadId();
}

_Use_decl_annotations_
context_id_t _cdecl pcd_CreateContextId(void** ppvContext)
{
	PETHREAD ptThread = NULL;

	// Increment handle for prevent reuse the same thread id
	ptThread = PsGetCurrentThread();
	ObReferenceObject(ptThread);


	*ppvContext = (void*)ptThread;
	ptThread = NULL;

	return pcd_GetContextId();
}

_Use_decl_annotations_
void _cdecl pcd_ReleaseContext(void* pvContext)
{
	PETHREAD ptThread = (PETHREAD)pvContext;
	if (NULL == ptThread)
	{
		return;
	}

	ObDereferenceObject(ptThread);
	ptThread = NULL;
}

_Use_decl_annotations_
bool _cdecl pcd_CreateEntry(context_id_t hCid, void* pvContext, PPCD_ENTRY* pptEntry)
{
	bool bRet = false;
	PPCD_ENTRY ptEntry = NULL;

	ptEntry = std_malloc(sizeof(*ptEntry));
	if (NULL == ptEntry)
	{
		goto lblCleanup;
	}

	ptEntry->hCid = hCid;
	ptEntry->pvContext = pvContext;
	ptEntry->pfnDataRelease = NULL;
	ptEntry->pvData = NULL;
	
	*pptEntry = ptEntry;
	ptEntry = NULL;
	bRet = true;

lblCleanup:
	if (NULL != ptEntry)
	{
		pcd_ReleaseEntry(ptEntry);
		ptEntry = NULL;
	}

	return bRet;
}

_Use_decl_annotations_
void _cdecl pcd_ReleaseEntry(PPCD_ENTRY ptEntry)
{
	if (NULL == ptEntry)
	{
		return;
	}
	
	if (NULL != ptEntry->pfnDataRelease)
	{
		ptEntry->pfnDataRelease(ptEntry->pvData);
	}
	pcd_ReleaseContext(ptEntry->pvContext);
	std_free(ptEntry);
	ptEntry = NULL;
}

_Use_decl_annotations_
bool _cdecl pcd_Get(PPCD ptPcd, context_id_t hCid, PPCD_ENTRY* pptEntry)
{
	bool bRet = false;
	bool bFound = false;
	PPCD_ENTRY ptEntry = NULL;

	for (ptEntry = (PPCD_ENTRY)ptPcd->tList.Flink;
		ptEntry != (PPCD_ENTRY)&ptPcd->tList;
		ptEntry = (PPCD_ENTRY)ptEntry->tList.Flink)
	{
		if (ptEntry->hCid == hCid)
		{
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{ 
		goto lblCleanup;
	}

	*pptEntry = ptEntry;
	bRet = true;

lblCleanup:
	
	return bRet;
}

_Use_decl_annotations_
bool _cdecl PCD_Create(HPCD* phPcd)
{
	bool bRet = false;
	PPCD ptPcd = NULL;

	if (NULL == phPcd)
	{
		goto lblCleanup;
	}

	ptPcd = std_malloc(sizeof(*ptPcd));
	if (NULL == ptPcd)
	{
		goto lblCleanup;
	}
	InitializeListHead(&ptPcd->tList);

	bRet = MUTEX_Create(&ptPcd->hMutex);
	if (!bRet)
	{
		goto lblCleanup;
	}

	*phPcd = (HPCD)ptPcd;
	ptPcd = NULL;
	bRet = true;

lblCleanup:
	if (NULL != ptPcd)
	{
		PCD_Destroy((HPCD)ptPcd);
		ptPcd = NULL;
	}

	return bRet;
}

_Use_decl_annotations_
void _cdecl PCD_Destroy(HPCD hPcd)
{
	PPCD_ENTRY ptEntry = NULL;
	PPCD ptPcd = (PPCD)hPcd;
	
	if (NULL == ptPcd)
	{
		return;
	}

	ptEntry = (PPCD_ENTRY)RemoveHeadList(&ptPcd->tList);
	while (NULL != ptEntry)
	{
		pcd_ReleaseEntry(ptEntry);
		ptEntry = NULL;
		ptEntry = (PPCD_ENTRY)RemoveHeadList(&ptPcd->tList);
	}

	if (NULL == ptPcd->hMutex)
	{
		MUTEX_Destroy(ptPcd->hMutex);
		ptPcd->hMutex = NULL;
	}

	std_free(ptPcd);
	ptPcd = NULL;
}

_Use_decl_annotations_
bool _cdecl PCD_GetContext(HPCD hPcd, void** ppvData)
{
	bool bRet = false;
	PPCD ptPcd = (PPCD)hPcd;
	PPCD_ENTRY ptEntry = NULL;
	bool bIsAcquire = false;

	if ((NULL == ptPcd) || 
		(NULL == ppvData))
	{
		goto lblCleanup;
	}

	bRet = MUTEX_Lock(ptPcd->hMutex);
	if (!bRet)
	{
		goto lblCleanup;
	}
	bIsAcquire = true;

	bRet = pcd_Get(ptPcd, pcd_GetContextId(), &ptEntry);
	if (!bRet)
	{
		goto lblCleanup;
	}

	*ppvData = (void*)ptEntry->pvData;
	bRet = true;

lblCleanup:
	if (bIsAcquire)
	{
		(VOID)MUTEX_UnLock(ptPcd->hMutex);
		bIsAcquire = false;
	}

	return bRet;
}

_Use_decl_annotations_
bool _cdecl PCD_SetContext(HPCD hPcd, void* pvData, PFN_DATA_RELEASE pfnDataRelease)
{
	bool bRet = false;
	PPCD ptPcd = (PPCD)hPcd;
	context_id_t hCid = 0;
	void* pvContext = NULL;
	PPCD_ENTRY ptEntry = NULL;
	bool bIsAcquire = false;

	if (NULL == ptPcd)
	{
		goto lblCleanup;
	}

	bRet = MUTEX_Lock(ptPcd->hMutex);
	if (!bRet)
	{
		goto lblCleanup;
	}
	bIsAcquire = true;

	hCid = pcd_GetContextId();
	bRet = pcd_Get(ptPcd, hCid, &ptEntry);
	if (!bRet)
	{
		hCid = pcd_CreateContextId(&pvContext);
		bRet = pcd_CreateEntry(hCid, pvContext, &ptEntry);
		if (!bRet)
		{
			goto lblCleanup;
		}
		pvContext = NULL;
		InsertHeadList(&ptPcd->tList, &(ptEntry->tList));
	}

	ptEntry->pfnDataRelease = pfnDataRelease;
	ptEntry->pvData = pvData;
	bRet = true;

lblCleanup:
	if (NULL != pvContext)
	{
		pcd_ReleaseContext(pvContext);
		pvContext = NULL;
	}

	if (bIsAcquire)
	{
		(VOID)MUTEX_UnLock(ptPcd->hMutex);
		bIsAcquire = false;
	}

	return bRet;
}

_Use_decl_annotations_
bool _cdecl PCD_ReleaseContext(HPCD hPcd)
{
	bool bRet = false;
	PPCD ptPcd = (PPCD)hPcd;
	PPCD_ENTRY ptEntry = NULL;
	bool bIsAcquire = false;

	if (NULL == ptPcd)
	{
		goto lblCleanup;
	}
	
	bRet = MUTEX_Lock(ptPcd->hMutex);
	if (!bRet)
	{
		goto lblCleanup;
	}
	bIsAcquire = true;

	bRet = pcd_Get(ptPcd, pcd_GetContextId(), &ptEntry);
	if (!bRet)
	{
		goto lblCleanup;
	}

	pcd_ReleaseEntry(ptEntry);
	RemoveEntryList(&(ptEntry->tList));

	bRet = true;

lblCleanup:

	if (bIsAcquire)
	{
		(VOID)MUTEX_UnLock(ptPcd->hMutex);
		bIsAcquire = false;
	}

	return bRet;
}
