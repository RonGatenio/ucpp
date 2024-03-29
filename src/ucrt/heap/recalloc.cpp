//
// recalloc.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// Implementation of _recalloc()
//
#include <corecrt_internal.h>
#include <malloc.h>
#include <string.h>



// Reallocates a block of memory in the heap.
//
// This function reallocates the block pointed to by 'block' such that it is
// 'count * size' bytes in size.  The new size may be either greater or less
// than the original size of the block.  If the new size is greater than the
// original size, the new bytes are zero-filled.  This function shares its
// implementation with the realloc() function; consult the comments of that
// function for more information about the implementation.
//
// This function supports patching and therefore must be marked noinline.
// Both _recalloc_dbg and _recalloc_base must also be marked noinline
// to prevent identical COMDAT folding from substituting calls to _recalloc
// with either other function or vice versa.
extern "C" __declspec(noinline) _CRTRESTRICT void* __cdecl _recalloc(
    void*  const block,
    size_t const count,
    size_t const size
    )
{
    // Ensure that (count * size) does not overflow
    _VALIDATE_RETURN_NOEXC(count == 0 || (_HEAP_MAXREQ / count) >= size, ENOMEM, nullptr);

    return std_realloc(block, count * size);
}
