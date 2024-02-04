#include "Allocator_internal.h"


bool _cdecl ALLOCATOR_Create(HALLOCATOR* phAllocator)
{
	UNREFERENCED_PARAMETER(phAllocator);
	ExInitializeDriverRuntime(DrvRtPoolNxOptIn);
	return true;
}

void _cdecl ALLOCATOR_Destroy(HALLOCATOR hAllocator)
{
	UNREFERENCED_PARAMETER(hAllocator);
}

void* _cdecl ALLOCATOR_Allocate(HALLOCATOR hAllocator, size_t cbSize)
{
	UNREFERENCED_PARAMETER(hAllocator);
	PALLOCATION_BLOCK ptBlock = NULL;
	void* pvMemory = NULL;

	if (cbSize > (sizeof(*ptBlock) + cbSize))
	{
		goto lblCleanup;
	}
#pragma warning( push )
#pragma warning( disable : 4996)
	ptBlock = ExAllocatePoolWithTag(NonPagedPool, sizeof(*ptBlock) + cbSize, POOL_TAG);
#pragma warning( pop )
	if (NULL == ptBlock)
	{
		goto lblCleanup;
	}
	ptBlock->cbSize = cbSize;
	
	pvMemory = &(ptBlock->pbData[0]);
	memset(pvMemory, 0, cbSize);

lblCleanup:
	return pvMemory;
}

bool _cdecl ALLOCATOR_Free(HALLOCATOR hAllocator, void* pvMemory)
{
	UNREFERENCED_PARAMETER(hAllocator);
	bool bRet = false;
	PALLOCATION_BLOCK ptBlock = NULL;
	
	if (NULL == pvMemory)
	{
		goto lblCleanup;
	}

	ptBlock = CONTAINING_RECORD(pvMemory, ALLOCATION_BLOCK, pbData);

	ExFreePoolWithTag(ptBlock, POOL_TAG);
	bRet = true;

lblCleanup:
	return bRet;
}

bool _cdecl ALLOCATOR_GetSize(HALLOCATOR hAllocator, void* pvMemory, size_t* pcbSize)
{
	UNREFERENCED_PARAMETER(hAllocator);
	bool bRet = false;
	PALLOCATION_BLOCK ptBlock = NULL;

	if ((NULL == pvMemory) ||
		(NULL == pcbSize))
	{
		goto lblCleanup;
	}

	ptBlock = CONTAINING_RECORD(pvMemory, ALLOCATION_BLOCK, pbData);

	*pcbSize = ptBlock->cbSize;
	bRet = true;

lblCleanup:
	return bRet;
}

bool _cdecl ALLOCATOR_Realloc(HALLOCATOR hAllocator, void* pvMemory, size_t cbNewSize, void** ppvNewMemory)
{
	UNREFERENCED_PARAMETER(hAllocator);
	bool bRet = false;
	size_t cbOldSize = 0;
	void* pvNewMemory = NULL;

	if ((NULL == pvMemory) ||
		(0 == cbNewSize) ||
		(NULL == ppvNewMemory))
	{
		goto lblCleanup;
	}

	bRet = ALLOCATOR_GetSize(hAllocator, pvMemory, &cbOldSize);
	if (!bRet)
	{
		goto lblCleanup;
	}

	pvNewMemory = ALLOCATOR_Allocate(hAllocator, cbNewSize);
	if (NULL == pvNewMemory)
	{
		goto lblCleanup;
	}
	
	memcpy(pvNewMemory, pvMemory, min(cbOldSize, cbNewSize));

	ALLOCATOR_Free(hAllocator, pvMemory);
	pvMemory = NULL;

	*ppvNewMemory = pvNewMemory;
	pvNewMemory = NULL;
	bRet = true;

lblCleanup:
	if (NULL != pvNewMemory)
	{
		ALLOCATOR_Free(hAllocator, pvNewMemory);
		pvNewMemory = NULL;
	}

	return bRet;
}
