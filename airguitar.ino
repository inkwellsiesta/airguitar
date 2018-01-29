#include <Bounce.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>

#include "airguitar_audio.h"
#include "touch_sensors.h"

const int touch_threshold = 420;
const int untouch_threshold = 50;

Chord chord = Cmajor;

void setup() {
  audioSetup();
}

void loop() {
  // The sensors must calibrate for the first few iterations of the loop
  if (startup) {
    calibrateSensors();
    return; // This tell the program to ignore everything
            // else in the function.
  }

  updateSensors(touch_threshold, untouch_threshold);

  // Should we strum?
  if (!wasTouched(STRUM_SENSOR) && isTouched(STRUM_SENSOR)) {
    strum(chord);
  }

  // Choose the chord based on the fret that's being touched.
  int touchedFretNumber = findTouchedFret();
  // Chords are based on a major pentatonic scale, so they always sound good together.
  switch (touchedFretNumber) {
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


