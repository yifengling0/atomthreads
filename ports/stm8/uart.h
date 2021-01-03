
#include "atom.h"


/*
 * Perform UART startup initialization.
 */
extern int	uart_init(uint32_t baudrate);
extern char uart_getchar ();
extern int uart_have_data();
extern char uart_putchar (char c);
extern void debug_string(char *str);
extern void debug_value_string(char* prompt, uint32_t value, int radix, int prefix, int newline);

