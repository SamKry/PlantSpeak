#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>

class WifiHandler
{
public:
    int connect(const char *ssid, const char *password, int timeout);
    bool isConnected();
    int getRssi();
};

#endif // WIFIHANDLER_H
