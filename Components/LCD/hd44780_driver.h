/**
 * @file hd44780_driver.h
 * @brief HD44780 LCD Controller Driver Header
 * 
 * This header file provides the interface for controlling HD44780-compatible LCD displays
 * using a hardware abstraction layer. The driver supports 16x2 character displays with
 * 4-bit mode operation and can work with different hardware implementations through
 * the hardware interface abstraction.
 * 
 * @author Santiago Rincon
 * @date November 4, 2025
 */

#ifndef __HD44780_DRIVER_INC_
#define __HD44780_DRIVER_INC_

#include <stdint.h>
#include <stdbool.h>

#include "hd44780_interface.h"
#include "platform_ops.h"

#define HD44780_TOTAL_COLS  16  /** Total number of columns in the display */
#define HD44780_TOTAL_ROWS  2   /** Total number of rows in the display */

/**
 * @brief HD44780 LCD driver structure
 */
typedef struct hd44780_driver_ {
    const hd44780_interface_t *hw_interface;    /**< Hardware interface operations */
    const platform_ops_t *platform_ops;         /**< Platform operations */
    void *hw_context;                           /**< Hardware-specific context data */
    bool backlight_state;                       /**< Current backlight state (true=on, false=off) */
    bool initialized;                           /**< Driver initialization state */
    uint8_t current_col;                        /**< Current cursor column position (0 to HD44780_TOTAL_COLS-1) */
    uint8_t current_row;                        /**< Current cursor row position (0 to HD44780_TOTAL_ROWS-1) */

} hd44780_driver_t;

/**
 * @brief Initialize the HD44780 LCD display
 * 
 * @param[in,out] lcd Pointer to the HD44780 driver structure
 * @param[in] hw_interface Pointer to hardware interface operations
 * @param[in] platform_ops Pointer to platform operations
 * @param[in] hw_context Hardware-specific context data
 * @return true if initialization successful, false otherwise
 */
bool hd44780_init(hd44780_driver_t *lcd, 
                  const hd44780_interface_t *hw_interface, 
                  const platform_ops_t *platform_ops,
                  void *hw_context);

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
 * @brief Set cursor position to specified column and row (alias for hd44780_gotoxy)
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] col Column position (0 to HD44780_TOTAL_COLS-1)
 * @param[in] row Row position (0 to HD44780_TOTAL_ROWS-1)
 * @return true if cursor positioned successfully, false otherwise
 */
bool hd44780_set_cursor(hd44780_driver_t *lcd, uint8_t col, uint8_t row);

/**
 * @brief Print text at current cursor position with column limit and padding
 * 
 * @param[in] lcd Pointer to the HD44780 driver structure
 * @param[in] text Pointer to null-terminated string to display
 * @return true if text printed successfully, false otherwise
 */
bool hd44780_print(hd44780_driver_t *lcd, const char *text);

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
bool hd44780_print_at_line(hd44780_driver_t *lcd_drv, const char *text, uint8_t line);

#endif /* __HD44780_DRIVER_INC_ */
