#include <Arduino.h>

#include <sensors/Voltage.h>

/**
 * @brief Construct a new Voltage:: Voltage object
 *
 * @param signalPin the pin to read the voltage from
 */
Voltage::Voltage(int signalPin)
{
    _signalPin = signalPin;
}

/**
 * @brief Set up the voltage sensor
 *
 */
void Voltage::begin()
{
    pinMode(_signalPin, INPUT);
}

/**
 * @brief Read the voltage from the sensor
 *
 * @param samples the number of samples to take
 *
 * @return float the voltage
 */
float Voltage::read(int samples)
{
    // wait for the signal to be high (warmup)
    int startCounts = 0;
    while (analogRead(_signalPin) == 0 && startCounts < 10)
    {
        startCounts++;
        delay(delayTime);
    }

    // read the signal
    int sum = 0;
    for (int i = 0; i < samples; i++)
    {
        sum += readRAW();
        delay(delayTime);
    }
    int voltageRAW = sum / samples;
    return (voltageRAW * referenceVoltage / resolution) * voltageDivider;
}

// private
int Voltage::readRAW()
{
    return analogRead(_signalPin);
}