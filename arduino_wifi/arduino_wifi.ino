#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include "WifiHandler.h"

#define LED 13
#define P_RXD 8
#define P_TXD 9

#define GET_SERVER_TIME 0
#define GET_WEATHER_DATA 1
#define WAIT 2
#define SERIAL_WITH_ARDUINO 3

extern SoftwareSerial ESP8266_serial;
SoftwareSerial P_serial(P_RXD, P_TXD);
volatile byte status = GET_SERVER_TIME;
String tempString = "";
String sendMessage = "";

void setup() {
  // put your setup code here, to run once:
  P_serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
  ESP8266_serial.begin(9600);
  MsTimer2::set(1000, getDataCycle);
  delay(500);
  wifiSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(status){
  case GET_SERVER_TIME:
    getServerTime();
    status++;
    break;
  case GET_WEATHER_DATA:
    Serial.println("Get weather data.");
    getWeatherData();
    tempString = parseWeatherData();
    status++;
    break;
  case WAIT:
    String message = "";
    if(P_serial.available()){
      message = P_serial.readStringUntil('\n');
      if(message.equals("@")){
        status = SERIAL_WITH_ARDUINO;
      }
    }
  case SERIAL_WITH_ARDUINO:
    makeSendmessage();
    status = sendData();
    break;
  default:
    break;
  }
}

void makeSendmessage(){
  int temperature = tempString.toInt();
  String sign = "";
  if (temperature < 0)
    temperature = (-1)*temperature;
    sign = "1";
  else
    sign = "0";

  tempString = String(temperature);
  sendMessage = sign + "#" + tempString;
}

byte sendData() {
  P_serial.println(sendMessage);
  return WAIT;
}

void getDataCycle(){
  static byte count = 0;
  count++;

  if(count == 60){
    status = GET_SERVER_TIME;
    count = 0;
  }
}



