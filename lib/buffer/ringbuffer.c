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
#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"

#define min(a,b) ((a)<(b))?(a):(b)

void ringCreate(PRINGBUFFER ring, void *buffer, int nbytes)
{
    if (ring != NULL) {
        ring->size = nbytes;
        ring->buffer = buffer;

        ringClear(ring);
    }
}

void ringClear(PRINGBUFFER ring)
{
    ring->to = 0;
    ring->from = 0;
}

int ringGet(PRINGBUFFER ring, char* buffer, int maxbytes)
{
	int count = 0;
	int to = ring->to;
	int tmpBytes;
	int rngPos = 0;

	if (to >= ring->from)
	{
		count = min(maxbytes, to - ring->from);
		memcpy(buffer, &ring->buffer[ring->from], count);
		ring->from += count;
	}
	else
	{
		count = min(maxbytes, ring->size - ring->from);
		memcpy(buffer, &ring->buffer[ring->from], count);
		rngPos = ring->from + count;

		if (rngPos == ring->size)
		{
			tmpBytes = min(maxbytes - count, to);
			memcpy(buffer + count, ring->buffer, tmpBytes);
			ring->from = tmpBytes;
			count += tmpBytes;
		}
		else
			ring->from = rngPos;
	}
	return (count);
}

int ringPut(PRINGBUFFER ring, const char* buffer, int nbytes)
{
	int count = 0;
	int from = ring->from;
	int tmpBytes;
	int rngPos = 0;

	if (from > ring->to)
	{
		count = min(nbytes, from - ring->to - 1);
		memcpy(&ring->buffer[ring->to], buffer, count);
		ring->to += count;
	}
	else if (from == 0)
	{
		count = min(nbytes, ring->size - ring->to - 1);
		memcpy(&ring->buffer[ring->to], buffer, count);
		ring->to += count;
	}
	else
	{
		count = min(nbytes, ring->size - ring->to);
		memcpy(&ring->buffer[ring->to], buffer, count);
		rngPos = ring->to + count;

		if (rngPos == ring->size)
		{
			tmpBytes = min(nbytes - count, from - 1);
			memcpy(ring->buffer, buffer + count, tmpBytes);
			ring->to = tmpBytes;
			count += tmpBytes;
		}
		else
			ring->to = rngPos;
	}
	return (count);
}

int ringIsEmpty(PRINGBUFFER ring)
{
	return (ring->to == ring->from);
}

int ringIsFull(PRINGBUFFER ring)
{
	int n = ring->to - ring->from + 1;

	return ((n == 0) || (n == ring->size));
}

int ringFreeBytes(PRINGBUFFER ring)
{
	int n = ring->from - ring->to - 1;

	if (n < 0) {
		n += ring->size;
	}

	return (n);
}

int ringNewBytes(PRINGBUFFER ring)
{
	int n = ring->to - ring->from;

	if (n < 0) {
		n += ring->size;
	}

	return (n);
}