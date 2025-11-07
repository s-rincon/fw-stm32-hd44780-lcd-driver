/**
 * @file app_display.h
 * @brief Application Display Library Header
 * 
 * This library provides a high-level interface for display management in the application.
 * It wraps the HD44780 driver and provides functions for displaying messages and time
 * with automatic text centering.
 * 
 * @author Santiago Rincon
 * @date November 7, 2025
 */

#ifndef __APP_DISPLAY_INC_
#define __APP_DISPLAY_INC_

#include <stdint.h>
#include <stdbool.h>

#define DISPLAY_TEXT_LINE 0
#define DISPLAY_TIME_LINE 1

/**
 * @brief Initialize the application display
 * 
 * This function initializes all necessary components for the display including
 * the HD44780 driver, PCF8574 adapter, and platform operations.
 * 
 * @return true if initialization successful, false otherwise
 */
bool display_init(void);

/**
 * @brief Clear the entire display
 * 
 * @return true if display cleared successfully, false otherwise
 */
bool display_clear(void);

/**
 * @brief Show a message on line 0 with center alignment
 * 
 * @param[in] message Pointer to null-terminated string to display
 * @return true if message displayed successfully, false otherwise
 */
bool display_show_message(const char *message);

/**
 * @brief Show time on line 1 with center alignment
 * 
 * @param[in] secs Time in seconds to display in HH:MM:SS format
 * @return true if time displayed successfully, false otherwise
 */
bool display_show_time(uint32_t secs);

#endif /* __APP_DISPLAY_INC_ */