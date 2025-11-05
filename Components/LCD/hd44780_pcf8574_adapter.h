/**
 * @file hd44780_adapter_pcf8574.h
 * @brief Adapter layer between HD44780 interface and PCF8574 I/O expander
 *
 * This adapter connects the generic HD44780 hardware interface abstraction
 * with a PCF8574 I²C I/O expander. It allows using the same HD44780 driver
 * with multiple LCD modules by providing separate contexts for each.
 *
 * Each LCD instance can use a unique PCF8574 device (different I²C address or bus).
 *
 * @author Santiago Rincon
 * @date 2025-11-05
 */

#ifndef HD44780_ADAPTER_PCF8574_H
#define HD44780_ADAPTER_PCF8574_H

#include "hd44780_interface.h"
#include "pcf8574_driver.h"
#include <stdint.h>

/**
 * @brief Context for an HD44780 LCD connected via PCF8574
 */
typedef struct {
    pcf8574_driver_t *pcf_drv;          /**< Pointer to PCF8574 device instance */
    
} hd44780_pcf8574_context_t;

/**
 * @brief Global instance of the PCF8574 adapter interface
 *
 * This structure implements the hd44780_interface_t interface using
 * PCF8574-specific operations.
 */
extern const hd44780_interface_t hd44780_pcf8574_interface;

#endif /* HD44780_ADAPTER_PCF8574_H */
