#pragma once
#include "Mutex.h"
#include <ntddk.h>

typedef struct _MUTEX
{
	LONG volatile lSpinLock;
} MUTEX, * PMUTEX;