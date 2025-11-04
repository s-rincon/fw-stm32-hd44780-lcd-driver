/**
 * @file pcf8574.c
 * @brief PCF8574 I2C IO Expander Driver Implementation
 * 
 * Implementation of the PCF8574 8-bit I2C IO expander driver for STM32 HAL.
 * 
 * @author Santiago Rincón Carreño
 * @date October 30, 2025
 */

#include "pcf8574_driver.h"
#include "pcf8574_i2c_interface.h"

bool pcf8574_init(pcf8574_driver_t *driver, pcf8574_driver_config_t *config) {
    if ((driver == NULL) || (config == NULL)) {
        return false;
    }
    
    // Validate interface pointer exists
    if (config->intf == NULL) {
        return false;
    }
    
    // Validate interface function pointers exist
    if ((config->intf->init == NULL) || (config->intf->deinit == NULL) || (config->intf->write == NULL) || (config->intf->read == NULL)) {
        return false;
    }
    
    // Validate configuration parameters
    if ((config->i2c_address == 0) || (config->i2c_address > 0x7F)) {  // Valid 7-bit I2C address range
        return false;
    }
    
    if (config->i2c_timeout_ms == 0) {  // Timeout should be non-zero
        return false;
    }
    
    // Copy configuration to driver
    driver->intf = config->intf;
    driver->hw_instance = config->hw_instance;
    driver->i2c_timeout_ms = config->i2c_timeout_ms;
    driver->current_output = 0x00;
    
    // Convert 7-bit to HAL format (8-bit)
    driver->i2c_address = (config->i2c_address << 1);

    // Initialize I2C interface
    if (!driver->intf->init(driver->hw_instance)) {
        // Clear driver on init failure
        driver->intf = NULL;
        driver->hw_instance = NULL;
        return false;
    }
    
    // Test communication by writing 0xFF (all pins low)
    if (pcf8574_write_port(driver, 0x00)) {
        return true;
    }
    
    // If communication test fails, deinitialize interface
    driver->intf->deinit(driver->hw_instance);
    driver->intf = NULL;
    driver->hw_instance = NULL;
    return false;
}

bool pcf8574_deinit(pcf8574_driver_t *driver) {
    if (driver == NULL) {
        return false;
    }
    
    // Validate interface pointer and deinit function exist
    if ((driver->intf == NULL) || (driver->intf->deinit == NULL)) {
        return false;
    }
    
    // Deinitialize I2C interface
    if (!driver->intf->deinit(driver->hw_instance)) {
        return false;
    }
    
    // Clear driver structure
    driver->intf = NULL;
    driver->hw_instance = NULL;
    driver->i2c_address = 0x00;
    driver->i2c_timeout_ms = 0;
    driver->current_output = 0x00;
    
    return true;
}

bool pcf8574_read_port(pcf8574_driver_t *driver, uint8_t *data) {
    if ((driver == NULL) || (data == NULL)) {
        return false;
    }
    
    // Validate interface pointer and read function exist
    if ((driver->intf == NULL) || (driver->intf->read == NULL)) {
        return false;
    }
    
    // Use interface to read data
    uint8_t read_address = driver->i2c_address | 0x01;
    if (driver->intf->read(driver->hw_instance, read_address, data, 1, driver->i2c_timeout_ms)) {
        driver->current_output = *data; 
        return true;
    }
    
    return false;
}

bool pcf8574_write_port(pcf8574_driver_t *driver, uint8_t data) {
    if (driver == NULL) {
        return false;
    }
    
    // Validate interface pointer and write function exist
    if ((driver->intf == NULL) || (driver->intf->write == NULL)) {
        return false;
    }
    
    uint8_t write_address = driver->i2c_address;
    if (driver->intf->write(driver->hw_instance, write_address, &data, 1, driver->i2c_timeout_ms)) {
        driver->current_output = data;
        return true;
    }
    
    return false;
}

bool pcf8574_read_pin(pcf8574_driver_t *driver, uint8_t pin, bool *state) {
    if ((driver == NULL) || (state == NULL)) {
        return false;
    }
    
    uint8_t port_data;
    if (!pcf8574_read_port(driver, &port_data)) {
        return false;
    }
    
    *state = (port_data & pin) ? true : false;
    return true;
}

bool pcf8574_write_pin(pcf8574_driver_t *driver, uint8_t pin, bool set_reset) {
    if (driver == NULL) {
        return false;
    }
    
    // Validate interface pointer and write function exist (used by pcf8574_write_port)
    if ((driver->intf == NULL) || (driver->intf->write == NULL)) {
        return false;
    }
    
    uint8_t new_state;

    if (set_reset) {
        // Set pin high (OR with pin mask)
        new_state = driver->current_output | pin;
    } else {
        // Set pin low (AND with inverted pin mask)
        new_state = driver->current_output & ~pin;
    }
    
    return pcf8574_write_port(driver, new_state);
}
