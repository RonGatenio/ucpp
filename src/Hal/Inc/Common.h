#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
EXTERN_C_START
#endif

void* __cdecl std_malloc(size_t const size);
void __cdecl std_free(void* const block);


#ifdef __cplusplus
EXTERN_C_END
#endif