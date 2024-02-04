#pragma once
#include "PerContextData.h"
#include "Mutex.h"
#include <ntddk.h>
#include <stddef.h>

typedef uintptr_t context_id_t;
typedef struct _PCD_ENTRY
{
	LIST_ENTRY tList;
	context_id_t hCid;
	void* pvContext;
	PFN_DATA_RELEASE pfnDataRelease;
	void* pvData;
} PCD_ENTRY, * PPCD_ENTRY;
static_assert(0 == offsetof(PCD_ENTRY, tList), "");

typedef struct _PCD
{
	LIST_ENTRY tList;
	HMUTEX hMutex;
} PCD, * PPCD;

context_id_t 
_cdecl 
pcd_GetContextId(
	VOID
);

context_id_t
_cdecl
pcd_CreateContextId(
	_Out_ void** ppvContext
);

void
_cdecl
pcd_ReleaseContext(
	_Out_opt_ void* pvContext
);


bool 
_cdecl 
pcd_CreateEntry(
	_In_ context_id_t hCid, 
	_In_opt_ void* pvContext,
	_Out_ PPCD_ENTRY* pptEntry
);

void 
_cdecl 
pcd_ReleaseEntry(
	_In_ PPCD_ENTRY ptEntry
);

bool 
_cdecl 
pcd_Get(
	_In_ PPCD ptPcd, 
	_In_ context_id_t hCid,
	_Out_ PPCD_ENTRY* pptEntry
);
