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
#include <RCSwitch.h>

const char* ssid = "GOAT";
const char* password = "goatseatgrass";
const int ledPin = LED_BUILTIN;
WebThingAdapter* adapter;
RCSwitch mySwitch = RCSwitch();

const char* codes[] = {
    "010000000101010100110011",
    "010000000101010100111100",
    "010000000101010111000011",
    "010000000101010111001100",
    "010000000101011100000011",
    "010000000101011100001100",
    "010000000101110100000011",
    "010000000101110100001100",
    "010000000111010100000011",
    "010000000111010100001100"
};
  
int lengths[] = {
  166,
  166,
  167,
  167,
  167,
  166,
  166,
  167,
  166,
  167
};

const char* ledTypes[] = {"OnOffSwitch", "Light", nullptr};

ThingDevice led9("led9", "Built-in LED", ledTypes);
ThingDevice led10("led10", "Built-in LED", ledTypes);
ThingDevice led11("led11", "Built-in LED", ledTypes);
ThingDevice led12("led12", "Built-in LED", ledTypes);
ThingDevice led13("led13", "Built-in LED", ledTypes);

ThingProperty led9On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led10On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led11On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led12On("on", "", BOOLEAN, "OnOffProperty");
ThingProperty led13On("on", "", BOOLEAN, "OnOffProperty");

bool lastOn1 = false;
bool lastOn2 = false;
bool lastOn3 = false;
bool lastOn4 = false;
bool lastOn5 = false;

void setup(void){
  
  mySwitch.enableTransmit(10);

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
    digitalWrite(LED_BUILTIN, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  digitalWrite(LED_BUILTIN, LOW); // active low led

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Change this below value for each new switch on your network.
  adapter = new WebThingAdapter("w27", WiFi.localIP());

  led9.addProperty(&led9On);
  led10.addProperty(&led10On);
  led11.addProperty(&led11On); 
  led12.addProperty(&led12On);
  led13.addProperty(&led13On);
  
  adapter->addDevice(&led9);
  adapter->addDevice(&led10);
  adapter->addDevice(&led11);
  adapter->addDevice(&led12);
  adapter->addDevice(&led13);
  adapter->begin();
  
  Serial.println("HTTP server started");

  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led9.id);
  
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");  
  Serial.println(led10.id);

  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led11.id);
  
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led12.id);
  
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led13.id);
}

void loop(void){
  adapter->update();
  bool on1 = led9On.getValue().boolean;
  bool on2 = led10On.getValue().boolean;
  bool on3 = led11On.getValue().boolean;
  bool on4 = led12On.getValue().boolean;
  bool on5 = led13On.getValue().boolean;
 
  //digitalWrite(LED_BUILTIN, HIGH);
  
  if (on1 != lastOn1){
    mySwitch.setPulseLength(lengths[9]);
    mySwitch.send(on1 ? codes[0] : codes[1]);
    Serial.print(led9.id);
    Serial.print(": ");
    Serial.println(on1);
  }

  if (on2 != lastOn2){ 
    mySwitch.setPulseLength(lengths[4]);
    mySwitch.send(on2 ? codes[2] : codes[3]);
    Serial.print(led10.id);
    Serial.print(": ");
    Serial.println(on2);
  }
  
  if ( on3 != lastOn3 ){
    mySwitch.send(on3 ? codes[4] : codes[5]);
    Serial.print(led11.id);  
    Serial.print(": ");
    Serial.println(on3);
  }
  if (on4 != lastOn4) {  
    mySwitch.send(on4 ? codes[6] : codes[7]);
    Serial.print(led12.id);
    Serial.print(": ");
    Serial.println(on4);
  }
  
  if (on5 != lastOn5) {
    mySwitch.send(on5 ? codes[8] : codes[9]);
    Serial.print(led13.id);
    Serial.print(": ");
    Serial.println(on5);
  }
   
  lastOn1 = on1;
  lastOn2 = on2;
  lastOn3 = on3;
  lastOn4 = on4;
  lastOn5 = on5;
  
  //digitalWrite(LED_BUILTIN, LOW);
}
