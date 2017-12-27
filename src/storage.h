#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

/**
 * A credentials struct
 */
struct Credentials
{
    char ssid[32];
    char password[32];
};

/**
 *  Load credentials from EEPROM
 */
void loadCredentials(Credentials *credentials);

/**
 * Store credentials in EEPROM
 */
void saveCredentials(Credentials *credentials);

#endif