#include "atomtimer.h"
#include <windows.h>

uint8_t atomTimerDelay(uint32_t ticks)
{
	Sleep(ticks * 1000 / SYSTEM_TICKS_PER_SEC);
	return 0;
}