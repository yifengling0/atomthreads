/*
 * Copyright (c) 2020, yifengling0. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. No personal names or organizations' names associated with the
 *    Atomthreads project may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE ATOMTHREADS PROJECT AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include "debug_port.h"
#include "debug.h"
#include "mem.h"
#include "console.h"
#include "numberical.h"
#include <stddef.h>

enConsoleResult fnpob(int argc, char **argv)
{
	enConsoleResult ret = CE_OK;

	if (argc == 3) {
		volatile uint8_t* pAddress = (uint8_t*)(void*)StringToInt(argv[1]);
		uint16_t data = (uint8_t)StringToInt(argv[2]);

		if (IsRamReadWriteble(pAddress)) {
			*pAddress = data;
			DebugPrintValue(NULL, data, 16, 2, 1);
			ret = CE_OK;
		}
		else {
			ret = CE_ACCESS_FORBIDDEN;
		}
	}
	else {
		ret = CE_PARAM_ERROR;
	}

	return ret;
}

enConsoleResult fnpow(int argc, char** argv)
{
	enConsoleResult ret = CE_OK;

	if (argc == 3) {
		volatile uint16_t* pAddress = (uint16_t*)(void*)StringToInt(argv[1]);
		uint16_t data = (uint16_t)StringToInt(argv[2]);

		if (IsRamReadWriteble(pAddress)) {
			*pAddress = data;
			DebugPrintValue(NULL, data, 16, 4, 1);
			ret = CE_OK;
		}
		else {
			ret = CE_ACCESS_FORBIDDEN;
		}
	}
	else {
		ret = CE_PARAM_ERROR;
	}

	return ret;
}

enConsoleResult fnpod(int argc, char** argv)
{
	enConsoleResult ret = CE_OK;

	if (argc == 3) {
		volatile uint32_t* pAddress = (uint32_t*)(void*)StringToInt(argv[1]);
		uint32_t data = (uint32_t)StringToInt(argv[2]);

		if (IsRamReadWriteble(pAddress)) {
			*pAddress = data;
			DebugPrintValue(NULL, data, 16, 8, 1);
			ret = CE_OK;
		}
		else {
			ret = CE_ACCESS_FORBIDDEN;
		}
	}
	else {
		ret = CE_PARAM_ERROR;
	}

	return ret;
}


enConsoleResult fnpib(int argc, char** argv)
{
	enConsoleResult ret = CE_OK;

	if (argc == 2) {
		volatile uint8_t* pAddress = (uint8_t*)(void*)StringToInt(argv[1]);

		if (IsRamReadable(pAddress)) {
			uint8_t data = *(pAddress);
			DebugPrintValue(NULL, data, 16, 2, 1);
			ret = CE_OK;
		}
		else {
			ret = CE_ACCESS_FORBIDDEN;
		}
	}
	else {
		ret = CE_PARAM_ERROR;
	}

	return ret;
}

enConsoleResult fnpiw(int argc, char** argv)
{
	enConsoleResult ret = CE_OK;

	if (argc == 2) {
		volatile uint16_t* pAddress = (uint16_t*)(void*)StringToInt(argv[1]);

		if (IsRamReadable(pAddress)) {
			uint16_t data = *(pAddress);
			DebugPrintValue(NULL, data, 16, 4, 1);
			ret = CE_OK;
		}
		else {
			ret = CE_ACCESS_FORBIDDEN;
		}
	}
	else {
		ret = CE_PARAM_ERROR;
	}

	return ret;
}

enConsoleResult fnpid(int argc, char** argv)
{
	enConsoleResult ret = CE_OK;

	if (argc == 2) {
		volatile uint32_t* pAddress = (uint32_t*)(void*)StringToInt(argv[1]);

		if (IsRamReadable(pAddress)) {
			uint32_t data = *(pAddress);
			DebugPrintValue(NULL, data, 16, 8, 1);
			ret = CE_OK;
		}
		else {
			ret = CE_ACCESS_FORBIDDEN;
		}
	}
	else {
		ret = CE_PARAM_ERROR;
	}

	return ret;
}

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
enConsoleResult dump(int argc, char** argv)
{
	enConsoleResult ret = CE_OK;

    if(argc < 3) return CE_PARAM_ERROR;

	volatile uint8_t* pAddressStart = (uint8_t*)StringToInt(argv[1]);

    if (!IsRamReadable(pAddressStart)) return CE_ACCESS_FORBIDDEN;

	int len = StringToInt(argv[2]);

	char result[10] = { 0 };

	unsigned char* buf = (unsigned char*)pAddressStart;
	int i, j;

	for (i = 0; i < len; i += 16)
	{
		DebugPrintValue(result, (uint32_t)(void*)(buf + i), 16, 8, 0);
		DebugPrint(" ");
		for (j = 0; j < 16; j++)
			if (i + j < len) {
				DebugPrintValue(result, buf[i + j], 16, 2, 0);
				DebugPrint(" ");
			}
			else {
				DebugPrint("   ");
			}
		DebugPrint(" ");

		for (j = 0; j < 16; j++)
			if (i + j < len) {
				if (__is_print(buf[i + j])) {
					PortDebugPutChar(buf[i + j]);
				}
				else {
					DebugPrint(".");
				}
			}
		DebugPrint("\r\n");
	}

	return (ret);
}

static COMMAND _debugCmds[] = {
	{DEFAULT_NODE, "pob", fnpob },
	{DEFAULT_NODE, "pow", fnpow },
	{DEFAULT_NODE, "pod", fnpod },
	{DEFAULT_NODE, "pib", fnpib },
	{DEFAULT_NODE, "piw", fnpiw },
	{DEFAULT_NODE, "pid", fnpid },
	{DEFAULT_NODE, "dump", dump },
};

void DebugPrint(const char* str)
{
	const char* s = str;
	while (*s != '\0') {
		PortDebugPutChar(*s);
		s++;
	}
}

void DebugPrintValue(char* prompt, uint32_t value, int radix, int prefix, int newline)
{
	char buf[11] = { 0 };

	if (prompt != NULL) {
		DebugPrint(prompt);
	}

	IntToString(value, buf, radix, prefix);
	DebugPrint(buf);

	if (newline) {
		DebugPrint("\r\n");
	}
}

void DebugInit()
{
	for (int i = 0; i < sizeof(_debugCmds) / sizeof(COMMAND); i++) {
		ConsoleRegCmd(&_debugCmds[i]);
	}
}

