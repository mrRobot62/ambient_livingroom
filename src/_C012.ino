//#######################################################################################################
//########################### Controller Plugin 011: FHEM MQTT
//#######################################
//#######################################################################################################

#define CPLUGIN_012
#define CPLUGIN_ID_012 12
#define CPLUGIN_NAME_012 "FHEM MQTT"
#define INFO_LINES 3

unsigned long last_millis = 0;
// String log = "";
// char log[80];

boolean CPlugin_012(byte function, struct EventStruct *event, String &string) {
  boolean success = false;
  // String log = "";
  switch (function) {
  case CPLUGIN_PROTOCOL_ADD: {
    Protocol[++protocolCount].Number = CPLUGIN_ID_012;
    Protocol[protocolCount].usesMQTT = true;
    Protocol[protocolCount].usesTemplate = true;
    Protocol[protocolCount].usesAccount = false;
    Protocol[protocolCount].usesPassword = false;
    Protocol[protocolCount].defaultPort = 1883;
    break;
  }

  case CPLUGIN_GET_DEVICENAME: {
    string = F(CPLUGIN_NAME_012);
    break;
  }

  case CPLUGIN_PROTOCOL_TEMPLATE: {
    strcpy_P(Settings.MQTTsubscribe, PSTR("/FHEM/#"));
    strcpy_P(Settings.MQTTpublish, PSTR("/%sysname%/%tskname%/%valname%"));
    break;
  }

  case CPLUGIN_PROTOCOL_RECV: {
    // FHEM topic structure: /FHEM/Floor/Room/device/<plugin>/group/<number>

    //  Topic 0 = FHEM, 1=Floor (e.g. 1st), 2=Room (e.g kitchen)
    //  3=device (e.g. AmbientLight), 4=plugin (e.g. _P199_FastLED)
    //  5=group (e.g hsv, pattern, switch)
    //  6=number (e.g. 16(pin))
    //
    //  Example for _P199_FastLED: /FHEM/Floor/Room/device/_P199_FastLED
    //    .../_P199_FastLED/hsv     => payload three values
    //                                  (hue, staturation,brightness)
    //    .../_P199_FastLED/pattern/id => payload two values
    //                                  (param1,param2)
    //    .../_P199_FastLED/switch => payload two values (ON/OFF)
    //    .../_P199_FastLED/dimmer => payload one value (0..100)
    //
    //  Example : SWITCH (_P001_Switch)
    //  ... /device/_P001_Switch/gpio/<port>  => payload ON/OFF
    // Split topic into array
    // start up from FHEM
    String tmpTopic = event->String1.substring(1);
    addLog(LOG_LEVEL_INFO, tmpTopic);

    //
    // 1st - split topic string into a list
    String topicSplit[10];
    int SlashIndex = tmpTopic.indexOf('/');
    byte count = 0;
    while (SlashIndex > 0 && count < 10 - 1) {
      topicSplit[count] = tmpTopic.substring(0, SlashIndex);
      tmpTopic = tmpTopic.substring(SlashIndex + 1);
      SlashIndex = tmpTopic.indexOf('/');
      count++;
    }
    topicSplit[count] = tmpTopic;

    // 2nd - Check if 5th entry is a plugin name
    //       Check 6th entry it's a group or command entry
    // a ESPEasy plugin starts everytime with an "_" => _P199_FastLED
    String plugin = topicSplit[4];
    String group = topicSplit[5];
    struct EventStruct TempEvent;

    //
    // 3rd last (4) topic is the ESPEasy-Plugin name
    // 2nd last is the group/command (5) for given EASPEasy-Plugin
    // last is(can) be a value like portnumber, ...
    // payload is everytime the value for this group

    // start 5th entry with "_" - if yes, it is a plugin
    if (plugin.substring(0, 1) == "_") {
      String msg = "";
      char tmp[20];
      TempEvent.Source = VALUE_SOURCE_MQTT;
      /*
      sprintf_P(log,
        PSTR("call plugin %s, Group %s, ID %s, Payload %s"),
        plugin.toCharArray(tmp, 20);

      )
      */
      msg = "call plugin (";
      msg += plugin;

      msg += ") Group (";
      msg += group;

      if (count > 5) {
        msg += ") ID (";
        msg += topicSplit[count];
      }

      msg += ") Payload (";
      msg += event->String2;
      msg += ")";

      addLog(LOG_LEVEL_INFO, msg);

      // transmitted payload is allways read as string and converted to
      // plugin relevant values inside the plugin, except simple values like
      // ON/OFF it's done below
      // => String2 allways store payload
      // => String1 allways store group
      String payload = event->String2;
      TempEvent.String2 = payload;
      TempEvent.String1 = group;
      // if not set par1+2 to -1 (not used)
      TempEvent.Par1 = TempEvent.Par2 = TempEvent.Par3 = -1;
      //
      // if an ID is given convert it into number and store value into event
      // allways as event.Par1
      if (count > 5) {

        msg = " value: ";
        msg += topicSplit[6];
        addLog(LOG_LEVEL_INFO, msg);

        TempEvent.Par1 = topicSplit[6].toInt();
      }

      //
      // check payload of boolean values
      // if set, use event.Par2 to store setting
      event->String2.toLowerCase();
      if (event->String2 == "true" || event->String2 == "on" ||
          event->String2 == "an") {
        TempEvent.Par2 = 1;
      } else {
        TempEvent.Par2 = 0;
      }
      //
      // call this plugin
      PluginCall(PLUGIN_WRITE, &TempEvent, plugin);
    } else {
      String msg = "no valid plugin set via MQTT (FHEM)";
      addLog(LOG_LEVEL_INFO, msg);
    }
    break;
  }

  case CPLUGIN_PROTOCOL_SEND: {
    statusLED(true);

    if (ExtraTaskSettings.TaskDeviceValueNames[0][0] == 0)
      PluginCall(PLUGIN_GET_DEVICEVALUENAMES, event, dummyString);

    String pubname = Settings.MQTTpublish;
    pubname.replace("%sysname%", Settings.Name);
    pubname.replace("%tskname%", ExtraTaskSettings.TaskDeviceName);
    pubname.replace("%id%", String(event->idx));

    String value = "";
    byte DeviceIndex =
        getDeviceIndex(Settings.TaskDeviceNumber[event->TaskIndex]);
    byte valueCount = getValueCountFromSensorType(event->sensorType);
    /*
    for (byte x = 0; x < valueCount; x++) {
      String tmppubname = pubname;
      tmppubname.replace("%valname%",
                         ExtraTaskSettings.TaskDeviceValueNames[x]);
      if (event->sensorType == SENSOR_TYPE_LONG) {
        value = (unsigned long)UserVar[event->BaseVarIndex] +
                ((unsigned long)UserVar[event->BaseVarIndex + 1] << 16);
      } else {
        value = toString(UserVar[event->BaseVarIndex + x],
                         ExtraTaskSettings.TaskDeviceValueDecimals[x]);
      }
      MQTTclient.publish(tmppubname.c_str(), value.c_str(),
                         Settings.MQTTRetainFlag);
    }
    */
    char infoText[INFO_LINES][40];
    LoadCustomTaskSettings(event->TaskIndex, (byte *)&infoText,
                           sizeof(infoText));

    String tmppubname = pubname;
    tmppubname.replace("%valname%", ExtraTaskSettings.TaskDeviceValueNames[0]);
    value = toString(UserVar[event->BaseVarIndex + 0],
                     ExtraTaskSettings.TaskDeviceValueDecimals[0]);
    value += " ";
    value += ExtraTaskSettings.TaskDeviceValueNames[1];
    value += ": ";
    value += toString(UserVar[event->BaseVarIndex + 1],
                      ExtraTaskSettings.TaskDeviceValueDecimals[0]);
    value += " ";
    value += ExtraTaskSettings.TaskDeviceValueNames[2];
    value += ": ";
    value += toString(UserVar[event->BaseVarIndex + 2],
                      ExtraTaskSettings.TaskDeviceValueDecimals[0]);
    value += " InfoText:'";
    for (byte x = 0; x < INFO_LINES; x++) {
      String tmpString = infoText[x];
      if (tmpString.length()) {
        value += tmpString;
        value += "/";
      }
    }
    value += "'";
    MQTTclient.publish(tmppubname.c_str(), value.c_str(),
                       Settings.MQTTRetainFlag);
    /*
    char buf[80];
    sprintf("Broker message TO: %s VALUE: %s", tmppubname.c_str(),
            value.c_str());
    */
    addLog(LOG_LEVEL_INFO, "Broker message sent");
    break;
  }
    return success;
  }
}
