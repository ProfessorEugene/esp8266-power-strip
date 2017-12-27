#include "udpserver.h"
#include "strip.h"

WiFiUDP UDPServer;

void setupUDPServer(int udpPort)
{
    UDPServer.begin(udpPort);
}

void processNextUDPRequest()
{
    int packetSize = UDPServer.parsePacket();
    if (packetSize)
    {
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, UDPServer.remoteIP().toString().c_str(), UDPServer.remotePort());
        uint8_t incomingPacket[1];
        int len = UDPServer.readBytes(incomingPacket, 1);
        if (len == 1)
        {
            setStripStatus(incomingPacket[0]);
            uint8_t replyPacket[] = { (uint8_t)getStripStatus() };
            UDPServer.beginPacket(UDPServer.remoteIP(), UDPServer.remotePort());
            UDPServer.write(replyPacket, sizeof(uint8_t));
            UDPServer.endPacket();
        }
    }
}