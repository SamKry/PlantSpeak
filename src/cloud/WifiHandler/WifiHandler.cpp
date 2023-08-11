#include "cloud/WifiHandler/WifiHandler.h"

/**
 * @brief Connects to WiFi with the given ssid and password and waits for the connection to be established.
 *
 * @param ssid  The ssid of the WiFi network to connect to.
 * @param password  The password of the WiFi network to connect to.
 * @param timeout   The timeout in seconds to wait for the connection to be established.
 * 
 * @return int  The time it took to connect to WiFi in milliseconds.
 */
int WifiHandler::connect(const char *ssid, const char *password)
{
    long startTime = millis();
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(_hostname.c_str());
    
    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.print(ssid);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED && millis() - startTime < _connectionTimeout * 1000)
        {
            Serial.print(".");
            delay(500);
        }
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println(F("Failed to connect to WiFi"));
            return millis() - startTime;
        }

        Serial.println("\nConnected with IP: " + WiFi.localIP().toString());
    }
    return millis() - startTime;
}

/**
 * @brief Checks if the device is connected to WiFi.
 *
 * @return true if the device is connected to WiFi.
 * @return false  if the device is not connected to WiFi.
 */
bool WifiHandler::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

/**
 * @brief Gets the RSSI of the WiFi connection.
 *
 * @return int  The RSSI of the WiFi connection.
 */
int WifiHandler::getRssi()
{
    return WiFi.RSSI();
}