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
UTILS_Assert(
	void* assertion,
	void* filename,
	uint32_t linenumber,
	char* message
);

uint32_t
UTILS_DbgPrint(
	uint32_t ComponentId,
	uint32_t Level,
	char const* Format,
	...
);

#ifdef __cplusplus
EXTERN_C_END
#endif