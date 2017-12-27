#include "wifi.h"

Credentials *wifiStationCredentials;

boolean wifiConnectRequested = false;
/** Last time I tried to connect to WLAN */
long wifiLastConnectTry = 0;
/** Current WLAN status */
int wifiStatus = WL_IDLE_STATUS;

const char *httpHostname;
int httpPort;

void connectWifi()
{
    Serial.println("Connecting as wifi client...");
    WiFi.disconnect();
    WiFi.begin(wifiStationCredentials->ssid, wifiStationCredentials->password);
    int connRes = WiFi.waitForConnectResult();
    Serial.print("connRes: ");
    Serial.println(connRes);
}

/* public methods */

void setupWifi(IPAddress *apIP, IPAddress *apNetMask, Credentials *apCredentials, Credentials *stationCredentials, const char *hostName, int port)
{
    wifiStationCredentials = stationCredentials;
    httpHostname = hostName;
    httpPort = port;
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAPConfig(*apIP, *apIP, *apNetMask);
    WiFi.softAP(apCredentials->ssid, apCredentials->password);
    delay(500); // Without delay I've seen the IP address blank
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void processNextWifiRequest()
{
    if (wifiConnectRequested)
    {
        Serial.println("Connect requested");
        wifiConnectRequested = false;
        connectWifi();
        wifiLastConnectTry = millis();
    }
    int s = WiFi.status();
    {
        if (s == 0 && millis() > (wifiLastConnectTry + 60000))
        {
            /* If WLAN disconnected and idle try to connect */
            /* Don't set retry time too low as retry interfere the softAP operation */
            wifiConnectRequested = true;
        }
        if (wifiStatus != s)
        { // WLAN status change
            Serial.print("Status: ");
            Serial.println(s);
            wifiStatus = s;
            if (s == WL_CONNECTED)
            {
                /* Just connected to WLAN */
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(wifiStationCredentials->ssid);
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());

                registerMDNS(httpHostname, httpPort);
                // // Setup MDNS responder
                // if (!MDNS.begin(myHostname))
                // {
                //     Serial.println("Error setting up MDNS responder!");
                // }
                // else
                // {
                //     Serial.println("mDNS responder started");
                //     // Add service to MDNS-SD
                //     MDNS.addService("http", "tcp", 80);
                // }

            }
            else if (s == WL_NO_SSID_AVAIL)
            {
                WiFi.disconnect();
            }
        }
    }
}

void requestWifiConnection()
{
    wifiConnectRequested = true;
}