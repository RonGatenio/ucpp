#pragma once
#include "Common.h"
#ifdef __cplusplus
EXTERN_C_START
#endif

typedef struct MUTEX* HMUTEX;

bool
_cdecl
MUTEX_Create(HMUTEX* phMutex);

void
_cdecl
MUTEX_Destroy(HMUTEX hMutex);

bool
_cdecl
MUTEX_Lock(HMUTEX hMutex);

bool
_cdecl
MUTEX_UnLock(HMUTEX hMutex);

#ifdef __cplusplus
EXTERN_C_END
#endif