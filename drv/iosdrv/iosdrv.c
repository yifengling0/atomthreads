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


#include "iosdrv.h"
#include <string.h>
#include "console.h"
#include "debug.h"

SLIST_HEAD(Pool, IONode);
static struct Pool _header;

static IOSDrv* FindDrv(const char *name)
{
	IONode* item;

	SLIST_FOREACH(item, &_header, node) {
		IOSDrv* it = (IOSDrv*)item;
		if (strcmp(it->name,name)== 0) {
			return it;
		}
	}

	return NULL;
}

#ifdef USE_CONSOLE  
static enConsoleResult iosdrv(int argc, char** argv)
{
	IONode* item;
	int count = 0;

	DebugPrint("ios driver:\r\n");

	SLIST_FOREACH(item, &_header, node) {
		IOSDrv* it = (IOSDrv*)item;
		DebugPrint("\t");
		DebugPrint(it->name);
		count++;
		if (count % 8 == 0) {
			DebugPrint("\r\n");
		}
	}
	
	DebugPrint("\r\n");

	return CE_OK;
}


static COMMAND _iosCommand = {
	DEFAULT_NODE,
	"ios",
	iosdrv,
};
#endif
      
void IosDrvInit(void)
{
	SLIST_INIT(&_header);
#ifdef USE_CONSOLE    
	ConsoleRegCmd(&_iosCommand);
#endif    
}

int IosDrvInstall(IOSDrv* drv, const char *name)
{
	if (FindDrv(name) == NULL) {
		strncpy(drv->name, name, FILE_NAME_SIZE);
		SLIST_INSERT_HEAD(&_header, (IONode*)drv, node);
		return TRUE;
	}

	return FALSE;
}


FD open(const char* name, int flags, int mode)
{
	IOSDrv* drv = FindDrv(name);

	if (drv != NULL) {
		if (drv->popen((FD)drv, flags, mode)) {
			return (FD)drv;
		}
	}

	return (FD)(ERR_FD);
}

int close(FD fd)
{
	if (fd != ERR_FD) {
		IOSDrv* drv = (IOSDrv*)fd;
		return drv->pclose(fd);
	}

	return FALSE;
}

int read(FD fd, char* buffer, size_t maxbytes)
{
	if (fd != ERR_FD) {
		IOSDrv* drv = (IOSDrv*)fd;
		return drv->pread(fd, buffer, maxbytes);
	}

	return NG;
}

int write(FD fd, const char* buffer, size_t nbytes)
{
	if (fd != ERR_FD) {
		IOSDrv* drv = (IOSDrv*)fd;
		return drv->pwrite(fd, buffer, nbytes);
	}

	return NG;
}

int ioctl(FD fd, int function, void* arg)
{
	if (fd != ERR_FD) {
		IOSDrv* drv = (IOSDrv*)fd;
		return drv->pioctl(fd, function, arg);
	}

	return NG;
}
