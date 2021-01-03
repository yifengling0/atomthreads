/* Memory regions for STM32F411RE, 512K flash, 128K RAM. */

MEMORY
{
    rom (rx) : ORIGIN = 0x08000000, LENGTH = 32K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

/* Include main opencm3 linker script */
INCLUDE libopencm3_stm32l0.ld
