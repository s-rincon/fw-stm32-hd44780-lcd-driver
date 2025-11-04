/**
 * @file hd44780_pcf8574_driver.c
 * @brief HD44780 LCD Driver using PCF8574 I2C IO Expander
 * 
 * This driver provides the same interface as the original LCD driver but uses
 * the PCF8574 driver for I2C communication instead of direct HAL calls.
 * 
 * @author Santiago Rincón Carreño
 * @date November 4, 2025
 */

#include "hd44780_driver.h"
#include <string.h>

// HD44780 Commands
#define HD44780_CMD_CLEAR_DISPLAY       0x01
#define HD44780_CMD_RETURN_HOME         0x02
#define HD44780_CMD_ENTRY_MODE_SET      0x04
#define HD44780_CMD_DISPLAY_CONTROL     0x08
#define HD44780_CMD_CURSOR_SHIFT        0x10
#define HD44780_CMD_FUNCTION_SET        0x20
#define HD44780_CMD_SET_CGRAM_ADDR      0x40
#define HD44780_CMD_SET_DDRAM_ADDR      0x80

// Entry Mode Set flags
#define HD44780_ENTRY_RIGHT             0x00
#define HD44780_ENTRY_LEFT              0x02
#define HD44780_ENTRY_SHIFT_INCREMENT   0x01
#define HD44780_ENTRY_SHIFT_DECREMENT   0x00

// Display Control flags
#define HD44780_DISPLAY_ON              0x04
#define HD44780_DISPLAY_OFF             0x00
#define HD44780_CURSOR_ON               0x02
#define HD44780_CURSOR_OFF              0x00
#define HD44780_BLINK_ON                0x01
#define HD44780_BLINK_OFF               0x00

// Function Set flags
#define HD44780_8BIT_MODE               0x10
#define HD44780_4BIT_MODE               0x00
#define HD44780_2_LINE                  0x08
#define HD44780_1_LINE                  0x00
#define HD44780_5x10_DOTS               0x04
#define HD44780_5x8_DOTS                0x00

// DDRAM addresses for each row
#define HD44780_ROW0_ADDR               0x00
#define HD44780_ROW1_ADDR               0x40

static bool hd44780_send_en_pulse(HD44780_PCF8574_HandleTypeDef *lcd_drv) {
    if (lcd_drv == NULL) {
        return false;
    }

    HAL_Delay(1);
    if (!pcf8574_write_pin(&lcd_drv->pcf8574_driver, HD44780_PIN_EN, true)) {
        return false;
    }

    HAL_Delay(1);
    if (!pcf8574_write_pin(&lcd_drv->pcf8574_driver, HD44780_PIN_EN, false)) {
        return false;
    }

    HAL_Delay(1);

    return true;
}

static bool hd44780_write_nibble(HD44780_PCF8574_HandleTypeDef *lcd_drv, uint8_t nibble, bool is_data) {
    if (lcd_drv == NULL) {
        return false;
    }

    uint8_t data = (nibble & 0xF0);

    data |= (is_data) ? HD44780_PIN_RS : 0x00;

    data |= (lcd_drv->backlight_state) ? HD44780_PIN_BL : 0x00;

    if (!pcf8574_write_port(&lcd_drv->pcf8574_driver, data)) {
        return false;
    }

    return hd44780_send_en_pulse(lcd_drv);
}

static bool hd44780_write_byte(HD44780_PCF8574_HandleTypeDef *lcd_drv, uint8_t byte, bool is_data) {
    if (lcd_drv == NULL) {
        return false;
    }

    // Send high nibble
    if (!hd44780_write_nibble(lcd_drv, byte & 0xF0, is_data)) {
        return false;
    }

    // Send low nibble
    if (!hd44780_write_nibble(lcd_drv, (byte << 4) & 0xF0, is_data)) {
        return false;
    }

    return true;
}

bool hd44780_init(HD44780_PCF8574_HandleTypeDef *lcd_drv, pcf8574_driver_config_t *pcf8574_config) {
    if (lcd_drv == NULL) {
        return false;
    }

    HAL_Delay(100);

    /* Initialize PCF8574 IO Expander */
    if (!pcf8574_init(&lcd_drv->pcf8574_driver, pcf8574_config)) {
        return false;
    }
    
    /** Write 0x00 at startup */
    if (!pcf8574_write_port(&lcd_drv->pcf8574_driver, 0x00)) {
        return false;
    }
    
    /* Enable backlight by default */
    lcd_drv->backlight_state = true;
    if (!hd44780_backlight(lcd_drv, true)) {
        return false;
    }

    HAL_Delay(10);

    if (!hd44780_write_nibble(lcd_drv, 0x30, 0)) {
        return false;
    }
    HAL_Delay(1);

    if (!hd44780_write_nibble(lcd_drv, 0x30, 0)) {
        return false;
    }
    HAL_Delay(1);

    if (!hd44780_write_nibble(lcd_drv, 0x30, 0)) {
        return false;
    }
    HAL_Delay(1);

    if (!hd44780_write_nibble(lcd_drv, 0x20, 0)) {
        return false;
    }
    HAL_Delay(1);

    // Entry mode set: increment cursor, no shift
    if (!hd44780_send_cmd(lcd_drv, HD44780_CMD_ENTRY_MODE_SET | HD44780_ENTRY_LEFT | HD44780_ENTRY_SHIFT_DECREMENT)) {
        return false;
    }
    
    // Display on, cursor off, blink off
    if (!hd44780_display_control(lcd_drv, true, false, false)) {
        return false;
    }

    return true;
}

bool hd44780_send_cmd(HD44780_PCF8574_HandleTypeDef *lcd_drv, uint8_t cmd) {
    if (lcd_drv == NULL) {
        return false;
    }

    if (!hd44780_write_byte(lcd_drv, cmd, 0)) {
        return false;
    }

    HAL_Delay(5);

    return true;
}

bool hd44780_send_data(HD44780_PCF8574_HandleTypeDef *lcd_drv, uint8_t data) {
    return true;
}

bool hd44780_putchar(HD44780_PCF8574_HandleTypeDef *lcd_drv, char ch) {
    return true;
}

bool hd44780_puts(HD44780_PCF8574_HandleTypeDef *lcd_drv, const char *str) {
    return true;
}

bool hd44780_gotoxy(HD44780_PCF8574_HandleTypeDef *lcd_drv, uint8_t col, uint8_t row) {
    return true;
}

bool hd44780_clear(HD44780_PCF8574_HandleTypeDef *lcd_drv) {
    return true;
}

bool hd44780_backlight(HD44780_PCF8574_HandleTypeDef *lcd_drv, bool state) {
    return true;
}

bool hd44780_display_control(HD44780_PCF8574_HandleTypeDef *lcd_drv, bool display_on, bool cursor_on, bool blink_on) {
    return true;
}
