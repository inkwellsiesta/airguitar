#include <Audio.h>

#include "strum.h"


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
AudioControlSGTL5000     theAudio;

void audioSetup() {
  AudioMemory(15);

  theAudio.enable();
  theAudio.volume(0.6);
  
  float gainValue = 0.5;
  mixer1.gain(0, gainValue);
  mixer1.gain(1, gainValue);
  mixer1.gain(2, gainValue);
  mixer1.gain(3, gainValue);
  mixer2.gain(1, gainValue);
  mixer2.gain(2, gainValue);

  delay(700);
}

