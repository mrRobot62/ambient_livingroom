//#######################################################################################################
//#################################### Plugin 199: FastLED
//#######################################
//#######################################################################################################

// Command: FastLED <led nr>,<red>,<green>,<blue>

#include "WoodLamp.h"

#define PLUGIN_199
#define PLUGIN_ID_199 199
#define PLUGIN_NAME_199 "FastLED"
#define PLUGIN_VALUENAME1_199 "FastLED"

#define COLOR_ORDER GRB
#define CHIPSET_DEFAULT WS2811
#define NUM_LEDS_DEFAULT 60
#define LED_PIN_DEFAULT 5

CRGB leds[NUM_LEDS_DEFAULT];

boolean initialized = false;

boolean Plugin_199(byte function, struct EventStruct *event, String &string) {
  boolean success = false;

  switch (function) {

  case PLUGIN_DEVICE_ADD: {
    Device[++deviceCount].Number = PLUGIN_ID_199;
    Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
    Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
    Device[deviceCount].Ports = 0;
    Device[deviceCount].PullUpOption = false;
    Device[deviceCount].InverseLogicOption = false;
    Device[deviceCount].FormulaOption = false;
    Device[deviceCount].ValueCount = 4;
    Device[deviceCount].SendDataOption = false;
    Device[deviceCount].TimerOption = true;
    break;
  }

  case PLUGIN_GET_DEVICENAME: {
    string = F(PLUGIN_NAME_199);
    break;
  }

  case PLUGIN_GET_DEVICEVALUENAMES: {
    strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0],
             PSTR(PLUGIN_VALUENAME1_199));
    break;
  }

  case PLUGIN_WEBFORM_LOAD: {
    char tmpString[128];
    string += F("<TR><TD>GPIO:<TD>");
    addPinSelect(false, string, "taskdevicepin1",
                 Settings.TaskDevicePin1[event->TaskIndex]);

    sprintf_P(tmpString, PSTR("<TR><TD>Led Count:<TD><input type='text' "
                              "name='plugin_199_leds' size='3' value='%u'>"),
              Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
    string += tmpString;
    /* -- Pattern -- */
    String options[6];
    options[0] = F("Single Color");
    options[1] = F("Gradient");
    options[2] = F("Fire");
    options[3] = F("double Fire");
    options[4] = F("Rainbow");
    options[5] = F("Wave");

    int optionValues[6];
    optionValues[0] = 0;
    optionValues[1] = 1;
    optionValues[2] = 2;
    optionValues[3] = 3;
    optionValues[4] = 4;
    optionValues[5] = 5;
    byte choice = ExtraTaskSettings.TaskDevicePluginConfigLong[1];
    string += F("<TR><TD>Pattern:<TD><select name='plugin_199_pattern'>");
    for (byte x = 0; x < 6; x++) {
      string += F("<option value='");
      string += optionValues[x];
      string += "'";
      if (choice == optionValues[x])
        string += F(" selected");
      string += ">";
      string += options[x];
      string += F("</option>");
    }
    string += F("</select>");

    /* -- Parameter 1-3 --*/

    sprintf_P(tmpString, PSTR("<TR><TD>Parameter 1:<TD><input type='text' "
                              "name='plugin_199_par1' size='3' value='%u'>"),
              ExtraTaskSettings.TaskDevicePluginConfigLong[2]);
    string += tmpString;

    sprintf_P(tmpString, PSTR("<TR><TD>Parameter 2:<TD><input type='text' "
                              "name='plugin_199_par2' size='3' value='%u'>"),
              ExtraTaskSettings.TaskDevicePluginConfigLong[3]);
    string += tmpString;

    sprintf_P(tmpString, PSTR("<TR><TD>Parameter 3:<TD><input type='text' "
                              "name='plugin_199_par3' size='3' value='%u'>"),
              ExtraTaskSettings.TaskDevicePluginConfigLong[4]);
    string += tmpString;

    success = true;
    break;
  }

  case PLUGIN_WEBFORM_SAVE: {
    String plugin1 = WebServer.arg(F("plugin_199_leds"));
    Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin1.toInt();
    String plugin2 = WebServer.arg(F("plugin_199_gpio"));
    Settings.TaskDevicePluginConfig[event->TaskIndex][1] = plugin2.toInt();
    success = true;
    break;
  }

  case PLUGIN_INIT: {
    if (!initialized) {
      FastLED
          .addLeds<CHIPSET_DEFAULT, LED_PIN_DEFAULT, COLOR_ORDER>(
              leds, NUM_LEDS_DEFAULT)
          .setCorrection(TypicalLEDStrip);
      initialized = true;
    }
    success = true;
    break;
  }

  case PLUGIN_WRITE: {
    if (initialized) {
      String tmpString = string;
      int argIndex = tmpString.indexOf(',');
      if (argIndex)
        tmpString = tmpString.substring(0, argIndex);

      if (tmpString.equalsIgnoreCase(F("FastLED"))) {
        char Line[80];
        char TmpStr1[80];
        TmpStr1[0] = 0;
        string.toCharArray(Line, 80);
        int Par4 = 0;
        if (GetArgv(Line, TmpStr1, 5))
          Par4 = str2int(TmpStr1);
        /*
      Plugin_199_pixels->setPixelColor(
          event->Par1 - 1,
          Plugin_199_pixels->Color(event->Par2, event->Par3, Par4));
      Plugin_199_pixels
          ->show(); // This sends the updated pixel color to the hardware.
          */
        success = true;
      }
    }
    break;
  }
  }
  return success;
}

void FIRE(uint8_t mode, uint16_t par1, uint16_t par2, uint16_t par3) {}

void RAINBOW(uint8_t mode, uint16_t par1, uint16_t par2, uint16_t par3) {}

void GRADIENT(uint8_t mode, uint16_t par1, uint16_t par2, uint16_t par3) {}
