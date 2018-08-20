static unsigned long wait_goal = 0;

void wait_begin() {
  wait_goal = micros();
}

void wait_add(byte tick) {
  wait_goal += (unsigned long)tick * WAIT_RESOLUTION * 1000;
}

void wait_invoke() {
  while (wait_goal > micros()) {
    delayMicroseconds(8);
  }
}
