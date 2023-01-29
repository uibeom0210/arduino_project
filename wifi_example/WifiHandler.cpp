/**
  ******************************************************************************
  * @file           : WifiHandler.cpp
  * @brief          : Using Wi-Fi application.
					This file was created based on the <SoftwareSerial>
  *                 This file contains set up function for ESP8266 and send data
					function.
  ******************************************************************************
  */

#include "WifiHandler.h"

SoftwareSerial ESP8266_serial(WIFI_RXD, WIFI_TXD);

String getWeatherDataServiceKey = "GET /1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey=vwa%2FDDlSjq1JMK4NX8ZJ983sV0p3pCZUSE%2FD3q6PIv193RHWhA35iY8mKkmifefajFco03SsO2lCru6g0LPPYw%3D%3D&pageNo=1&numOfRows=10&dataType=JSON&";
String getWeatherDataBaseData = "";
String getWeatherDataLastData = "&nx=58&ny=126\r\n";
String rawData = "";
String parsedData1 = "";
String parsedData2 = "";

void wifiSetup() {
  String assignedIP = "";
  String timeHeader = "";
  sendAT("AT+RST\r\n", 2000);
  sendAT("AT+CWMODE=3\r\n", 1000);
  sendAT("AT+CWSAP=\"KCCI_STC_S\",\"kcci098#\",11,0\r\n", 1000);
  sendAT("AT+CWSAP?\r\n", 1000);

  String income_CIFSR = "";
  do {
    income_CIFSR = sendAT("AT+CIFSR\r\n", 5000);
  } while (income_CIFSR.indexOf("0.0.0.0") != -1);
  assignedIP = getIpAdress(income_CIFSR);

  sendAT("AT+CIPSTART=\"TCP\",\"27.101.215.194\",80\r\n", 1000);
  timeHeader = getServerTime();
  Serial.println(timeHeader);
  getWeatherDataBaseData = makeBaseData(timeHeader);
  Serial.println(getWeatherDataBaseData);
  sendAT("AT+CIPMODE=1\r\n", 1000);
  sendAT("AT+CIPMUX=0\r\n", 1000);

  delay(2000);
}

String makeBaseData(String timeString) {
  char str[40];
  unsigned int length = timeString.length();
  timeString.toCharArray(str, length);
  int i = 0;
  char *sArr[7];
  char *ptr = NULL;

  ptr = strtok(str, " ");
  while (ptr != NULL) {
    sArr[i++] = ptr;
    ptr = strtok(NULL, " ");
  }
  String day = sArr[2];
  String rawMonth = sArr[3];
  String month = convertMonth(rawMonth);
  String year = sArr[4];
  String rawTime = sArr[5];
  String time = makeBaseTime(rawTime);
  String baseData = "base_date=" + year + month + day + "&base_time=" + time;

  return baseData;
}

String convertMonth(String rawMonth) {
  if (rawMonth == "Jan") return "01";
  else if (rawMonth == "Feb") return "02";
  else if (rawMonth == "Mar") return "03";
  else if (rawMonth == "Apr") return "04";
  else if (rawMonth == "May") return "05";
  else if (rawMonth == "Jun") return "06";
  else if (rawMonth == "Jul") return "07";
  else if (rawMonth == "Aug") return "08";
  else if (rawMonth == "Sep") return "09";
  else if (rawMonth == "Oct") return "10";
  else if (rawMonth == "Nov") return "11";
  else if (rawMonth == "Dec") return "12";
}

String makeBaseTime(String rawTime) {
  char str[10];
  unsigned int length = rawTime.length();
  rawTime.toCharArray(str, length);
  int i = 0;
  char *sArr[3];
  char *ptr = NULL;

  ptr = strtok(str, ":");
  while (ptr != NULL) {
    sArr[i++] = ptr;
    ptr = strtok(NULL, ":");
  }
  int korMinute = atoi(sArr[1]);
  int korHour = atoi(sArr[0]) + 9;
  String realHour = "";
  if (korHour > 24) {
    int overflow = korHour - 24;
    korHour = overflow;
  }

  if(korHour <= 2) korHour = 23;
  else if(korHour <= 5) korHour = 2;
  else if(korHour <= 8) korHour = 5;
  else if(korHour <= 11) korHour = 8;
  else if(korHour <= 14) korHour = 11;
  else if(korHour <= 17) korHour = 14;
  else if(korHour <= 20) korHour = 17;
  else if(korHour <= 23) korHour = 20;

  if(korHour < 10)
    realHour = "0" + String(korHour);
  else
    realHour = String(korHour);

  String time = realHour + "00";
  return time;
}

String getIpAdress(String income) {
  char str[50];
  unsigned int length = income.length();
  income.toCharArray(str, length);
  int i = 0;
  char *sArr[5];
  char *ptr = NULL;

  ptr = strtok(str, "\r\n");
  while (ptr != NULL) {
    sArr[i++] = ptr;
    ptr = strtok(NULL, "\r\n");
    //Serial.println(sArr[i-1]);
  }
  return sArr[2];
}

void getWeatherData() {
  
  sendAT("AT+CIPSTART=\"TCP\",\"27.101.215.194\",80\r\n", 1000);
  sendAT("AT+CIPSEND\r\n", 1000);
  ESP8266_serial.print(getWeatherDataServiceKey);
  ESP8266_serial.print(getWeatherDataBaseData);
  ESP8266_serial.print(getWeatherDataLastData);
  long int time = millis();
  while ((time + 5000) > millis()) {
    while (ESP8266_serial.available()) {
      char c = ESP8266_serial.read();
      rawData += c;
    }
  }
}

String parseWeatherData(){
  sendAT("+++", 1000);
  Serial.println("My data");
  Serial.println(rawData);
  parsedData1 = rawData.substring(rawData.indexOf("TMP")-1, rawData.indexOf("UUU")+3);
  Serial.println("My parsed data1");
  Serial.println(parsedData1);
  parsedData2 = parsedData1.substring(parsedData1.indexOf("fcstValue") + 12, parsedData1.indexOf("nx") - 3);
  Serial.println("My parsed data2");
  Serial.println(parsedData2);
  return parsedData2;
}

String getServerTime() {
  String income_HEAD = "";
  sendAT("AT+CIPMODE=0\r\n", 1000);
  sendAT("AT+CIPSEND=6\r\n", 1000);
  ESP8266_serial.print("HEAD\r\n");

  while (1) {
    if (ESP8266_serial.available()) {
      if (ESP8266_serial.find("Date")) {
        income_HEAD = ESP8266_serial.readStringUntil('\r');
      }
      //Serial.println(income_HEAD);
    } else
      break;
  }
  return income_HEAD;
}

String sendAT(String atCommand, const int timeout) {
  String response = "";
  ESP8266_serial.print(atCommand);
  long int time = millis();

  while ((time + timeout) > millis()) {
    while (ESP8266_serial.available()) {
      char c = ESP8266_serial.read();
      response += c;
    }
  }
#if DEBUG_AT
  Serial.print(response);
#endif
  return response;
}