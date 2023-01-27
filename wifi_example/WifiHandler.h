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


void wifiSetup();

String getIpAdress(char str[], String income);
void getData();
void sendData(String command);
String sendAT(String atCommand, const int timeout);

#endif // WifiHandler_h