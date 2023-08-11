#include <Arduino.h>
#include <sensors/CapacitiveWaterLevel/CapacitiveWaterLevel.h>

/**
 * @brief Construct a new Capacitive Water Level:: Capacitive Water Level object
 *
 * @param signalPin the pin to read the signal from
 */
CapacitiveWaterLevel::CapacitiveWaterLevel(int signalPin)
{
    _signalPin = signalPin;
    AirValue = 3500;
    WaterValue = 1400;
}

/**
 * @brief Begin the sensor
 *
 * @note This method should be called in the `setup` function.
 *
 */
void CapacitiveWaterLevel::begin()
{
    pinMode(_signalPin, INPUT);
}

/**
 * @brief Read the water level
 *
 * @return int the water level in percentage
 */
int CapacitiveWaterLevel::read()
{
    return map(readRaw(1, _delayTime), AirValue, WaterValue, 0, 100);
}

/**
 * @brief Read the water level
 *
 * @param samples the number of samples to read
 * @return int the water level in percentage
 */
int CapacitiveWaterLevel::read(int samples)
{
    return map(readRaw(samples, _delayTime), AirValue, WaterValue, 0, 100);
}

void CapacitiveWaterLevel::setDelayTime(int delayTime)
{
    _delayTime = delayTime;
}

int CapacitiveWaterLevel::getDelayTime()
{
    return _delayTime;
}

/**
 * @brief Set the Air Value
 *
 * @param AirValue the air value
 *
 * @note The air value is the value when the sensor is in the air.
 */
void CapacitiveWaterLevel::setAirValue(int AirValue)
{
    AirValue = AirValue;
}

int CapacitiveWaterLevel::getAirValue()
{
    return AirValue;
}

/**
 * @brief Set the Water Value
 *
 * @param WaterValue the water value
 *
 * @note The water value is the value when the sensor is in the water.
 */
void CapacitiveWaterLevel::setWaterValue(int WaterValue)
{
    WaterValue = WaterValue;
}

int CapacitiveWaterLevel::getWaterValue()
{
    return WaterValue;
}

/**
 * !!! NOT IMPLEMENTED YET !!!
 */
void CapacitiveWaterLevel::calibrate()
{
    // TODO: implement calibration
}

// private
int CapacitiveWaterLevel::readRaw(int samples, int delayTime)
{
    int value = 0;
    for (int i = 0; i < samples; i++)
    {
        value += analogRead(_signalPin);
        ;
        delay(delayTime);
    }
    return value / samples;
}
