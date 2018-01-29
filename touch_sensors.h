#define VARIANCE_TOUCH    420
#define VARIANCE_UNTOUCH   50


#define SENSORS 4

int s[SENSORS];
bool was_touched[SENSORS], is_touched[SENSORS];
int avg[SENSORS];
int pins[SENSORS] = {0, 1, 16, 17};

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

void updateSensor(int i) {
  was_touched[i] = is_touched[i];
  s[i] = readSensor(i);

  int diff = s[i] - avg[i];
  int variance = diff / (avg[i] >> 10);
  Serial.printf("%6d \n", variance);
  if (!was_touched[i]) {
    if (variance >= VARIANCE_TOUCH) {
      is_touched[i] = true;
    }
  }
  else if (variance <= VARIANCE_UNTOUCH) {
    is_touched[i] = false;
  }

  adjustAvg(i, diff);
}

void updateSensors() {
  for (int i = 0; i < SENSORS; i++) {
    updateSensor(i);
  }
}

bool wasTouched(int i) {
  return was_touched[i];
}

bool isTouched(int i) {
  return is_touched[i];
}

