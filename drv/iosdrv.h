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


#ifndef _IOSDRVH_
#define _IOSDRVH_

#include "atom.h"
#include <stddef.h>
#include "queue.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_FD (-1)
#define FILE_NAME_SIZE (8)

typedef struct IONode IONode;

struct IONode {
    SLIST_ENTRY(IONode) node;
};

typedef int FD;

typedef int (*PFOPEN)(FD fd, int flags, int mode);
typedef int (*PFCLOSE)(FD fd);
typedef int (*PFREAD)(FD fd, char* buffer, size_t maxbytes);
typedef int (*PFWRITE)(FD fd, const char* buffer, size_t nbytes);
typedef int (*PFIOCTL)(FD fd, int function, void* arg);

typedef struct _iosDrv_t {
    SLIST_ENTRY(IONode) node;
    char name[FILE_NAME_SIZE];
    PFOPEN popen;
    PFCLOSE pclose;
    PFREAD pread;
    PFWRITE pwrite;
    PFIOCTL pioctl;
}IOSDrv;

extern FD open(const char* name, int flags, int mode);
extern int close(FD fd);
extern int read(FD fd, char* buffer, size_t maxbytes);
extern int write(FD fd, const char* buffer, size_t nbytes);
extern int ioctl(FD fd, int function, void* arg);


void IosDrvInit(void);
int IosDrvInstall(IOSDrv* drv, const char *name);

#ifdef __cplusplus
}
#endif

#endif