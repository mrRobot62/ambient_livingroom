//#######################################################################################################
//#################################### Plugin 199: FastLED
//#######################################
//#######################################################################################################

// Command: FastLED <led nr>,<red>,<green>,<blue>

//#include "AmbientLight.h"

#define PLUGIN_199
#define PLUGIN_ID_199 199
#define PLUGIN_NAME_199 "FastLED"
#define PLUGIN_VALUENAME1_199 "_P199_FastLED"

boolean initialized = false;
boolean test = false;
// AmbientLight *ambientLight;

boolean Plugin_199(byte function, struct EventStruct *event, String &string) {
  boolean success = false;
  String paramString = string;
  // ambientLight = ambientLight->getInstance();
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
    // Witty_init();
    success = true;
    IPAddress ip = WiFi.localIP();
    String log = F("LocalIP: ");
    log += ip.toString();
    addLog(LOG_LEVEL_INFO, log);
    AmbientLight_init();
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

  /*

    write information back zu server

    possible commands
    hsv     => user set HSV color. Data is stored in user pattern
            .../AmbientLight1/hsv/<id>
    rgb     => user set RGB color. Data is stored in user pattern
            .../AmbientLight1/rgb/<id>
    pattern => user set pattern ID. (see PatternFuncTyp list)
            .../AmbientLight1/pattern/<id>
    test    => create a test pattern
    witty   => use a witty board
    switch  => ON/OFF led stripe
    dimmer  => set generell brightness for current pattern
    }
  */
  case PLUGIN_WRITE: {
    String log = "PLUGIN_WRITE: ";
    String cmd = parseString(string, 1);
    // String cmd2 = parseString(string, 2);
    log += "CMD (" + cmd + ") ";
    // log += "CMD2 (" + cmd2 + ") " ;
    log += "String 1 (" + event->String1 + ") ";
    log += "String 2 (" + event->String2 + ") ";
    log += "Par1 (" + String(event->Par1) + ") "; // id
    log += "Par2 (" + String(event->Par2) + ") "; // on/off (e.g. for switch)
    log += "Par3 (" + String(event->Par3) + ") "; // -1

    addLog(LOG_LEVEL_INFO, log);
    cmd.toLowerCase();
    initialized = false;
    if (cmd == "hsv") {
      log = "(hsv)";
      log += "with ID: ";
      log += String(event->Par1);
      log += " payload(";
      log += event->String2;
      log += ")";
      addLog(LOG_LEVEL_INFO, log);
      //
      setHSVColor(event->Par1, event->String2);
      //
      initialized = true;
      success = true;
    } else if (cmd == "rgb") {
      log = "(rgb)";
      addLog(LOG_LEVEL_INFO, log);
      //
      setRGBColor(event->Par1, event->String2);
      //
      initialized = true;
      success = true;
    } else if (cmd == "pattern") {
      log = "(pattern)";
      addLog(LOG_LEVEL_INFO, log);
      //

      //
      initialized = true;
      success = true;
    } else if (cmd == "test") {
      log = "(test)";
      addLog(LOG_LEVEL_INFO, log);
      initialized = true;
      success = true;
      TEST_Simple(event->Par1);
    } else if (cmd == "witty") {
      success = true;
      addLog(LOG_LEVEL_INFO, "Witty command");

      String rgb[3];
      rgb[0] = parseString(event->String2, 1);
      rgb[1] = parseString(event->String2, 2);
      rgb[2] = parseString(event->String2, 3);
      rgb[0].toLowerCase();
      rgb[1].toLowerCase();
      rgb[2].toLowerCase();
      Witty_setRed(0, false);
      Witty_setGreen(0, false);
      Witty_setBlue(0, false);

      log = "(" + rgb[0] + "/";
      log += rgb[1] + "/";
      log += rgb[2] + ")";
      addLog(LOG_LEVEL_INFO, log);

      if (rgb[0] == "on")
        Witty_setRed(500);
      if (rgb[1] == "on")
        Witty_setGreen(500);
      if (rgb[2] == "on")
        Witty_setBlue(500);
    } else if (cmd == "pattern") {
      success = true;
      addLog(LOG_LEVEL_INFO, "not implemented yet");

    } else if (cmd == "switch") {
      initialized = true;
      success = true;
      //      addLog(LOG_LEVEL_INFO, "not implemented yet");
      if (event->Par1 >= 0 && event->Par1 <= 16) {
        log = "SW GPIO(";
        log += String(event->Par1) + ") ";
        log += (event->Par2) ? "ON" : "OFF";
        addLog(LOG_LEVEL_INFO, log);
        pinMode(event->Par1, OUTPUT);
        digitalWrite(event->Par1, constrain(event->Par2, 0, 1));
      } else {
        log += "wrong gpio port (" + String(event->Par1) + ")";
        addLog(LOG_LEVEL_INFO, log);
      }
    } else if (cmd == "dimmer") {
      initialized = true;
      success = true;
      // addLog(LOG_LEVEL_INFO, "not implemented yet");
      int p1 = event->Par1;
      if (p1 >= 0 && p1 <= 100) {
        addLog(LOG_LEVEL_INFO, "set brightness(" + String(p1) + ")");
        // ambientLight->setBrightness(p1);
        // ambientLight->update();
      }
    }
    break;
  }
  case PLUGIN_ONCE_A_SECOND: {
    String log = "";
    if (initialized) {
      // ambientLight->update();
      log = "Current Pattern ";
      log += getCurrentPattern();
      log += " Brightness: ";
      log += getBrightness();
      addLog(LOG_LEVEL_INFO, log);
      gHue++;
    } else {
      AmbientLightOff();
    }
    break;
  }

  case PLUGIN_TEN_PER_SECOND: {
    if (initialized) {
      update();
      // TEST_Simple1();
    }
    break;
  }
  }
  return success;
}
