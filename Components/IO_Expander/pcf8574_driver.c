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


bool pcf8574_init(pcf8574_driver_t *driver, I2C_HandleTypeDef *hi2c, uint8_t i2c_address, uint32_t i2c_timeout_ms) {
    if ((driver == NULL) || (hi2c == NULL)) {
        return false;
    }
    
    driver->hi2c = hi2c;
    driver->timeout_ms = i2c_timeout_ms;
    driver->i2c_address = i2c_address << 1;
    driver->current_state = 0x00;
    
    // Test communication by writing 0xFF (all pins high)
    if (pcf8574_write_port(driver, 0xFF)) {
        return true;
    }
    
    return false;
}

bool pcf8574_read_port(pcf8574_driver_t *driver, uint8_t *data) {
    if ((driver == NULL) || (driver->hi2c == NULL) || (data == NULL)) {
        return false;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(
        driver->hi2c,
        driver->i2c_address | 0x01,
        data,
        1,
        driver->timeout_ms
    );
    
    if (status == HAL_OK) {
        driver->current_state = *data;
        return true;
    }
    
    return false;
}

bool pcf8574_write_port(pcf8574_driver_t *driver, uint8_t data)
{
    if ((driver == NULL) || (driver->hi2c == NULL)) {
        return false;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(
        driver->hi2c,
        PCF8574_I2C_ADDRESS,
        &data,
        1,
        driver->timeout_ms
    );
    
    if (status == HAL_OK) {
        driver->current_state = data;
        return true;
    }
    
    return false;
}

bool pcf8574_read_pin(pcf8574_driver_t *driver, uint8_t pin, bool *state)
{
    if ((driver == NULL) || (driver->hi2c == NULL) || (state == NULL)) {
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
    if ((driver == NULL) || (driver->hi2c == NULL)) {
        return false;
    }
    
    uint8_t new_state;
    
    if (set_reset) {
        // Set pin high (OR with pin mask)
        new_state = driver->current_state | pin;
    } else {
        // Set pin low (AND with inverted pin mask)
        new_state = driver->current_state & ~pin;
    }
    
    return pcf8574_write_port(driver, new_state);
}