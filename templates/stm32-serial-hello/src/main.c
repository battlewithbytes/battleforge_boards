/**
 * STM32F103C8T6 Serial Hello World
 * Prints "Hello World" on USART1 (PA9=TX, PA10=RX)
 * Baud rate: 115200, 8N1
 * System clock: 8MHz (HSI)
 *
 * Connect a USB-Serial adapter:
 *   PA9 (TX) -> RX on adapter
 *   PA10 (RX) -> TX on adapter
 *   GND -> GND
 *
 * Platform provides: startup.s, system.c (SystemInit)
 */

#include "stm32f1xx.h"
#include <stdint.h>

/*===========================================================================
 * UART Functions
 *===========================================================================*/
static void uart_init(void) {
    /* Enable GPIOA and USART1 clocks */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    /* Configure PA9 (TX) as alternate function push-pull, 50MHz */
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4);

    /* Configure PA10 (RX) as input floating */
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |= (0x4 << 8);

    /* 115200 baud @ 8MHz: BRR = 8000000 / 115200 = 69 */
    USART1->BRR = 69;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

static void uart_putc(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

static void uart_puts(const char *str) {
    while (*str) {
        if (*str == '\n') uart_putc('\r');
        uart_putc(*str++);
    }
}

static void delay(volatile uint32_t count) {
    while (count--) __asm__("nop");
}

/*===========================================================================
 * Main Application
 *===========================================================================*/
int main(void) {
    uint32_t counter = 0;

    uart_init();

    uart_puts("\n==============================\n");
    uart_puts("  STM32F103 Serial Hello World\n");
    uart_puts("==============================\n\n");

    while (1) {
        uart_puts("Hello World! Count: ");

        /* Print counter (simple decimal conversion) */
        char buf[12];
        int i = 10;
        buf[11] = '\0';
        uint32_t n = counter++;
        do {
            buf[i--] = '0' + (n % 10);
            n /= 10;
        } while (n > 0 && i >= 0);
        uart_puts(&buf[i + 1]);

        uart_puts("\n");

        delay(500000);  /* ~1 second delay at 8MHz */
    }

    return 0;
}
