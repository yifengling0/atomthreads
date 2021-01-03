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


#include "pin.h"
#include "io.h"
#include "kassert.h"

static int pin_read( const Pin* pin )
{
    int ret = -1;

    switch(pin->Protected.Width){
        case W8:
        ret = IO_IN(pin->Protected.Port) & pin->Protected.Bit;
        break;
        case W16:
        ret = IO_INW(pin->Protected.Port) & pin->Protected.Bit;
        break;
        case W32:
        ret = IO_INL(pin->Protected.Port) & pin->Protected.Bit;
        break;
        default:
        kassert(0);
        break;
    }

    return ret;
}

static void pin_set(const Pin* pin)
{
    switch(pin->Protected.Width){
        case W8:
        IO_SET(pin->Protected.Port, pin->Protected.Bit);
        break;
        case W16:
        IO_SETW(pin->Protected.Port, pin->Protected.Bit);
        break;
        case W32:
        IO_SETL(pin->Protected.Port, pin->Protected.Bit);
        break;
        default:
        kassert(0);
        break;
    }
}

static void pin_clear(const Pin* pin)
{
    switch(pin->Protected.Width){
        case W8:
        IO_CLR(pin->Protected.Port, pin->Protected.Bit);
        break;
        case W16:
        IO_CLRW(pin->Protected.Port, pin->Protected.Bit);
        break;
        case W32:
        IO_CLRL(pin->Protected.Port, pin->Protected.Bit);
        break;
        default:
        kassert(0);
        break;
    }
}

static	void pin_write( const Pin* pin, int data )
{
    if( data > 0 ){
        pin_set(pin);
    }else{
        pin_clear(pin);
    }
}
    
static void pin_toggle( const Pin* pin )
{
    switch(pin->Protected.Width){
        case W8:
        IO_TGL(pin->Protected.Port, pin->Protected.Bit);
        break;
        case W16:
        IO_TGLW(pin->Protected.Port, pin->Protected.Bit);
        break;
        case W32:
        IO_TGLL(pin->Protected.Port, pin->Protected.Bit);
        break;
        default:
        kassert(0);
        break;
    }
}

void PinCreate(Pin* pin, void *port, enPortWidth width, int bit)
{
    pin->read = pin_read;
    pin->write = pin_write;
    pin->toggle = pin_toggle;

    pin->Protected.Width = width;
    pin->Protected.Bit = bit;
    pin->Protected.Port = port;
}