// -*- mode: c++;  c-basic-offset: 2 -*-
/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Based on the RGBLamp example
 * Tested on Arduino Mega with Ethernet Shield
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Arduino.h>
#include <stdio.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char* deviceTypes[] = {"MultiLevelSensor", "Sensor", nullptr};
ThingDevice device("fuelsensor", "FuelSensor", deviceTypes);
ThingProperty property("level", "FuelLevel", NUMBER, "LevelProperty");
WebThingAdapter* adapter;

const char* ssid = "GOAT";
const char* password = "goatseatgrass";
const int pingPin = 15; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 13; // Echo Pin of Ultrasonic Sensor
const int ledPin = 2;

void setup(void) {
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
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
    //digitalWrite(ledPin, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(3000);
  adapter = new WebThingAdapter("fuelsensor", WiFi.localIP());
  device.addProperty(&property);
  adapter->addDevice(&device);
  Serial.println("Starting HTTP server");
  adapter->begin();
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println("fuelsensor");
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
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(3000);
  return (int) distance;
}

void loop(void) {
  digitalWrite(ledPin, HIGH); 
  int value = getValue();
  double percent = (double) 100. - (value/90.*100.);
  ThingPropertyValue levelValue;
  levelValue.number = percent;
  property.setValue(levelValue);
  delay(100);
  adapter->update();
  
  value = getValue();
  percent = (double) 100. - (value/90.*100.);
  levelValue.number = percent;
  property.setValue(levelValue);  
  delay(100);
  adapter->update();
  digitalWrite(ledPin, LOW); 
  //ESP.deepSleep(15e7);
  ESP.deepSleep(10e6);
}
