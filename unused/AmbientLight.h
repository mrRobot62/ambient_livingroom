#ifndef _WOODLAMP_H_
#define _WOODLAMP_H_

#include <FastLED.h>

#define COLOR_ORDER GRB
#define CHIPSET_DEFAULT WS2811
#define NUM_LEDS_DEFAULT 65
#define LED_PIN_DEFAULT 0
#define COLOR_NUMBERS 20
/*******************************************************************************
*
* AmbientLight
* controlled via FHEM. Data will be send via MQTT
*
* Control mechanism
* Color (R - G - B) from 0..255
* Saturation from 0..255
* FixColor pattern (0..17)
*
*
*******************************************************************************/
class AmbientLight {
public:
  static AmbientLight *getInstance() {
    return (!_instance) ? _instance = new AmbientLight() : _instance;
  }

  void setHSV(uint16_t hue, uint8_t sat, uint8_t bright);
  void setBrightness(uint8_t bright);
  void setFixColor(unsigned long value);
  void update() {
    this->gHue++;
    FastLED.show();
  }
  bool setPayload(String group, String payload, String &logMsg);
  void testPattern(uint8_t id = 0);
  void off(bool clearData = true) { FastLED.clear(clearData); };
  uint8_t getHue() { return gHue; }

private:
  const int colorValues[COLOR_NUMBERS] = {
      0x000000, 0xF0F8FF, 0xFAEBD7, 0xF0FFFF, 0x00FFFF, 0x00008B, 0x008B8B,
      0xB8860B, 0xFF8C00, 0x00CED1, 0x4B0082, 0xE0FFFF, 0x191970, 0xFFA500,
      0xFF4500, 0x4169E1, 0x40E0D0, 0xEE82EE, 0xFFFFFF};
  AmbientLight() { this->init(); };
  ~AmbientLight();
  AmbientLight(const AmbientLight &);
  AmbientLight &operator=(const AmbientLight &);
  static AmbientLight *_instance;

  void init();

  CRGB leds[NUM_LEDS_DEFAULT];
  uint8_t gBrightness, gHue;

  typedef void (*PatternFunc)(long p1, long p2, long p3);

  typedef struct {
    PatternFunc mPattern;
    uint16_t param1;
    uint16_t param2;
    uint16_t param3;
  } PatternFuncType;

  PatternFuncType patterns[] = {
      {FixColor, 0x000000, 0, 0}, {FixColor, 0xF0F8FF, 0, 0},
      {FixColor, 0xFAEBD7, 0, 0}, {FixColor, 0xF0FFFF, 0, 0},
      {FixColor, 0x00FFFF, 0, 0}, {FixColor, 0x00008B, 0, 0},
      {FixColor, 0x008B8B, 0, 0}, {FixColor, 0xB8860B, 0, 0},
      {FixColor, 0xFF8C00, 0, 0}, {FixColor, 0x00CED1, 0, 0},
      {FixColor, 0x4B0082, 0, 0}, {FixColor, 0xE0FFFF, 0, 0},
      {FixColor, 0x191970, 0, 0}, {FixColor, 0xFFA500, 0, 0},
      {FixColor, 0xFF4500, 0, 0}, {FixColor, 0x4169E1, 0, 0},
      {FixColor, 0x40E0D0, 0, 0}, {FixColor, 0xEE82EE, 0, 0},
      {FixColor, 0xFFFFFF, 0, 0},
  };

  void FixColor(long p1, long p2, long p3);
  void GradientColor(long p1, long p2, long p3);
};

#endif
