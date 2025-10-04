# ESP32 WiFi Deauther
Simple on boot ESP32-Wroom WiFi deauther, no configuration, just deauthing nearby WiFi on power

![Image](pic/IMG_1672.JPEG)

## DISCLAIMER
Ethical purposes only

## Hardware 
### Required
* ESP32-Wroom
### Optionnal
* 3.7 LiPo battery
* TP4056
* 3 Pins Slider

## Firmware
### Web
Download the [firmware.bin](src/firmware.bin) file and use an [Online Web Flasher](https://esptool.spacehuhn.com/)

### VSCode
1. Download VSCode
2. Add PlatformIO extension
3. Build using the files on this repo

## Use
Power on the ESP32, it will first scan for AP then loop the deauth with a small delay
