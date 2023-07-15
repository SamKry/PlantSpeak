#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <sensors/DhtSensor.h>

/**
 * @brief Construct a new Dht Sensor:: Dht Sensor object
 *
 * @param signalPin the pin to read the signal from
 */
DhtSensor::DhtSensor(int signalPin)
    : _signalPin(signalPin), _dht(signalPin, DHT11)
{
}

/**
 * @brief Begin the sensor
 *
 */
void DhtSensor::begin()
{
    _dht.begin();
}

/**
 * @brief Read the temperature
 *
 * @param samples the number of samples to read
 */
void DhtSensor::read(int samples)
{
    float temperature = 0;
    float humidity = 0;
    int successfulReads = 0;

    for (int i = 0; i < samples; i++)
    {
        delay(delayTime);

        if (_dht.read())
        {
            float tempValue = _dht.readTemperature();
            float humValue = _dht.readHumidity();

            if (!isnan(tempValue) && !isnan(humValue))
            {
                temperature += tempValue;
                humidity += humValue;
                successfulReads++;
            }
        }
    }

    if (successfulReads > 0)
    {
        _temperature = temperature / successfulReads;
        _humidity = humidity / successfulReads;
    }
    else
    {
        // No successful readings, set temperature and humidity to an invalid value
        _temperature = NAN;
        _humidity = NAN;
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    return;
}

/**
 * @brief Get the Temperature
 *
 * @return float the temperature
 *
 * @note This method does not read the sensor, it returns the last read value.
 * Call `read` to update the value.
 */
float DhtSensor::getTemperature()
{
    return _temperature;
}

/**
 * @brief Get the Humidity
 *
 * @return float the humidity
 *
 * @note This method does not read the sensor, it returns the last read value.
 * Call `read` to update the value.
 */
float DhtSensor::getHumidity()
{
    return _humidity;
}
