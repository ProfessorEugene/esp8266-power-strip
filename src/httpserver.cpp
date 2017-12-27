#include "httpserver.h"
#include "util.h"
#include "storage.h"

/* declare http server */
ESP8266WebServer httpServer;
const char *httpServerHostName;
IPAddress httpServerApIP;
Credentials *httpServerApCredentials;
Credentials *httpServerStationCredentials;


/* internal methods */

/** Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
boolean captivePortal()
{
    if (!isIp(httpServer.hostHeader()) && httpServer.hostHeader() != (String(httpServerHostName) + ".local"))
    {
        httpServer.sendHeader("Location", String("http://") + toStringIp(httpServer.client().localIP()), true);
        httpServer.send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
        httpServer.client().stop();             // Stop is needed because we sent no content length
        return true;
    }
    return false;
}

/** Handle root or redirect to captive portal */
void handleRoot()
{
    if (captivePortal())
    { // If caprive portal redirect instead of displaying the page.
        return;
    }
    httpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    httpServer.sendHeader("Pragma", "no-cache");
    httpServer.sendHeader("Expires", "-1");
    httpServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    httpServer.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    
    httpServer.sendContent(CP_HTML);
    // httpServer.sendContent(
    //     "<html><head></head><body>"
    //     "<h1>HELLO WORLD!!</h1>");
    // if (httpServer.client().localIP() == httpServerApIP)
    // {
    //     httpServer.sendContent(String("<p>You are connected through the soft AP: ") + httpServerApCredentials->ssid + "</p>");
    // }
    // else
    // {
    //     httpServer.sendContent(String("<p>You are connected through the wifi network: ") + httpServerStationCredentials->ssid + "</p>");
    // }
    // httpServer.sendContent(
    //     "<p>You may want to <a href='/wifi'>config the wifi connection</a>.</p>"
    //     "</body></html>");
    httpServer.client().stop(); // Stop is needed because we sent no content length
}

/** Wifi config page handler */
void handleWifi()
{
    httpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    httpServer.sendHeader("Pragma", "no-cache");
    httpServer.sendHeader("Expires", "-1");
    httpServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    httpServer.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    httpServer.sendContent(
        "<html><head></head><body>"
        "<h1>Wifi config</h1>");
    if (httpServer.client().localIP() == httpServerApIP)
    {
        httpServer.sendContent(String("<p>You are connected through the soft AP: ") + httpServerApCredentials->ssid + "</p>");
    }
    else
    {
        httpServer.sendContent(String("<p>You are connected through the wifi network: ") + httpServerStationCredentials->ssid + "</p>");
    }
    httpServer.sendContent(
        "\r\n<br />"
        "<table><tr><th align='left'>SoftAP config</th></tr>");
    httpServer.sendContent(String() + "<tr><td>SSID " + String(httpServerApCredentials->ssid) + "</td></tr>");
    httpServer.sendContent(String() + "<tr><td>IP " + toStringIp(httpServerApIP) + "</td></tr>");
    httpServer.sendContent(
        "</table>"
        "\r\n<br />"
        "<table><tr><th align='left'>WLAN config</th></tr>");
    httpServer.sendContent(String() + "<tr><td>SSID " + String(httpServerStationCredentials->ssid) + "</td></tr>");
    httpServer.sendContent(String() + "<tr><td>IP " + toStringIp(WiFi.localIP()) + "</td></tr>");
    httpServer.sendContent(
        "</table>"
        "\r\n<br />"
        "<table><tr><th align='left'>WLAN list (refresh if any missing)</th></tr>");
    Serial.println("scan start");
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n > 0)
    {
        for (int i = 0; i < n; i++)
        {
            httpServer.sendContent(String() + "\r\n<tr><td>SSID " + WiFi.SSID(i) + String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : " *") + " (" + WiFi.RSSI(i) + ")</td></tr>");
        }
    }
    else
    {
        httpServer.sendContent(String() + "<tr><td>No WLAN found</td></tr>");
    }
    httpServer.sendContent(
        "</table>"
        "\r\n<br /><form method='POST' action='wifisave'><h4>Connect to network:</h4>"
        "<input type='text' placeholder='network' name='n'/>"
        "<br /><input type='password' placeholder='password' name='p'/>"
        "<br /><input type='submit' value='Connect/Disconnect'/></form>"
        "<p>You may want to <a href='/'>return to the home page</a>.</p>"
        "</body></html>");
    httpServer.client().stop(); // Stop is needed because we sent no content length
}

/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave()
{
    Serial.println("wifi save");
    httpServer.arg("n").toCharArray(httpServerStationCredentials->ssid, sizeof(httpServerStationCredentials->ssid) - 1);
    httpServer.arg("p").toCharArray(httpServerStationCredentials->password, sizeof(httpServerStationCredentials->password) - 1);
    httpServer.sendHeader("Location", "wifi", true);
    httpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    httpServer.sendHeader("Pragma", "no-cache");
    httpServer.sendHeader("Expires", "-1");
    httpServer.send(302, "text/plain", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    httpServer.client().stop();             // Stop is needed because we sent no content length
    saveCredentials(httpServerStationCredentials);
    /* TODO: verify this works */
    if (strlen(httpServerStationCredentials->ssid) > 0)
    {
        requestWifiConnection();
    }
}

void handleNotFound()
{
    if (captivePortal())
    { // If caprive portal redirect instead of displaying the error page.
        return;
    }
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += httpServer.uri();
    message += "\nMethod: ";
    message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += httpServer.args();
    message += "\n";

    for (uint8_t i = 0; i < httpServer.args(); i++)
    {
        message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
    }
    httpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    httpServer.sendHeader("Pragma", "no-cache");
    httpServer.sendHeader("Expires", "-1");
    httpServer.send(404, "text/plain", message);
}

void handleGetStripStatus()
{   
    String message = "{\"status\":";
    message += getStripStatus();
    message += "}";
    if(httpServer.hasArg("c")){
        message = httpServer.arg("c") + "(" + message + ");";
    }
    httpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    httpServer.sendHeader("Pragma", "no-cache");
    httpServer.sendHeader("Expires", "-1");
    // httpServer.sendHeader("Access-Control-Allow-Origin", "*");
    httpServer.send(200, "application/javascript", message);
    httpServer.client().stop();
}

void handleSetStripStatus()
{
    int newStatus = httpServer.arg("s").toInt();
    setStripStatus(newStatus);
    handleGetStripStatus();
}

// void handleControlPanel(){
//     httpServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
//     httpServer.sendHeader("Pragma", "no-cache");
//     httpServer.sendHeader("Expires", "-1");
//     httpServer.send(404, "text/html", CP_HTML);
// }

/* externally visible methods */

void setupHttpServer(int port, char const *hostName, IPAddress *apIP, Credentials *apCredentials, Credentials *stationCredentials)
{
    httpServer = ESP8266WebServer(port);
    httpServerHostName = hostName;
    httpServerApIP = *apIP;
    httpServerApCredentials = apCredentials;
    httpServerStationCredentials = stationCredentials;

    /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
    httpServer.on("/", handleRoot);
    httpServer.on("/wifi", handleWifi);
    httpServer.on("/wifisave", handleWifiSave);
    /* app specific stuff */
    httpServer.on("/set", handleSetStripStatus);
    httpServer.on("/get", handleGetStripStatus);
    /* android captive portal */
    httpServer.on("/generate_204", handleRoot);
    /* MS captive portal */
    httpServer.on("/fwlink", handleRoot);
    /* general not found handler */
    httpServer.onNotFound(handleNotFound);
    /* start her up */
    httpServer.begin();
}

void processNextHttpRequest()
{
    httpServer.handleClient();
}
