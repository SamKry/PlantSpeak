#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>

class WifiHandler
{
public:
    int connect(const char *ssid, const char *password);
    bool isConnected();
    int getRssi();
    void setConnectionTimeout(int connectionTimeout) { _connectionTimeout = connectionTimeout; }
    int getConnectionTimeout() { return _connectionTimeout; }

private:
    int _connectionTimeout = 10;
};

#endif // WIFIHANDLER_H
