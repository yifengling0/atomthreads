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

/*
 *
 *	System log output library 
 *
 *	The system log is displayed on the debugging console.
 */

#ifndef	__SYSLOGH__
#define __SYSLOGH__


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Level
 */
#define LOG_EMERG	0		/* System down */
#define LOG_ALERT	1		/* Error - immediate termination required */
#define LOG_CRIT	2		/* Error - due to failure */
#define LOG_ERR		3		/* Other error */
#define LOG_WARNING	4		/* Caution */
#define LOG_NOTICE	5		/* Reference information */
#define LOG_INFO	6		/* General information */
#define LOG_DEBUG	7		/* Debugging information */

#define LOG_PRIMASK	0x0007U		/* Level mask value */

/*
 * Facility
 */
#define LOG_OTHER	(0 << 3)	/* Other */
#define LOG_KERN	(1 << 3)	/* Kernel */
#define LOG_DRV	    (3 << 3)	/* Device driver */

#define LOG_NFACILITIES	16		/* Facility number */
#define LOG_FACMASK	0x0078U		/* Facility mask value */

/*
 * Generate system log mask value 
 */
#define LOG_MASK(level)		( 1 << (level) )
#define LOG_UPTO(level)		( (1 << ((level) + 1)) - 1 )

/*
 * priority = level | facility
 */
extern void Syslog( int priority, const char *format, ... );
extern void SyslogSetMask( int mask );

#ifdef __cplusplus
}
#endif
#endif /* __SYSLOGH__ */
