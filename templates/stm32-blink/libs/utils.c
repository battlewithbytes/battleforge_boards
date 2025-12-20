/**
 * Utility Library Implementation
 *
 * Common utility functions for STM32 projects.
 */

#include "utils.h"

void utils_delay(volatile unsigned int count) {
    while (count--) {
        __asm__("nop");
    }
}

uint32_t utils_square(uint32_t x) {
    return x * x;
}

uint32_t utils_clamp(uint32_t value, uint32_t min, uint32_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
