## LockEase - Your Smart Lock System

[](https://www.google.com/search?q=https://github.com/your-repo-link)

LockEase is a sophisticated smart lock system designed for enhanced home security and seamless integration with Home Assistant. It features multi-factor access control, an intuitive OLED display, and robust security measures to keep your property safe and accessible.

-----

## Features

🔑 **Access & Security Features**

  * **PIN Code Access:**
      * User-defined PINs for arming and disarming the alarm.
      * Input via a responsive 4x4 matrix keypad.
      * Instant status updates (Disarmed / Armed) upon valid PIN entry.
      * Visual feedback (status LED flash) for incorrect PINs.
  * **RFID Access:**
      * RC522 RFID reader for quick and secure access.
      * Predefined RFID tags grant instant "Access Granted" and disarm the system.
      * "Access Denied" feedback for unknown tags.
      * Temporary OLED messages provide clear user guidance.
  * **Admin Access:**
      * Secure Admin PIN (default: 2468) for configuration access.
      * Quick activation via specific keypad sequence (A → B).
      * Dedicated Admin PIN entry mode on the OLED.
      * Admin Menu unlocked for 60 seconds after correct PIN entry, with auto-lock for security.
      * Clear feedback for wrong Admin PINs and locked status.

⚙️ **Admin Menu (Page 5)**

  * Accessible only when `admin_unlocked = true`.
  * OLED menu options include:
      * `Reset Config`
      * `Toggle Display`
      * `Other...`
  * Displays "Admin Locked" when not accessible.

📟 **Display (SSD1306 OLED 128x64)**

  * **Welcome Screen:** "LockEase v1.0" displayed for 8 seconds at boot.
  * **Home Page:** Real-time clock and current alarm status.
  * **Device Info Page:** Displays WiFi IP, signal strength, and lock status.
  * **Home Assistant Data Page:** Placeholder for displaying various Home Assistant entities.
  * **RFID Scanning Page:** Guides users with a "Place your card" prompt.
  * **PIN Entry Page:** Shows entered digits for clarity.
  * **Custom Messages:**
      * Display temporary messages (5s duration) pushed from Home Assistant (`input_text.oled_message`).
      * Features a custom text wrapping function for readability.

💡 **Lights & Indicators**

  * **Onboard LED (GPIO15):** Provides general feedback.
  * **WS2812 RGB LED strip (3 LEDs):**
      * Vibrant status notifications.
      * Blinks green when the device is online.

⏱ **Timers & Auto Behaviors**

  * **Welcome Screen Timeout:** Automatically disappears after 8 seconds.
  * **Auto Return to Home Page:** Returns after 10 seconds of user inactivity.
  * **Admin Unlock Timeout:** Auto-locks Admin Menu after 60 seconds with a clear message.
  * **PIN Collectors Timeout:**
      * User PIN: 5 seconds.
      * Admin PIN: 20 seconds.

🔄 **Relays & Control**

  * **GPIO19 Relay:** Controls the door lock mechanism (Locked/Unlocked states).
  * **Home Assistant Restart Switch:** Allows remote restarting of the ESP device.

🌐 **Home Assistant Integration**

  * **API Encryption:** Ensures secure communication.
  * **Alarm State Reading:** Integrates with `alarm_control_panel.alarmo`.
  * **Custom OLED Messages:** Via `input_text.oled_message`.
  * **Status Publishing:** Publishes `alarm_control`, `entered_pin`, and `last_rfid` states.

📡 **System Info**

  * **WiFi IP & Signal Strength Sensor:** Provides real-time network information.
  * **Device Online Status Sensor:** Monitors device connectivity.
  * **Auto Reconnect + Fallback AP Mode:** Ensures continuous operation with "LockEase v1.0" SSID as a fallback.

-----

## Getting Started

*(Detailed instructions on how to set up and configure LockEase, including hardware requirements, wiring diagrams, and software flashing steps, will go here.)*

-----

## Configuration

*(Information on how to configure PINs, RFID tags, Home Assistant integration, and other settings will be provided here.)*

-----

## Contributing

We welcome contributions\! If you have suggestions for improvements, new features, or bug fixes, please open an issue or submit a pull request.

-----

## License

This project is licensed under the MIT License - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.
