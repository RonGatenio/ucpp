#pragma once
#include "Common.h"
#ifdef __cplusplus
EXTERN_C_START
#endif

bool
_cdecl
UTILS_IsXmmSupport(
	VOID
);

void
_cdecl
UTILS_Abort(
	VOID
);

#ifdef __cplusplus
EXTERN_C_END
#endif