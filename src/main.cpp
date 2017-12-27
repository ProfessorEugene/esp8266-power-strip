#include "util.h"
#include "storage.h"
#include "httpserver.h"
#include "wifi.h"
#include "dnsservers.h"
#include "udpserver.h"

/* DNS server port */
const int DNS_PORT = 53;
/* http server port */
const int HTTP_PORT = 80;
/* UDP server port */
const int UDP_PORT = 1024;
/* http server host name */
const char *HOST_NAME = "strip";
/* AP ip address */
IPAddress apIP(192, 168, 4, 1);
/* API netmask */
IPAddress apNetMask(255, 255, 255, 0);
/* AP credentials */
struct Credentials apCredentials;
const char *AP_PASSWORD = "12345678";
/* Station credentials */
Credentials stationCredentials;

void setup()
{
    strcpy(apCredentials.ssid, HOST_NAME);
    strcpy(apCredentials.password, AP_PASSWORD);
    Serial.begin(115200);
    setupStrip();
    setupDNSServer(DNS_PORT, &apIP);
    setupUDPServer(UDP_PORT);
    setupHttpServer(HTTP_PORT, HOST_NAME, &apIP, &apCredentials, &stationCredentials);
    setupWifi(&apIP, &apNetMask, &apCredentials, &stationCredentials, HOST_NAME, HTTP_PORT);
}

void loop()
{
    // Serial.println("<");
    long st = millis();
    processNextWifiRequest();
    // Serial.print("wifi: "); Serial.println(millis() - st);st = millis();
    processNextDNSRequest();
    // Serial.print("dns: "); Serial.println(millis() - st);st = millis();
    processNextUDPRequest();
    // Serial.print("udp: "); Serial.println(millis() - st);st = millis();
    processNextHttpRequest();
    // Serial.print("http: "); Serial.println(millis() - st);st = millis();
    // Serial.println(">");
}

void loop_test_creds()
{
    struct Credentials creds;
    Serial.println("start loop");
    loadCredentials(&creds);
    Serial.println("loaded credentials");
    Serial.println(creds.ssid);
    /* update ssid */
    strcpy(creds.ssid, "TEST");
    saveCredentials(&creds);
    delay(500);
    Serial.println("end loop");
}
