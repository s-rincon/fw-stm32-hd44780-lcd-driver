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

static bool pcf8574_is_initialized(pcf8574_driver_t *driver) {
    return (driver != NULL) && (driver->initialized);
}

bool pcf8574_init(pcf8574_driver_t *io_drv, 
                    const pcf8574_interface_t *hw_interface,
                    void *hw_context,
                    uint8_t i2c_address,
                    uint32_t i2c_timeout_ms) {
    if (io_drv == NULL) {
        return false;
    }

    io_drv->initialized = false;

    /** Valdiate interface and hw context */
    if ((hw_context == NULL) || (!pcf8574_interface_validate(hw_interface))) {
        return false;
    }

    /** Validate i2c address */
    if ((i2c_address == 0) || (i2c_address > 0x7F)) {
        return false;
    }

    /** Copy configuration to driver */
    io_drv->initialized = true;
    io_drv->hw_interface = hw_interface;
    io_drv->hw_context = hw_context;
    io_drv->i2c_timeout_ms = i2c_timeout_ms;

    /** Convert 7-bit to HAL format */
    io_drv->i2c_address = (i2c_address << 1);

    /** Init interface */
    if (io_drv->hw_interface->init(io_drv->hw_context)) {
        /** Test communication by writing 0xFF (all pins low) */
        if (pcf8574_write_port(io_drv, 0x00)) {
            return true;
        }
    }

    /** Deinit driver if failed */
    pcf8574_deinit(io_drv);
    return false;
}

bool pcf8574_deinit(pcf8574_driver_t *io_drv) {
    if (io_drv == NULL) {
        return false;
    }
    
    /** Deinitialize I2C interface */
    if (!io_drv->hw_interface->deinit(io_drv->hw_context)) {
        return false;
    }

    /** Clear driver structure */
    io_drv->hw_interface = NULL;
    io_drv->hw_context = NULL;
    io_drv->i2c_address = 0x00;
    io_drv->current_output = 0;
    io_drv->i2c_timeout_ms = 0;
    io_drv->initialized = false;
    
    return true;
}

bool pcf8574_read_port(pcf8574_driver_t *io_drv, uint8_t *data) {
    if (!pcf8574_is_initialized(io_drv) || (data == NULL)) {
        return false;
    }

    // Use interface to read data
    uint8_t read_address = io_drv->i2c_address | 0x01;
    if (io_drv->hw_interface->read(io_drv->hw_context, read_address, data, 1, io_drv->i2c_timeout_ms)) {
        io_drv->current_output = *data; 
        return true;
    }
    
    return false;
}

bool pcf8574_write_port(pcf8574_driver_t *io_drv, uint8_t data) {
    if (!pcf8574_is_initialized(io_drv)) {
        return false;
    }
    
    uint8_t write_address = io_drv->i2c_address;
    if (io_drv->hw_interface->write(io_drv->hw_context, write_address, &data, 1, io_drv->i2c_timeout_ms)) {
        io_drv->current_output = data;
        return true;
    }
    
    return false;
}

bool pcf8574_read_pin(pcf8574_driver_t *io_drv, uint8_t pin, bool *state) {
    if ((!pcf8574_is_initialized(io_drv)) || (state == NULL)) {
        return false;
    }
    
    uint8_t port_data;
    if (!pcf8574_read_port(io_drv, &port_data)) {
        return false;
    }
    
    *state = (port_data & pin) ? true : false;
    return true;
}

bool pcf8574_write_pin(pcf8574_driver_t *io_drv, uint8_t pin, bool set_reset) {
    if (!pcf8574_is_initialized(io_drv)) {
        return false;
    }
    
    uint8_t new_state;

    if (set_reset) {
        // Set pin high (OR with pin mask)
        new_state = io_drv->current_output | pin;
    } else {
        // Set pin low (AND with inverted pin mask)
        new_state = io_drv->current_output & ~pin;
    }
    
    return pcf8574_write_port(io_drv, new_state);
}
