#ifndef __HD44780_DRIVER_INC_
#define __HD44780_DRIVER_INC_

#include <stdint.h>
#include <stdbool.h>

#include "pcf8574_driver.h"

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

typedef struct {
    pcf8574_driver_t *pcf8574_driver;    
    bool backlight_state;                

} HD44780_PCF8574_HandleTypeDef;

bool hd44780_init(HD44780_PCF8574_HandleTypeDef *lcd);
bool hd44780_send_cmd(HD44780_PCF8574_HandleTypeDef *lcd, uint8_t cmd);
bool hd44780_send_data(HD44780_PCF8574_HandleTypeDef *lcd, uint8_t data);
bool hd44780_putchar(HD44780_PCF8574_HandleTypeDef *lcd, char ch);
bool hd44780_puts(HD44780_PCF8574_HandleTypeDef *lcd, const char *str);
bool hd44780_gotoxy(HD44780_PCF8574_HandleTypeDef *lcd, uint8_t col, uint8_t row);
bool hd44780_clear(HD44780_PCF8574_HandleTypeDef *lcd);
bool hd44780_backlight(HD44780_PCF8574_HandleTypeDef *lcd, bool state);
bool hd44780_display_control(HD44780_PCF8574_HandleTypeDef *lcd, bool display_on, bool cursor_on, bool blink_on);


#endif /* __HD44780_DRIVER_INC_ */