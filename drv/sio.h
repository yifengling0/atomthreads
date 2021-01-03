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


#ifndef _SIOH_
#define _SIOH_

#ifdef __cplusplus
extern "C" {
#endif

#include "iosdrv.h"
#include "ringbuffer.h"

/* serial device I/O controls */
#define SIO_BAUD_SET		0x1000
#define SIO_BAUD_GET		0x1001

#define SIO_HW_OPTS_SET		0x1002
#define SIO_HW_OPTS_GET		0x1003

#define SIO_MODE_SET		0x1004
#define SIO_MODE_GET		0x1005

#define SIO_RX_ISEMPTY		0x1006

    typedef enum
    {
        ssb1Bit,
        ssb2Bit,
    } enSioStopBit;

    typedef enum
    {
        spNone,	
        spOdd,
        spEven,
    } enSioParity;

    typedef enum
    {
        seNone = 0x00,	
        seOverRun = 0x01,	
        seParity = 0x02,	
        seFraming = 0x04,	
    } enSioError;

    typedef struct _sioParam {
        const char *name;
        uint32_t baud;
        int dataBits;
        enSioStopBit stopBit;
        enSioParity parity;
    }SIO_PARAM;


    
    typedef struct SioBase SioBase;
    typedef int (*FSioOpen)(SioBase* sio, int flags, int mode);
    typedef void (*FSioSend) (SioBase* sio);
    typedef int (*FSioIoctl)(SioBase*sio, int function, void* arg);
    typedef int (*FSioClose) (SioBase* sio);

    struct SioBase {
        IOSDrv super;
        struct {
            RINGBUFFER *rxBuffer;
            RINGBUFFER *txBuffer;
            FSioOpen Open;
            FSioSend Send;
            FSioClose Close;
            FSioIoctl Ioctl;
        }Protected;
    };

    extern void SioCreate(SioBase* sio, const char *name, RINGBUFFER *rxBuffer, RINGBUFFER *txBuffer, 
            FSioOpen Open, FSioSend Send, FSioClose Close, FSioIoctl Ioctl);

#ifdef __cplusplus
}
#endif

#endif  /* sioh */