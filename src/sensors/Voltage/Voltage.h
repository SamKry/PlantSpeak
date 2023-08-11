
#ifndef VOLTAGE_H
#define VOLTAGE_H

#include <Arduino.h>

class Voltage
{
public:
    Voltage(int signalPin);
    void begin();
    float read(int samples);
    float getVoltage() { return _voltage; }

private:
    int _signalPin;
    float _voltage;
    double referenceVoltage = 3.3;
    int resolution = 4095;
    double voltageDivider = 2.5;
    int delayTime = 10;
    int readRAW();
};

#endif // VOLTAGE_H