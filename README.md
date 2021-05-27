# espresso-bot

EspressoBot can make your vintage espresso lever machine smarter!

The Telegram Bot runs on a ESP8266 based control board FW that enables:

- PID control of the boiler water temperature
- WiFi connectivity
- Remote commands to:
    - Turn On/Off the machine
    - Set the target water temperature
    - Set PID parameters

# HW Parts

- [Wemos Lolin D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html)
- [Relay shield](https://www.wemos.cc/en/latest/d1_mini_shield/relay.html)
- [OLED shield](https://www.wemos.cc/en/latest/d1_mini_shield/oled_0_66.html)
- [TI ADS1115 breakout board](https://www.adafruit.com/product/1085)
- NTC sensor temperature probe

# Setup
Install the following libraries in the Arduino IDE. Search in the library manager or download directly from the links

- [Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit_SSD1306_Wemos_OLED](https://github.com/stblassitude/Adafruit_SSD1306_Wemos_OLED)
- [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO)

[Adafruit_ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15) it's included in the local files of the repository, it was modified to set the ADC in Continuous conversion mode

# Getting Started

To run the Bot in your on ESP8266, create a new Bot using [BotFather](https://t.me/botfather).

Once obtained your bot Access Token create a local `arduino_secrets.h` file and add your `BOT_TOKEN` and local WiFi `SSID` / `PSW`

    #define SECRET_SSID "XXXXXXXX"                                            // your network SSID (name)
    #define SECRET_PSW  "XXXXXXXX"                                            // your network key
    #define SECRET_BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token
