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


#ifndef _IOH_
#define _IOH_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IO_IN(a)       (*(volatile uint8_t *)(a))
#define IO_OUT(a,c)    (*(volatile uint8_t *)(a)=(c))
#define IO_INW(a)      (*(volatile uint16_t *)(a))
#define IO_OUTW(a,c)   (*(volatile uint16_t *)(a)=(c))
#define IO_INL(a)      (*(volatile uint32_t *)(a))
#define IO_OUTL(a,c)   (*(volatile uint32_t *)(a)=(c))
#define IO_SET(a,c)    (*(volatile uint8_t *)(a)|=(c))
#define IO_CLR(a,c)    (*(volatile uint8_t *)(a)&=~(c))
#define IO_TGL(a,c)   (*(volatile uint8_t *)(a)^=(c))
#define IO_SETW(a,c)   (*(volatile uint16_t *)(a)|=(c))
#define IO_TGLW(a,c)   (*(volatile uint16_t *)(a)^=(c))
#define IO_CLRW(a,c)   (*(volatile uint16_t *)(a)&=~(c))
#define IO_SETL(a,c)   (*(volatile uint32_t *)(a)|=(c))
#define IO_CLRL(a,c)   (*(volatile uint32_t *)(a)&=~(c))
#define IO_TGLL(a,c)   (*(volatile uint32_t *)(a)^=(c))



#ifdef __cplusplus
}
#endif

#endif /* _IOH_ */