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

void wifiSetup(){
	sendData("AT+RST\r\n", 2000);
	sendData("AT+CWMODE=3\r\n", 1000);
	sendData("AT+CWSAP=\"KCCI_STC_S\",\"kcci098#\",11,0\r\n", 1000);
	sendData("AT+CWSAP?\r\n", 1000);

	String income_CIFSR = "";
	do {
    income_CIFSR = sendData("AT+CIFSR\r\n", 5000);
	}while (income_CIFSR.indexOf("0.0.0.0") != -1);
	sendData("AT+CIPMUX=1\r\n", 1000);
	sendData("AT+CIPSERVER=1,80\r\n", 1000);
	Serial.println("available");
}

String sendData(String command, const int timeout){
	String response = "";
	ESP8266_serial.print(command);
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