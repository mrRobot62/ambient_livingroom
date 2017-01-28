#ifndef _WOODLAMP_H_
#define _WOODLAMP_H_

#include <FastLED.h>

#define COLOR_ORDER GRB
#define CHIPSET_DEFAULT WS2811
#define NUM_LEDS_DEFAULT 60
#define LED_PIN_DEFAULT 5

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
  void update() { FastLED.show(); }

private:
  AmbientLight() { this->init(); };
  ~AmbientLight();
  AmbientLight(const AmbientLight &);
  AmbientLight &operator=(const AmbientLight &);
  static AmbientLight *_instance;

  void init();

  CRGB leds[NUM_LEDS_DEFAULT];
  uint8_t brightness;
};

#endif
