#pragma once
#include "Allocator.h"
#include <ntddk.h>
#define offsetof(s,m)   (size_t)( (ptrdiff_t)&(((s *)0)->m) )

#define POOL_TAG ('None')

#pragma warning( push )
#pragma warning( disable : 4200)

typedef struct _ALLOCATION_BLOCK
{
	SIZE_T cbSize;
	__declspec(align(16)) uint8_t pbData[0];
} ALLOCATION_BLOCK, * PALLOCATION_BLOCK;
static_assert(offsetof(ALLOCATION_BLOCK, pbData) == 16, "Aling faild");
static_assert(sizeof(ALLOCATION_BLOCK) == 16, "Size faild");

#pragma warning( pop )
