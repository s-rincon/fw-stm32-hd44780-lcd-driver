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

bool pcf8574_i2c_interface_init(pcf8574_i2c_interface_t *interface, void *hw_instance) {
    if ((interface == NULL) || (interface->init == NULL)) {
        return false;
    }
    
    return interface->init(hw_instance);
}

bool pcf8574_i2c_interface_deinit(pcf8574_i2c_interface_t *interface, void *hw_instance) {
    if ((interface == NULL) || (interface->deinit == NULL) || (hw_instance == NULL)) {
        return false;
    }
    
    return interface->deinit(hw_instance);
}

bool pcf8574_i2c_interface_write(pcf8574_i2c_interface_t *interface, void *hw_instance, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms) {
    if ((interface == NULL) || (interface->write == NULL) || (hw_instance == NULL) || (pdata == NULL) || (data_len == 0)) {
        return false;
    }
    
    return interface->write(hw_instance, address, pdata, data_len, timeout_ms);
}

bool pcf8574_i2c_interface_read(pcf8574_i2c_interface_t *interface, void *hw_instance, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms) {
    if ((interface == NULL) || (interface->write == NULL) || (hw_instance == NULL) || (pdata == NULL) || (data_len == 0)) {
        return false;
    }
    
    return interface->read(hw_instance, address, pdata, data_len, timeout_ms);
}