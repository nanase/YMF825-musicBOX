void setup_port() {
  PORT_SS_DDR_R |= PORT_SS_DDR_V;
  PORT_IC_DDR_R |= PORT_IC_DDR_V;
  pinMode(PIN_BT_PLAY, MODE_BT_PLAY);
  pinMode(PIN_BT_NEXT, MODE_BT_NEXT);
}

void enable_lch()  { PORT_SS &= ~BIT_SS_LCH; }
void enable_rch()  { PORT_SS &= ~BIT_SS_RCH; }
void enable_lrch() { PORT_SS &= ~BIT_SS_LR;  }
void disable_ss()  { PORT_SS |=  BIT_SS_LR;  }

void enable_ic()  { PORT_IC &= ~BIT_IC; }
void disable_ic() { PORT_IC |=  BIT_IC; }
