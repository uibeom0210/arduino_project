#include <SoftwareSerial.h>
#include "WifiHandler.h"

#define LED 13
#define P_RXD 8
#define P_TXD 9

#define GET_WEATHER_DATA 0
#define WAIT 1
#define SERIAL_WITH_ARDUINO 2

extern SoftwareSerial ESP8266_serial;
SoftwareSerial P_serial(P_RXD, P_TXD);
byte status = 0;
String tempString = "";
String sendMessage = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  P_serial.begin(9600);
  ESP8266_serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  wifiSetup();
}

void loop() {
  // put your main code here, to run repeatedly:

  switch(status){
  case GET_WEATHER_DATA:
    Serial.println("Get weather data.");
    getWeatherData();
    tempString = parseWeatherData();
    status++;
    break;
  case WAIT:
    

  case SERIAL_WITH_ARDUINO:
    makeSendmessage();
    status = sendData();
  default:
    break;
  }
}

void makeSendmessage(){
  int temperature = tempString.toInt();
  String sign = "";
  if (temperature < 0)
    sign = "1";
  else
    sign = "0";

  sendMessage = sign + "#" + tempString;
}

byte sendData(String command) {
  P_serial.print(command);
  return WAIT;
}


