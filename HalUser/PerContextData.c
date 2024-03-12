#include "PerContextData_internal.h"
#include <Windows.h>

typedef struct _PCD_ENTRY
{
	PFN_DATA_RELEASE pfnDataRelease;
	void* pvData;
} PCD_ENTRY, * PPCD_ENTRY;

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
	std_free(ptEntry);
}

bool _cdecl pcd_CreateEntry(PPCD_ENTRY* pptEntry)
{
	bool bRet = false;
	PPCD_ENTRY ptEntry = NULL;

	ptEntry = std_malloc(sizeof(*ptEntry));
	if (NULL == ptEntry)
	{
		goto lblCleanup;
	}

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
bool _cdecl PCD_Create(HPCD* phPcd)
{
	bool bRet = false;
	DWORD dwTlsIndex = TLS_OUT_OF_INDEXES;

	if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
	{
		goto lblCleanup;
	}

	*phPcd = (HPCD)dwTlsIndex;
	dwTlsIndex = TLS_OUT_OF_INDEXES;
	bRet = true;

lblCleanup:
	if (TLS_OUT_OF_INDEXES != dwTlsIndex)
	{
		TlsFree(dwTlsIndex);
		dwTlsIndex = TLS_OUT_OF_INDEXES;
	}

	return bRet;
}

_Use_decl_annotations_
void _cdecl PCD_Destroy(HPCD hPcd)
{
	DWORD dwTlsIndex = (DWORD)hPcd;

	if (TLS_OUT_OF_INDEXES != dwTlsIndex)
	{
		TlsFree(dwTlsIndex);
		dwTlsIndex = TLS_OUT_OF_INDEXES;
	}
}

_Use_decl_annotations_
bool _cdecl PCD_GetContext(HPCD hPcd, void** ppvData)
{
	void* data = NULL;

	data = TlsGetValue((DWORD)hPcd);
	
	if ((data == NULL) && (GetLastError() != ERROR_SUCCESS)) {
		return false;
	}

	*ppvData = data;

	return true;
}

_Use_decl_annotations_
bool _cdecl PCD_SetContext(HPCD hPcd, void* pvData, PFN_DATA_RELEASE pfnDataRelease)
{
	bool bRes = false;
	PPCD_ENTRY ptEntry = NULL;

	bRes = pcd_CreateEntry(&ptEntry);
	if (!bRes) {
		goto lblCleanup;
	}

	ptEntry->pfnDataRelease = pfnDataRelease;
	ptEntry->pvData = pvData;

	bRes = TlsSetValue((DWORD)hPcd, pvData);
	if (!bRes) {
		goto lblCleanup;
	}

	bRes = true;
	
lblCleanup:
	return bRes;
}

_Use_decl_annotations_
bool _cdecl PCD_ReleaseContext(HPCD hPcd)
{
	PPCD_ENTRY ptEntry = NULL;

	if (!PCD_GetContext(hPcd, &ptEntry))
		return false;

	pcd_ReleaseEntry(ptEntry);

	return true;
}
