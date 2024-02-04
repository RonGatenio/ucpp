 //
 // winapi_thunks.cpp
 //
 //      Copyright (c) Microsoft Corporation. All rights reserved.
 //
 // Definitions of wrappers for Windows API functions that cannot be called
 // by the statically-linked CRT code because they are not part of the MSDK.
 //

#pragma once


extern"C" PVOID __vcrt_EncodePointer(
    _In_ PVOID const ptr
);

extern"C" PVOID __vcrt_DecodePointer(
    _In_ PVOID const ptr
);
