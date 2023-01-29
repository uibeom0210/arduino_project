/**
  ******************************************************************************
  * @file           : WifiHandler.h
  * @brief          : Header for WifiHandler.cpp file created for using the Wi-Fi.
  *                   This file contains the common defines of the application.
					This file is available in Arduino.
  ******************************************************************************
  */
#ifndef WifiHandler_h
#define WifiHandler_h

#include <SoftwareSerial.h>
#include <Stream.h>
#include "wiring_private.h"

/******************************************************************************
* Definitions
******************************************************************************/
#define WIFI_RXD 6
#define WIFI_TXD 7

#define SERVER_IP "27.101.215.194"
#define DEBUG_AT 1
#define DEBUG_TEMP 1

void wifiSetup();
String makeBaseData(String timeString);
String convertMonth(String rawMonth);
String makeBaseTime(String rawTime);
String getIpAdress(String income);
void getWeatherData();
String parseWeatherData();
void getServerTime();
String sendAT(String atCommand, const int timeout);

#endif // WifiHandler_h