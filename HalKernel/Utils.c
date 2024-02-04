#include "Utils.h"
#include <ntddk.h>
#include <intrin.h>

bool
_cdecl
UTILS_IsXmmSupport(
	VOID
)
{
	return ExIsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE);
}

void
_cdecl
UTILS_Abort(
	VOID
)
{
#ifdef _DEBUG
	__debugbreak();
#else
	__ud2();
#endif // DEBUG
}