#pragma once
#include "Common.h"

#ifdef __cplusplus
EXTERN_C_START
#endif

typedef struct ALLOCATOR* HALLOCATOR;

bool
_cdecl
ALLOCATOR_Create(HALLOCATOR* phAllocator);

void
_cdecl
ALLOCATOR_Destroy(HALLOCATOR hAllocator);

void*
_cdecl
ALLOCATOR_Allocate(HALLOCATOR hAllocator, size_t cbSize);

bool
_cdecl
ALLOCATOR_Free(HALLOCATOR hAllocator, void* pvMemory);

bool
_cdecl
ALLOCATOR_GetSize(HALLOCATOR hAllocator, void* pvMemory, size_t* pcbSize);

bool
_cdecl
ALLOCATOR_Realloc(HALLOCATOR hAllocator, void* pvMemory, size_t cbNewSize, void** ppvNewMemory);

#ifdef __cplusplus
EXTERN_C_END
#endif