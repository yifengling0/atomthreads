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
#include "sio.h"

static int sio_open(FD fd, int flags, int mode)
{
	SioBase* sio = (SioBase*)fd;
	return sio->Protected.Open(sio, flags, mode);
}

static int sio_close(FD fd)
{
	struct SioBase* sio = (SioBase*)fd;
	return sio->Protected.Close(sio);
}

static int sio_read(FD fd, char* buffer, size_t maxbytes)
{
	CRITICAL_STORE;
	SioBase* sio = (SioBase*)fd;

	CRITICAL_START();
	int count = ringGet(sio->Protected.rxBuffer, buffer, maxbytes);
	CRITICAL_END();
	
	return count;
}

static int sio_write(FD fd, const char* buffer, size_t nbytes)
{
	CRITICAL_STORE;
	struct SioBase* sio = (SioBase*)fd;

	CRITICAL_START();
	int count = ringPut(sio->Protected.txBuffer, buffer, nbytes);
	CRITICAL_END();

	if (count > 0) {
		sio->Protected.Send(sio);
	}

	return count;
}

static int sio_ioctl(FD fd, int function, void* arg)
{
	struct SioBase* sio = (SioBase*)fd;

	int ret = sio->Protected.Ioctl(sio, function, arg);
	uint8_t *isEmpty = (uint8_t*)arg;

	if( ret == -1 ){
		switch(function){
			case SIO_RX_ISEMPTY:
			*isEmpty = ringIsEmpty(sio->Protected.rxBuffer) ?  1 : 0;
			ret = OK;
			break;
			default:
			ret = NG;
		}
	}

	return (ret);
}


void SioCreate(SioBase* sio, const char *name, RINGBUFFER *rxBuffer, RINGBUFFER *txBuffer, 
            FSioOpen Open, FSioSend Send, FSioClose Close, FSioIoctl Ioctl)
{
	sio->super.popen = sio_open;
	sio->super.pioctl = sio_ioctl;
	sio->super.pread = sio_read;
	sio->super.pwrite = sio_write;
	sio->super.pclose = sio_close;

	sio->Protected.Open = Open;
	sio->Protected.Send = Send;
	sio->Protected.Close = Close;
	sio->Protected.Ioctl = Ioctl;

	IosDrvInstall(&sio->super, name);
	sio->Protected.rxBuffer = rxBuffer;
	sio->Protected.txBuffer = txBuffer;
}