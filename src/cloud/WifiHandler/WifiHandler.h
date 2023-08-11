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
    void setHostname(String hostname) { _hostname = hostname; }
    String getHostname() { return _hostname; }

private:
    int _connectionTimeout = 10;
    String _hostname = "ESP32-PlantSpeak";
};

#endif // WIFIHANDLER_H
