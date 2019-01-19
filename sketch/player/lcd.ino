#include <LiquidCrystal.h>

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

// y = =255 - pow((255 - x) / 255, 4) * 255
static const unsigned char PWM_LUT[] PROGMEM = {
    0, 4, 8, 12, 16, 19, 23, 27, 31, 34, 38, 41, 45, 48, 52, 55, 58, 61, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 98,
    100, 103, 106, 109, 111, 114, 116, 119, 121, 124, 126, 129, 131, 133, 135, 138, 140, 142, 144, 146, 148, 151, 153,
    155, 157, 159, 160, 162, 164, 166, 168, 170, 171, 173, 175, 176, 178, 180, 181, 183, 184, 186, 187, 189, 190, 192,
    193, 194, 196, 197, 198, 200, 201, 202, 203, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
    219, 220, 221, 222, 223, 224, 224, 225, 226, 227, 228, 228, 229, 230, 230, 231, 232, 232, 233, 234, 234, 235, 236,
    236, 237, 237, 238, 238, 239, 239, 240, 240, 241, 241, 242, 242, 242, 243, 243, 244, 244, 244, 245, 245, 246, 246,
    246, 246, 247, 247, 247, 248, 248, 248, 248, 249, 249, 249, 249, 250, 250, 250, 250, 250, 251, 251, 251, 251, 251,
    252, 252, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

#define PWM_VALUE(x) ((int)(pgm_read_byte(PWM_LUT + x)))

void lcdInitialize() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.noBlink();
  lcd.noCursor();
}

void lcdSetBackground(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(LCD_PIN_R, 255 - PWM_VALUE(r));
  analogWrite(LCD_PIN_G, 255 - PWM_VALUE(g));
  analogWrite(LCD_PIN_B, 255 - PWM_VALUE(b));
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
