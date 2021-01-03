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


#include "atomport.h"
#include "event.h"
#include "atom.h"
#include "atomqueue.h"

static ATOM_QUEUE _event_que;
static ATOM_QUEUE *p_event_que = NULL;

void atomExEventInit(uint8_t* buff_ptr, int unit_size, uint32_t max_count)
{
    if (atomQueueCreate(&_event_que, buff_ptr, unit_size, max_count) == ATOM_OK)
    {
        p_event_que = &_event_que;
    }
}

uint8_t atomExSetEvent(ATOM_EX_EVENT* event)
{
    uint8_t ret;

    if (p_event_que != NULL) {
        ret = atomQueuePut(&_event_que, 0, (uint8_t*)event);
    }
    else {
        ret = ATOM_ERR_QUEUE;
    }

    return (ret);
}

uint8_t atomExWaitEvent(ATOM_EX_EVENT* event, int timeout)
{
    uint8_t ret;

    if (p_event_que != NULL) {
        ret = atomQueueGet(&_event_que, timeout, (uint8_t*)event);
    }
    else {
        ret = ATOM_ERR_QUEUE;
    }

    return (ret);
}

uint8_t atomExClearEvent(ATOM_EX_EVENT* event, int timeout)
{
    uint8_t ret = ATOM_OK;

    if (p_event_que != NULL) {
        // clear all message
        while (atomQueueGet(&_event_que, -1,  (uint8_t*)event) == ATOM_OK);
    }
    else {
        ret = ATOM_ERR_QUEUE;
    }

    return (ret);
}

