void setupPort() {
  PORT_SS_DDR_R |= PORT_SS_DDR_V;
  PORT_IC_DDR_R |= PORT_IC_DDR_V;
  pinMode(PIN_BT_PLAY, MODE_BT_PLAY);
  pinMode(PIN_BT_NEXT, MODE_BT_NEXT);
}

void enableLch()  { PORT_SS &= ~BIT_SS_LCH; }
void enableRch()  { PORT_SS &= ~BIT_SS_RCH; }
void enableLRch() { PORT_SS &= ~BIT_SS_LR;  }
void disableSS()  { PORT_SS |=  BIT_SS_LR;  }

void enableIC()  { PORT_IC &= ~BIT_IC; }
void disableIC() { PORT_IC |=  BIT_IC; }
