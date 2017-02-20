//---------------------------------------
// AmbientLight - Module
//
//
//---------------------------------------

#include <FastLED.h>

#define COLOR_ORDER GRB
#define CHIPSET_DEFAULT WS2812B
#define NUM_LEDS_DEFAULT 20
#define DATA_PIN 0
#define MAX_USER_PATTERNS 5
#define DEFAULT_BRIGHTNESS 200

CRGB leds[NUM_LEDS_DEFAULT];
uint8_t gBrightness, gHue, gCurrentPattern, gLastPattern;

typedef void (*CallBackFunc)(long color1, long color2, long color3,
                             long timeout, uint16_t mode1, uint16_t mode2);

typedef struct {
  CallBackFunc func;
  long color1;
  long color2;
  long color3;
  long brightness;
  long timeout;   // how long should this pattern showed
  uint16_t mode1; // used for different functionalities inside pattern
  uint16_t mode2; // see mode1
} T_callbackFunc;

T_callbackFunc tempData;

T_callbackFunc patterns[] = {
    {FixRGBColor, 0x000000, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xF0F8FF, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xFAEBD7, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xF0FFFF, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x00FFFF, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x00008B, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x008B8B, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xB8860B, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xFF8C00, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x00CED1, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x4B0082, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xE0FFFF, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x191970, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xFFA500, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xFF4500, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x4169E1, 0, 0, 0, 0, 0},
    {FixRGBColor, 0x40E0D0, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xEE82EE, 0, 0, 0, 0, 0},
    {FixRGBColor, 0xFFFFFF, 0, 0, 0, 0, 0},
    {GradientRGBColor, 0xFAEBD7, 0x00FFFF, 0, 0, 0, 0},
    {GradientRGBColor, 0xFAEBD7, 0x00FFFF, 0, 0, 1, 0},

    //
    // space for special (send from broker) patterns
    //
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

int callPatterns[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

//
// convert a payload into a CallBackFunctionType
//
// seven values separated with :
// for RGB
// color1:color2:color3:timeout:mode1:mode2
//
// for HSV
// hue1:hue2:bright:timeout:mode1:mode2
//
// if less then seven values, rest is set to 0
//
void convertPayload(String payload) {
  String log = "";
  long v;
  uint8_t idx = payload.indexOf(':');
  byte cnt = 0;
  tempData.color1 = 0;
  tempData.color2 = 0;
  tempData.color3 = 0;
  tempData.timeout = 0;
  tempData.func = 0;
  tempData.mode1 = 0;
  tempData.mode2 = 0;
  log = "convertPayload => ";
  while (idx > 0 && cnt < 5) {
    switch (cnt) {
    case 0:
      tempData.color1 = long(payload.substring(0, idx).toFloat());
      log += F(" Color1:");
      log += tempData.color1;
      break;
    case 1:
      tempData.color2 = long(payload.substring(0, idx).toFloat());
      log += F(" Color2:");
      log += tempData.color2;
      break;
    case 2:
      tempData.color3 = long(payload.substring(0, idx).toFloat());
      log += F(" Color3:");
      log += tempData.color3;
      break;
    case 3:
      tempData.timeout = long(payload.substring(0, idx).toFloat());
      log += F(" TimeOut:");
      log += tempData.timeout;
      break;
    case 4:
      tempData.mode1 = payload.substring(0, idx).toInt();
      log += F(" Mode1:");
      log += tempData.mode1;
      break;
    }
    payload = payload.substring(idx + 1);
    idx = payload.indexOf(':');
    cnt++;
  }
  tempData.mode2 = payload.toInt();
  log += F(" Mode2:");
  log += tempData.mode2;
  // addLog(LOG_LEVEL_INFO, log);
}

/*
void setPatternChain(String list) {
  uint8_t size = sizeof(patterns) / sizeof(patterns[0]);
  byte idx = list.indexOf(':');
  byte cnt = 0;
  while (idx > 0 && cnt < (size - 1)) {
    callPattern[cnt] = list.substring(0, idx).toInt();
    idx = list.indexOf(':');
    cnt++;
  }
  callPattern[cnt] = list.toInt();
}
*/

/**
  show a fix color on led stripe
  color = color1

  if mode1 == 1, every update color is added with gHue
**/
void FixRGBColor(long color1, long color2, long color3, long timeout,
                 uint16_t mode1, uint16_t mode2) {

  if (mode1 == 0) {
    fill_solid(leds, NUM_LEDS_DEFAULT, CRGB(color1));
  } else {
    fill_solid(leds, NUM_LEDS_DEFAULT, CRGB(color1 + gHue));
  }
}

/**
  show a fix color on led stripe
  color = hue1

  if mode1 == 1, every update color is added with gHue
**/
void FixHSVColor(long hue1, long hue2, long bright, long timeout,
                 uint16_t mode1, uint16_t mode2) {

  if (mode1 == 0) {
    fill_solid(leds, NUM_LEDS_DEFAULT, CHSV(hue1, 255, bright));
  } else {
    fill_solid(leds, NUM_LEDS_DEFAULT, CHSV(hue1 + gHue, 255, bright));
  }
}

/**
  show a gradient color on led stripe
  from color1 to color2

  if mode1 == 1, every update color1 & color2 is added with gHue
**/
void GradientRGBColor(long color1, long color2, long color3, long timeout,
                      uint16_t mode1, uint16_t mode2) {

  if (mode1 == 0) {
    fill_gradient_RGB(leds, NUM_LEDS_DEFAULT, CRGB(color1), CRGB(color2));
  } else {
    fill_gradient_RGB(leds, NUM_LEDS_DEFAULT, CRGB(color1 + gHue),
                      CRGB(color2 + gHue));
  }
}

/**
  show a gradient color on led stripe
  from hue1 to hue2

  if mode1 == 1, every update hue1 & hue2 is added with gHue
**/
void GradientHSVColor(long hue1, long hue2, long bright, long timeout,
                      uint16_t mode1, uint16_t mode2) {

  if (mode1 == 0) {
    fill_gradient(leds, NUM_LEDS_DEFAULT, CHSV(hue1, 255, bright),
                  CHSV(hue2, 255, bright));
  } else {
    fill_gradient(leds, NUM_LEDS_DEFAULT, CHSV(hue1 + gHue, 255, bright),
                  CHSV(hue2 + gHue, 255, bright));
  }
}

/**
  set user fix color with HSV values

  call from _P199_FastLED module => case PLUGIN_WRITE:
**/
void setHSVColor(int id, String payload) {
  String log = "";
  uint8_t size = sizeof(patterns) / sizeof(patterns[0]);
  id = constrain(id, 0, MAX_USER_PATTERNS);
  convertPayload(payload);
  // insert new data into user pattern area of the pattern list
  // user data are the last entries in the list
  id = size - MAX_USER_PATTERNS + id;
  //
  // put temporare data into pattern user list
  log = F("setHSVColor() =>");
  log += " Size (";
  log += String(size);
  log += ") ";
  log += "UserID (";
  log += String(id);
  log += ") ";
  for (uint8_t x = 0; x < 7; x++) {
    switch (x) {
    case 0:
      patterns[id].func = FixHSVColor;
      break;
    case 1:
      patterns[id].color1 = map(tempData.color1, 0, 100, 0, 255);
      log += "(";
      log += patterns[id].color1;
      log += "/";
      log += tempData.color1;
      log += ")";
      break;
    case 2:
      patterns[id].color2 = map(tempData.color2, 0, 100, 0, 255);
      log += "(";
      log += patterns[id].color2;
      log += "/";
      log += tempData.color2;
      log += ")";
      break;
    case 3:
      patterns[id].color3 = map(tempData.color3, 0, 100, 0, 255);
      log += "(";
      log += patterns[id].color3;
      log += "/";
      log += tempData.color3;
      log += ")";
      gBrightness = patterns[id].color3;
      break;
    case 4:
      patterns[id].timeout = tempData.timeout;
      log += "(";
      log += patterns[id].timeout;
      log += "/";
      log += tempData.timeout;
      log += ")";
      break;
    case 5:
      patterns[id].mode1 = tempData.mode1;
      log += "(";
      log += patterns[id].mode1;
      log += "/";
      log += tempData.mode1;
      log += ")";
      break;
    case 6:
      patterns[id].mode2 = tempData.mode2;
      log += "(";
      log += patterns[id].mode2;
      log += "/";
      log += tempData.mode2;
      log += ")";
      break;
    }
  }
  addLog(LOG_LEVEL_INFO, log);
  gCurrentPattern = id;
}

/**
  set user fix color pattern with RGB color

    call from _P199_FastLED module => case PLUGIN_WRITE:
**/
void setRGBColor(int id, String payload) {
  uint8_t size = sizeof(patterns) / sizeof(patterns[0]);
  id = constrain(id, 0, MAX_USER_PATTERNS);
  // insert new data into user pattern area of the pattern list
  // user data are the last entries in the list
  id = size - id;
  convertPayload(payload);
  //
  // put temporare data into pattern user list
  for (uint8_t x = 0; x < 7; x++) {
    switch (x) {
    case 0:
      patterns[id].func = FixRGBColor;
      break;
    case 1:
      patterns[id].color1 = tempData.color1;
      break;
    case 2:
      patterns[id].color2 = tempData.color2;
      break;
    case 3:
      patterns[id].color3 = tempData.color3;
      break;
    case 4:
      patterns[id].timeout = tempData.timeout;
      break;
    case 5:
      patterns[id].mode1 = tempData.mode1;
      break;
    case 6:
      patterns[id].mode2 = tempData.mode2;
      break;
    }
  }
  gCurrentPattern = id;
}

void setPattern(int id, String payload) {
  uint8_t size = sizeof(patterns) / sizeof(patterns[0]);
  id = constrain(id, 0, size - MAX_USER_PATTERNS);
  convertPayload(payload);
  gCurrentPattern = id;
}

/**
  used from update() to re-run current pattern
**/
void callPattern() {
  String log = "";
  // read parameter for this pattern
  // if (gCurrentPattern != gLastPattern) {
  //  gCurrentPattern = gLastPattern;
  //}

  long color1 = patterns[gCurrentPattern].color1;
  long color2 = patterns[gCurrentPattern].color2;
  long color3 = patterns[gCurrentPattern].color3;
  long timeout = patterns[gCurrentPattern].timeout;
  uint16_t mode1 = patterns[gCurrentPattern].mode1;
  uint16_t mode2 = patterns[gCurrentPattern].mode2;

  log = "call function ";
  // log += String(patterns[gCurrentPattern].func);
  log += " Color1:";
  log += String(color1);
  addLog(LOG_LEVEL_INFO, log);
  // read address for this pointer
  CallBackFunc func = patterns[gCurrentPattern].func;

  // call this pattern
  func(color1, color2, color3, timeout, mode1, mode2);

  FastLED.show();
}

void AmbientLightOff() { FastLED.clear(); }
uint8_t getCurrentPattern() { return gCurrentPattern; }
uint8_t getLastPattern() { return gLastPattern; }
uint8_t getBrightness() { return gBrightness; }
/**
  Update the LED stripe and call current pattern
  at least call FastLED.update()
**/
void update(int pattern = -1) {
  if (pattern >= 0) {
    uint8_t numberOfPatterns = sizeof(patterns) / sizeof(patterns[0]);
    gCurrentPattern = constrain(pattern, 0, numberOfPatterns);
    addLog(LOG_LEVEL_INFO, "update with new patternID");
  }
  callPattern();
}

void TEST_Simple(int id) {
  int c = 60;
  if ((millis() - 5000) == 0)
    c = random(0, 360);
  switch (id) {
  case 1:
    FastLED.setBrightness(255);
    fill_gradient(leds, NUM_LEDS_DEFAULT, CHSV(c, 100, 100),
                  CHSV(c + 60, 100, 100));
    break;
  case 0:
  default:
    FastLED.setBrightness(255);
    fill_rainbow(leds, NUM_LEDS_DEFAULT, gHue++);
    break;
  }
  FastLED.show();
}

void AmbientLight_init() {
  gCurrentPattern = 0;
  gLastPattern = 0;
  FastLED
      .addLeds<CHIPSET_DEFAULT, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS_DEFAULT)
      .setCorrection(TypicalSMD5050);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}
// color palettes - left/right go into black
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE(orange_gp){
    0,   0,    0,    0,    //
    128, 0xFF, 0xA5, 0x00, //
    255, 0,    0,    0     //
};

DEFINE_GRADIENT_PALETTE(indigo_gp){
    0,   0,    0,    0,    //
    128, 0x4B, 0x00, 0x82, //
    255, 0,    0,    0     //
};
