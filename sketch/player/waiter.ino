static unsigned long waitGoal = 0;

void waitBegin() {
  waitGoal = micros();
}

void waitAdd(byte tick) {
  waitGoal += (unsigned long)tick * WAIT_RESOLUTION * 1000;
}

void waitInvoke() {
  while (waitGoal > micros()) {
    delayMicroseconds(8);
  }
}
