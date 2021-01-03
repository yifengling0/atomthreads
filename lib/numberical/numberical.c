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


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "numberical.h"

int IsHex( char *strSrc )
{
    int ret = 0;
    
    while( '\0' != *strSrc ){
        if( ( *strSrc < '0' || *strSrc > '9' )
        &&  ( *strSrc < 'A' || *strSrc > 'F' )
        &&  ( *strSrc < 'a' || *strSrc > 'f' ) ){
            ret = 1;
            break;
        }else{
            strSrc++;
        }
    }
    return ret;
}


int IsDec( char *strSrc )
{
	int ret = 0;

	while( '\0' != *strSrc ){
		if( *strSrc < '0' || *strSrc > '9' ){
			ret = 1;
			break;
		}else{
			strSrc++;
		}	
	} 
	return ret;
}

int IsLimit( uint32_t dwSrc, uint32_t dwMin, uint32_t dwMax )
{
    int ret = 0;

    if( dwSrc < dwMin || dwSrc > dwMax ){
        ret = 1;
    }

    return ret;
}

static void hex_prefix(char* src, int c)
{
	size_t len = strlen(src);
    size_t remind = c - len;
	char dest[9] = {0};

	while (remind > 0) {
		dest[remind - 1] = '0';
		remind--;
	}

	strcat(dest, src);
	strcpy(src, dest);
}

void IntToString(unsigned long val, char *buf, unsigned radix, int prefix)
{
	char *p;/*pointer to traverse string*/
	char *firstdig;/*pointer to first digit*/
	char temp;/*temp char*/
	unsigned digval;/*value of digit*/
 
	p = buf;
	firstdig = p;/*save pointer to first digit*/
 
	do{
		digval =(unsigned)(val % radix);
		val /= radix;/*get next digit*/
 
		/*convert to ascii and store */
		if(digval > 9)
			*p++ = (char)(digval - 10 + 'A');/*a letter*/
		else 
			*p++ = (char)(digval + '0');/*a digit*/
	}while(val > 0);
 
		/*We now have the digit of the number in the buffer,but in reverse
		order.Thus we reverse them now.*/
 
	*p-- = '\0';/*terminate string;p points to last digit*/
 
	do{
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;/*swap *p and *firstdig*/
		--p;
		++firstdig;/*advance to next two digits*/
	}while(firstdig < p);/*repeat until halfway*/

    if(prefix > 0){
      hex_prefix(buf, prefix);
    }
}


uint32_t StringToInt( char *strSrc )
{
    uint32_t   dwRet = 0;
    uint32_t   dwNum = 0;
    size_t  j       = 0;
    uint32_t   dwTemp   = 0;
    size_t iLen = strlen( strSrc );

    for( size_t idx = 0; idx < iLen; idx++ ){

        if( strSrc[idx] >= '0' && strSrc[idx] <= '9' ){
            dwNum = strSrc[idx] - '0';

        }else if( strSrc[idx] >= 'A' && strSrc[idx] <= 'F' ){
            dwNum = strSrc[idx] - 'A' + 10;

        }else if( strSrc[idx] >= 'a' && strSrc[idx] <= 'f' ){
            dwNum = strSrc[idx] - 'a' + 10;

        }else{
            
        }
        dwTemp = dwNum;
        for( j = iLen - idx - 1; j > 0; j-- ){
            dwTemp *= 16;
        }
        dwRet += dwTemp;
    }
    return dwRet;
}