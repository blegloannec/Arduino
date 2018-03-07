# BMP180 & Ethernet

[Ethernet library](https://www.arduino.cc/en/Reference/Ethernet)

[Adafruit BMP085/180 driver](https://github.com/adafruit/Adafruit_BMP085_Unified)

[BMP180 datasheet](http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf)

`get_measurements.sh` script fruitfully combined with a few [conky](http://conky.sourceforge.net/variables.html) lines:
```
${if_match "${exec "~/.weather/get_measurements.sh"}" == "up"}Ambiant temperature: ${tail ~/.weather/conky_temp 1} °C
${execgraph "tail -1 ~/.weather/conky_temp_perc" 50,299}
Atmospheric pressure: ${tail ~/.weather/conky_press 1} hPa
${execgraph "tail -1 ~/.weather/conky_press_perc"}
$endif
```
