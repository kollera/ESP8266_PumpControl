void ReadConfigParam() {
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_root, json["mqtt_root"]);
          //hc_sr04_interval = json["hc_sr04_interval"];
          //pin_hcsr04_trigger = json["pin_hcsr04_trigger"];
          //pin_hcsr04_echo = json["pin_hcsr04_echo"];
          //pin_sda = json["pin_sda"];
          //pin_scl = json["pin_scl"];
          //i2caddress_pfc8574 = json["i2caddress_pfc8574"];
          //i2caddress_oled = json["i2caddress_oled"];
          
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}

void CallWiFiManager() {
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  //set config save notify callback
  //wifiManager.setSaveConfigCallback(saveConfigCallback);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
  }
  
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

}

void MQTT_reconnect() {
  char topic[50];
  // Loop until we're reconnected
  //while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      memset(&topic[0], 0, sizeof(topic));
      snprintf (topic, sizeof(topic), "%s#", mqtt_root);
      Serial.println(topic);
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  //}
}

void MQTT_callback(char* topic, byte* payload, unsigned int length) {
  char msg[length+1];
  memset(msg,'-', length);
  msg[length]='\0';
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg[i]=(char)payload[i];  
  }
  Serial.print("Message: ");
  Serial.println(msg);

  if(strstr(topic,mqtt_root)) {
    //Serial.println(topic+strlen(MQTT_ROOT));
    /* ------------ Status PLAY / PAUSE / STOP ------------- */
    if (strcmp(topic+strlen(mqtt_root),"Ventil1/on-for-timer")==0)                { PCF8574_onfortimer(msg,65); }
    if (strcmp(topic+strlen(mqtt_root),"Ventil2/on-for-timer")==0)                { PCF8574_onfortimer(msg,66); }
    
    if (strstr(topic+strlen(mqtt_root),"on-for-timer")) { 
      Serial.println("on-for-timer gefunden");
      char port[1];
      strncpy(port, topic+strlen(mqtt_root)+6, 1); // mqtt_root/ventil1/on-for-timer
      //TODO: Länge finden um auch zb. Port 11 abbiden zu können
      Serial.print("on-for-timer: Pin gefunden: ");Serial.print(port);
      //PCF8574_onfortimer(msg,port);
    }
  }
}
