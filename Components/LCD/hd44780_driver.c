/**
 * @file hd44780_driver.c
 * @brief HD44780 LCD Controller Driver Implementation
 *
 * This file implements the HD44780 LCD controller driver using PCF8574 I2C IO expander
 * for hardware communication. The implementation provides a complete interface for
 * controlling 16x2 character LCD displays in 4-bit mode.
 *
 * @author Santiago Rincón Carreño
 * @date November 4, 2025
 */

#include "hd44780_driver.h"
#include <string.h>
#include <stdio.h>

/** Definition of commands */
#define HD44780_CMD_CLEAR_DISPLAY       0x01    /** Clear display and return home */
#define HD44780_CMD_RETURN_HOME         0x02    /** Return cursor to home position */
#define HD44780_CMD_ENTRY_MODE_SET      0x04    /** Set cursor move direction */
#define HD44780_CMD_DISPLAY_CONTROL     0x08    /** Display on/off control */
#define HD44780_CMD_CURSOR_SHIFT        0x10    /** Cursor/display shift */
#define HD44780_CMD_FUNCTION_SET        0x20    /** Interface data length, lines, font */
#define HD44780_CMD_SET_CGRAM_ADDR      0x40    /** Set CGRAM address */
#define HD44780_CMD_SET_DDRAM_ADDR      0x80    /** Set DDRAM address */

/** Definition of entry mode values */
#define HD44780_ENTRY_RIGHT             0x00    /** Decrement cursor position */
#define HD44780_ENTRY_LEFT              0x02    /** Increment cursor position */
#define HD44780_ENTRY_SHIFT_INCREMENT   0x01    /** Shift display right */
#define HD44780_ENTRY_SHIFT_DECREMENT   0x00    /** No display shift */

/** Definition of display control values */
#define HD44780_DISPLAY_ON              0x04    /** Display on */
#define HD44780_DISPLAY_OFF             0x00    /** Display off */
#define HD44780_CURSOR_ON               0x02    /** Cursor on */
#define HD44780_CURSOR_OFF              0x00    /** Cursor off */
#define HD44780_BLINK_ON                0x01    /** Cursor blink on */
#define HD44780_BLINK_OFF               0x00    /** Cursor blink off */

/** Definition of values for function set */
#define HD44780_8BIT_MODE               0x10    /** 8-bit interface */
#define HD44780_4BIT_MODE               0x00    /** 4-bit interface */
#define HD44780_2_LINE                  0x08    /** 2-line display */
#define HD44780_1_LINE                  0x00    /** 1-line display */
#define HD44780_5x10_DOTS               0x04    /** 5x10 font */
#define HD44780_5x8_DOTS                0x00    /** 5x8 font */

/** Definition of DDRAM addresses */
#define HD44780_ROW0_ADDR               0x00    /** DDRAM address for row 0 */
#define HD44780_ROW1_ADDR               0x40    /** DDRAM address for row 1 */

/**
 * @brief Check if the HD44780 driver is initialized
 */
static bool hd44780_is_initialized(hd44780_driver_t *lcd_drv) {
    if (lcd_drv == NULL) {
        return false;
    }
    return lcd_drv->initialized;
}

/**
 * @brief Generate enable pulse for HD44780 communication
 *
 * @param[in] lcd_drv Pointer to the HD44780 driver structure
 */
static bool hd44780_send_en_pulse(hd44780_driver_t *lcd_drv) {
    if (lcd_drv == NULL || lcd_drv->hw_interface == NULL || lcd_drv->platform_ops == NULL) {
        return false;
    }

    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_EN, true)) {
        return false;
    }

    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_EN, false)) {
        return false;
    }

    return true;
}

/**
 * @brief Write a 4-bit nibble to HD44780 controller
 * @param lcd_drv Pointer to the HD44780 driver structure
 * @param nibble Nibble to send (upper 4 bits)
 * @param is_data true for data, false for command
 * @return true if nibble sent successfully, false otherwise
 */
static bool hd44780_write_nibble(hd44780_driver_t *lcd_drv, uint8_t nibble, bool is_data) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    /** Set data bits D4-D7 */
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_D4, (nibble & 0x10) != 0)) return false;
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_D5, (nibble & 0x20) != 0)) return false;
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_D6, (nibble & 0x40) != 0)) return false;
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_D7, (nibble & 0x80) != 0)) return false;

    /** Set RS pin (register select) */
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_RS, is_data)) {
        return false;
    }

    /** Set RW pin to write mode */
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_RW, false)) {
        return false;
    }

    /** Set backlight */
    if (!lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_BL, lcd_drv->backlight_state)) {
        return false;
    }

    /** Send enable pulse */
    return hd44780_send_en_pulse(lcd_drv);
}

/**
 * @brief Write a complete byte to HD44780 controller
 *
 * @param[in] lcd_drv Pointer to the HD44780 driver structure
 * @param[in] byte Byte to send
 * @param[in] is_data true for data, false for command
 */
static bool hd44780_write_byte(hd44780_driver_t *lcd_drv, uint8_t byte, bool is_data) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    /** Send high nibble */
    if (!hd44780_write_nibble(lcd_drv, byte & 0xF0, is_data)) {
        return false;
    }

    /** Send low nibble */
    if (!hd44780_write_nibble(lcd_drv, (byte << 4) & 0xF0, is_data)) {
        return false;
    }

    return true;
}

bool hd44780_init(hd44780_driver_t *lcd_drv, const hd44780_interface_t *hw_interface, const platform_ops_t *platform_ops, void *hw_context) {
    if (lcd_drv == NULL) {
        return false;
    }

    lcd_drv->initialized = false;

    if ((hw_interface == NULL) || (platform_ops == NULL) ||
        (!hd44780_interface_validate(hw_interface)) || (!platform_ops_validate(platform_ops))) {
        return false;
    }

    lcd_drv->hw_interface = hw_interface;
    lcd_drv->platform_ops = platform_ops;
    lcd_drv->hw_context = hw_context;
    lcd_drv->initialized = true;

    /** Enable backlight by default */
    lcd_drv->backlight_state = true;
    
    /** Initialize cursor position tracking */
    lcd_drv->current_col = 0;
    lcd_drv->current_row = 0;

    /** Step 1. Power on delay > 100 ms */
    lcd_drv->platform_ops->delay_ms(100);

    /** Initialize hardware interface */
    if (!lcd_drv->hw_interface->init(lcd_drv->hw_context)) {
        return false;
    }

    lcd_drv->platform_ops->delay_ms(10);

    /** Step 2. Instruction 00110000b (30h), then delay > 4.1 ms */
    if (!hd44780_write_nibble(lcd_drv, 0x30, 0)) {
        lcd_drv->initialized = false;
        return false;
    }
    lcd_drv->platform_ops->delay_ms(5);

    /** Step 3. Instruction 00110000b (30h), then delay > 100 us */
    if (!hd44780_write_nibble(lcd_drv, 0x30, 0)) {
        lcd_drv->initialized = false;
        return false;
    }
    lcd_drv->platform_ops->delay_ms(1);

    /** Step 4. Instruction 00110000b (30h), then delay > 100 us */
    if (!hd44780_write_nibble(lcd_drv, 0x30, 0)) {
        lcd_drv->initialized = false;
        return false;
    }
    lcd_drv->platform_ops->delay_ms(1);

    /** Step 5. Instruction 00100000b (20h), then delay > 100 us */
    if (!hd44780_write_nibble(lcd_drv, 0x20, 0)) {
        lcd_drv->initialized = false;
        return false;
    }
    lcd_drv->platform_ops->delay_ms(1);

    /** Step 6. Send Function Set with proper configuration */
    if (!hd44780_send_cmd(lcd_drv, HD44780_CMD_FUNCTION_SET | HD44780_4BIT_MODE | HD44780_2_LINE | HD44780_5x8_DOTS)) {
        lcd_drv->initialized = false;
        return false;
    }

    /** Step 7. Turn off display */
    if (!hd44780_send_cmd(lcd_drv, HD44780_CMD_DISPLAY_CONTROL | HD44780_DISPLAY_OFF)) {
        lcd_drv->initialized = false;
        return false;
    }

    /** Step 8. Clear display */
    if (!hd44780_clear(lcd_drv)) {
        lcd_drv->initialized = false;
        return false;
    }

    /** Step 9. Configure entry mode set: increment cursor, no shift */
    if (!hd44780_send_cmd(lcd_drv, HD44780_CMD_ENTRY_MODE_SET | HD44780_ENTRY_LEFT | HD44780_ENTRY_SHIFT_DECREMENT)) {
        lcd_drv->initialized = false;
        return false;
    }

    /** Step 10-11. Turn display on and turn cursor off */
    if (!hd44780_display_control(lcd_drv, true, false, false)) {
        lcd_drv->initialized = false;
        return false;
    }

    if (!hd44780_set_cursor(lcd_drv, 0, 0)) {
        lcd_drv->initialized = false;
        return false;
    }

    return true;
}

bool hd44780_send_cmd(hd44780_driver_t *lcd_drv, uint8_t cmd) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    if (!hd44780_write_byte(lcd_drv, cmd, false)) {
        return false;
    }

    return true;
}

bool hd44780_send_data(hd44780_driver_t *lcd_drv, uint8_t data) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    if (!hd44780_write_byte(lcd_drv, data, true)) {
        return false;
    }

    return true;
}

bool hd44780_putchar(hd44780_driver_t *lcd_drv, char ch) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    /** Check if we're at the end of the current row */
    if (lcd_drv->current_col >= HD44780_TOTAL_COLS) {
        return false; /** Cannot write beyond column limit */
    }

    /** Send the character */
    if (!hd44780_send_data(lcd_drv, (uint8_t)ch)) {
        return false;
    }

    /** Update position tracking */
    lcd_drv->current_col++;

    return true;
}


/**
 * @brief Print text with column control and padding
 * @param lcd_drv Pointer to the HD44780 driver structure
 * @param text Pointer to null-terminated string to display
 * @return true if text printed successfully, false otherwise
 */
bool hd44780_print(hd44780_driver_t *lcd_drv, const char *text) {
    if ((!hd44780_is_initialized(lcd_drv)) || (text == NULL)) {
        return false;
    }

    uint8_t start_col = lcd_drv->current_col;
    
    /* Calculate how many characters we can write from current position to end of line */
    if (start_col >= HD44780_TOTAL_COLS) {
        return false; /* Already at end of line */
    }
    
    uint8_t chars_available = HD44780_TOTAL_COLS - start_col;
    uint32_t input_len = strlen(text);
    uint8_t text_chars = (input_len < chars_available) ? (uint8_t)input_len : chars_available;
    
    /* Write the text characters (up to available space) */
    for (uint8_t i = 0; i < text_chars; i++) {
        if (!hd44780_putchar(lcd_drv, text[i])) {
            return false;
        }
    }
    
    /* Fill remaining columns with spaces to reach HD44780_TOTAL_COLS */
    for (uint8_t i = text_chars; i < chars_available; i++) {
        if (!hd44780_putchar(lcd_drv, ' ')) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Set cursor position (alias for hd44780_gotoxy)
 * @param lcd_drv Pointer to the HD44780 driver structure
 * @param col Column position (0 to HD44780_TOTAL_COLS-1)
 * @param row Row position (0 to HD44780_TOTAL_ROWS-1)
 * @return true if cursor positioned successfully, false otherwise
 */
bool hd44780_set_cursor(hd44780_driver_t *lcd_drv, uint8_t col, uint8_t row) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    uint8_t address;
    uint8_t valid_col = (col < HD44780_TOTAL_COLS) ? col : 0;
    uint8_t valid_row = (row < HD44780_TOTAL_ROWS) ? row : 0;

    switch (valid_row) {
        case 0:
            address = HD44780_ROW0_ADDR + valid_col;
            break;
        case 1:
            address = HD44780_ROW1_ADDR + valid_col;
            break;
        default:
            return false;
    }

    /* Update position tracking */
    lcd_drv->current_col = valid_col;
    lcd_drv->current_row = valid_row;

    return hd44780_send_cmd(lcd_drv, HD44780_CMD_SET_DDRAM_ADDR | address);
}

bool hd44780_clear(hd44780_driver_t *lcd_drv) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    /* Reset position tracking */
    lcd_drv->current_col = 0;
    lcd_drv->current_row = 0;

    return hd44780_send_cmd(lcd_drv, HD44780_CMD_CLEAR_DISPLAY);
}

bool hd44780_backlight(hd44780_driver_t *lcd_drv, bool state) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    lcd_drv->backlight_state = state;

    return lcd_drv->hw_interface->write_pin(lcd_drv->hw_context, HD44780_PIN_BL, lcd_drv->backlight_state);
}

bool hd44780_display_control(hd44780_driver_t *lcd_drv, bool display_on, bool cursor_on, bool blink_on) {
    if (!hd44780_is_initialized(lcd_drv)) {
        return false;
    }

    uint8_t cmd = HD44780_CMD_DISPLAY_CONTROL;
    cmd |= (display_on) ? HD44780_DISPLAY_ON : 0x00;
    cmd |= (cursor_on) ? HD44780_CURSOR_ON : 0x00;
    cmd |= (blink_on) ? HD44780_BLINK_ON : 0x00;

    return hd44780_send_cmd(lcd_drv, cmd);
}

/**
 * @brief Display text at specified line
 * @param lcd_drv Pointer to the HD44780 driver structure
 * @param text Pointer to null-terminated string to display
 * @param line Line number (0 to HD44780_TOTAL_ROWS-1)
 * @return true if text displayed successfully, false otherwise
 */
bool hd44780_print_at_line(hd44780_driver_t *lcd_drv, const char *text, uint8_t line) {
    if (!hd44780_is_initialized(lcd_drv) || (text == NULL) || (line >= HD44780_TOTAL_ROWS)) {
        return false;
    }

    /** Set cursor to beginning of specified line */
    if (!hd44780_set_cursor(lcd_drv, 0, line)) {
        return false;
    }

    /** Print text with padding to fill the entire line */
    return hd44780_print(lcd_drv, text);
}
