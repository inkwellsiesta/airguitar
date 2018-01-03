#define SENSORS 4
bool touched[SENSORS];
int avg[SENSORS];
int pins[SENSORS] = {0, 1, 16, 17};


int startup = 64;

void calibrateSensor(int i, int s) {
  avg[i] += s;
  if (--startup == 0) {
    avg[i] /= 64;
  }
}

int readSensor(int i) {
  return touchRead(pins[i]) << 10;
}

void adjustAvg(int diff) {
  if (diff > 0) {
    avg[i] += diff >> 13; // increase average slowly
  } else {
    avg[i] += diff >> 6;  // decrease average rapidly
  }
}

