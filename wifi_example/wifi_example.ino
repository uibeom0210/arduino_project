#include <SoftwareSerial.h>
#include "WifiHandler.h"

#define LED 13

extern SoftwareSerial ESP8266_serial;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ESP8266_serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(500);
  wifiSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  //getData();
}

