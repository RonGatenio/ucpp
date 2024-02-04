/*
 * PROJECT:   Universal C++ RunTime (UCXXRT)
 * FILE:      stdalloc.cpp
 * DATA:      2022/11/17
 *
 * PURPOSE:   Universal C++ RunTime
 *
 * LICENSE:   Relicensed under The MIT License from The CC BY 4.0 License
 *
 * DEVELOPER: MiroKaku (miro.kaku AT Outlook.com)
 */

#include <Allocator.h>

HALLOCATOR g_hAllocator = nullptr;

extern "C" bool __cdecl std_heap_init()
{
    return ALLOCATOR_Create(&g_hAllocator);
}

extern "C" void __cdecl std_heap_uninit()
{
    ALLOCATOR_Destroy(g_hAllocator);
    g_hAllocator = nullptr;
}

extern "C" void* __cdecl std_malloc(size_t const size)
{
    return ALLOCATOR_Allocate(g_hAllocator, size);
}

extern "C" void __cdecl std_free(void* const block)
{
    if (block)
    {
        ALLOCATOR_Free(g_hAllocator, block);
    }
}

extern "C" _CRT_HYBRIDPATCHABLE __declspec(noinline) size_t __cdecl std_msize(void* const block)
{
   size_t cbSize = 0;

   if (!ALLOCATOR_GetSize(g_hAllocator, block, &cbSize))
   {
       return 0;
   }

   return cbSize;
}

extern "C" _CRT_HYBRIDPATCHABLE __declspec(noinline) _CRTRESTRICT void* __cdecl std_realloc(
    void* const block,
    size_t const size
)
{
    void* pvNewBlock = nullptr;
    if (!ALLOCATOR_Realloc(g_hAllocator, block, size, &pvNewBlock))
    {
        return nullptr;
    }

    return pvNewBlock;
}
