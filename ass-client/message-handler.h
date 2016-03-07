void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  // Ignoring topic as there is only the on we subscribed to
  StaticJsonBuffer<200> jsonBuffer;
  char charBuffer[200] = ""; // zero out buffer
  memcpy(charBuffer, payload, length);
  
  JsonObject& root = jsonBuffer.parseObject(charBuffer);
  if(root.success()) {
    // make sure animation starts at zero
    g_gotoStart = true; 
    g_frames = root["frames"];
    g_delay = root["delay"];
  }
}

void transmitHeartBeat() {
  static uint32_t beatNbr = 0;
  StaticJsonBuffer<100> jsonBuffer;
  char transmitBuffer[100];
  JsonObject& root = jsonBuffer.createObject();
  root["id"] = ESP_ID;
  root["beat"] = beatNbr++;

  // publish something like this
  // {"id":0,"beat":10}
  root.printTo(transmitBuffer, sizeof(transmitBuffer));
  mqttClient.publish(TOPIC_HEARTBEAT, transmitBuffer);
}

bool reconnect() {
  char clientId[50] = { 0 };
  // connect with uniqe id
  snprintf(clientId, sizeof(clientId), "fredrik_40_ESP%d", ESP_ID); 
  reconnects++;
  mqttClient.connect(clientId); 
  return mqttClient.connected();
}

void setupMqtt() {
  // broker to use
  mqttClient.setServer(mqtt_broker, 1883);
  // Runs on subscribed topics
  mqttClient.setCallback(callback);
}

