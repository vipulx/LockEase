esphome:
  name: nodemotion
  friendly_name: NodeMotion
  includes:
    - ttp226.h

esp8266:
  board: nodemcuv2

logger:

api:
  encryption:
    key: "sg285XxW89RmvHnQ30lDKGk/NBl2m4fgFecbGYqte8E="

ota:
  - platform: esphome
    password: "1e33691eb2313fbfe57b7d3f972771df"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
  ap:
    ssid: "Nodemotion Fallback Hotspot"
    password: "aZH3M8iTA7Yq"

captive_portal:

ttp229_bsf:
  sdo_pin: D2
  scl_pin: D1

# Define reusable template for all keys
binary_sensor:
  - &key_template
    platform: ttp229_bsf
    filters:
      - delayed_on: 50ms
      - delayed_off: 50ms
    on_press:
      then:
        - homeassistant.service:
            service: notify.mobile_app
            data:
              message: !lambda 'return "Key " + to_string(id(key_number)) + " Pressed";'
    on_release:
      then:
        - homeassistant.service:
            service: notify.mobile_app
            data:
              message: !lambda 'return "Key " + to_string(id(key_number)) + " Released";'

  # Now just copy with channel & name
  - <<: *key_template
    id: key1
    channel: 0
    name: "Key 1"
  - <<: *key_template
    id: key2
    channel: 1
    name: "Key 2"
  - <<: *key_template
    id: key3
    channel: 2
    name: "Key 3"
  - <<: *key_template
    id: key4
    channel: 3
    name: "Key 4"
  # ...continue for all 15 keys
