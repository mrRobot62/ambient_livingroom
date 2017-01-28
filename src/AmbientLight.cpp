#include "AmbientLight.h"

AmbientLight *AmbientLight::_instance = nullptr;

void AmbientLight::init() {

  FastLED
      .addLeds<CHIPSET_DEFAULT, LED_PIN_DEFAULT, COLOR_ORDER>(leds,
                                                              NUM_LEDS_DEFAULT)
      .setCorrection(TypicalLEDStrip);
}

void AmbientLight::setHSV(uint16_t hue, uint8_t sat, uint8_t bright) {
  if (hue < 0 || sat < 0 || bright < 0)
    return;
  hue = constrain(hue, 0, 360);
  sat = constrain(sat, 0, 100);
  brightness = constrain(bright, 0, 100);
  FastLED.showColor(CHSV(hue, sat, brightness));
}

void AmbientLight::setFixColor(unsigned long value) {}

void AmbientLight::setBrightness(uint8_t value) {
  brightness = constrain(value, 0, 100);
}
