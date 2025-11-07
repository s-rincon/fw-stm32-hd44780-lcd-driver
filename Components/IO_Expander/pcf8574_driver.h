/**
 * @file pcf8574.h
 * @brief PCF8574 I2C IO Expander Driver for STM32 HAL
 * 
 * Simple driver for the PCF8574 8-bit I2C IO expander. Provides port-level
 * and pin-level operations with internal state caching for efficiency.
 * 
 * @author Santiago Rincón Carreño
 * @date October 30, 2025
 */

#ifndef __PCF8574_DRIVER_INC_
#define __PCF8574_DRIVER_INC_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "pcf8574_interface.h"

/**
 * @brief PCF8574 base I2C address (7-bit address shifted left)
 * @note This corresponds to the write address. For read operations, the HAL
 *       automatically sets the LSB to 1. The actual 7-bit address is 0x27.
 */
#define PCF8574_I2C_DEFAULT_ADDRESS     0x27

/**
 * @brief PCF8574 pin bit masks for individual pin operations
 */
#define PCF8574_PIN_0   0x01    /**< Pin P0 - Bit mask for pin 0 */
#define PCF8574_PIN_1   0x02    /**< Pin P1 - Bit mask for pin 1 */
#define PCF8574_PIN_2   0x04    /**< Pin P2 - Bit mask for pin 2 */
#define PCF8574_PIN_3   0x08    /**< Pin P3 - Bit mask for pin 3 */
#define PCF8574_PIN_4   0x10    /**< Pin P4 - Bit mask for pin 4 */
#define PCF8574_PIN_5   0x20    /**< Pin P5 - Bit mask for pin 5 */
#define PCF8574_PIN_6   0x40    /**< Pin P6 - Bit mask for pin 6 */
#define PCF8574_PIN_7   0x80    /**< Pin P7 - Bit mask for pin 7 */

/**
 * @brief PCF8574 driver handle structure
 * 
 */
typedef struct pcf8574_driver_ {
    const pcf8574_interface_t *hw_interface;
    void *hw_context;
    uint8_t current_output;
    uint8_t i2c_address;
    uint32_t i2c_timeout_ms;
    bool initialized;

} pcf8574_driver_t;

/**
 * @brief Initialize the PCF8574 driver instance
 * @param driver Pointer to the PCF8574 driver handle to initialize
 * @param hw_interface Pointer to the PCF8574 I2C interface structure
 * @param hw_context Pointer to hardware-specific context data
 * @param i2c_address 7-bit I2C address of the PCF8574 device
 * @param i2c_timeout_ms Timeout in milliseconds for I2C transactions
 * @return true if initialization successful, false otherwise
 */
bool pcf8574_init(pcf8574_driver_t *io_drv,
                    const pcf8574_interface_t *hw_interface,
                    void *hw_context,
                    uint8_t i2c_address,
                    uint32_t i2c_timeout_ms);

/**
 * @brief Deinitialize the PCF8574 driver instance
 * @param driver Pointer to the PCF8574 driver handle to deinitialize
 * @return true if deinitialization successful, false otherwise
 */
bool pcf8574_deinit(pcf8574_driver_t *io_drv);

/**
 * @brief Read the entire 8-bit port state
 * @param driver Pointer to the initialized PCF8574 driver handle
 * @param data Pointer to store the read 8-bit port data
 * @return true if read successful, false on I2C communication error
 */
bool pcf8574_read_port(pcf8574_driver_t *io_drv, uint8_t *data);

/**
 * @brief Write the entire 8-bit port state
 * @param driver Pointer to the initialized PCF8574 driver handle
 * @param data 8-bit data to write to the port
 * @return true if write successful, false on I2C communication error
 */
bool pcf8574_write_port(pcf8574_driver_t *io_drv, uint8_t data);

/**
 * @brief Read the state of a specific pin
 * @param driver Pointer to the initialized PCF8574 driver handle
 * @param pin Pin mask (use PCF8574_PIN_x constants)
 * @param state Pointer to store the pin state (true = high, false = low)
 * @return true if read successful, false on I2C communication error
 */
bool pcf8574_read_pin(pcf8574_driver_t *io_drv, uint8_t pin, bool *state);

/**
 * @brief Write to a specific pin (set or reset)
 * @param driver Pointer to the initialized PCF8574 driver handle
 * @param pin Pin mask (use PCF8574_PIN_x constants)
 * @param set_reset Pin action: true = set pin high, false = reset pin low
 * @return true if write successful, false on I2C communication error
 */
bool pcf8574_write_pin(pcf8574_driver_t *io_drv, uint8_t pin, bool set_reset);

#endif /* __PCF8574_DRIVER_INC_ */
