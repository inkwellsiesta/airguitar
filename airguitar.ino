#include <Audio.h>
#include <Bounce.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

#include "strum.h"
#include "touch_sensors.h"

// Special thanks to Matthew Rahtz - http://amid.fish/karplus-strong/
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(string2, 0, mixer1, 1);
AudioConnection          patchCord3(string3, 0, mixer1, 2);
AudioConnection          patchCord4(string4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 0);
AudioConnection          patchCord6(string5, 0, mixer2, 1);
AudioConnection          patchCord7(string6, 0, mixer2, 2);
AudioConnection          patchCord8(mixer2, 0, i2s1, 0);
AudioConnection          patchCord9(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

const float *chord;

void setup() {
  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);

  float gainValue = 0.5;
  mixer1.gain(0, gainValue);
  mixer1.gain(1, gainValue);
  mixer1.gain(2, gainValue);
  mixer1.gain(3, gainValue);
  mixer2.gain(1, gainValue);
  mixer2.gain(2, gainValue);

  delay(700);

  chord = Cmajor;
}


#define VARIANCE_TOUCH    420
#define VARIANCE_UNTOUCH   50

void loop() {
  int s[SENSORS];
  int diff, variance;

  for (int i = 0; i < SENSORS; i++) {
    s[i] = readSensor(i);

    if (startup) {
      calibrateSensor(i, s[i]);
      continue;
    }

    diff = s[i] - avg[i];
    variance = diff / (avg[i] >> 10);
    Serial.printf("%6d \n", variance);
    if (touched[i] == false) {
      if (variance >= VARIANCE_TOUCH) {
        touched[i] = true;
        if (i == STRUM_SENSOR) {
          strum(chord);
        }
      }
    } else {
      if (variance <= VARIANCE_UNTOUCH) {
        touched[i] = false;
      }
    }

    adjustAvg(diff);
  }

  if (!startup) {
    int i = 1;
    int touchedChordNumber = 0;
    for (; i < SENSORS; i++) {
      if (touched[i]) {
        touchedChordNumber = i;
      }
    }
    // Chords are based on a major pentatonic scale, so they always sound good together.
    switch (touchedChordNumber) {
      case 0: // "open" chord (i.e. not touching any of the frets)
        chord = Cmajor;
        break;
      case 1: // touching the first fret
        chord = Fmajor;
        break;
      case 2: // touching the second fret
        chord = Gmajor;
        break;
      case 3: // touching the third fret
        chord = Bminor;
        break;
      default: // We shouldn't ever get here, but it's good to have a failsafe just in case.
        chord = Cmajor;
    }
  }
}


