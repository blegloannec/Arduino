# ST7735
Playing with a ST7735 TFT LCD display

Adafruit [TFT LCD display](https://www.adafruit.com/products/802) & [tutorial](https://learn.adafruit.com/1-8-tft-display).

**NB:** The SPI is the fastest way to communicate with the display. The SPI lines are wired to the Arduino Uno pins 10-13 (and to the ICSP pins) which makes is compatible with the shield pinout (which does not use ICSP). For a use with the Leonardo board, on which the SPI lines are only wired to the ISCP pins, the display cannot be used as a shield anymore, check out [Arduino SPI lines](https://www.arduino.cc/en/Reference/SPI) for wiring.

## Requirements

Adafruit [ST7735 lib](https://github.com/adafruit/Adafruit-ST7735-Library) & [GFX lib](https://github.com/adafruit/Adafruit-GFX-Library).
