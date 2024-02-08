#pragma once
#include "Mutex.h"

typedef struct _MUTEX
{
	long volatile lSpinLock;
} MUTEX, * PMUTEX;