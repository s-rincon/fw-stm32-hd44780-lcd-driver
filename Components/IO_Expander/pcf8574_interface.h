#ifndef __PCF8574_INTERFACE_INC_
#define __PCF8574_INTERFACE_INC_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief PCF8574 interface structure
 */
typedef struct pcf8574_interface_ {
    /**
     * @brief Initialize the I2C interface
     * @param[in] interface Pointer to the PCF8574 I2C interface structure
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @return true if initialization successful, false otherwise
     */
    bool (*init)(void *context);

    /**
     * @brief Deinitialize the I2C interface
     * @param[in] interface Pointer to the PCF8574 I2C interface structure
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @return true if deinitialization successful, false otherwise
     */
    bool (*deinit)(void *context);

    /**
     * @brief Write data using the I2C interface
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @param[in] address I2C device address (7-bit address shifted left)
     * @param[in] pdata Pointer to data buffer to write
     * @param[in] data_len Number of bytes to write
     * @param[in] timeout_ms Timeout in milliseconds for the I2C transaction
     * @return true if write successful, false otherwise
     */
    bool (*write)(void *context, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms);

    /**
     * @brief Read data using the I2C interface
     * @param[in] hw_instance Pointer to the hardware-specific I2C instance
     * @param[in] address I2C device address (7-bit address shifted left)
     * @param[out] pdata Pointer to buffer to store read data
     * @param[in] data_len Number of bytes to read
     * @param[in] timeout_ms Timeout in milliseconds for the I2C transaction
     * @return true if read successful, false otherwise
     */
    bool (*read)(void *context, uint8_t address, uint8_t *pdata, uint32_t data_len, uint32_t timeout_ms);

} pcf8574_interface_t;

/**
 * @brief Validate hardware interface completeness
 * 
 * @param[in] interface Pointer to hardware interface structure to validate
 * @retval true Interface is valid and ready for use
 * @retval false Interface is invalid (NULL pointer or missing functions)
 */
static inline bool pcf8574_interface_validate(const pcf8574_interface_t *interface)
{
    return (interface != NULL) &&
           (interface->init != NULL) &&
           (interface->deinit != NULL) &&
           (interface->write != NULL) &&
           (interface->read != NULL);
}

#endif /* __PCF8574_INTERFACE_INC_ */
