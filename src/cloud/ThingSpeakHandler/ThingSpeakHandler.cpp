#include <Arduino.h>
#include <WiFi.h>
#include <cloud/ThingSpeakHandler/ThingSpeakHandler.h>
#include <ThingSpeak.h>

WiFiClient client;
int humidityFild = 1;

/**
 * @brief Construct a new Thing Speak Handler:: Thing Speak Handler object
 *
 * @param chanelNumber the chanel number in ThingSpeak
 * @param APIKey the API key of the chanel
 *
 */
ThingSpeakHandler::ThingSpeakHandler(long chanelNumber, const char *APIKey)
{
    _chanelNumber = chanelNumber;
    _APIKey = APIKey;
    ThingSpeak.begin(client);
}

/**
 * @brief Post the data to ThingSpeak
 *
 * @return int the response code from ThingSpeak.
 *
 * @note The response code should be 200 if the post was successful.
 * `setField` should be called before calling this method.
 *
 * @see https://www.mathworks.com/help/thingspeak/writedata.html
 */
int ThingSpeakHandler::post()
{
    return ThingSpeak.writeFields(_chanelNumber, _APIKey);
}

/**
 * @brief Set the a field value
 *
 * @param fieldNumber the field number in ThingSpeak
 * @param value the value to set as string with max length of 255 bytes
 * @return Code of 200 if successful. Code of -101 if value is out of range or string is too long (> 255 bytes)
 *
 * @note The response code should be 200 if the post was successful.
 * `post` does not publish the data to ThingSpeak. It only sets the data in the library. To publish the data, call `post`.
 */
int ThingSpeakHandler::setField(int fieldNumber, String value)
{
    return ThingSpeak.setField(fieldNumber, value);
}

/**
 * @brief Set the status of the chanel
 *
 * @param status the status to set as string with max length of 255 bytes
 * @return int Code of 200 if successful. Code of -101 if value is out of range or string is too long (> 255 bytes)
 *
 * @note The response code should be 200 if the post was successful.
 * `setStatus` does not publish the data to ThingSpeak. It only sets the data in the library. To publish the data, call `post`.
 */
int ThingSpeakHandler::setStatus(String status)
{
    return ThingSpeak.setStatus(status);
}