/**
 * @file pcf8574_i2c_stm32_adapter.c
 * @brief STM32 HAL I2C adapter implementation for PCF8574
 * 
 * This file implements the generic I2C interface using STM32 HAL I2C
 * functions specifically for PCF8574 devices. It provides efficient
 * I2C communication with proper error handling and timeout management.
 * 
 * @author Santiago Rincon
 * @date 2025-11-07
 */

#include "pcf8574_i2c_stm32_adapter.h"

/**
 * @brief Initialize STM32 I2C interface
 * @param context Pointer to pcf8574_stm32_i2c_context_t structure
 * @return true if initialization successful, false otherwise
 */
static bool pcf8574_stm32_i2c_init(void *context) {
	pcf8574_stm32_i2c_context_t *ctx = (pcf8574_stm32_i2c_context_t *)context;
    
    if ((ctx == NULL) || (ctx->hi2c == NULL)) {
        return false;
    }

    /** I2C peripheral is already initialized by HAL_I2C_Init() in main.c */
    
    return true;
}

/**
 * @brief Deinitialize STM32 I2C interface
 * @param context Pointer to pcf8574_stm32_i2c_context_t structure
 * @return true if deinitialization successful, false otherwise
 */
static bool pcf8574_stm32_i2c_deinit(void *context) {
	pcf8574_stm32_i2c_context_t *ctx = (pcf8574_stm32_i2c_context_t *)context;

    if (ctx == NULL) {
        return false;
    }

    ctx->hi2c = NULL;
    return true;
}

/**
 * @brief Write data to I2C device using STM32 HAL
 * @param context Pointer to pcf8574_stm32_i2c_context_t structure
 * @param address I2C device address (8-bit format)
 * @param data Pointer to data buffer to write
 * @param data_len Number of bytes to write
 * @param timeout_ms Timeout in milliseconds
 * @return true if write successful, false otherwise
 */
static bool pcf8574_stm32_i2c_write(void *context, uint8_t address, uint8_t *data, uint32_t data_len, uint32_t timeout_ms) {
	pcf8574_stm32_i2c_context_t *ctx = (pcf8574_stm32_i2c_context_t *)context;

    if ((ctx == NULL) || (ctx->hi2c == NULL) || (data == NULL) || (data_len == 0)) {
        return false;
    }

    return (HAL_I2C_Master_Transmit(ctx->hi2c, address, data, data_len, timeout_ms) == HAL_OK);
}

/**
 * @brief Read data from I2C device using STM32 HAL
 * @param context Pointer to pcf8574_stm32_i2c_context_t structure
 * @param address I2C device address (8-bit format)
 * @param data Pointer to buffer to store read data
 * @param data_len Number of bytes to read
 * @param timeout_ms Timeout in milliseconds
 * @return true if read successful, false otherwise
 */
static bool pcf8574_stm32_i2c_read(void *context, uint8_t address, uint8_t *data, uint32_t data_len, uint32_t timeout_ms) {
	pcf8574_stm32_i2c_context_t *ctx = (pcf8574_stm32_i2c_context_t *)context;

    if ((ctx == NULL) || (ctx->hi2c == NULL) || (data == NULL) || (data_len == 0)) {
        return false;
    }
    return (HAL_I2C_Master_Receive(ctx->hi2c, address, data, data_len, timeout_ms) == HAL_OK);
}

/**
 * @brief Static I2C interface structure for STM32 HAL
 */
const pcf8574_interface_t pcf8574_i2c_stm32_interface = {
    .init = pcf8574_stm32_i2c_init,
    .deinit = pcf8574_stm32_i2c_deinit,
    .write = pcf8574_stm32_i2c_write,
    .read = pcf8574_stm32_i2c_read,
};

/**
 * @brief Get the PCF8574 I2C interface implementation for STM32
 * @return Pointer to the PCF8574 interface structure
 */
const pcf8574_interface_t *pcf8574_i2c_stm32_get_interface(void) {
    return &pcf8574_i2c_stm32_interface;
}
