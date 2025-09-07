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

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Doordash Fallback Hotspot"
    password: "Xl8dbl2PNe66"
captive_portal:
# --- Security codes ---
globals:
  - id: disarm_code
    type: std::string
    initial_value: '"0152"'
  - id: arm_code
    type: std::string
    initial_value: '"9999"'

# --- Alarm control state ---
text_sensor:
  - platform: template
    id: alarm_control
    name: "Alarm Control"

  - platform: template
    id: entered_pin
    name: "Entered PIN"

# --- Keypad Matrix (4x4) ---
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

# Key Collector
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
    on_progress:
      - logger.log:
          format: "input progress: '%s', started by '%c'"
          args: [ 'x.c_str()', "(start == 0 ? '~' : start)" ]
    on_result:
      - logger.log:
          format: "input result: '%s', started by '%c', ended by '%c'"
          args: [ 'x.c_str()', "(start == 0 ? '~' : start)", "(end == 0 ? '~' : end)" ]
    on_timeout:
      - logger.log:
          format: "input timeout: '%s', started by '%c'"
          args: [ 'x.c_str()', "(start == 0 ? '~' : start)" ]

 # Status Light
# light:
#   - platform: esp32_rmt_led_strip
#     rgb_order: GRB
#     chipset: WS2812
#     pin: GPIO15
#     num_leds: 1
#     name: "Status Light"
#     id: status_light
#     restore_mode: ALWAYS_OFF
