#include <SD.h>
#include <ArduinoSound.h>
#include <ArduinoLowPower.h>


const int volume = 80;

const int doorbell_pin = 5;

const int number_chimes = 11;

const int max_rings_per_chime = 6;

const int debounce_delay_ms = 50;

int current_chime = 0;

int current_number_rings = 0;

// variable representing the Wave File
SDWaveFile waveFile;


void setup() {
  // configure LED
  pinMode(LED_BUILTIN, OUTPUT);
  // configure pin 'doorbell_pin' as an input and enable the internal pull-up resistor
  pinMode(doorbell_pin, INPUT_PULLUP);
  LowPower.attachInterruptWakeup(doorbell_pin, play_chime, FALLING);

  // setup the SD card, depending on your shield of breakout board
  // you may need to pass a pin number in begin for SS
  // Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }

  load_chime(current_chime);

  // adjust the playback volume 0 - 100
  AudioOutI2S.volume(volume);

  // check if the I2S output can play the wave file
  if (!AudioOutI2S.canPlay(waveFile)) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }

  LowPower.deepSleep();
}

void loop() {
  // Debounce the signal. The button/switch is subject to
  // electromagnetic interference. This helps prevents unwanted triggers.
  bool doorbell_pressed = false;
  int reading = digitalRead(doorbell_pin);
  if (reading == LOW) {
    delay(debounce_delay_ms);
    reading = digitalRead(doorbell_pin);
    if (reading == LOW) {
      doorbell_pressed = true;
    }
  }

  if (doorbell_pressed) {
    doorbell_pressed = false;

    if (!AudioOutI2S.isPlaying()) {
      // turn LED on during playback, for debugging
      digitalWrite(LED_BUILTIN, HIGH);

      AudioOutI2S.play(waveFile);
      // wait until playback is done
      while (AudioOutI2S.isPlaying()) {
        delay(100);
      }
      digitalWrite(LED_BUILTIN, LOW);

      current_number_rings ++;

      // change chime type after reaching a predefined number of rings
      if (current_number_rings >= max_rings_per_chime) {
        current_number_rings = 0;
        current_chime = (current_chime + 1) % number_chimes;
        load_chime(current_chime);
      }
    }
  }

  LowPower.deepSleep();
}

void play_chime() {
  // do nothing, just wake up
}

void load_chime(int chime_number) {
  char chime_filename[16];
  sprintf(chime_filename, "chime_%d.wav", chime_number);

  waveFile = SDWaveFile(chime_filename);

  // check if the WaveFile is valid
  if (!waveFile) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
}
