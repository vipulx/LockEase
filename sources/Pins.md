### Keypad Matrix (4x4)
This section outlines the pin connections for the matrix keypad. The rows are connected to the ESP32's output pins, and the columns are connected to the input pins. The configuration specifies `has_diodes: true`, which is a common practice for matrix keypads.

| Keypad Row | ESP32 GPIO Pin |
|:----------:|:--------------:|
| Row 1      | GPIO5          |
| Row 2      | GPIO4          |
| Row 3      | GPIO3          |
| Row 4      | GPIO2          |

| Keypad Column | ESP32 GPIO Pin |
|:-------------:|:--------------:|
| Column 1      | GPIO8          |
| Column 2      | GPIO9          |
| Column 3      | GPIO7          |
| Column 4      | GPIO6          |

***

### RFID RC522 (SPI)
The RFID reader uses the SPI (Serial Peripheral Interface) protocol for communication. This table details the pin assignments for the SPI bus and the Chip Select (CS) pin.

| RC522 Pin (Label) | ESP32 GPIO Pin | Description           |
|:-----------------:|:--------------:|:----------------------|
| CLK               | GPIO36         | SPI Clock (SCLK)      |
| MOSI              | GPIO34         | Master Out Slave In   |
| MISO              | GPIO33         | Master In Slave Out   |
| CS (SDA)          | GPIO35         | Chip Select           |

***

### OLED Display (SSD1306 I2C)
The OLED display uses the I2C (Inter-Integrated Circuit) communication protocol, requiring two pins: one for data and one for the clock.

| OLED Pin (Label) | ESP32 GPIO Pin | Description        |
|:----------------:|:--------------:|:-------------------|
| SDA              | GPIO17         | I2C Data           |
| SCL              | GPIO18         | I2C Clock          |
| VCC/GND          | 3.3V/GND       | Power supply pins  |

***

### Other Components
This table lists the connections for the remaining components, including the LEDs and the lock relay.

| Component      | ESP32 GPIO Pin | Notes                                    |
|:---------------|:--------------:|:-----------------------------------------|
| Onboard LED    | GPIO15         | Controls the built-in LED on the board.  |
| Status Light   | GPIO16         | WS2812 RGB LED strip (3 LEDs).           |
| Lock Relay     | GPIO19         | Controls the door lock.                  |
