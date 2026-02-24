/**
 * @file platform_ops_stm32.c
 * @brief STM32 HAL implementation of platform operations
 * 
 * This file provides STM32 HAL-specific implementations of the common
 * platform operations interface. It serves as a concrete implementation
 * that can be used by any driver requiring platform services on STM32.
 * 
 * @author Santiago Rincon
 * @date 2025-11-07
 */

#include "platform_ops.h"
#include "main.h"

/**
 * @brief STM32 HAL millisecond delay implementation
 */
static void stm32_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

static const platform_ops_t stm32_platform_ops = {
    .delay_ms = stm32_delay_ms
};

const platform_ops_t* platform_ops_get_instance(void) {
    return &stm32_platform_ops;
}
