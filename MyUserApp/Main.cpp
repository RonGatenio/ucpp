/*
 * PROJECT:   Universal C++ RunTime (UCXXRT)
 * FILE:      ucxxrt.h
 * DATA:      2021/05/03
 *
 * PURPOSE:   Universal C++ RunTime
 *
 * LICENSE:   Relicensed under The MIT License from The CC BY 4.0 License
 *
 * DEVELOPER: MiroKaku (miro.kaku AT Outlook.com)
 */

#pragma once

#include <ucxxrt.h>
#include <vector>

 // Warnings which disabled for compiling
#if _MSC_VER >= 1200
#pragma warning(push)
// nonstandard extension used : nameless struct/union
#pragma warning(disable:4201)
// 'struct_name' : structure was padded due to __declspec(align())
#pragma warning(disable:4324)
// 'enumeration': a forward declaration of an unscoped enumeration must have an
// underlying type (int assumed)
#pragma warning(disable:4471)
#endif


#ifdef __KERNEL_MODE
#   ifndef  _KERNEL_MODE
#       define  _KERNEL_MODE __KERNEL_MODE
#   endif
#else
//#   error user mode is not supported.
#endif

#ifndef _CRTIMP
#   define _CRTIMP
#endif

#ifndef _VCRTIMP
#define _VCRTIMP _CRTIMP
#endif


//#include <fltKernel.h>
#include <stdint.h>


#ifndef _ByteSwap16
#define _ByteSwap16(x) (                            \
    ((uint16_t(x) & uint16_t(0xFF << 8)) >> 8) |    \
    ((uint16_t(x) & uint16_t(0xFF >> 0)) << 8)      \
)
#endif

#ifndef _ByteSwap32
#define _ByteSwap32(x) (                            \
    ((uint32_t(x) & uint32_t(0xFF << 24)) >> 24) |  \
    ((uint32_t(x) & uint32_t(0xFF << 16)) >>  8) |  \
    ((uint32_t(x) & uint32_t(0xFF <<  8)) <<  8) |  \
    ((uint32_t(x) & uint32_t(0xFF <<  0)) << 24)    \
)
#endif

#ifndef _ByteSwap64
#define _ByteSwap64(x) (                            \
    ((uint64_t(x) & uint64_t(0xFF << 56)) >> 56) |  \
    ((uint64_t(x) & uint64_t(0xFF << 48)) >> 40) |  \
    ((uint64_t(x) & uint64_t(0xFF << 40)) >> 24) |  \
    ((uint64_t(x) & uint64_t(0xFF << 32)) >>  8) |  \
    ((uint64_t(x) & uint64_t(0xFF << 24)) <<  8) |  \
    ((uint64_t(x) & uint64_t(0xFF << 16)) << 24) |  \
    ((uint64_t(x) & uint64_t(0xFF <<  8)) << 40) |  \
    ((uint64_t(x) & uint64_t(0xFF <<  0)) << 56) |  \
)
#endif

#include <Windows.h>
#include <iostream>  // For std::ostream and std::streambuf
#include <cstring>   // For std::memset
#include <iostream>
#include <sstream>
#include<string>

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif

using namespace std;

#define PRINT(msg) WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg "\n", sizeof(msg), NULL, 0)
#define PRINT_(msg, len) WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), (msg), (len), NULL, 0)

void func()
{
    vector<int> v(10);
    std::stringstream s;

    s << "Hi there ";
    try
    {
        PRINT("[*] func - HELLO");
        
        v.push_back(1);
        v.push_back(22);
        v.push_back(333);
        
        throw exception();
    }
    catch (const exception&)
    {
        PRINT("[!] func - EXCEPTION");
        v.push_back(4444);
        //printf("EXCEPTION\n");
        //cout << "EXCEPTION!\n";
    }
    v.push_back(55555);

    s << "Vector contains: ";
    for (auto i : v) {
        s << i << ", ";
    }
    s << "\n";

    PRINT_(s.str().c_str(), s.str().length());

    PRINT("[*] func - RETURN func");
}


extern "C" unsigned long ModuleMain(unsigned long state, void*)
{
    if (state == 1) {
        PRINT("[*] ModuleMain - LOAD");
        func();
    }
    else {
        PRINT("[*] ModuleMain - UNLOAD");
    }
    PRINT("[*] ModuleMain - RETURN");
    return 0;
}

