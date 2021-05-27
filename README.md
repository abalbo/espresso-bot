# espresso-bot

EspressoBot can make your vintage espresso lever machine smarter!

The Telegram Bot runs on a ESP8266 based control board FW that enables:

- PID control of the boiler water temperature
- WiFi connectivity
- Remote commands to:
    - Turn On/Off the machine
    - Set the target water temperature
    - Set PID parameters

![Screenshot from 2021-05-27 08-37-29](https://user-images.githubusercontent.com/31935729/119865708-3e4aa180-bf1c-11eb-9c60-13659eaadcbb.png)

# HW Parts

- [Wemos Lolin D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html)
- [Relay shield](https://www.wemos.cc/en/latest/d1_mini_shield/relay.html)
- [OLED shield](https://www.wemos.cc/en/latest/d1_mini_shield/oled_0_66.html)
- [TI ADS1115 breakout board](https://www.adafruit.com/product/1085)
- NTC sensor temperature probe

# Setup

First, install [ESP8266 core](https://github.com/esp8266/Arduino) in the Arduino IDE:
- Install the current upstream Arduino IDE at the 1.8.9 level or later. The current version is on the Arduino website.
- Start Arduino and open the Preferences window.
- Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the File>Preferences>Additional Boards Manager URLs field of the Arduino IDE. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation)

Install the following libraries in the Arduino IDE. Search in the library manager or download directly from the links:

- [Universal-Arduino-Telegram-Bot](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit_SSD1306_Wemos_OLED](https://github.com/stblassitude/Adafruit_SSD1306_Wemos_OLED)
- [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO)

[Adafruit_ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15) it's included in the local files of the repository, it was modified to set the ADC in Continuous conversion mode

# Getting Started

To run the Bot in your on ESP8266, create a new Bot talking to [BotFather](https://t.me/botfather) and following few simple [steps](https://core.telegram.org/bots).

Once obtained your bot Access Token create a local `arduino_secrets.h` file and add your `BOT_TOKEN` and local WiFi `SSID` / `PSW`

    #define SECRET_SSID "XXXXXXXX"                                            // your network SSID (name)
    #define SECRET_PSW  "XXXXXXXX"                                            // your network key
    #define SECRET_BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token
