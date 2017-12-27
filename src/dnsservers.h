#ifndef DNS_H_INCLUDED
#define DNS_H_INCLUDED

#include <ESP8266mDNS.h>
#include <DNSServer.h>

void setupDNSServer(int dnsPort, IPAddress *apIP);

void processNextDNSRequest();

void registerMDNS(const char *hostName, int port);

#endif