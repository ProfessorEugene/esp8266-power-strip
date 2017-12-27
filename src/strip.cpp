#include "strip.h"

int pins[4] = {14, 12, 13, 5};
int pinStatus = 0;

void setupStrip()
{
    for (int pinIdx = 0; pinIdx < sizeof(pins) / sizeof(int); pinIdx++)
    {
        pinMode(pins[pinIdx], OUTPUT);
    }
}

int getStripStatus()
{
    return pinStatus;
}

void setStripStatus(int status)
{
    Serial.print("Setting pins to ");
    Serial.print(status);
    Serial.println();
    pinStatus = status;
    for (int pinIdx = 0; pinIdx < sizeof(pins) / sizeof(int); pinIdx++)
    {
        boolean status =
            ((pinStatus >> (sizeof(int) - (pinIdx + 1))) & 0x1) == 0x1;
        Serial.print("pin ");
        Serial.print(pinIdx);
        Serial.println(status);
        if (status)
        {
            Serial.print("putting pin ");
            Serial.print(pinIdx);
            Serial.print("(");
            Serial.print(pins[pinIdx]);
            Serial.print(")");
            Serial.println(" LOW");
            digitalWrite(pins[pinIdx], LOW);
        }
        else
        {
            Serial.print("putting pin ");
            Serial.print(pinIdx);
            Serial.print("(");
            Serial.print(pins[pinIdx]);
            Serial.print(")");
            Serial.println(" HIGH");
            digitalWrite(pins[pinIdx], HIGH);
        }
    }
}