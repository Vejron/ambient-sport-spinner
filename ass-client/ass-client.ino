#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

// Tell lady Ada wee are using the esp8266
#define ESP8266_L
#include <Adafruit_NeoPixel.h>

//Tabs
#include "settings.h"
#include "globals.h"
#include "message-handler.h"

void setup() {
  
  // Stepper
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);

  // optical switch
  pinMode(OPTO_PIN, INPUT);
  
  // WS2812 setup
  pinMode(NEO_PIN, OUTPUT);
  strip.begin();
  
  //test strip
  startupLedSequence();

  // WIFI & MQTT setup
  Serial.begin(115200);
  setupWifi();
  setupMqtt();
  stepper.setMaxSpeed(8000);
  stepper.setAcceleration(6000);
}

// 4th step
// 200 * 4 = 800 steps/rev
// 6 stops = 800 / 6 = 133.3 steps = 133 * 6 & 2 extra
void loop() {
  const int STEPS_PER_FULL_ROTATION = 6;
  static uint32_t steps = 0;
  static byte nextColor;

  // Reconnect if for some reason we got disconnected
  if (!mqttClient.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > RECONNECT_DELAY) {
      lastReconnectAttempt = now;
      // Show progress
      reconnectSpinner();
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
        // Once connected, publish an announcement...
        StaticJsonBuffer<200> jsonBuffer;
        char transmitBuffer[50];
        JsonObject& root = jsonBuffer.createObject();
        root["id"] = ESP_ID;
        root["reconnects"] = reconnects;
        root["time"] = millis();

        // publish something like this
        //{"id":0,"reconnects":10,"time":100000000}
        root.printTo(transmitBuffer, sizeof(transmitBuffer));
        mqttClient.publish(TOPIC_STATUS, transmitBuffer);
        
        // And resubscribe as all subscriptions 
        // will be lost on disconnect
        mqttClient.subscribe(TOPIC_RUN);
      }
    }
  } else {
    // should we set known start
    if (g_gotoStart) {
      g_gotoStart = false;
      digitalWrite(ENABLE_PIN, LOW);
      findStart();
    }

    // turn stepper
    if (stepper.distanceToGo() == 0 && g_frames >= 0)
    {
          // flash
          strip.setBrightness(255);
          setColor(Wheel(nextColor += 13));
          delay(g_delay);
          setColor(0);
          
          // move to next frame
          if(g_frames > 0) {
            steps++;
            if(steps % STEPS_PER_FULL_ROTATION)
              stepper.move(-133);
            else 
              stepper.move(-(133 + 2));  // extra steps at last turn to make up for fraction part
          } else {
            //shutdown stepper so that it doesn't melt the plastic :)
            digitalWrite(ENABLE_PIN, HIGH); // active low
          }
          g_frames--;
    }
    stepper.run();

    // heart beat transmission
    long now = millis();
    if (now - lastBeat > HEART_BEAT_RATE) {
      lastBeat = now;
      transmitHeartBeat();
    }
  }
  // Neded to update internals of pubSub
  mqttClient.loop();
}

void oneStep(bool dir) {
  digitalWrite(DIRECTION_PIN, dir);
  digitalWrite(STEP_PIN, HIGH);
  delay(2);
  digitalWrite(STEP_PIN, LOW);
  delay(2);
}

void findStart() {
  bool nowOpto, lastOpto;
  lastOpto = digitalRead(OPTO_PIN);

  for(;;) {
    oneStep(true);
    nowOpto = digitalRead(OPTO_PIN);
    if(nowOpto && !lastOpto) {
       // edge detected
       setColor(0xff0000);
       delay(200);
       break;
    }
    lastOpto = nowOpto;
  }
  
  // back upp some to get to start
  setColor(0x0000ff);
  for(int i = 0; i < 75; i++) {
    oneStep(false);
  }
  
  // done animation
  if(true) {
    for(int i = 0; i < 3; i++) {
      setColor(0x00ff00);
      delay(200);
      setColor(0);
      delay(200);
    }
  }
}

void setColor(uint32_t c) {
   for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void setColor(uint8_t r, uint8_t g, uint8_t b) {
   for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void startupLedSequence() {
  colorWipe(0xff0000,200);
  delay(500);
  colorWipe(0x00ff00,200);
  delay(500);
  colorWipe(0x0000ff,200);
  delay(500);
  strip.clear();
  strip.show();
}

void reconnectSpinner() {
  static byte c = 0;
  static uint8_t i = 0;
  strip.clear();
  strip.setPixelColor(i, Wheel(c += 16));
  strip.setBrightness(50); // dimmed down
  strip.show();
  if(++i >= strip.numPixels())
    i = 0;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
