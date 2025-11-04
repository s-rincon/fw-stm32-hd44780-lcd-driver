/**
 * @file hd44780_driver.h
 * @brief HD44780 LCD Controller Driver Header
 * 
 * This header file provides the interface for controlling HD44780-compatible LCD displays
 * using the PCF8574 I2C IO expander. The driver supports 16x2 character displays with
 * 4-bit mode operation to minimize pin usage.
 * 
 * @author Santiago Rincon
 * @date November 4, 2025
 */

#ifndef __HD44780_DRIVER_INC_
#define __HD44780_DRIVER_INC_

#include <stdint.h>
#include <stdbool.h>

#include "pcf8574_driver.h"

/** Enable startup test sequence (0=disabled, 1=enabled) */
#define HD44780_STARTUP_TEST_ENABLE 1   

#define HD44780_PIN_RS      PCF8574_PIN_0   /** Register Select pin */
#define HD44780_PIN_RW      PCF8574_PIN_1   /** Read/Write pin (usually tied to GND) */
#define HD44780_PIN_EN      PCF8574_PIN_2   /** Enable pin */
#define HD44780_PIN_BL      PCF8574_PIN_3   /** Backlight control pin */
#define HD44780_PIN_D4      PCF8574_PIN_4   /** Data bit 4 */
#define HD44780_PIN_D5      PCF8574_PIN_5   /** Data bit 5 */
#define HD44780_PIN_D6      PCF8574_PIN_6   /** Data bit 6 */
#define HD44780_PIN_D7      PCF8574_PIN_7   /** Data bit 7 */


#define HD44780_TOTAL_COLS  16  /** Total number of columns in the display */
#define HD44780_TOTAL_ROWS  2   /** Total number of rows in the display */

/**
 * @brief HD44780 LCD driver structure
 */
typedef struct hd44780_driver_ {
    pcf8574_driver_t pcf8574_driver;  /**< PCF8574 I2C IO expander driver instance */
    bool backlight_state;             /**< Current backlight state (true=on, false=off) */
} hd44780_driver_t;

/**
 * @brief Initialize the HD44780 LCD display
 * 
 * @param[in,out] lcd Pointer to the HD44780 driver structure
 * @param[in] pcf8574_config Pointer to PCF8574 configuration structure
 * @return true if initialization successful, false otherwise
 */
bool hd44780_init(hd44780_driver_t *lcd, pcf8574_driver_config_t *pcf8574_config);

/**
 * @brief Send a command to the HD44780 controller
 *  
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] cmd Command byte to send
 * @return true if command sent successfully, false otherwise
 */
bool hd44780_send_cmd(hd44780_driver_t *lcd, uint8_t cmd);

/**
 * @brief Send data to the HD44780 controller
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] data Data byte to send
 * @return true if data sent successfully, false otherwise
 */
bool hd44780_send_data(hd44780_driver_t *lcd, uint8_t data);

/**
 * @brief Display a single character at current cursor position
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] ch Character to display
 * @return true if character displayed successfully, false otherwise
 */
bool hd44780_putchar(hd44780_driver_t *lcd, char ch);

/**
 * @brief Display a string at current cursor position
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] str Pointer to null-terminated string to display
 * @return true if string displayed successfully, false otherwise
 */
bool hd44780_puts(hd44780_driver_t *lcd, const char *str);

/**
 * @brief Set cursor position to specified column and row
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] col Column position (0 to HD44780_TOTAL_COLS-1)
 * @param[in] row Row position (0 to HD44780_TOTAL_ROWS-1)
 * @return true if cursor positioned successfully, false otherwise
 */
bool hd44780_gotoxy(hd44780_driver_t *lcd, uint8_t col, uint8_t row);

/**
 * @brief Clear the entire display
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @return true if display cleared successfully, false otherwise
 */
bool hd44780_clear(hd44780_driver_t *lcd);

/**
 * @brief Control the display backlight
 * 
 * @param[in,out] lcd Pointer to the HD44780 driver structure
 * @param[in] state Desired backlight state (true=on, false=off)
 * @return true if backlight control successful, false otherwise
 */
bool hd44780_backlight(hd44780_driver_t *lcd, bool state);

/**
 * @brief Control display, cursor, and blink settings
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] display_on Enable/disable entire display (true=on, false=off)
 * @param[in] cursor_on Enable/disable cursor visibility (true=on, false=off)
 * @param[in] blink_on Enable/disable cursor blinking (true=on, false=off)
 * @return true if display control successful, false otherwise
 */
bool hd44780_display_control(hd44780_driver_t *lcd, bool display_on, bool cursor_on, bool blink_on);

/**
 * @brief Display text at specified line
 * 
 * @param[in] lcd_drv Pointer to the HD44780 driver structure
 * @param[in] text Pointer to null-terminated string to display
 * @param[in] line Line number (0 to HD44780_TOTAL_ROWS-1)
 * @return true if text displayed successfully, false otherwise
 */
bool hd44780_display_text_at_line(hd44780_driver_t *lcd_drv, const char *text, uint8_t line);

#endif /* __HD44780_DRIVER_INC_ */
