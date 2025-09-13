esphome:
  name: doordash
  friendly_name: DoorDash

esp32:
  board: lolin_s2_mini
  framework:
    type: esp-idf

# Enable logging
logger:

# Enable Home Assistant API
api:
  id: homeassistant_api
  encryption:
    key: "rR1cNifOJ4A7CzSfwLfX+H6uWZWU59znkl8OtoFJWts="

ota:
  - platform: esphome
    password: "09a4224b996964a522cddf9dc9efa810"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  ap:
    ssid: "Doordash Fallback Hotspot"
    password: "Xl8dbl2PNe66"
captive_portal:

# ------------------------
# Globals
# ------------------------
globals:
  - id: disarm_code
    type: std::string
    initial_value: '"0152"'
  - id: arm_code
    type: std::string
    initial_value: '"9999"'
  - id: show_welcome
    type: bool
    initial_value: 'true'

  - id: current_pin
    type: std::string
    initial_value: ""

  - id: entering_pin_page
    type: bool
    initial_value: 'false'

# ------------------------
# Time
# ------------------------
time:
  - platform: homeassistant
    id: home_time

# ------------------------
# Interval to remove welcome screen after 2s
# ------------------------
interval:
  - interval: 5s
    then:
      - lambda: |-
          id(show_welcome) = false;

# ------------------------
# Lights
# ------------------------
light:
  - platform: esp32_rmt_led_strip
    rgb_order: GRB
    chipset: WS2812
    pin: GPIO16
    num_leds: 3
    name: "Status Light"
    id: status_light
    restore_mode: ALWAYS_OFF

# ------------------------
# Text Sensors
# ------------------------
text_sensor:
  - platform: template
    id: alarm_control
    name: "Alarm Control"

  - platform: template
    id: entered_pin
    name: "Entered PIN"

  - platform: template
    id: last_rfid
    name: "Last RFID Tag"

  - platform: homeassistant
    id: alarm_status
    entity_id: alarm_control_panel.alarmo
    internal: true

  - platform: wifi_info
    ip_address:
      name: "LockEase IP"
      id: my_ip

# ------------------------
# RFID RC522
# ------------------------
spi:
  clk_pin: GPIO36   # SCK
  mosi_pin: GPIO34  # MOSI
  miso_pin: GPIO33  # MISO


rc522_spi:
  cs_pin: GPIO35 #SDA
  update_interval: 1s
  id: my_rfid
  on_tag:
    then:
      - text_sensor.template.publish:
          id: last_rfid
          state: !lambda 'return x;'
      - lambda: |-
          ESP_LOGI("rfid", "RFID Tag detected: %s", x.c_str());

          // Example: Allow specific RFID UIDs
          if (x == "32-CA-68-3F" || x == "42-11-E0-3F") {
            id(alarm_control).publish_state("Disarmed");
            ESP_LOGI("rfid", "System Disarmed by RFID");
          } else {
            id(status_light).turn_on();
            ESP_LOGW("rfid", "Unauthorized RFID: %s", x.c_str());
          }

      - delay: 1s
      - light.turn_off: status_light

  on_tag_removed:
    then:
      - lambda: |-
          ESP_LOGI("rfid", "RFID Tag removed");


# ------------------------
# Keypad Matrix (4x4)
# ------------------------
matrix_keypad:
  id: my_keypad
  rows:
    - pin: GPIO5
    - pin: GPIO4
    - pin: GPIO3
    - pin: GPIO2
  columns:
    - pin: GPIO8
    - pin: GPIO9
    - pin: GPIO7
    - pin: GPIO6
  keys: "123A456B789C*0#D"
  has_diodes: true
  on_key:
    - lambda: |-
        if (x != '*' && x != '#') {
          if (!id(entering_pin_page)) {
            id(entering_pin_page) = true;
          }
          id(current_pin) += x;
          ESP_LOGD("keypad", "Current PIN input: %s", id(current_pin).c_str());
        }

key_collector:
  - id: pincode_reader
    source_id: my_keypad
    min_length: 4
    max_length: 4
    end_keys: "#"
    end_key_required: false
    clear_keys: "*"
    allowed_keys: "0123456789"
    timeout: 5s
    on_result:
      - text_sensor.template.publish:
          id: entered_pin
          state: !lambda 'return x;'
      - lambda: |-
          ESP_LOGI("keypad", "Final PIN entered: %s", x.c_str());
          if (x == id(disarm_code)) {
            id(alarm_control).publish_state("Disarmed");
            ESP_LOGI("keypad", "System Disarmed by PIN");
          } else if (x == id(arm_code)) {
            id(alarm_control).publish_state("Armed");
            ESP_LOGI("keypad", "System Armed by PIN");
          } else {
            ESP_LOGW("keypad", "Invalid PIN Entered");
            id(status_light).turn_on();
          }
          id(current_pin) = "";
          id(entering_pin_page) = false;

      - delay: 1s
      - light.turn_off: status_light

    on_timeout:
      - lambda: |-
          ESP_LOGW("keypad", "PIN entry timeout, resetting input");
          id(current_pin) = "";
          id(entering_pin_page) = false;


# ------------------------
# OLED Display (SSD1306 0.96")
# ------------------------
i2c:
  sda: GPIO17
  scl: GPIO18
  scan: true

display:
  - platform: ssd1306_i2c
    model: "SSD1306 128x64"
    address: 0x3C
    id: oled_display
    lambda: |-
      if (id(show_welcome)) {
        it.printf(70, 25, id(font3), TextAlign::CENTER, "EaseLock");
      } else if (id(entering_pin_page)) {
        // Show password entry page
        it.printf(10, 0, id(font2), "Enter PIN");
        it.printf(8, 25, id(fontd), "%s", id(current_pin).c_str());
      } else {
        // Normal home page
        it.strftime(0, 0, id(fontd), "%H:%M", id(home_time).now());
        it.printf(32, 45, id(font2), "%s", id(alarm_status).state.c_str());
      }


# ------------------------
# Fonts
# ------------------------
font:
  - file: "ARIAL.TTF"
    id: font1
    size: 12

  - file: "ARIAL.TTF"
    id: font2
    size: 18

  - file: "ARIAL.TTF"
    id: font3
    size: 43

  - file: "fonts/digital-7m.ttf"
    id: fontd
    size: 54

# Restart switch to allow restarting from Home Assistant
switch:
  - platform: restart
    name: "EaseLock ESP Restart"

binary_sensor:
  - platform: status
    name: "Device Status"
    on_state:
      then:
        - light.turn_on:
            id: status_light
            red: 0
            green: 100%
            blue: 0
        - delay: 500ms
        - light.turn_off: status_light
