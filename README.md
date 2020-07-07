# doorbell
Custom wired doorbell chime with an Arduino MKR Zero

## Intro

This is a small DYI project to replace a very old doorbell chime with
a custom-made using an Arduino board. There are plenty of commerical
products out there, but they are either wireless or very bulky. My old
doorbell chime was built into an electric box, and I wanted to keep
the form factor, while improving the sound.

The MKR Zero is a very nice board for such a project. It has an
integrated SD card reader and can play sounds through I2S. The initial
inspiration came from the following
[tutorial](https://www.arduino.cc/en/Tutorial/ArduinoSoundWavePlayback).

## Hardware
* Arduino [MKR Zero](https://store.arduino.cc/usa/arduino-mkrzero)
* [DAC/AMP](https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp)
* [Speaker](https://www.adafruit.com/product/1314)

## Software
