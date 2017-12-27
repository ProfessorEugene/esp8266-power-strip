/* prevent double includes */
#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <Arduino.h>
#include <ESP8266WiFi.h>


/**
 * Return true if supplied string is an IPv4 address
 */
boolean isIp(String str);

/**
 * Convert IPAddress into a String
 */
String toStringIp(IPAddress ip);

#endif