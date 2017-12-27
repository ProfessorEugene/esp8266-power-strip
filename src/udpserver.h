#ifndef UDPSERVER_H_INCLUDED
#define UDPSERVER_H_INCLUDED

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void setupUDPServer(int udpPort);

void processNextUDPRequest();

#endif