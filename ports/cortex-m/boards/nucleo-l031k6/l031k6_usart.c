#include "iosdrv.h"

#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>

#include "l031k6_usart.h"
#include "atom.h"

#define USE_SEND_BLOCKING (1) 
#define TMP_SIZE (10)

#define RX_BUF_SIZE (256)
#define TX_BUF_SIZE (64)

static uint8_t _rxBuffer[RX_BUF_SIZE];
static uint8_t _txBuffer[TX_BUF_SIZE];

static RINGBUFFER _rxRingBuffer;
static RINGBUFFER _txRingBuffer;

static int usart_close (SioBase* sio)
{
    usart_disable_rx_interrupt(USART2);
    usart_disable_tx_interrupt(USART2);
    usart_disable(USART2);

    return (OK);
}

static int usart_open(SioBase* sio, int flags, int mode)
{
    usart_enable_rx_interrupt(USART2);
    usart_enable(USART2);

    return (TRUE);
}

static void usart_do_send (SioBase* sio)
{
#if USE_SEND_BLOCKING
    char buffer[TMP_SIZE];
    int count = 0;
    CRITICAL_STORE;
    
    CRITICAL_START();
    do{
        count = ringGet(&_txRingBuffer, buffer, TMP_SIZE);
        for(int i=0; i < count; i++){
            usart_send_blocking(USART2, buffer[i]);
        }
    }while(count > 0);
    CRITICAL_END();
#else
    usart_enable_tx_interrupt(USART2);
#endif
}

static int usart_ioctl(SioBase*sio, int function, void* arg)
{
    return NG;
}

static int get_stop_bit(enSioStopBit stopBit)
{
    if(stopBit == ssb2Bit){
        return USART_CR2_STOP_2_0BIT;
    }else{
        return USART_CR2_STOP_1_0BIT;
    }
}

static int get_parity(enSioParity parity)
{
    int ret = USART_PARITY_NONE;
    switch(parity){
        case spNone:
        ret = USART_PARITY_NONE;
        break;
        case spOdd:
        ret = USART_PARITY_ODD;
        break;
        case spEven:
        ret = USART_PARITY_EVEN;
        break;
        default:
        break;
    }

    return (ret);
}


void usart2_handler(void)
{
    CRITICAL_STORE;

    if((USART_ISR(USART2) & USART_ISR_RXNE) == USART_ISR_RXNE){
        uint8_t c = usart_recv(USART2);
        CRITICAL_START();
        ringPut(&_rxRingBuffer, (char*)&c, 1);
        CRITICAL_END();
    }


    if((USART_ISR(USART2) & USART_ISR_TXE) == USART_ISR_TXE){
        int count = 0;
        char c;
        CRITICAL_START();
        count = ringGet(&_txRingBuffer, &c, 1);
        CRITICAL_END();
        if( count > 0){
            usart_send(USART2, c);
        }else{
            usart_disable_tx_interrupt(USART2);
        }
    }
}


#pragma weak usart2_isr = usart2_handler

void l031k6UsartCreate(l031k6Usart* sio, SIO_PARAM *param)
{
    ringCreate(&_rxRingBuffer, _rxBuffer, RX_BUF_SIZE);
	ringCreate(&_txRingBuffer, _txBuffer, TX_BUF_SIZE);

    SioCreate(&sio->super, param->name, &_rxRingBuffer, &_txRingBuffer, usart_open, usart_do_send,
            usart_close, usart_ioctl);

    memcpy((void*)&sio->Protected.param, (void*)param, sizeof(SIO_PARAM));

    rcc_peripheral_enable_clock(&RCC_IOPENR, RCC_IOPENR_IOPAEN);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART2EN);
    rcc_periph_clock_enable(RCC_USART2);

    usart_disable(USART2);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF , GPIO_PUPD_NONE, GPIO2 | GPIO15);
    gpio_set_af(GPIOA, GPIO_AF4, GPIO2 | GPIO15);

    usart_set_baudrate(USART2, param->baud);
    usart_set_databits(USART2, param->dataBits);
    usart_set_parity(USART2, get_parity(param->parity));
    usart_set_stopbits(USART2, get_stop_bit(param->stopBit));
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

}

void l031k6UsartGetParam(l031k6Usart* sio, SIO_PARAM *param)
{
    memcpy(param, &sio->Protected.param, sizeof(SIO_PARAM));
}