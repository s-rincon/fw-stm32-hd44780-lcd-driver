/**
 * @file io_expander_interface.h
 * @brief Generic IO Expander Interface Header
 * 
 * Generic interface for IO expanders that allows different IO expander
 * chips to be used interchangeably through a common API.
 * 
 * @author Santiago Rincón Carreño
 * @date October 30, 2025
 */

#ifndef __PCF8574_I2C_INTERFACE_INC_
#define __PCF8574_I2C_INTERFACE_INC_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief PCF8574 I2C interface structure
 * 
 * Function pointer structure that defines the I2C communication interface
 * for the PCF8574 driver. This allows the driver to work with different
 * I2C implementations by providing the necessary low-level functions.
 */
typedef struct pcf8574_i2c_interface_ {
    /**
     * @brief Initialize the I2C interface
     * @param[in] interface Pointer to the PCF8574 I2C interface structure
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @return true if initialization successful, false otherwise
     */
    bool (*init)(void *hw_instance);

    /**
     * @brief Deinitialize the I2C interface
     * @param[in] interface Pointer to the PCF8574 I2C interface structure
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @return true if deinitialization successful, false otherwise
     */
    bool (*deinit)(void *hw_instance);

    /**
     * @brief Write data using the I2C interface
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @param[in] address I2C device address (7-bit address shifted left)
     * @param[in] pdata Pointer to data buffer to write
     * @param[in] data_len Number of bytes to write
     * @param[in] timeout_ms Timeout in milliseconds for the I2C transaction
     * @return true if write successful, false otherwise
     */
    bool (*write)(void *hw_instance, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms);

    /**
     * @brief Read data using the I2C interface
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @param[in] address I2C device address (7-bit address shifted left)
     * @param[out] pdata Pointer to buffer to store read data
     * @param[in] data_len Number of bytes to read
     * @param[in] timeout_ms Timeout in milliseconds for the I2C transaction
     * @return true if read successful, false otherwise
     */
    bool (*read)(void *hw_instance, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms);

} pcf8574_i2c_interface_t;

#endif /* __PCF8574_I2C_INTERFACE_INC_ */