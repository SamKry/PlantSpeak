
#ifndef DHTSENSOR_H
#define DHTESENSOR_H

#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_Sensor.h>

class DhtSensor
{
public:
    DhtSensor(int signalPin);
    void begin();
    void read(int samples);
    float getTemperature();
    float getHumidity();

private:
    int _signalPin;
    int delayTime = 10;
    float _temperature;
    float _humidity;
    int readRAW();
    DHT _dht;
};

#endif // DHTSENSOR_H