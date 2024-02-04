 //
 // winapi_thunks.cpp
 //
 //      Copyright (c) Microsoft Corporation. All rights reserved.
 //
 // Definitions of wrappers for Windows API functions that cannot be called
 // by the statically-linked CRT code because they are not part of the MSDK.
 //
#include <vcruntime_internal.h>
#include "winapi_thunks.h"

extern"C" PVOID __vcrt_EncodePointer(
    _In_ PVOID const ptr
)
{
    return __crt_fast_encode_pointer(ptr);
}

extern"C" PVOID __vcrt_DecodePointer(
    _In_ PVOID const ptr
)
{
    return __crt_fast_decode_pointer(ptr);
}
