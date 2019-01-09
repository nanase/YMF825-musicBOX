void setupPort() {
  PORT_SS_DDR_R |= PORT_SS_DDR_V;
  pinMode(PIN_BT_PLAY, INPUT_PULLUP);
  pinMode(PIN_BT_NEXT, INPUT_PULLUP);
}

void enableLch()  { PORT_SS &= ~BIT_SS_LCH; }
void enableRch()  { PORT_SS &= ~BIT_SS_RCH; }
void enableLRch() { PORT_SS &= ~BIT_SS_LR;  }
void disableSS()  { PORT_SS |=  BIT_SS_LR;  }
