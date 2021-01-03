#include "atom.h"
#include "atomport.h"
#include "debug/debug_port.h"
#include <stdio.h>
#include <conio.h>


int IsRamReadable(volatile void* address)
{
	return 1;
}

int IsRamReadWriteble(volatile void* address)
{
	return 1;
}

void PortDebugPutChar(char c)
{
	printf("%c",c);
}

int PortDebugIsReadable()
{
	return 1;
}

uint8_t PortDebugRead8()
{
	return (_getch());
}