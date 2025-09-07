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
    entity_id: alarm_control_panel.your_alarm_entity_id
    internal: true

  - platform: wifi_info
    ip_address:
      name: "LockEase IP"
      id: my_ip

# ------------------------
# Keypad Matrix (4x4)
# ------------------------
matrix_keypad:
  id: my_keypad
  rows:
    - pin: GPIO2
    - pin: GPIO4
    - pin: GPIO6
    - pin: GPIO8
  columns:
    - pin: GPIO9
    - pin: GPIO7
    - pin: GPIO5
    - pin: GPIO3
  keys: "123A456B789C*0#D"
  has_diodes: true

key_collector:
  - id: pincode_reader
    source_id: my_keypad
    min_length: 4
    max_length: 4
    end_keys: "#"
    end_key_required: True
    clear_keys: "*"
    allowed_keys: "0123456789"
    timeout: 5s
    on_result:
      - text_sensor.template.publish:
          id: entered_pin
          state: !lambda 'return x;'
      - if:
          condition:
            lambda: 'return x == id(disarm_code);'
          then:
            - text_sensor.template.publish:
                id: alarm_control
                state: "Disarmed"
            - logger.log: "System Disarmed by PIN"
      - if:
          condition:
            lambda: 'return x == id(arm_code);'
          then:
            - text_sensor.template.publish:
                id: alarm_control
                state: "Armed"
            - logger.log: "System Armed by PIN"
      - if:
          condition:
            lambda: 'return x != id(disarm_code) && x != id(arm_code);'
          then:
            - logger.log: "Invalid PIN Entered"

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
        // Show centered large "EaseLock" welcome message
        it.printf(0, 16, id(fontd), TextAlign::CENTER, "EaseLock");
      } else {
        // Show time centered at top
        it.strftime(0, 0, id(fontd), TextAlign::CENTER, "%H:%M:%S", id(home_time).now());
        // Show Alarm status below time, left aligned
        it.printf(0, 48, id(font2), "Alarm: %s", id(alarm_status).state.c_str());
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
    size: 20

  - file: "ARIAL.TTF"
    id: font3
    size: 43

  - file: "fonts/digital-7m.ttf"
    id: fontd
    size: 43

# Restart switch to allow restarting from Home Assistant
switch:
  - platform: restart
    name: "EaseLock ESP Restart"
    id: esp_restart
