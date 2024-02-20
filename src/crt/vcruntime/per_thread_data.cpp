//
// per_thread_data.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Per-Thread Data (PTD) used by the VCRuntime.
//
#include <vcruntime_internal.h>
#include <stdint.h>
#include <PerContextData.h>


static HPCD g_hPcd = NULL;

__vcrt_ptd* NTAPI __vcrt_ptd_table_allocate()
{
    __vcrt_ptd* ptPtd = nullptr;
    ptPtd = (__vcrt_ptd*)calloc(1, sizeof(*ptPtd));
    return ptPtd;
}

VOID __vcrt_ptd_table_free(
    void* ptPtd
)
{
    if (NULL == ptPtd)
    {
        return;
    }
    free(ptPtd);
}

static __vcrt_ptd* __cdecl store_and_initialize_ptd()
{
    __vcrt_ptd* ptPtd = nullptr;
    ptPtd = __vcrt_ptd_table_allocate();
#if defined _M_X64 || defined _M_ARM || defined _M_ARM64 || defined _M_HYBRID
    ptPtd->_CatchStateInParent = INVALID_CATCH_SPECIFIC_STATE;
#endif

    if (!PCD_SetContext(g_hPcd, ptPtd, __vcrt_ptd_table_free))
    {
        __vcrt_ptd_table_free(ptPtd);
        return nullptr;
    }

    return ptPtd;
}

extern "C" bool __cdecl __vcrt_initialize_ptd()
{
    if (!PCD_Create(&g_hPcd))
    {
        return false;
    }

    if (!store_and_initialize_ptd())
    {
        __vcrt_uninitialize_ptd();
        return false;
    }

    return true;
}

extern "C" bool __cdecl __vcrt_uninitialize_ptd()
{
    PCD_Destroy(g_hPcd);
    g_hPcd = NULL;
    return true;
}

extern "C" __vcrt_ptd* __cdecl __vcrt_getptd_noexit()
{
    // The EH runtime relies on the PTD.  If we attempt to allocate a PTD and
    // that allocation attempt fails by raising a structured exception, the EH
    // runtime may intercept that structured exception and re-attempt to allocate
    // a PTD.  This may result in infinite recursion that causes a stack overflow.
    //
    // To handle this case more gracefully, we set the PTD pointer to a
    // sentinel value for the duration of the allocation attempt.  This allows us
    // to detect this unwanted reentrancy.

    // First see if we've already created per-thread data for this thread:
    __vcrt_ptd* ptPtd = nullptr;
    
    // Is thread safe because the current thread is the only one can add to him PCD
    if (!PCD_GetContext(g_hPcd, (void**) & ptPtd))
    { 
        ptPtd = store_and_initialize_ptd();
    }

    return ptPtd;
}

// Returns the PTD if one exists; null otherwise.  Does not attempt to allocate a new PTD.
extern "C" __vcrt_ptd* __cdecl __vcrt_getptd_noinit()
{
    __vcrt_ptd* ptPtd = nullptr;

    // Is thread safe because the current thread is the only one can add to him PCD
    if (!PCD_GetContext(g_hPcd, (void**)&ptPtd))
    {
        return nullptr;
    }

    return ptPtd;
}

extern "C" __vcrt_ptd* __cdecl __vcrt_getptd()
{
    __vcrt_ptd* const ptd = __vcrt_getptd_noexit();
    if (ptd == nullptr)
    {
        abort();
    }

    return ptd;
}

#if defined _VCRT_SAT_1
// This function accesses the base vcruntime's PTD by inferring its location from internal fields
// Via the exported functions below.
extern "C"
{
    void** __cdecl __current_exception();
    int*   __cdecl __processing_throw();
}

// Disable optimizations to prevent UB of memory accesses from creeping in.
#pragma optimize ("", off)
extern "C" RENAME_BASE_PTD(__vcrt_ptd)* __cdecl RENAME_BASE_PTD(__vcrt_getptd)()
{
    uint8_t* const curexception_offset = reinterpret_cast<uint8_t *>(__current_exception());
    uint8_t* const processingthrow_offset = reinterpret_cast<uint8_t *>(__processing_throw());

    RENAME_BASE_PTD(__vcrt_ptd)* const infer_from_curexception = reinterpret_cast<RENAME_BASE_PTD(__vcrt_ptd) *>(curexception_offset - offsetof(RENAME_BASE_PTD(__vcrt_ptd), _curexception));
    RENAME_BASE_PTD(__vcrt_ptd)* const infer_from_processingthrow = reinterpret_cast<RENAME_BASE_PTD(__vcrt_ptd) *>(processingthrow_offset - offsetof(RENAME_BASE_PTD(__vcrt_ptd), _ProcessingThrow));

    if (infer_from_curexception != infer_from_processingthrow)
    {
      //  abort();
    }

    return infer_from_curexception;
}
#pragma optimize ("", on)

RENAME_BASE_PTD(__vcrt_ptd)* __cdecl RENAME_BASE_PTD(__vcrt_getptd_noinit)()
{
    return RENAME_BASE_PTD(__vcrt_getptd)();
}

#endif

extern "C" void __cdecl __vcrt_freeptd()
{
    // If the argument is null, get the pointer for this thread. Note that we
    // must not call __vcrt_getptd, because it will allocate a new per-thread
    // data if one does not already exist.

    PCD_ReleaseContext(g_hPcd);
}
