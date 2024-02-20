//
// per_thread_data.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Per-Thread Data (PTD) used by the AppCRT.
//
#include <corecrt_internal.h>
#include <stddef.h>
#include <PerContextData.h>


static HPCD g_hPcd = NULL;


__acrt_ptd* NTAPI __acrt_ptd_table_allocate()
{
    __acrt_ptd* ptPtd = nullptr;
    ptPtd = (__acrt_ptd*)_calloc_crt(1, sizeof(*ptPtd));
    return ptPtd;
}

VOID __acrt_ptd_table_free(
    void* pvPtd
)
{
    __acrt_ptd* ptPtd = (__acrt_ptd*)pvPtd;
    if (NULL == ptPtd)
    {
        return;
    }

    _free_crt(ptPtd->_strerror_buffer);
    _free_crt(ptPtd->_wcserror_buffer);
    _free_crt(ptPtd->_beginthread_context);
    free(ptPtd);
}


static __acrt_ptd* __cdecl store_and_initialize_ptd()
{
    __acrt_ptd* ptPtd = nullptr;
    ptPtd = __acrt_ptd_table_allocate();

    if (!PCD_SetContext(g_hPcd, ptPtd, __acrt_ptd_table_free))
    {
        __acrt_ptd_table_free(ptPtd);
        return nullptr;
    }

    return ptPtd;
}

extern "C" bool __cdecl __acrt_initialize_ptd()
{
    if (!PCD_Create(&g_hPcd))
    {
        return false;
    }

    if (!store_and_initialize_ptd())
    {
        __acrt_uninitialize_ptd(false);
        return false;
    }

    return true;
}

extern "C" bool __cdecl __acrt_uninitialize_ptd(bool)
{
    PCD_Destroy(g_hPcd);
    g_hPcd = NULL;
    return true;
}

extern "C" __acrt_ptd* __cdecl __acrt_getptd_noexit()
{
    __acrt_ptd* ptPtd = nullptr;

    // Is thread safe because the current thread is the only one can add to him PCD
    if (!PCD_GetContext(g_hPcd, (void**)&ptPtd))
    {
        ptPtd = store_and_initialize_ptd();
    }

    return ptPtd;
}

extern "C" __acrt_ptd* __cdecl __acrt_getptd()
{
    __acrt_ptd* const ptd = __acrt_getptd_noexit();
    if (ptd == nullptr)
    {
      //  abort();
    }

    return ptd;
}

extern "C" void __cdecl __acrt_freeptd()
{
    PCD_ReleaseContext(g_hPcd);
}
