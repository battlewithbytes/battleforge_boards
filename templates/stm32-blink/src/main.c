/**
 * STM32F103C8T6 (Blue Pill) LED Blink Example
 * Uses CMSIS headers for proper register definitions.
 * The onboard LED is connected to PC13 (active low).
 *
 * Platform provides: startup.s, system.c (SystemInit)
 */

#include "stm32f1xx.h"
#include <stdint.h>
#include "utils.h"  /* Include user library header */

/*===========================================================================
 * Main Application
 *===========================================================================*/
int main(void) {
    /* Enable GPIOC clock (bit 4 of RCC_APB2ENR) */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* Configure PC13 as output push-pull, max speed 2MHz
     * PC13 is configured in CRH (high register, pins 8-15)
     * Each pin uses 4 bits: CNF[1:0] MODE[1:0]
     * MODE = 0b10 (2MHz output)
     * CNF  = 0b00 (push-pull)
     * PC13 is at bits 20-23 of CRH
     */
    GPIOC->CRH &= ~(0xF << 20);  /* Clear PC13 config bits */
    GPIOC->CRH |= (0x2 << 20);   /* Set MODE=0b10, CNF=0b00 */

    /* Main loop - blink LED */
    while (1) {
        /* Toggle PC13 using ODR (Output Data Register) */
        GPIOC->ODR ^= GPIO_ODR_ODR13;

        /* Delay using library function - adjust count for different blink rates */
        utils_delay(100000);
    }

    return 0;
}
