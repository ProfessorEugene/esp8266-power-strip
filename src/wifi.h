/* prevent double includes */
#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include "storage.h"
#include "dnsservers.h"

void setupWifi(IPAddress *apIP, IPAddress *apNetMask, Credentials *apCredentials, Credentials *stationCredentials,const char *hostName, int port);

void processNextWifiRequest();

void requestWifiConnection();

#endif