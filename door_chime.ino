#include <SD.h>
#include <ArduinoSound.h>
#include <ArduinoLowPower.h>


const int doorbell_pin = 0;

const int number_chimes = 5;

const int max_rings_per_chime = 10;

int current_chime = 0;

int current_number_rings = 0;

bool doorbell_pressed = false;

// variable representing the Wave File
SDWaveFile waveFile;


void setup() {
  // For debugging
  // start serial connection
  Serial.begin(9600);

  if (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
    delay(5000);
  }

  // configure LED
  pinMode(LED_BUILTIN, OUTPUT);
  // configure pin 'doorbell_pin' as an input and enable the internal pull-up resistor
  pinMode(doorbell_pin, INPUT_PULLUP);
  LowPower.attachInterruptWakeup(doorbell_pin, play_chime, FALLING);

  // setup the SD card, depending on your shield of breakout board
  // you may need to pass a pin number in begin for SS
  // Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    if (Serial) {
      Serial.println("initialization failed!");
    }
    digitalWrite(LED_BUILTIN, HIGH);
    return;
  }
  if (Serial) {
    Serial.println("initialization done.");
  }

  load_chime(current_chime);

  // print out some info. about the wave file
  if (Serial) {
    Serial.print("Bits per sample = ");
    Serial.println(waveFile.bitsPerSample());

    long channels = waveFile.channels();
    Serial.print("Channels = ");
    Serial.println(channels);

    long sampleRate = waveFile.sampleRate();
    Serial.print("Sample rate = ");
    Serial.print(sampleRate);
    Serial.println(" Hz");

    long duration = waveFile.duration();
    Serial.print("Duration = ");
    Serial.print(duration);
    Serial.println(" seconds");
  }

  // adjust the playback volume 0 - 100
  AudioOutI2S.volume(85);

  // check if the I2S output can play the wave file
  if (!AudioOutI2S.canPlay(waveFile)) {
    if (Serial) {
      Serial.println("unable to play wave file using I2S!");
    }
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }

  if (Serial) {
    Serial.println("Sleeping...");
  }
  LowPower.deepSleep();
}

void loop() {
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
  doorbell_pressed = true;
}

void load_chime(int chime_number) {
  char chime_filename[16];
  sprintf(chime_filename, "chime_%d.wav", chime_number);

  waveFile = SDWaveFile(chime_filename);

  // check if the WaveFile is valid
  if (!waveFile) {
    if (Serial) {
      Serial.println("wave file is invalid!");
    }
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
}
