/**
 * @file hd44780_pcf8574_adapter.c
 * @brief Implementation of HD44780 interface adapter for PCF8574 I/O expander
 */

#include "hd44780_pcf8574_adapter.h"

#include <stddef.h>

#include "main.h"

static bool hd44780_pcf8574_init(void *context) {
    hd44780_pcf8574_context_t *ctx = (hd44780_pcf8574_context_t *)context;

    if ((ctx == NULL) || (ctx->pcf_drv == NULL)) {
        return false;
    }
    return pcf8574_write_port(ctx->pcf_drv, 0);
}

static bool hd44780_pcf8574_deinit(void *context) {
    if (context == NULL) {
        return false;
    }

    (void)context;
    return true;
}

static bool hd44780_pcf8574_write_pin(void *context, hd44780_interface_pin_t pin, bool state) {
    hd44780_pcf8574_context_t *ctx = (hd44780_pcf8574_context_t *)context;
    if (ctx == NULL) {
        return false;
    }

    uint8_t bit_mask;

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

static bool hd44780_pcf8574_write_port(void *context, uint8_t data) {
    hd44780_pcf8574_context_t *ctx = (hd44780_pcf8574_context_t *)context;
    if (ctx == NULL) {
        return false;
    }

    return pcf8574_write_port(ctx->pcf_drv, data);
}

static void hd44780_pcf8574_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

const hd44780_interface_t hd44780_pcf8574_interface = {
    .init = hd44780_pcf8574_init,
    .deinit = hd44780_pcf8574_deinit,
    .write_pin = hd44780_pcf8574_write_pin,
    .write_port = hd44780_pcf8574_write_port,
    .delay_ms = hd44780_pcf8574_delay_ms
};
