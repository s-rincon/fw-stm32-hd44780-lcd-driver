# STM32 HD44780 LCD Driver with PCF8574 I²C I/O Expander

A modular, hardware-abstracted driver for controlling HD44780-compatible LCD displays through the PCF8574 I²C I/O expander on STM32 microcontrollers.

---

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Hardware Requirements](#hardware-requirements)
- [How to Use](#how-to-use)
  - [Step 1: Copy Driver Files](#step-1-copy-driver-files)
  - [Step 2: Define Adapters](#step-2-define-adapters)
  - [Step 3: Initialize the Display](#step-3-initialize-the-display)
  - [Step 4: Use LCD Functions](#step-4-use-lcd-functions)
- [API Reference](#api-reference)
  - [Display Functions](#display-functions)
  - [HD44780 Driver Functions](#hd44780-driver-functions)
  - [PCF8574 Driver Functions](#pcf8574-driver-functions)
- [Project Structure](#project-structure)
- [License](#license)
- [Author](#author)

---

## Overview

This project provides a complete, production-ready driver stack for interfacing HD44780 LCD displays with STM32 microcontrollers via the PCF8574 I²C I/O expander. The implementation uses a layered architecture with hardware abstraction interfaces, making it portable and reusable across different STM32 families and hardware configurations.

The driver supports 16×2 character displays in 4-bit mode with full control over display operations including text positioning, cursor control, and backlight management.

---

## Features

- 🔌 **Hardware Abstraction Layer**: Generic interfaces allow easy porting to different platforms
- 📦 **Modular Design**: Separate drivers for PCF8574 and HD44780 with clean adapter pattern
- 🔄 **I²C Communication**: Efficient I²C-based control using only 2 wires plus power
- 💡 **Backlight Control**: Software-controlled backlight on/off
- 📝 **Rich Text Functions**: Print strings, characters, and formatted text
- 📍 **Cursor Management**: Position text anywhere on the 16×2 display
- ⚡ **State Caching**: Optimizes I²C transactions by caching output state
- 🧩 **Easy Integration**: Drop-in driver files with minimal configuration
- 🎯 **Application Layer**: High-level display functions with text centering and time display

---

## Architecture

The driver uses a three-layer architecture for maximum flexibility and reusability:

```
┌─────────────────────────────────┐
│   Application Layer             │  ← app_display.c
│   (Text centering, formatting)  │     (Optional high-level wrapper)
├─────────────────────────────────┤
│   HD44780 LCD Driver            │  ← hd44780_driver.c
│   (Display logic, commands)     │     (Generic LCD driver)
├─────────────────────────────────┤
│   HD44780 ↔ PCF8574 Adapter     │  ← hd44780_pcf8574_generic_adapter.c
│   (Pin mapping, bit operations) │     (Hardware adapter)
├─────────────────────────────────┤
│   PCF8574 I/O Expander Driver   │  ← pcf8574_driver.c
│   (Port/pin operations)         │     (Generic I/O expander driver)
├─────────────────────────────────┤
│   PCF8574 ↔ STM32 I²C Adapter   │  ← pcf8574_i2c_stm32_adapter.c
│   (HAL I2C implementation)      │     (Platform-specific adapter)
├─────────────────────────────────┤
│   STM32 HAL I²C                 │  ← STM32 HAL Driver
└─────────────────────────────────┘
```

**Key Benefits:**
- **Portability**: Swap platform adapters to use with different MCUs
- **Reusability**: Use the same HD44780 driver with different I/O expanders or direct GPIO
- **Testability**: Each layer can be tested independently
- **Maintainability**: Clear separation of concerns

---

## Hardware Requirements

### Components
- **STM32 Microcontroller**: Any STM32 with I²C peripheral (tested on STM32F429ZI)
- **HD44780 LCD Display**: 16×2 character LCD (4-bit mode)
- **PCF8574 I/O Expander**: I²C-based 8-bit I/O expander module
- **Pull-up Resistors**: 4.7kΩ on I²C lines (SDA, SCL) if not already present on PCF8574 module

### Connections

**PCF8574 to STM32:**
| PCF8574 | STM32   |
|---------|---------|
| SDA     | I²C SDA |
| SCL     | I²C SCL |
| VCC     | 5V      |
| GND     | GND     |

**PCF8574 to HD44780 LCD:**
| PCF8574 Pin | LCD Pin | Function          |
|-------------|---------|-------------------|
| P0          | RS      | Register Select   |
| P1          | RW      | Read/Write        |
| P2          | EN      | Enable            |
| P3          | BL      | Backlight Control |
| P4          | D4      | Data Bit 4        |
| P5          | D5      | Data Bit 5        |
| P6          | D6      | Data Bit 6        |
| P7          | D7      | Data Bit 7        |

> **Note:** Default I²C address is `0x27`. Adjust in your code if your module uses a different address.

---

## How to Use

### Step 1: Copy Driver Files

Copy the driver files to your STM32 project:

**Required Core Files:**
```
Components/
├── IO_Expander/
│   ├── pcf8574_driver.c
│   ├── pcf8574_driver.h
│   ├── pcf8574_interface.h
│   ├── pcf8574_i2c_stm32_adapter.c
│   └── pcf8574_i2c_stm32_adapter.h
└── LCD/
    ├── hd44780_driver.c
    ├── hd44780_driver.h
    ├── hd44780_interface.h
    ├── hd44780_pcf8574_generic_adapter.c
    └── hd44780_pcf8574_generic_adapter.h
```

**Optional Application Layer:**
```
Components/
└── Common/
    ├── platform_ops.h
    └── platform_ops_stm32.c

Core/
├── Inc/
│   └── app_display.h
└── Src/
    └── app_display.c
```

### Step 2: Define Adapters

In your source file (e.g., `main.c` or dedicated display module), define the driver instances and adapter contexts:

```c
#include "pcf8574_driver.h"
#include "hd44780_driver.h"
#include "pcf8574_i2c_stm32_adapter.h"
#include "hd44780_pcf8574_generic_adapter.h"

/* External I2C handle defined by STM32CubeMX */
extern I2C_HandleTypeDef hi2c1;

/* Step 1: Define PCF8574 driver instance */
static pcf8574_driver_t pcf8574;

/* Step 2: Create PCF8574 adapter context for STM32 HAL */
static pcf8574_stm32_i2c_context_t pcf8574_stm32_i2c_context = {
    .hi2c = &hi2c1,  // Point to your I2C handle
};

/* Step 3: Create HD44780 to PCF8574 adapter context */
static hd44780_pcf8574_context_t hd44780_pcf8574_context = {
    .pcf_drv = &pcf8574  // Point to PCF8574 driver instance
};

/* Step 4: Define HD44780 driver instance */
static hd44780_driver_t hd44780_lcd;
```

### Step 3: Initialize the Display

Initialize the driver stack from bottom to top:

```c
bool lcd_init(void) {
    /* Initialize PCF8574 I/O Expander */
    if (!pcf8574_init(
            &pcf8574,                           // Driver instance
            pcf8574_i2c_stm32_get_interface(),  // STM32 I2C adapter
            &pcf8574_stm32_i2c_context,         // STM32 I2C context
            PCF8574_I2C_DEFAULT_ADDRESS,        // I2C address (0x27)
            100)) {                             // Timeout in ms
        return false;
    }

    /* Initialize HD44780 LCD */
    if (!hd44780_init(
            &hd44780_lcd,                       // Driver instance
            hd44780_pcf8574_get_interface(),    // PCF8574 adapter
            platform_ops_get_instance(),        // Platform operations (delay)
            &hd44780_pcf8574_context)) {        // PCF8574 context
        return false;
    }

    return true;
}
```

### Step 4: Use LCD Functions

Once initialized, use the LCD driver functions:

```c
int main(void) {
    /* Initialize HAL and peripherals */
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();

    /* Initialize LCD */
    if (!lcd_init()) {
        // Handle initialization error
        Error_Handler();
    }

    /* Clear display and turn on backlight */
    hd44780_clear(&hd44780_lcd);
    hd44780_backlight(&hd44780_lcd, true);

    /* Display text on line 0 */
    hd44780_set_cursor(&hd44780_lcd, 0, 0);
    hd44780_print(&hd44780_lcd, "Hello, STM32!");

    /* Display text on line 1 */
    hd44780_set_cursor(&hd44780_lcd, 0, 1);
    hd44780_print(&hd44780_lcd, "LCD Driver v1.0");

    while (1) {
        /* Your application code */
    }
}
```

**Using the Application Layer (Optional):**

For higher-level text operations with centering:

```c
#include "app_display.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_I2C1_Init();

    /* Initialize display using application layer */
    if (!display_init()) {
        Error_Handler();
    }

    /* Show centered message on line 0 */
    display_show_message("Welcome!");

    /* Show time in HH:MM:SS format on line 1 */
    display_show_time(3661);  // Displays "01:01:01"

    while (1) {
        /* Update display every second */
        static uint32_t seconds = 0;
        display_show_time(seconds++);
        HAL_Delay(1000);
    }
}
```

---

## API Reference

### Display Functions

Application-layer functions for simplified display operations:

```c
/* Initialize the complete display stack */
bool display_init(void);

/* Clear the entire display */
bool display_clear(void);

/* Show centered message on line 0 */
bool display_show_message(const char *message);

/* Show time in HH:MM:SS format on line 1 */
bool display_show_time(uint32_t secs);
```

### HD44780 Driver Functions

Core LCD driver functions:

```c
/* Initialize HD44780 display */
bool hd44780_init(hd44780_driver_t *lcd, 
                  const hd44780_interface_t *hw_interface,
                  const platform_ops_t *platform_ops,
                  void *hw_context);

/* Send command to LCD */
bool hd44780_send_cmd(hd44780_driver_t *lcd, uint8_t cmd);

/* Send data byte to LCD */
bool hd44780_send_data(hd44780_driver_t *lcd, uint8_t data);

/* Display single character at current position */
bool hd44780_putchar(hd44780_driver_t *lcd, char ch);

/* Set cursor to column and row */
bool hd44780_set_cursor(hd44780_driver_t *lcd, uint8_t col, uint8_t row);

/* Print null-terminated string at current position */
bool hd44780_print(hd44780_driver_t *lcd, const char *text);

/* Print text at specific line (0 or 1) */
bool hd44780_print_at_line(hd44780_driver_t *lcd, const char *text, uint8_t line);

/* Clear entire display */
bool hd44780_clear(hd44780_driver_t *lcd);

/* Control backlight (true=on, false=off) */
bool hd44780_backlight(hd44780_driver_t *lcd, bool state);

/* Control display, cursor, and blink */
bool hd44780_display_control(hd44780_driver_t *lcd, 
                              bool display_on, 
                              bool cursor_on, 
                              bool blink_on);
```

### PCF8574 Driver Functions

I/O expander driver functions:

```c
/* Initialize PCF8574 driver */
bool pcf8574_init(pcf8574_driver_t *io_drv,
                  const pcf8574_interface_t *hw_interface,
                  void *hw_context,
                  uint8_t i2c_address,
                  uint32_t i2c_timeout_ms);

/* Read entire 8-bit port */
bool pcf8574_read_port(pcf8574_driver_t *io_drv, uint8_t *data);

/* Write entire 8-bit port */
bool pcf8574_write_port(pcf8574_driver_t *io_drv, uint8_t data);

/* Read single pin state */
bool pcf8574_read_pin(pcf8574_driver_t *io_drv, uint8_t pin, bool *state);

/* Write single pin state */
bool pcf8574_write_pin(pcf8574_driver_t *io_drv, uint8_t pin, bool state);

/* Toggle single pin */
bool pcf8574_toggle_pin(pcf8574_driver_t *io_drv, uint8_t pin);
```

---

## Project Structure

```
fw-stm32-pcf8574-lcd-driver/
├── Components/                         # Reusable driver components
│   ├── Common/
│   │   ├── platform_ops.h              # Platform abstraction header
│   │   └── platform_ops_stm32.c        # STM32 platform operations
│   ├── IO_Expander/
│   │   ├── pcf8574_driver.c            # PCF8574 driver implementation
│   │   ├── pcf8574_driver.h            # PCF8574 driver header
│   │   ├── pcf8574_interface.h         # PCF8574 hardware interface
│   │   ├── pcf8574_i2c_stm32_adapter.c # STM32 HAL I2C adapter
│   │   └── pcf8574_i2c_stm32_adapter.h # STM32 HAL I2C adapter header
│   └── LCD/
│       ├── hd44780_driver.c            # HD44780 driver implementation
│       ├── hd44780_driver.h            # HD44780 driver header
│       ├── hd44780_interface.h         # HD44780 hardware interface
│       ├── hd44780_pcf8574_generic_adapter.c # HD44780↔PCF8574 adapter
│       └── hd44780_pcf8574_generic_adapter.h # HD44780↔PCF8574 header
├── Core/                               # STM32 application code
│   ├── Inc/
│   │   ├── app_display.h               # Application display header
│   │   ├── main.h                      # Main header
│   │   └── stm32f4xx_hal_conf.h        # HAL configuration
│   └── Src/
│       ├── app_display.c               # Application display implementation
│       ├── main.c                      # Main application entry point
│       └── stm32f4xx_hal_msp.c         # HAL MSP initialization
├── Drivers/                            # STM32 HAL drivers
│   ├── CMSIS/                          # ARM CMSIS headers
│   └── STM32F4xx_HAL_Driver/           # STM32F4 HAL library
├── Debug/                              # Build outputs
├── HD44780-Driver.ioc                  # STM32CubeMX project file
└── README.md                           # This file
```

---

## License

This project is provided as-is for educational and development purposes.

---

## Author

**Santiago Rincón Carreño**  

Embedded Software Developer

🌐 [GitHub Account](https://github.com/s-rincon)  

💼 [LinkedIn](https://www.linkedin.com/in/santiago-rinconc)

📧 [Gmail](mailto:santiagorinconc.05@gmail.com)

---

## Version

Current Release: **v1.0**

For project updates and version history, see the commit log.