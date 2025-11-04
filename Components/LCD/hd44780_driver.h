#ifndef __HD44780_DRIVER_INC_
#define __HD44780_DRIVER_INC_

#include <stdint.h>
#include <stdbool.h>

#include "pcf8574_driver.h"

#define HD44780_STARTUP_TEST_ENABLE 1

/**
 * @brief PCF8574 to HD44780 pin mapping
 */
#define HD44780_PIN_RS      PCF8574_PIN_0  // Register Select
#define HD44780_PIN_RW      PCF8574_PIN_1  // Read/Write (usually tied to GND)
#define HD44780_PIN_EN      PCF8574_PIN_2  // Enable
#define HD44780_PIN_BL      PCF8574_PIN_3  // Backlight
#define HD44780_PIN_D4      PCF8574_PIN_4  // Data bit 4
#define HD44780_PIN_D5      PCF8574_PIN_5  // Data bit 5
#define HD44780_PIN_D6      PCF8574_PIN_6  // Data bit 6
#define HD44780_PIN_D7      PCF8574_PIN_7  // Data bit 7

#define HD44780_TOTAL_COLS  16
#define HD44780_TOTAL_ROWS  2

typedef struct hd44780_driver_ {
    pcf8574_driver_t pcf8574_driver;
    bool backlight_state;                

} hd44780_driver_t;

bool hd44780_init(hd44780_driver_t *lcd, pcf8574_driver_config_t *pcf8574_config);
bool hd44780_send_cmd(hd44780_driver_t *lcd, uint8_t cmd);
bool hd44780_send_data(hd44780_driver_t *lcd, uint8_t data);
bool hd44780_putchar(hd44780_driver_t *lcd, char ch);
bool hd44780_puts(hd44780_driver_t *lcd, const char *str);
bool hd44780_gotoxy(hd44780_driver_t *lcd, uint8_t col, uint8_t row);
bool hd44780_clear(hd44780_driver_t *lcd);
bool hd44780_backlight(hd44780_driver_t *lcd, bool state);
bool hd44780_display_control(hd44780_driver_t *lcd, bool display_on, bool cursor_on, bool blink_on);

bool hd44780_display_text_at_line(hd44780_driver_t *lcd_drv, const char *text, uint8_t line);

#endif /* __HD44780_DRIVER_INC_ */
