#pragma once
#include "Mutex.h"

typedef struct _MUTEX
{
	long volatile lSpinLock;
	DWORD dwThreadId;
} MUTEX, * PMUTEX;