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
#define SERVER_IP "27.101.215.194"

SoftwareSerial ESP8266_serial(WIFI_RXD, WIFI_TXD);
String assignedIP = "";
String income_wifi = "";

// char* getWeatherData[6] = {"GET /",
//                       "1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey",
//                       "=vwa%2FDDlSjq1JMK4NX8ZJ983sV0p3pCZUSE%2FD3q6PIv193RHWhA35iY8",
//                       "mKkmifefajFco03SsO2lCru6g0LPPYw%3D%3D&pageNo=1&numOfRows=10&d",
//                       "ataType=JSON&base_date=20230127&base_time=1200&nx=58&ny=126\r\n"};

String getWeatherData1 = "GET /1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey=vwa%2FDDlSjq1JMK4NX8ZJ983sV0p3pCZUSE%2FD3q6PIv193RHWhA35iY8mKkmifefajFco03SsO2lCru6g0LPPYw%3D%3D&pageNo=1&numOfRows=10&dataType=JSON&base_date=20230127&base_time=1100&nx=58&ny=126\r\n";

void wifiSetup(){
	sendAT("AT+RST\r\n", 2000);
	sendAT("AT+CWMODE=3\r\n", 1000);
	sendAT("AT+CWSAP=\"KCCI_STC_S\",\"kcci098#\",11,0\r\n", 1000);
	sendAT("AT+CWSAP?\r\n", 1000);
	String income_CIFSR = "";
	do {
    income_CIFSR = sendAT("AT+CIFSR\r\n", 5000);
	}while (income_CIFSR.indexOf("0.0.0.0") != -1);

  char cifsr[50];
  assignedIP = getIpAdress(cifsr, income_CIFSR);

  sendAT("AT+CIPSTART=\"TCP\",\"27.101.215.194\",80\r\n",1000);
  sendAT("AT+CIPMODE=1\r\n",1000);
  sendAT("AT+CIPMUX=0\r\n",1000);
  sendAT("AT+CIPSEND\r\n",1000);
  sendData(getWeatherData1);
  
	Serial.println("available");
}

String getIpAdress(char str[], String income){
  unsigned int length = income.length();
  income.toCharArray(str, length);
  int i = 0;
  char *sArr[5];
  char * ptr = NULL;

  ptr = strtok(str, "\r\n");
  while(ptr != NULL){
    sArr[i++] = ptr;
    ptr = strtok(NULL, "\r\n");
  }
  Serial.println(sArr[2]);
  return sArr[2];
}

void getData(){
  if (ESP8266_serial.available()) {
    if (ESP8266_serial.find("+IPD")) {
      income_wifi = ESP8266_serial.readStringUntil('\r');
    }
    Serial.println(income_wifi);
    //return income_wifi;
  } 
}

String sendAT(String atCommand, const int timeout){
	String response = "";
	ESP8266_serial.print(atCommand);
	long int time = millis();
  
	while ((time + timeout) > millis()) {
		while (ESP8266_serial.available()) {
			char c = ESP8266_serial.read();
			response += c;
		}
	}
	Serial.print(response);
	return response;
}

void sendData(String command){
	String response = "";
	ESP8266_serial.print(command);
  char c;
	while(1){
    while (ESP8266_serial.available()) {
			c = ESP8266_serial.read();
			response += c;
		}
    if(c == '\r'){
      break;
    }
  }
  Serial.println(response);
	// Serial.print(response);
	// return response;
}

