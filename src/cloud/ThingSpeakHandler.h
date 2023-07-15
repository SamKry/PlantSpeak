// functions to send data to Thingspeak cloud

#ifndef THINGSPEAKHANDLER_H
#define THINGSPEAKHANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ThingSpeak.h>

class ThingSpeakHandler
{
public:
    ThingSpeakHandler(long chanelNumber, const char *APIKey);
    int setField(int fieldNumber, String value);
    int setStatus(String status);
    int post();

private:
    WiFiClient _client;
    long _chanelNumber;
    const char *_APIKey;
};

#endif // THINGSPEAKHANDLER_H
