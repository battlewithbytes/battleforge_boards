/**
 * STM32F103C8T6 UART Echo Example
 * Echoes characters received on USART1 (PA9=TX, PA10=RX)
 * Baud rate: 115200
 * System clock: 8MHz (HSI)
 *
 * Platform provides: startup.s, system.c (SystemInit)
 */

#include "stm32f1xx.h"
#include <stdint.h>

/*===========================================================================
 * UART Functions
 *===========================================================================*/
void uart_init(void) {
    /* Enable GPIOA and USART1 clocks */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    /* Configure PA9 (TX) as alternate function push-pull */
    GPIOA->CRH &= ~(0xF << 4);   /* Clear PA9 config */
    GPIOA->CRH |= (0xB << 4);    /* MODE=11 (50MHz), CNF=10 (AF push-pull) */

    /* Configure PA10 (RX) as input floating */
    GPIOA->CRH &= ~(0xF << 8);   /* Clear PA10 config */
    GPIOA->CRH |= (0x4 << 8);    /* MODE=00 (input), CNF=01 (floating) */

    /* Configure USART1: 115200 baud, 8N1 */
    /* Assuming 8MHz clock: BRR = 8000000 / 115200 = 69.44 ~ 69 (0x45) */
    USART1->BRR = 0x45;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void uart_send_char(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

char uart_recv_char(void) {
    while (!(USART1->SR & USART_SR_RXNE));
    return USART1->DR;
}

void uart_send_string(const char *str) {
    while (*str) {
        uart_send_char(*str++);
    }
}

/*===========================================================================
 * Main Application
 *===========================================================================*/
int main(void) {
    uart_init();

    uart_send_string("STM32F103C8T6 UART Echo Ready\r\n");
    uart_send_string("Type characters to echo them back...\r\n\n");

    while (1) {
        char c = uart_recv_char();
        uart_send_char(c);  /* Echo back */

        /* Send newline on carriage return */
        if (c == '\r') {
            uart_send_char('\n');
        }
    }

    return 0;
}
