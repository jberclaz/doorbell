# doorbell
Custom wired doorbell chime with an Arduino MKR Zero

## Intro

This is a small DYI project to replace a very old doorbell chime with
a custom-made one using an Arduino board. There are plenty of commerical
products out there, but they are either wireless or very bulky. My old
doorbell chime was built into an electric box, and I wanted to keep
the form factor, while improving the sound.

The MKR Zero is a very nice board for such a project. It has an
integrated SD card reader and can play sounds through I2S. 
To convert audio signal into sound, it needs an external board that                                                                                                                                                                            
acts as a DAC/Amp as well as a loudspeaker.

## Software

The initial
inspiration for this project came from the following
[tutorial](https://www.arduino.cc/en/Tutorial/ArduinoSoundWavePlayback). This
program uses the [Arduino Sound
Library](https://www.arduino.cc/en/Reference/ArduinoSound) to load a
wave file stored on a SD card and play it. We can pretty much copy and
paste it, but we need to add an external trigger. This can be simply
done by connecting the doorbell button to a digital pin with a pullup
resistance.

We could just leave it like that, and have the main loop check for the
state of the digital pin. However, it is a bit wasteful. The doorbell
button is only triggered rarely and the microcontroller would be
sitting idle most of the time. Fortunately, the MKR Zero comes with a
MCU equipped with low power features. We thus use the [Low Power
library](https://www.arduino.cc/en/Reference/ArduinoLowPower) to put
the board to sleep when it is not being used. The library is able to
tie a wakeup even to a digital pin state change, which is perfect for
our purpose. We can simply have the main loop play a chime and go to
sleep when it's done.

The Low Power library features 3 different sleep modes: Idle, sleep
and deep sleep. *Deep sleep* is the most efficient of them but its
wake up time is supposed to be slower. For the doorbell chime
application, though, I could not notice any difference between the
normal sleep and the deep sleep mode, so I went for deep sleep.

Using an SD card of 1 GB or more to store a door bell chime is quite a
waste of storage space. For that reason, I added a feature allowing to
play several different tunes. I originally wanted to take advantage of
the RTC the MKR Zero provides to play a different tune every
month. However, it turns out that the I2S and RTC libraries do not
work together. So I ended up rotating among a few different chimes
after the doorbell has been pressed for a predefined number of
time. It is as simple as loading a new wave file from the SD card.

## Hardware
Here is the hardware I used for this project:
* Arduino [MKR Zero](https://store.arduino.cc/usa/arduino-mkrzero)
* [DAC/Amp](https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp)
* [Speaker](https://www.adafruit.com/product/1314)
* [USB power adadapter](https://www.digikey.com/product-detail/en/phihong-usa/PSA05A-050QL6-H/993-1327-ND/5418479)

In addition, you will need a soldering iron, some
[wire](https://www.amazon.com/s?k=electrical+wire+arduino&ref=nb_sb_noss_2)
to connect the boards and [screws and
spacers](https://www.amazon.com/Sutemribor-Female-Spacer-Standoff-Assortment/dp/B075K3QBMX/ref=sr_1_2?dchild=1&keywords=m2.5+screws+spacers&qid=1594096190&sr=8-2)
to attach the boards.

The MKR Zero is powered by the USB power adapter. The DAC/Amp takes
power from the MKR Zero 5V output. It also works with 3.3V, but will
result in lower volume.

The data connection between the Zero and the DAC/Amp is made by 3
wires connected to pins A6, D2 and D3 on the Zero and DIN, BCLK and
LRC on the amp, respectively.

The loudspeaker is connected to the amp's + and - ports.

Finally, the doorbell button should be connected to one digital pin
(pin 0 in my listing) and the ground pin on the MKR Zero.

Note that many doorbell buttons are *lit* by a small bulb. This means
that the circuit is closed, even when the button is not pressed. It is
a problem for our digital pin, because it will understand that the
button is constantly pressed. This issue can be simply solved by
opening the doorbell button and removing the bulb.
