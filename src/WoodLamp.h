#ifndef _WOODLAMP_H_
#define _WOODLAMP_H_

#include <FastLED.h>

class WoodLamp {
  WoodLamp(uint8_t gpio) {
    void run();
    void updateIR(uint16_t irvalue);
  }
};
#endif
