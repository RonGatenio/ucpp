#include "Utils.h"
#include <windows.h>
#include <intrin.h>
#include <stdarg.h>

bool
_cdecl
UTILS_IsXmmSupport(
	VOID
)
{
	return IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE);
}

void
_cdecl
UTILS_Assert(
	void* assertion,
	void* filename,
	uint32_t linenumber,
	char* message
)
{
	UNREFERENCED_PARAMETER(assertion);
	UNREFERENCED_PARAMETER(filename);
	UNREFERENCED_PARAMETER(linenumber);
	UNREFERENCED_PARAMETER(message);
#ifdef _DEBUG
	__debugbreak();
#else
	__ud2();
#endif // DEBUG
}

uint32_t
UTILS_DbgPrint(
	uint32_t ComponentId,
	uint32_t Level,
	char const * Format,
	...
)
{
	va_list list = NULL;
	uint32_t res = 0;

	UNREFERENCED_PARAMETER(ComponentId);
	UNREFERENCED_PARAMETER(Level);

	va_start(list, Format);
	//res = vprintf(Format, list);
	va_end(list);
	return res;
}