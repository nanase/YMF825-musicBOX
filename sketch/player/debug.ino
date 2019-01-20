extern LiquidCrystal lcd;

void debugInitialize() {
#ifdef DEBUG_MODE
  PSerial.begin(9600);
  debugInfo("Setup");
#endif
}

void debugInfo(const char *message) {
#ifdef DEBUG_MODE
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("* ");
  lcd.print(message);

  PSerial.print("[INFO] ");
  PSerial.println(message);
#endif
}

void debugWarn(const char *message) {
#ifdef DEBUG_MODE
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("! ");
  lcd.print(message);

  PSerial.print("[WARN] ");
  PSerial.println(message);
#endif
}

void debugError(const char *message) {
#ifdef DEBUG_MODE
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("# ");
  lcd.print(message);

  PSerial.print("[ERROR] ");
  PSerial.println(message);
#endif

#ifdef HALT_ON_ERROR
  while (true)
    delay(1);
#endif
}
