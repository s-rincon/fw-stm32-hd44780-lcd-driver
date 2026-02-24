/**
 * @file pcf8574_i2c_stm32_adapter.h
 * @brief STM32 HAL I2C adapter for PCF8574 I2C interface
 * 
 * This adapter implements the generic I2C interface using STM32 HAL
 * I2C functions. It provides the bridge between the generic I2C interface
 * and the STM32 HAL I2C driver specifically for PCF8574 usage.
 * 
 * @author Santiago Rincon
 * @date 2025-11-07
 */

#ifndef __PCF8574_I2C_STM32_ADAPTER_INC_
#define __PCF8574_I2C_STM32_ADAPTER_INC_

#include <stdint.h>
#include <stdbool.h>
#include "pcf8574_interface.h"
#include "main.h"

/**
 * @brief STM32 I2C adapter context structure for PCF8574
 */
typedef struct pcf8574_stm32_i2c_context_ {
    I2C_HandleTypeDef *hi2c;                 /**< STM32 HAL I2C handle */
} pcf8574_stm32_i2c_context_t;

/**
 * @brief Get the STM32 I2C interface for PCF8574
 * @return Pointer to the PCF8574 interface structure
 */
const pcf8574_interface_t *pcf8574_i2c_stm32_get_interface(void);

#endif /* __PCF8574_I2C_STM32_ADAPTER_INC_ */
