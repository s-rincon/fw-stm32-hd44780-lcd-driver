/**
 * @file hd44780_interface.h
 * @brief Hardware abstraction interface for HD44780 LCD controllers
 * 
 * This interface provides a hardware-agnostic abstraction layer for HD44780
 * LCD controllers, allowing the same driver logic to work with different
 * hardware implementations such as IO expanders and direct GPIO.
 * 
 * @author Santiago Rincon
 * @date 2025-11-05
 */

#ifndef HD44780_INTERFACE_H
#define HD44780_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief HD44780 interface pin definitions
 */
typedef enum hd44780_interface_pin_ {
    HD44780_PIN_RS = 0x00,  /**< Register Select: 0=Command, 1=Data */
    HD44780_PIN_RW,         /**< Read/Write: 0=Write, 1=Read (usually tied to GND) */
    HD44780_PIN_EN,         /**< Enable: Falling edge triggers LCD operation */
    HD44780_PIN_BL,         /**< Backlight: 0=Off, 1=On */
    HD44780_PIN_D4,         /**< Data bit 4 (LSB in 4-bit mode) */
    HD44780_PIN_D5,         /**< Data bit 5 */
    HD44780_PIN_D6,         /**< Data bit 6 */
    HD44780_PIN_D7,         /**< Data bit 7 (MSB in 4-bit mode) */
} hd44780_interface_pin_t;

/**
 * @brief HD44780 hardware interface operations structure
 * 
 * This structure contains function pointers for all hardware-specific
 * operations required by the HD44780 driver. Different hardware
 * implementations provide their own implementation of these operations
 * through this common interface.
 */
typedef struct hd44780_interface_ {
    /**
     * @brief Initialize the hardware interface
     * 
     * @param[in] context Hardware-specific context data pointer
     * @retval true Initialization successful
     * @retval false Initialization failed
     */
    bool (*init)(void *context);
    
    /**
     * @brief Deinitialize the hardware interface
     * 
     * @param[in] context Hardware-specific context data pointer
     * @retval true Deinitialization successful
     * @retval false Deinitialization failed or not supported
     */
    bool (*deinit)(void *context);
    
    /**
     * @brief Write state to a specific LCD pin
     * 
     * @param[in] context Hardware-specific context data pointer
     * @param[in] pin Pin identifier from hd44780_interface_pin_t enumeration
     * @param[in] state Pin state: true for logic high, false for logic low
     * @retval true Pin write successful
     * @retval false Pin write failed or pin not supported
     */
    bool (*write_pin)(void *context, hd44780_interface_pin_t pin, bool state);
    
    /**
     * @brief Write data to the LCD port
     * 
     * @param[in] context Hardware-specific context data pointer
     * @param[in] data 8-bit data value to write to the LCD port
     * @retval true Port write successful
     * @retval false Port write failed or not supported
     */
    bool (*write_port)(void *context, uint8_t data);
    
} hd44780_interface_t;

/**
 * @brief Validate hardware interface completeness
 * 
 * @param[in] interface Pointer to hardware interface structure to validate
 * @retval true Interface is valid and ready for use
 * @retval false Interface is invalid (NULL pointer or missing functions)
 */
static inline bool hd44780_interface_validate(const hd44780_interface_t *interface)
{
    return (interface != NULL) &&
           (interface->init != NULL) &&
           (interface->deinit != NULL) &&
           (interface->write_pin != NULL) &&
           (interface->write_port != NULL);
}

#endif /* HD44780_INTERFACE_H */
