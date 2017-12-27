#include "storage.h"
#include "util.h"
#include <EEPROM.h>

/** Load WLAN credentials from EEPROM */
void loadCredentials(Credentials *credentials)
{
    // struct Credentials credentials;
    EEPROM.begin(512);
    EEPROM.get(0, credentials->ssid);
    EEPROM.get(0 + sizeof(credentials->ssid), credentials->password);
    char ok[2 + 1];
    EEPROM.get(0 + sizeof(credentials->ssid) + sizeof(credentials->password), ok);
    EEPROM.end();
    if (String(ok) != String("OK"))
    {
        credentials->ssid[0] = 0;
        credentials->password[0] = 0;
    }
}

/** Store WLAN credentials to EEPROM */
void saveCredentials(Credentials *credentials)
{
    EEPROM.begin(512);
    EEPROM.put(0, credentials->ssid);
    EEPROM.put(0 + sizeof(credentials->ssid), credentials->password);
    char ok[2 + 1] = "OK";
    EEPROM.put(0 + sizeof(credentials->ssid) + sizeof(credentials->password), ok);
    EEPROM.commit();
    EEPROM.end();
}