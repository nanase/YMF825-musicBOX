static unsigned long waitGoal = 0;

extern volatile unsigned long timer0_millis;
extern volatile unsigned long timer0_overflow_count;

static void clearTCNT0() {
  cli();
  timer0_millis = 0;
  timer0_overflow_count = 0;
  TCNT0 = 0;
  sei();
}

void waitBegin() {
  clearTCNT0();
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
