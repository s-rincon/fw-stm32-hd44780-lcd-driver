/**
 * @file app_display.c
 * @brief Application Display Library Implementation
 * 
 * This file implements the high-level display interface that wraps the HD44780 driver
 * and provides application-specific functionality including text centering and
 * standardized message/time display.
 * 
 * @author Santiago Rincon
 * @date 2025-11-07
 */

#include "app_display.h"

#include <string.h>
#include <stdio.h>

#include "main.h"
#include "platform_ops.h"
#include "pcf8574_driver.h"
#include "hd44780_driver.h"
#include "pcf8574_i2c_stm32_adapter.h"
#include "hd44780_pcf8574_generic_adapter.h"

/** External I2C handle (defined in main.c) */
extern I2C_HandleTypeDef hi2c1;

/** Define PCF8574 driver */
static pcf8574_driver_t pcf8574;
      
/** Create PCF8574 adapter context */
static pcf8574_stm32_i2c_context_t pcf8574_stm32_i2c_context = {
	.hi2c = &hi2c1,
};

/** Create HD4780 to PCF8574 adapter context */
static hd44780_pcf8574_context_t hd44780_pcf8574_context = {
    .pcf_drv = &pcf8574
};
    
/** Define HD44780 driver */
static hd44780_driver_t hd44780_lcd;

/**
 * @brief Calculate centered text position and create padded string
 * 
 * @param[in] text Input text to center
 * @param[out] centered_text Output buffer for centered text (must be at least 17 chars)
 */
static void center_text(const char *text, char *centered_text) {
    uint32_t text_len = strlen(text);
    
    /** Clear the output buffer */
    memset(centered_text, ' ', HD44780_TOTAL_COLS);
    centered_text[HD44780_TOTAL_COLS] = '\0';
    
    /** If text is longer than display width, truncate it */
    if (text_len > HD44780_TOTAL_COLS) {
        memcpy(centered_text, text, HD44780_TOTAL_COLS);
        return;
    }
    
    /** Calculate starting position for centering */
    uint8_t start_pos = (HD44780_TOTAL_COLS - text_len) / 2;
    
    /** Copy text to centered position */
    memcpy(centered_text + start_pos, text, text_len);
}

bool display_init(void) {
    // Step 1. Initialize the PCF8574 driver
    if (pcf8574_init(&pcf8574, pcf8574_i2c_stm32_get_interface(), &pcf8574_stm32_i2c_context, PCF8574_I2C_DEFAULT_ADDRESS, 100)) {
        // Step 2. Initialize the HD44780 driver
        return hd44780_init(&hd44780_lcd, hd44780_pcf8574_get_interface(), platform_ops_get_instance(), &hd44780_pcf8574_context);
    }

    return false;
}

bool display_clear(void) {
    return hd44780_clear(&hd44780_lcd);
}

bool display_show_message(const char *message) {
    char centered_text[HD44780_TOTAL_COLS + 1];
    center_text(message, centered_text);
	return hd44780_print_at_line(&hd44780_lcd, centered_text, DISPLAY_TEXT_LINE);
}

bool display_show_time(uint32_t secs) {
    char time_text[HD44780_TOTAL_COLS + 1];
    char centered_text[HD44780_TOTAL_COLS + 1];
    snprintf(time_text, sizeof(time_text), "%02lu:%02lu:%02lu", secs / 3600, (secs % 3600) / 60, secs % 60);
    center_text(time_text, centered_text);
    return hd44780_print_at_line(&hd44780_lcd, centered_text, DISPLAY_TIME_LINE);
}
