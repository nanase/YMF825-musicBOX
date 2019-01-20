LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_E, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6,
                  LCD_PIN_D7);

static uint8_t fade_target_r     = 0;
static uint8_t fade_target_g     = 0;
static uint8_t fade_target_b     = 0;
static int32_t fade_current_r    = 0;
static int32_t fade_current_g    = 0;
static int32_t fade_current_b    = 0;
static int32_t fade_speed        = 100;
const static int32_t FADE_FACTOR = 100;

void lcdInitialize() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.noBlink();
  lcd.noCursor();
}

void lcdSetBackground(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(LCD_PIN_R, 255 - r);
  analogWrite(LCD_PIN_G, 255 - g);
  analogWrite(LCD_PIN_B, 255 - b);
}

void lcdSetFadeColor(uint8_t r, uint8_t g, uint8_t b) {
  fade_target_r = r;
  fade_target_g = g;
  fade_target_b = b;
}

static void fading(int32_t *current, int32_t target) {
  target *= FADE_FACTOR;

  if ((*current < target && (*current += fade_speed) > target) ||
      (*current > target && (*current -= fade_speed) < target))
    *current = target;
}

void lcdFade() {
  fading(&fade_current_r, fade_target_r);
  fading(&fade_current_g, fade_target_g);
  fading(&fade_current_b, fade_target_b);

  lcdSetBackground((uint8_t)(fade_current_r / FADE_FACTOR),
                   (uint8_t)(fade_current_g / FADE_FACTOR),
                   (uint8_t)(fade_current_b / FADE_FACTOR));
}

void lcdSetFadeSpeed(int32_t speed) {
  fade_speed = speed;
}

void lcdPrint(const char *text) {
#ifndef DEBUG_MODE
  lcd.print(text);
#endif
}

void lcdWrite(char character) {
#ifndef DEBUG_MODE
  lcd.write(character);
#endif
}
