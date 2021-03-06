# CleanR
CleanR is a DIY air purifier project.

**Goal** of the project is to create an air purifier which is cheaper and more silent than the commercially available ones.

## Components
Current revision is based on the following components:
* NF-A14 industrialPPC-24V-3000 Q100 IP67 PWM
* Random HEPA filter(*should not matter as long as it works*)
* 3D printed case(*see resources*)
* USB-C power input
* DC-DC step-up module

## Assembly
![schematic](https://raw.githubusercontent.com/Levente96/CleanR/main/resources/schematics/Schematic.png "Schematic")
For the filter, I found an [alternative](https://www.aliexpress.com/item/32861099829.html), which you should cut in half (alongside the longer side), so the filter will be 145mm "tall".
I have also attached strigs from the bottom to the fan's bottom screws to load the bottom up to the filer when lifted.

## Perforamnce
The goal is to beat the following performance:
> 320m^3/h (recommended for up to 106m^2) of air for ~100$ with 31-61dB(A)

Simplifying this means you got 3m^3/h for each m^2 which means you are paying '~1$/m^2'.
Our performance is the following:
> 260m^3/h (according to Noctua specs) with up to 41dB(A) of noise

Calculating the "optimal" m^2 for this device would be 86 m^2 which is still larger than I need, for around 1/3-rd of the cost.

This design is **2.59 times** more value ($/cleaned air) than the Xiaomi Air Purifier 3C.

## Cost
The goal of all time to keep the cost under ~100$. Currently it costs:

| Item | Cost |
| ---- | ---: |
| Fan  | 33$  |
| Filter | 3$ |
| DC Converter | 1$ |
| Connector + cables | <1$|
| 3D printed box | ~3$ |
| ESP-01 | 1$ |
| 1K ohm resistor| <1$ |
| TIP122 | <1$ |
| **Total:** | **42$** |


> **Note:** The development environment only works in linux with legacy dependencies, use it on a virtualbox.

## Roadmap
* `v1.0`: Simplest version, with no other optioans than plugged in and running, and poweroff state :heavy_check_mark:
* `v2.0`: ESP8266 built-in for controlling the fan-speed according to preprogrammed timestamps :heavy_check_mark:
* `v3.0`: ~ESP8266 with full IoT features inluding AWS integration and wireless access~ basic web interface :heavy_check_mark: , OTA :heavy_check_mark: , redesign :heavy_check_mark:
* `v4.0`: Mesh network of these devices with standalone air quality measurement point

## Resources
* [Noctua](https://noctua.at/en/nf-a14-industrialppc-24v-3000-q100-ip67-pwm)
* [Air purifier tests](https://smartairfilters.com/en/blog/xiaomi-purifier-auto-mode-leaves-air-unsafe-86-hours/)
* [Noctua PWM white paper](https://noctua.at/pub/media/wysiwyg/Noctua_PWM_specifications_white_paper.pdf)
* [ESP Tool](https://github.com/espressif/esptool)
* [Expressif FreeRTOS SDK](https://github.com/espressif/ESP8266_RTOS_SDK)
* [Expressif IDF](https://github.com/espressif/esp-idf)

---
[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=T5EL5GCBGGAE6)
