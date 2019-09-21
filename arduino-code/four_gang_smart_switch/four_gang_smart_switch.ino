/**
 * Simple server compliant with Mozilla's proposed WoT API
 * Originally based on the HelloServer example
 * Tested on ESP8266, ESP32, Arduino boards with WINC1500 modules (shields or
 * MKR1000)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"
const char* ssid = "GOAT";
const char* password = "goatseatgrass";
const int ledPin = LED_BUILTIN;
WebThingAdapter* adapter;

const int pin1=16; 
const int pin2=5;
const int pin3=4; 
const int pin4=0;

const char* ledTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice led5("led5", "Built-in LED", ledTypes);
ThingDevice led6("led6", "Built-in LED", ledTypes);
ThingDevice led7("led7", "Built-in LED", ledTypes);
ThingDevice led8("led8", "Built-in LED", ledTypes);

ThingProperty led5On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led6On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led7On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led8On("on", "", BOOLEAN, "OnOffProperty");

bool lastOn1 = false;
bool lastOn2 = false;
bool lastOn3 = false;
bool lastOn4 = false;

void setup(void){
  
  pinMode(pin1, OUTPUT); 
  digitalWrite(pin1, LOW);
  pinMode(pin2, OUTPUT);
  digitalWrite(pin2, LOW);
  pinMode(pin3, OUTPUT);
  digitalWrite(pin3, LOW);
  pinMode(pin4, OUTPUT);
  digitalWrite(pin4, LOW);
  
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
    digitalWrite(ledPin, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  digitalWrite(ledPin, HIGH); // active low led

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Change this below value for each new switch on your network.
  adapter = new WebThingAdapter("w26", WiFi.localIP());

  led5.addProperty(&led5On);
  led6.addProperty(&led6On);
  led7.addProperty(&led7On);
  led8.addProperty(&led8On);
  
  adapter->addDevice(&led5);
  adapter->addDevice(&led6);
  adapter->addDevice(&led7);
  adapter->addDevice(&led8);
  adapter->begin();
  
  Serial.println("HTTP server started");

  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led5.id);
  
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");  
  Serial.println(led6.id);

  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led7.id);
  
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led8.id);
}

void loop(void){
  adapter->update();
  bool on1 = led5On.getValue().boolean;
  bool on2 = led6On.getValue().boolean;
  bool on3 = led7On.getValue().boolean;
  bool on4 = led8On.getValue().boolean;
  
  if (on1 != lastOn1 || on2 != lastOn2 || on3 != lastOn3 || on4 != lastOn4) {
    digitalWrite(pin1, on1 ? LOW : HIGH); // active low led
    Serial.print(led5.id);
    Serial.print(": ");
    Serial.println(on1);

    digitalWrite(pin2, on2 ? LOW : HIGH); // active low led
    Serial.print(led6.id);
    Serial.print(": ");
    Serial.println(on2);

    digitalWrite(pin3, on3 ? LOW : HIGH); // active low led
    Serial.print(led7.id);  
    Serial.print(": ");
    Serial.println(on3);

    digitalWrite(pin4, on4 ? LOW : HIGH); // active low led
    Serial.print(led8.id);
    Serial.print(": ");
    Serial.println(on4);
  } 
  lastOn1 = on1;
  lastOn2 = on2;
  lastOn3 = on3;
  lastOn4 = on4;
}
