#ifndef HTTPSERVER_H_INCLUDED
#define HTTPSERVER_H_INCLUDED

#include <ESP8266WebServer.h>
#include "storage.h"
#include "wifi.h"
#include "strip.h"
#include "markup.h"

void setupHttpServer(int port, char const *hostName, IPAddress *apIP, Credentials *apCredentials, Credentials *stationCredentials);

void processNextHttpRequest();

#endif