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
  gBrightness = constrain(bright, 0, 100);
  FastLED.showColor(CHSV(hue, sat, gBrightness));
}

void AmbientLight::setFixColor(unsigned long value) {
  value = constrain(value, 0, COLOR_NUMBERS);
  fill_solid(this->leds, NUM_LEDS_DEFAULT, CRGB(this->colorValues[value]));
}

void AmbientLight::setBrightness(uint8_t value) {
  gBrightness = constrain(value, 0, 100);
}

bool AmbientLight::setPayload(String group, String payload, String &logMsg) {
  bool success = false;
  group.toLowerCase();
  if (group == "hsv") {
    // event->String2 stores payload
    // payload format for HSV is hue:sat:Bright
    int values[3];
    byte count = 0;
    int idx = payload.indexOf(':');
    while (idx > 0 && count < 2) {
      values[count] = payload.substring(0, idx).toInt();
      payload = payload.substring(idx + 1);
      idx = payload.indexOf(':');
      count++;
    }
    // last part of payload
    values[count] = payload.toInt();
    logMsg = "AmbientLight::HSV - ";
    logMsg += "H(" + String(values[0]) + ") ";
    logMsg += "S(" + String(values[1]) + ") ";
    logMsg += "V(" + String(values[2]) + ") ";

    setHSV(values[0], values[1], values[2]);

  } else if (group = "pattern") {
    logMsg = "AmbientLight::pattern not implemented yet";
  } else if (group == "switch") {
    logMsg = "AmbientLight::switch not implemented yet";
  } else if (group == "dimmer") {
    logMsg = "AmbientLight::dimmer not implemented yet";
  } else if (group == "gradient") {
    logMsg = " set gradient ";
    // 0 = from COLOR
    // 1 = to COLOR
    // 2 = brightness
    // 3 = move
    const uint8_t SIZE = 4;
    int values[SIZE];
    byte count = 0;
    int idx = payload.indexOf(':');
    while (idx > 0 && count < SIZE - 1) {
      values[count] = payload.substring(0, idx).toInt();
      payload = payload.substring(idx + 1);
      idx = payload.indexOf(':');
      count++;
    }
    // last part of payload
    values[count] = payload.toInt();
    logMsg = " from (" + String(values[0]) + ") ";
    logMsg += "to(" + String(values[1]) + ") ";
    logMsg += "bright (" + String(values[2]) + ") ";
    logMsg += "move (" + String(values[2]) + ") ";
  }
  return success;
}

void AmbientLight::testPattern(uint8_t id) {
  switch (id) {
  case 0:
    fill_rainbow(this->leds, NUM_LEDS_DEFAULT, gHue);
    break;
  case 1:
    break;
  case 2:
    break;
  default:
    break;
  }
}
