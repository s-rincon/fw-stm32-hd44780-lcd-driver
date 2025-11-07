/**
 * @file hd44780_pcf8574_adapter.c
 * @brief Implementation of HD44780 interface adapter for PCF8574 I/O expander
 * 
 * This file implements the HD44780 hardware interface using a PCF8574 I²C
 * I/O expander as the communication bridge. It provides pin-level and port-level
 * operations required by the generic HD44780 driver.
 * 
 * The adapter translates HD44780 interface calls into appropriate PCF8574
 * operations, handling the pin mapping between the logical HD44780 pins
 * and the physical PCF8574 I/O pins.
 * 
 * @author Santiago Rincon
 * @date 2025-11-07
 */

#include "hd44780_pcf8574_generic_adapter.h"

#include <stddef.h>
#include "main.h"

/**
 * @brief Initialize the HD44780 PCF8574 adapter interface
 * @param context Pointer to hd44780_pcf8574_context_t structure containing the PCF8574 driver instance
 * @return true if initialization successful, false otherwise
 */
static bool hd44780_pcf8574_init(void *context) {
    hd44780_pcf8574_context_t *ctx = (hd44780_pcf8574_context_t *)context;

    if ((ctx == NULL) || (ctx->pcf_drv == NULL)) {
        return false;
    }
    return pcf8574_write_port(ctx->pcf_drv, 0);
}

/**
 * @brief Deinitialize the HD44780 PCF8574 adapter interface
 * @param context Pointer to hd44780_pcf8574_context_t structure
 * @return true always (cleanup always successful)
 */
static bool hd44780_pcf8574_deinit(void *context) {
    if (context == NULL) {
        return false;
    }

    (void)context;
    return true;
}

/**
 * @brief Write to a specific HD44780 pin through PCF8574
 * @param context Pointer to hd44780_pcf8574_context_t structure
 * @param pin HD44780 logical pin identifier
 * @param state Desired pin state (true = high, false = low)
 * @return true if pin write successful, false otherwise
 */
static bool hd44780_pcf8574_write_pin(void *context, hd44780_interface_pin_t pin, bool state) {
    hd44780_pcf8574_context_t *ctx = (hd44780_pcf8574_context_t *)context;
    if (ctx == NULL) {
        return false;
    }

    uint8_t bit_mask;

    /** Map HD44780 logical pins to PCF8574 physical pins */
    switch (pin) {
        case HD44780_PIN_RS: bit_mask = PCF8574_PIN_0; break;
        case HD44780_PIN_RW: bit_mask = PCF8574_PIN_1; break;
        case HD44780_PIN_EN: bit_mask = PCF8574_PIN_2; break;
        case HD44780_PIN_BL: bit_mask = PCF8574_PIN_3; break;
        case HD44780_PIN_D4: bit_mask = PCF8574_PIN_4; break;
        case HD44780_PIN_D5: bit_mask = PCF8574_PIN_5; break;
        case HD44780_PIN_D6: bit_mask = PCF8574_PIN_6; break;
        case HD44780_PIN_D7: bit_mask = PCF8574_PIN_7; break;
        default: return false;
    }

    return pcf8574_write_pin(ctx->pcf_drv, bit_mask, state);
}

/**
 * @brief Write to all PCF8574 pins simultaneously
 * @param context Pointer to hd44780_pcf8574_context_t structure
 * @param data 8-bit value to write to the PCF8574 port
 * @return true if port write successful, false otherwise
 */
static bool hd44780_pcf8574_write_port(void *context, uint8_t data) {
    hd44780_pcf8574_context_t *ctx = (hd44780_pcf8574_context_t *)context;
    if (ctx == NULL) {
        return false;
    }

    return pcf8574_write_port(ctx->pcf_drv, data);
}

/**
 * @brief Static HD44780 interface structure for PCF8574 adapter
 */
const hd44780_interface_t hd44780_pcf8574_interface = {
    .init = hd44780_pcf8574_init,
    .deinit = hd44780_pcf8574_deinit,
    .write_pin = hd44780_pcf8574_write_pin,
    .write_port = hd44780_pcf8574_write_port
};

/**
 * @brief Get the HD44780 interface implementation for PCF8574 adapter
 * @return Pointer to the HD44780 interface structure
 */
const hd44780_interface_t *hd44780_pcf8574_get_interface(void) {
    return &hd44780_pcf8574_interface;
}
