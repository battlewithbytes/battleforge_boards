/**
 * Utility Library Header
 *
 * Common utility functions for STM32 projects.
 * This demonstrates user header inclusion from a libs folder.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * Simple busy-wait delay
 * @param count Number of loop iterations (approximate delay)
 */
void utils_delay(volatile unsigned int count);

/**
 * Square a number
 * @param x Input value
 * @return x squared
 */
uint32_t utils_square(uint32_t x);

/**
 * Clamp a value between min and max
 * @param value Input value
 * @param min Minimum bound
 * @param max Maximum bound
 * @return Clamped value
 */
uint32_t utils_clamp(uint32_t value, uint32_t min, uint32_t max);

#endif /* UTILS_H */
