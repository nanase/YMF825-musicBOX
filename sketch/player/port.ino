void setupPort() {
  pinMode(LCD_PIN_R, OUTPUT);
  pinMode(LCD_PIN_G, OUTPUT);
  pinMode(LCD_PIN_B, OUTPUT);

  analogWrite(LCD_PIN_R, 255);
  analogWrite(LCD_PIN_G, 255);
  analogWrite(LCD_PIN_B, 255);

  pinMode(LCD_PIN_D4, OUTPUT);
  pinMode(LCD_PIN_D5, OUTPUT);
  pinMode(LCD_PIN_D6, OUTPUT);
  pinMode(LCD_PIN_D7, OUTPUT);

  pinMode(LCD_PIN_RS, OUTPUT);
  pinMode(LCD_PIN_E, OUTPUT);

  PORT_SS_DDR_R |= PORT_SS_DDR_V;
  pinMode(PIN_BT_PLAY, INPUT_PULLUP);
  pinMode(PIN_BT_NEXT, INPUT_PULLUP);

  debugInfo("Port setup");
}

void enableLch() { PORT_SS &= ~BIT_SS_LCH; }
void enableRch() { PORT_SS &= ~BIT_SS_RCH; }
void enableLRch() { PORT_SS &= ~BIT_SS_LR; }
void disableSS() { PORT_SS |= BIT_SS_LR; }
