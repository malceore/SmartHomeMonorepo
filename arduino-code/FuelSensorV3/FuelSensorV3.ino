// -*- mode: c++;  c-basic-offset: 2 -*-
/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Based on the RGBLamp example
 * Tested on Arduino Mega with Ethernet Shield
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
 
#define RTCMEMORYSTART 65
#define RTCMEMORYLEN 127
extern "C" {
  #include "user_interface.h" // this is for the RTC memory read/write functions
}

#include <Arduino.h>
#include <stdio.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char* deviceTypes[] = {"MultiLevelSensor", "Sensor", nullptr};
ThingDevice device("fuelsensor", "Fuel Sensor", deviceTypes);
ThingProperty property("level", "Fuel Level", NUMBER, "LevelProperty");
WebThingAdapter* adapter;

typedef struct {
  int lastValue;
  int other;
} rtcStore;
// Now we store in RTC memory to stay over reset.
rtcStore rtcMem;

const char* ssid = "GOAT";
const char* password = "goatseatgrass";
const int pingPin = 15; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 13; // Echo Pin of Ultrasonic Sensor

void setup(void) {
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  Serial.println("");
    
  int buckets = (sizeof(rtcMem) / 4);
  int rtcPos = RTCMEMORYSTART * buckets;
  system_rtc_mem_read(rtcPos, &rtcMem, sizeof(rtcMem));
  int lastValue = rtcMem.lastValue;
  long value = getValue();
  int percent = (double) 100. - (value/90.*100.);

  Serial.print("percent ");
  Serial.println(percent);
  Serial.print("lastvalue ");  
  Serial.println(lastValue);
  
  if(lastValue != percent) {
    rtcMem.lastValue = percent;
    rtcMem.other = 0;
    system_rtc_mem_write(rtcPos, &rtcMem, buckets * 4);

    Serial.println("");
    Serial.print("Connecting to \"");
    Serial.print(ssid);
    Serial.println("\"");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
    // Wait for connection
    bool blink = true;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      blink = !blink;
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(300);
    adapter = new WebThingAdapter("fuelsensor", WiFi.localIP());
    device.addProperty(&property);
    adapter->addDevice(&device);
    adapter->begin();
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.print("/things/");
    Serial.println("fuelsensor");

    ThingPropertyValue levelValue;
    levelValue.number = percent;
    delay(3000);
    property.setValue(levelValue);  
    adapter->update();
    delay(3000);
    property.setValue(levelValue);  
    adapter->update();
  }
  
  //ESP.deepSleep(10e6); //DEBUGGING
  ESP.deepSleep(10e7);
}

long getValue(){
  float duration, distance;
  // Clears the trigPin
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  // Sets the pingPin on HIGH state for 10 micro seconds
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance=duration*0.034/2;
  return (int) distance;
}

void loop(void) {
}
