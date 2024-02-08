/*
 * PROJECT:   Universal C++ RunTime (UCXXRT)
 * FILE:      sys_main.cpp
 * DATA:      2021/05/03
 *
 * PURPOSE:   Universal C++ RunTime
 *
 * LICENSE:   Relicensed under The MIT License from The CC BY 4.0 License
 *
 * DEVELOPER: MiroKaku (miro.kaku AT Outlook.com)
 */

#include <sys_common.inl>


extern"C" unsigned long crt_init(void* hal_context)
{
    return __scrt_common_main(hal_context);
}

extern"C" void crt_shutdown(void* hal_context)
{
    __scrt_common_exit(hal_context);
}

