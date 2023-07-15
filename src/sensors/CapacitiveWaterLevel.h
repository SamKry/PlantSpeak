// functions to read the water level sensor

#ifndef CAPACITIVEWATERLEVEL_H
#define CAPACITIVEWATERLEVEL_H

#include <Arduino.h>

class CapacitiveWaterLevel
{
public:
    CapacitiveWaterLevel(int signalPin);
    void begin();
    int read();
    int read(int samples);
    void setDelayTime(int delayTime);
    int getDelayTime();
    void setAirValue(int AirValue);
    int getAirValue();
    void setWaterValue(int WaterValue);
    int getWaterValue();
    void calibrate();

private:
    int _signalPin;
    int _delayTime = 10;
    int AirValue;
    int WaterValue;
    int readRaw(int samples, int delayTime);
};

#endif // CAPACITIVEWATERLEVEL_H