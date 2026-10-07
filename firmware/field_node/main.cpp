#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* WIFI_SSID="JOTA-JOTI-2026";
const char* WIFI_PASSWORD="ScoutLink2026";
const char* CONTROLLER="http://192.168.4.1";

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif
const int BUTTON_PIN=4;
unsigned long lastHeartbeat=0,lastChallenge=0;
bool previousButton=HIGH;

void connectWiFi(){
  WiFi.mode(WIFI_STA); WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting to ScoutLink");
  unsigned long start=millis();
  while(WiFi.status()!=WL_CONNECTED && millis()-start<15000){delay(500);Serial.print(".");}
  Serial.println();
  if(WiFi.status()==WL_CONNECTED){Serial.print("Field node IP: ");Serial.println(WiFi.localIP());}
}

void heartbeat(){
  if(WiFi.status()!=WL_CONNECTED)return;
  HTTPClient http; http.begin(String(CONTROLLER)+"/api/heartbeat"); http.GET(); http.end();
}

void sendEvent(const char* eventName){
  if(WiFi.status()!=WL_CONNECTED)return;
  HTTPClient http; http.begin(String(CONTROLLER)+"/api/event");
  http.addHeader("Content-Type","application/json");
  String body="{"node":"FIELD-01","event":""+String(eventName)+"","value":1}";
  int code=http.POST(body); Serial.printf("Event sent: %s (%d)\n",eventName,code); http.end();
}

void fetchChallenge(){
  if(WiFi.status()!=WL_CONNECTED)return;
  HTTPClient http; http.begin(String(CONTROLLER)+"/api/challenge");
  int code=http.GET();
  if(code==HTTP_CODE_OK)Serial.println("Challenge: "+http.getString());
  http.end();
}

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  digitalWrite(LED_BUILTIN,LOW);
  connectWiFi();
  fetchChallenge();
}

void loop(){
  if(WiFi.status()!=WL_CONNECTED){connectWiFi();delay(1000);}
  bool button=digitalRead(BUTTON_PIN);
  if(previousButton==HIGH && button==LOW){
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    sendEvent("SCOUT_BUTTON");
    delay(80);
  }
  previousButton=button;
  if(millis()-lastHeartbeat>5000){heartbeat();lastHeartbeat=millis();}
  if(millis()-lastChallenge>10000){fetchChallenge();lastChallenge=millis();}
  delay(20);
}
