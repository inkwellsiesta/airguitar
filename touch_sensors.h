#define SENSORS 4

int pins[SENSORS] = {0, 1, 16, 17};

int s[SENSORS];
bool was_touched[SENSORS], is_touched[SENSORS];
int avg[SENSORS];

int startup = 64;

int readSensor(int i) {
  return touchRead(pins[i]) << 10;
}

void calibrateSensor(int i) {
  int s = readSensor(i);
  avg[i] += s;
  if (--startup == 0) {
    avg[i] /= 64;
  }
}

void calibrateSensors() {
  for (int i = 0; i < SENSORS; i++) {
    calibrateSensor(i);
  }
}

void adjustAvg(int i, int diff) {
  if (diff > 0) {
    avg[i] += diff >> 13; // increase average slowly
  } else {
    avg[i] += diff >> 6;  // decrease average rapidly
  }
}

int findVariance(int i) {
  s[i] = readSensor(i);

  int diff = s[i] - avg[i];
  int variance = diff / (avg[i] >> 10);
  Serial.printf("Pin %d:\t%6d \n", pins[i], variance);
  adjustAvg(i, diff);
  return variance;
}

void updateSensor(int i, int touch_threshold, int untouch_threshold) {
  was_touched[i] = is_touched[i];

  int variance = findVariance(i);

  if (!was_touched[i]) {
    if (variance >= touch_threshold) {
      is_touched[i] = true;
    }
  }
  else if (variance <= untouch_threshold) {
    is_touched[i] = false;
  }

}

void updateSensors(int touch_threshold, int untouch_threshold) {
  for (int i = 0; i < SENSORS; i++) {
    updateSensor(i, touch_threshold, untouch_threshold);
  }
}

bool wasTouched(int i) {
  return was_touched[i];
}

bool isTouched(int i) {
  return is_touched[i];
}

int findTouchedFret() {
  int i = 1;
  int touchedFretNumber = 0;
  for (; i < SENSORS; i++) {
    if (isTouched(i)) {
      touchedFretNumber = i;
    }
  }
  return touchedFretNumber;
}

