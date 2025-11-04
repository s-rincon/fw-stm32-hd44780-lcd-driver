/**
 * @file pcf8574_i2c_interface.c
 * @brief PCF8574 I2C Interface Implementation
 * 
 * Implementation of wrapper functions that encapsulate access to the
 * PCF8574 I2C interface structure function pointers.
 * 
 * @author Santiago Rincón Carreño
 * @date October 30, 2025
 */

#include "pcf8574_i2c_interface.h"

#include <stddef.h>

#include "stm32f4xx_hal.h"

bool pcf8574_i2c_interface_init(void *hw_instance) {
    if (hw_instance == NULL) {
        return false;
    }
    UNUSED(hw_instance);
    return true;
}

bool pcf8574_i2c_interface_deinit(void *hw_instance) {
    if (hw_instance == NULL) {
        return false;
    }
    UNUSED(hw_instance);
    return true;
}

bool pcf8574_i2c_interface_write(void *hw_instance, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms) {
    if ((hw_instance == NULL) || (pdata == NULL) || (data_len == 0)) {
        return false;
    }
    return HAL_I2C_Master_Transmit((I2C_HandleTypeDef *)hw_instance, address, pdata, data_len, timeout_ms) == HAL_OK;
}

bool pcf8574_i2c_interface_read(void *hw_instance, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms) {
    if ((hw_instance == NULL) || (pdata == NULL) || (data_len == 0)) {
        return false;
    }
    return HAL_I2C_Master_Receive((I2C_HandleTypeDef *)hw_instance, address, pdata, data_len, timeout_ms) == HAL_OK;
}