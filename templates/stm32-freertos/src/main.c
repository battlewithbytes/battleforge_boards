/**
 * STM32F103C8T6 FreeRTOS Example
 * Two tasks blinking the LED at different rates
 *
 * Note: Install FreeRTOS library from the Libraries panel first!
 * Platform provides: startup.s, system.c (SystemInit)
 */

#include "stm32f1xx.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/*===========================================================================
 * FreeRTOS Tasks
 *===========================================================================*/
void vTask1(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        GPIOC->ODR ^= GPIO_ODR_ODR13;  /* Toggle LED */
        vTaskDelay(pdMS_TO_TICKS(500)); /* 500ms delay */
    }
}

void vTask2(void *pvParameters) {
    (void)pvParameters;

    while (1) {
        /* This task just delays - demonstrates multiple tasks */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*===========================================================================
 * Main Application
 *===========================================================================*/
int main(void) {
    /* Enable GPIOC clock */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* Configure PC13 as output push-pull */
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x2 << 20);

    /* Create FreeRTOS tasks */
    xTaskCreate(vTask1, "Task1", 128, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task2", 128, NULL, 1, NULL);

    /* Start the scheduler - this never returns */
    vTaskStartScheduler();

    /* Should never reach here */
    while (1) {}

    return 0;
}
