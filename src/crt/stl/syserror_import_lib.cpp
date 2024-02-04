// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// This must be as small as possible, because its contents are
// injected into the msvcprt.lib and msvcprtd.lib import libraries.
// Do not include or define anything else here.
// In particular, basic_string must not be included here.

#include <__msvc_system_error_abi.hpp>
#include <winapi_thunks.h>


namespace {
    struct _Whitespace_bitmap_t {
        bool _Is_whitespace[256];

        constexpr _Whitespace_bitmap_t() noexcept : _Is_whitespace{} {
            _Is_whitespace[' ']  = true;
            _Is_whitespace['\n'] = true;
            _Is_whitespace['\r'] = true;
            _Is_whitespace['\t'] = true;
            _Is_whitespace['\0'] = true;
        }

        _NODISCARD constexpr bool _Test(const char _Ch) const noexcept {
            return _Is_whitespace[static_cast<unsigned char>(_Ch)];
        }
    };

    constexpr _Whitespace_bitmap_t _Whitespace_bitmap;
} // unnamed namespace

_EXTERN_C
_NODISCARD size_t __CLRCALL_PURE_OR_STDCALL __std_get_string_size_without_trailing_whitespace(
    const char* const _Str, size_t _Size) noexcept {
    while (_Size != 0 && _Whitespace_bitmap._Test(_Str[_Size - 1])) {
        --_Size;
    }

    return _Size;
}

void __CLRCALL_PURE_OR_STDCALL __std_system_error_deallocate_message(char* const _Str) noexcept
{
    free(_Str);
}
_END_EXTERN_C
