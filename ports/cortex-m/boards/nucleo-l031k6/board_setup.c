/*
 * Copyright (c) 2015, Tido Klaassen. All rights reserved.
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

#include <stdbool.h>
#include <errno.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>

#include "atomport.h"
#include "l031k6_usart.h"


/**
 * initialise and start SysTick counter. This will trigger the
 * sys_tick_handler() periodically once interrupts have been enabled
 * by archFirstThreadRestore()
 */
static void systick_setup(void)
{
    systick_set_frequency(SYSTEM_TICKS_PER_SEC, 32000000);

    systick_interrupt_enable();

    systick_counter_enable();
}

/**
 * Set up the core clock to something other than the internal 16MHz PIOSC.
 * Make sure that you use the same clock frequency in  systick_setup().
 */
static void clock_setup(void)
{
    rcc_clock_setup_in_hsi_out();
}

/**
 * Set up user LED and provide function for toggling it. This is for
 * use by the test suite programs
 */
static void test_led_setup(void)
{
    /* LED is connected to GPIO5 on port A */
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO3);
    gpio_set(GPIOB, GPIO3);
}

void test_led_toggle(void)
{
    gpio_toggle(GPIOB, GPIO3);
}

static SIO_PARAM usart2_param = {
    "tty2",
    115200,
    8,
    ssb1Bit,
    spNone
};

static l031k6Usart _l0_usart2;
static FD _debug_port = 0;

void stdcon_setup(void)
{
    _debug_port = open("tty2", 0, 0);
}

/**
 * Callback from your main program to set up the board's hardware before
 * the kernel is started.
 */
int board_setup(void)
{
    /* Disable interrupts. This makes sure that the sys_tick_handler will
     * not be called before the first thread has been started.
     * Interrupts will be enabled by archFirstThreadRestore().
     */
    cm_mask_interrupts(true);

    /* configure system clock, user LED and UART */
    clock_setup();
    test_led_setup();
    //usart_setup(115200);
    l031k6UsartCreate(&_l0_usart2, &usart2_param);
    stdcon_setup();
    /* initialise SysTick counter */
    systick_setup();

    /* Set exception priority levels. Make PendSv the lowest priority and
     * SysTick the second to lowest
     */
    nvic_set_priority(NVIC_PENDSV_IRQ, 0xFF);
    nvic_set_priority(NVIC_SYSTICK_IRQ, 0xFE);
    nvic_set_priority(28, 0xFD);
    nvic_enable_irq(28);

    return 0;
}



/**
 * _read and _write for STM32
 *
 * _read and _write are used by newlib's I/O routines (think printf, etc.)
 * If you want to use this code in your binary, you will have to initialise
 * the UART in your board's setup code and define STD_CON to your UART's
 * name in your board's Makefile
 */
int _read(int fd, void *buf, size_t count)
{
    int rcvd;

    if(fd <= 2){
        rcvd = read(_debug_port, buf, count);
    }else{
        rcvd = -1;
        errno = EIO;
    }

    return rcvd;
}

int _write(int fd, const void *buf, size_t count)
{
    int sent;

    if(fd <= 2){
        sent = write(_debug_port, buf, count);
    }else{
        errno = EIO;
        sent = -1;
    }

    return sent;
}


#include "debug.h"

void PortDebugPutChar(char c)
{
    write(_debug_port, &c, 1);
}

int  PortDebugIsReadable(void)
{
    uint8_t result;
       
    ioctl(_debug_port, SIO_RX_ISEMPTY, &result);

    return !result;
}
 
uint8_t PortDebugRead8(void)
{
    char c;
    read(_debug_port, &c, 1);

    return c;
}


int IsRamReadable(volatile void* address)
{
	return 1;
}

int IsRamReadWriteble(volatile void* address)
{
	return 1;
}
