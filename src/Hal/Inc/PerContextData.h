#pragma once
#include "Common.h"
#ifdef __cplusplus
EXTERN_C_START
#endif

typedef struct PCD* HPCD;
typedef void (*PFN_DATA_RELEASE)(void*);

bool
_cdecl
PCD_Create(
	_Out_ HPCD* phPcd
);

void
_cdecl
PCD_Destroy(
	_In_opt_ HPCD hPcd
);

bool
_cdecl
PCD_GetContext(
	_In_ HPCD hPcd,
	_Out_ void** ppvData
);

bool
_cdecl
PCD_SetContext(
	_In_ HPCD hPcd,
	_In_ void* pvData,
	_In_opt_ PFN_DATA_RELEASE pfnDataRelease
);

bool
_cdecl
PCD_ReleaseContext(
	_In_ HPCD hPcd
);

#ifdef __cplusplus
EXTERN_C_END
#endif