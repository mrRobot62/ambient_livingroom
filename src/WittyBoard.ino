
#ifndef __WHITTY_H__
#define __WHITTY_H

#include <inttypes.h>
#include <stdarg.h>

#define LED_GREEN 12 // GPIO12 = D6
#define LED_BLUE 13  // GPIO12 = D7
#define LED_RED 15   // GPIO15 = D8
#define LED_BLUE2 2  // GPIO02 = D4

#define BUTTON 4  // GPIO04 = D2
#define ADC0 A0   // ADC = A0
#define GPIO16 16 // = D0
#define GPIO14 14 // = D5
#define GPIO05 5  // = D1

/**
  Simple class to work with a Wittyboard

  see: http://www.schatenseite.de/2016/04/22/esp8266-witty-cloud-modul/

  To use in ESPEasy it is implemented as a singleton

**/

void Witty_init() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_BLUE2, OUTPUT);

  pinMode(BUTTON, INPUT);
  pinMode(ADC0, INPUT_PULLUP);
}

void Witty_setLED(uint8_t pin, bool on=true) { digitalWrite(pin, on); }
void Witty_setRGB(uint8_t pin, long ms = 0, bool on = true) {
  if (pin == LED_BLUE || pin == LED_BLUE2 || pin == LED_RED ||
      pin == LED_GREEN) {
    long last = millis();
    Witty_setLED(pin, on);
    while (millis() - last < ms) {
      ;
    }
    Witty_setLED(LED_RED, false);
  }
}

void Witty_setRed(uint16_t ms, bool on=true) { Witty_setRGB(LED_RED, ms,on); }

void Witty_setGreen(uint16_t ms, bool on=true) { Witty_setRGB(LED_GREEN, ms,on); }

void Witty_setBlue(uint16_t ms, bool on=true) { Witty_setRGB(LED_BLUE, ms,on); }

void Witty_setBlue2(uint16_t ms, bool on=true) { Witty_setRGB(LED_BLUE2, ms,on); }
#endif
