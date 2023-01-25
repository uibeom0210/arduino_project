#include <SoftwareSerial.h>
#include "WifiHandler.h"

#define LED 13

#ifndef WIFIRXD
#define WIFI_RXD 6
#define WIFI_TXD 7
#endif // WIFIRXD

extern SoftwareSerial ESP8266_serial;
String income_wifi = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ESP8266_serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(500);
  void wifiSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (ESP8266_serial.available()) {
    if (ESP8266_serial.find("+IPD")) {
      income_wifi = ESP8266_serial.readStringUntil('\r');
      String wifi_temp = income_wifi.substring(income_wifi.indexOf("GET /") + 5, income_wifi.indexOf("HTTP/1.1") - 1);
      if (wifi_temp == "LED_ON") {
        digitalWrite(LED, HIGH);
        delay(500);
      } else if (wifi_temp == "LED_OFF") {
        digitalWrite(LED, LOW);
        delay(500);
      } else {
        //
      }
      Serial.println(income_wifi);
    }
  } 
  else {
    delay(1000);
  }
}
