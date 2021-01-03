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


#include <stdio.h>
#include "console.h"
#include <string.h>
#include "atom.h"
#include "debug.h"

#define MAX_PARAM_COUNT (5)
#define PROMPT	"[maple~]#"

SLIST_HEAD(Pool, Node);
static struct Pool _header;

static char* _parameters[MAX_PARAM_COUNT] = { NULL };

static enConsoleResult help(int argc, char** argv)
{
	Node* item;
	int count = 0;

	DebugPrint("command:\r\n");

	SLIST_FOREACH(item, &_header, node) {
		COMMAND* cmd = (COMMAND*)item;
		if( strlen(cmd->name) > 0 ){
			DebugPrint("\t");
			DebugPrint(cmd->name);
			count++;
			if (count % 8 == 0) {
				DebugPrint("\r\n");
			}
		}
	}

	DebugPrint("\r\n");

	return CE_OK;
}

static COMMAND _helpCommand = {  
	DEFAULT_NODE,
	"help",
	help,
};

static enConsoleResult none(int argc, char** argv)
{
	return CE_OK;
}


static COMMAND _noneCommand = {
	DEFAULT_NODE,
	"",
	none,
};

/* Application threads' TCBs */
static ATOM_TCB console_tcb;

/* Main thread's stack area */
static uint8_t console_task_stack[CONSOLE_STACK_SIZE_BYTES];

#define CMD_LEN (32)
typedef struct _tCommand {
	char buffer[CMD_LEN];
	int index;
}tCommand;

static tCommand _debug_command;

static void print_result(enConsoleResult result)
{
    switch(result) {
    case CE_OK:
        break;
    case CE_NOT_FOUND:
        DebugPrint("Bad command!\r\n");
        break;
    case CE_PARAM_ERROR:
        DebugPrint("Parameter error!\r\n");
        break;
    case CE_RUNTIME_ERROR:
        DebugPrint("Runtime error!\r\n");
        break;
    case CE_ACCESS_FORBIDDEN:
        DebugPrint("Access forbidden!\r\n");
        break;
    default:
        DebugPrintValue("code:", result, 16, 0, 1);
        break;
    }
}

static void console_task (uint32_t data)
{
	enConsoleResult result;
	_debug_command.index = 0;
	memset(_debug_command.buffer, 0, CMD_LEN);
	DebugPrint(PROMPT);

	while (1) {
		if (PortDebugIsReadable()) {
			char c = PortDebugRead8();
			if (c == 0x0d) {
				DebugPrint("\r\n");
				result = ConsoleInput(_debug_command.buffer);
                print_result(result);
				DebugPrint(PROMPT);
				memset(_debug_command.buffer, 0, CMD_LEN);
				_debug_command.index = 0;
			}
			else {
				PortDebugPutChar(c);
				_debug_command.buffer[_debug_command.index] = c;
				_debug_command.index++;
			}
		}
		else {
			atomTimerDelay(2);
		}
	}
}

void ConsoleInit()
{
	SLIST_INIT(&_header);
	SLIST_INSERT_HEAD(&_header, (Node*)&_helpCommand, node);
	SLIST_INSERT_HEAD(&_header, (Node*)&_noneCommand, node);
}

void ConsoleRun()
{
	/* Create an application thread */
	uint8_t status = atomThreadCreate(&console_tcb,
		200, console_task, 0,
		&console_task_stack[0],
		CONSOLE_STACK_SIZE_BYTES,
		TRUE);

	if (status != ATOM_OK) {
		DebugPrint("console create task error!\r\n");
	}
}

void ConsoleRegCmd(PCOMMAND cmd)
{
	SLIST_INSERT_HEAD(&_header, (Node*)cmd, node);
}

static enConsoleResult RunCommand(int argc, char** argv)
{
	enConsoleResult ret = CE_NOT_FOUND;
	Node* item;

	SLIST_FOREACH(item, &_header, node) {
		COMMAND* cmd = (COMMAND*)item;
		if (strcmp(cmd->name, argv[0]) == 0) {
			ret = cmd->action(argc, argv);
			break;
		}
	}

	return ret;
}

enConsoleResult ConsoleInput(char *command)
{
	enConsoleResult ret;
	int count = 0;
	char* current = command;
	int remind = 0;

	for(char *p = command; *p != '\0'; p++){
		if (' ' == *p) {
			_parameters[count] = current;
			*p = '\0';
			current = p + 1;
			count++;
			remind = 0;
		}
		else {
			remind = 1;
		}
	}

	if (count == 0 || remind > 0) {
		_parameters[count] = current;
		count++;
	}
	
	ret = RunCommand(count, _parameters);

	return ret;
}
