#include "chords.h"

#define FINGER_DELAY 5
#define STRUM_SENSOR 0
#define FIRST_FRET_SENSOR 1

AudioSynthKarplusStrong  string1;
AudioSynthKarplusStrong  string2;
AudioSynthKarplusStrong  string3;
AudioSynthKarplusStrong  string4;
AudioSynthKarplusStrong  string5;
AudioSynthKarplusStrong  string6;

const float mVelocity = 1.0;
void strum(const float * chord)
{
  if (chord[5] > 20.0) string1.noteOn(chord[5], mVelocity);
  delay(FINGER_DELAY);
  if (chord[4] > 20.0) string2.noteOn(chord[4], mVelocity);
  delay(FINGER_DELAY);
  if (chord[3] > 20.0) string3.noteOn(chord[3], mVelocity);
  delay(FINGER_DELAY);
  if (chord[2] > 20.0) string4.noteOn(chord[2], mVelocity);
  delay(FINGER_DELAY);
  if (chord[1] > 20.0) string5.noteOn(chord[1], mVelocity);
  delay(FINGER_DELAY);
  if (chord[0] > 20.0) string6.noteOn(chord[0], mVelocity);
  delay(FINGER_DELAY);
}
