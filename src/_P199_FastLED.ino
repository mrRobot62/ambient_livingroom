//#######################################################################################################
//#################################### Plugin 199: FastLED
//#######################################
//#######################################################################################################

// Command: FastLED <led nr>,<red>,<green>,<blue>

#include "AmbientLight.h"

#define PLUGIN_199
#define PLUGIN_ID_199 199
#define PLUGIN_NAME_199 "FastLED"
#define PLUGIN_VALUENAME1_199 "_P199_FastLED"

boolean initialized = false;
AmbientLight *ambientLight;

boolean Plugin_199(byte function, struct EventStruct *event, String &string) {
  boolean success = false;
  String paramString = string;
  ambientLight = ambientLight->getInstance();
  switch (function) {

  /* create a new device */
  case PLUGIN_DEVICE_ADD: {
    Device[++deviceCount].Number = PLUGIN_ID_199;
    Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
    Device[deviceCount].VType = SENSOR_TYPE_SINGLE;
    Device[deviceCount].Ports = 0;
    Device[deviceCount].PullUpOption = false;
    Device[deviceCount].InverseLogicOption = false;
    Device[deviceCount].FormulaOption = false;
    Device[deviceCount].ValueCount = 1;
    Device[deviceCount].SendDataOption = false;
    Device[deviceCount].TimerOption = true;
    break;
  }

  /* return the device name */
  case PLUGIN_GET_DEVICENAME: {
    string = F(PLUGIN_NAME_199);
    break;
  }

  /* return a list of device names */
  case PLUGIN_GET_DEVICEVALUENAMES: {
    strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0],
             PSTR(PLUGIN_VALUENAME1_199));
    break;
  }

  /* load web site and publish data */
  case PLUGIN_WEBFORM_LOAD: {
    char tmpString[128];

    /* insert a java script function */
    /* java script did not work under ESPEasy :-(
    string += PSTR("<script>"
                   "function myFunction(field, val) {"
                   "   document.getElementById(field).innerHTML = val;"
                   "}"
                   "</script>");
    */
    /* -- Pattern -- */
    const uint8_t SIZE = 20;
    String options[SIZE] = {"",
                            F("AliceBlue"),
                            F("AliceBlue"),
                            F("AntiqueWhite"),
                            F("Azure"),
                            F("Cyan"),
                            F("DarkBlue"),
                            F("DarkCyan"),
                            F("DarkGoldenrod"),
                            F("DarkOrange"),
                            F("DarkTurquoise"),
                            F("Indigo"),
                            F("LightCyan"),
                            F("MidnightBlue"),
                            F("Orange"),
                            F("OrangeRed"),
                            F("RoyalBlue"),
                            F("Turquoise"),
                            F("Violet"),
                            F("White")};
    int optionValues[SIZE] = {0x000000, 0xF0F8FF, 0xFAEBD7, 0xF0FFFF, 0x00FFFF,
                              0x00008B, 0x008B8B, 0xB8860B, 0xFF8C00, 0x00CED1,
                              0x4B0082, 0xE0FFFF, 0x191970, 0xFFA500, 0xFF4500,
                              0x4169E1, 0x40E0D0, 0xEE82EE, 0xFFFFFF};

    long choice = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
    string += F("<TR><TD>Fix Color:<TD><select name='plugin_199_pattern'>");
    for (byte x = 0; x < SIZE; x++) {
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

    /* -- Color model HSV/HSB --*/

    sprintf_P(
        tmpString,
        PSTR("<TR><TD>Hue:<TD><input type='number' "
             "name='plugin_199_hue' min='-1' max='360' step='1' value='%u' "
             "oninput='mySlider('hue_val', this.value)'> (0-360) "),
        Settings.TaskDevicePluginConfig[event->TaskIndex][1]);
    string += tmpString;
    /*
    sprintf_P(tmpString, PSTR("<TD><p id='hue_val'> Value: %u</p></TD>"),
              Settings.TaskDevicePluginConfig[event->TaskIndex][1]);
    string += tmpString;
    */
    sprintf_P(tmpString, PSTR("<TR><TD>Saturation:<TD><input type='number' "
                              "name='plugin_199_saturation' min='-1' max='100' "
                              "oninput='mySlider('sat_val', this.value)' "
                              "step='1' value='%u'> (0-100) "),
              Settings.TaskDevicePluginConfig[event->TaskIndex][2]);
    string += tmpString;
    /*
    sprintf_P(tmpString, PSTR("<TD><p id='sat_val'> Value: %u</p></TD>"),
              Settings.TaskDevicePluginConfig[event->TaskIndex][2]);
    */
    sprintf_P(tmpString, PSTR("<TR><TD>Brightness:<TD><input type='number' "
                              "name='plugin_199_brightness' min='-1' max='100' "
                              "oninput='mySlider('bright_val', this.value)' "
                              "step='1' value='%u'> (0-100)"),
              Settings.TaskDevicePluginConfig[event->TaskIndex][3]);
    string += tmpString;
    /*
    sprintf_P(tmpString, PSTR("<TD><p id='bright_val'> Value: %u</p></TD>"),
              Settings.TaskDevicePluginConfig[event->TaskIndex][3]);
    */
    success = true;
    break;
  }

  /* save current web site data into flash */
  case PLUGIN_WEBFORM_SAVE: {
    String plugin = "";
    plugin = WebServer.arg(F("plugin_199_pattern"));
    Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin.toInt();
    plugin = WebServer.arg(F("plugin_199_hue"));
    Settings.TaskDevicePluginConfig[event->TaskIndex][1] = plugin.toInt();
    plugin = WebServer.arg(F("plugin_199_saturation"));
    Settings.TaskDevicePluginConfig[event->TaskIndex][2] = plugin.toInt();
    plugin = WebServer.arg(F("plugin_199_brightness"));
    Settings.TaskDevicePluginConfig[event->TaskIndex][3] = plugin.toInt();
    success = true;
    break;
  }

  /* initialize this plugin */
  case PLUGIN_INIT: {
    if (!initialized) {
      initialized = true;
    }
    success = true;
    break;
  }

  /* read data from Flash */
  case PLUGIN_READ: {
    byte id = 0;
    String log = F("current FastLED config ");
    UserVar[event->BaseVarIndex] =
        Settings.TaskDevicePluginConfig[event->TaskIndex][id];
    ;

    UserVar[event->BaseVarIndex + 1] =
        Settings.TaskDevicePluginConfig[event->TaskIndex][id + 1];

    UserVar[event->BaseVarIndex + 2] =
        Settings.TaskDevicePluginConfig[event->TaskIndex][id + 2];

    UserVar[event->BaseVarIndex + 3] =
        Settings.TaskDevicePluginConfig[event->TaskIndex][id + 3];

    log += F(" Pattern:");
    log += Settings.TaskDevicePluginConfig[event->TaskIndex][id];
    log += F(" Hue:");
    log += Settings.TaskDevicePluginConfig[event->TaskIndex][id + 1];
    log += F(" Sat:");
    log += Settings.TaskDevicePluginConfig[event->TaskIndex][id + 2];
    log += F(" Bright:");
    log += Settings.TaskDevicePluginConfig[event->TaskIndex][id + 3];
    addLog(LOG_LEVEL_INFO, log);
    success = true;
    break;
  }

  /* write information back zu server */
  case PLUGIN_WRITE: {
    String log = "PLUGIN_WRITE: ";
    String cmd = parseString(string, 1);
    addLog(LOG_LEVEL_INFO, ">> PLUGIN_WRITE..." + cmd);
    cmd.toLowerCase();
    if (cmd == "hsv") {
      success = true;
      // event->String2 stores payload
      // payload format for HSV is hue:sat:Bright
      int values[3];
      byte count = 0;
      String payload = event->String2;
      int idx = payload.indexOf(':');
      while (idx > 0 && count < 2) {
        values[count] = payload.substring(0, idx).toInt();
        payload = payload.substring(idx + 1);
        idx = payload.indexOf(':');
        count++;
      }
      // last part of payload
      values[count] = payload.toInt();
      log = "HSV - ";
      log += "H(" + String(values[0]) + ") ";
      log += "S(" + String(values[1]) + ") ";
      log += "V(" + String(values[2]) + ") ";
      addLog(LOG_LEVEL_INFO, log);
      ambientLight->setHSV(values[0], values[1], values[2]);

    } else if (cmd == "pattern") {
      success = true;
      addLog(LOG_LEVEL_INFO, "not implemented yet");

    } else if (cmd == "switch") {
      success = true;
      addLog(LOG_LEVEL_INFO, "not implemented yet");
      if (event->Par1 >= 0 && event->Par1 <= 16) {
        // valid gpio portnumber
      } else {
        log += "wrong gpio port (" + String(event->Par1) + ")";
        addLog(LOG_LEVEL_INFO, log);
      }
    } else if (cmd == "dimmer") {
      success = true;
      // addLog(LOG_LEVEL_INFO, "not implemented yet");
      int p1 = event->Par1;
      if (p1 >= 0 && p1 <= 100) {
        addLog(LOG_LEVEL_INFO, "set brightness(" + String(p1) + ")");
        ambientLight->setBrightness(p1);
        ambientLight->update();
      }
    }
    break;
  }

  case PLUGIN_TEN_PER_SECOND: {
    ambientLight->update();
    break;
  }
  }
  return success;
}
