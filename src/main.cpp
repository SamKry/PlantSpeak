#include <Arduino.h>

#include "actors/Led/Led.h"
#include "cloud/ThingSpeakHandler/ThingSpeakHandler.h"
#include "cloud/WifiHandler/WifiHandler.h"
#include "secrets.h"
#include "sensors/CapacitiveWaterLevel/CapacitiveWaterLevel.h"
#include "sensors/DhtSensor/DhtSensor.h"
#include "sensors/Voltage/Voltage.h"

const int NUMBER_OF_READINGS = 10;       // the number of readings to take from each sensor.
const int MAX_RETRIES = 5;               // the number of retries to send data to ThingSpeak.
String hostname = "ESP32-PlantSpeak-1";  // the hostname of the device, used for OTA updates and WiFi connection

long deepSleepTime = 3600L;  // the time to sleep in microsecondsis calculated based on the battery voltage, see
                                   // calculateSleepTime() for more info

Led led(16);  // internal LED

Voltage voltage(35);                  // check README for more info about the voltage sensor installation
CapacitiveWaterLevel waterLevel(32);  // built-in capacitive water level sensor
DhtSensor dhtSensor(22);              // built-in DHT sensor

WifiHandler wifiHandler;
ThingSpeakHandler thingSpeakHandler(SECRET_CH_ID, SECRET_WRITE_API_KEY);  // deined in src/secrets.h
RTC_DATA_ATTR int bootCount;                                              // track boot count
double currentWifiConnectionTime = 0;
const int WIFI_CONNECTION_TIMEOUT = 20;  // the time to wait for a WiFi connection (in seconds)

void setFields() {
  thingSpeakHandler.setField(1, String(waterLevel.getWaterLevel()));
  thingSpeakHandler.setField(2, String(dhtSensor.getTemperature()));
  thingSpeakHandler.setField(3, String(dhtSensor.getHumidity()));
  thingSpeakHandler.setField(4, String(voltage.getVoltage()));
  thingSpeakHandler.setField(5, String(wifiHandler.getRssi()));
  /* add more fields here if needed */
}

String getStatus() {
  return "Boot Count: " + String(bootCount) + ", WiFi connection time: " + String(currentWifiConnectionTime) +
         "s, Sleep time: " + String(deepSleepTime) + "s (" + String((deepSleepTime) / 60) + "min)";
}

void readSensors() {
  dhtSensor.read(NUMBER_OF_READINGS);
  voltage.read(NUMBER_OF_READINGS);
  waterLevel.read(NUMBER_OF_READINGS);
  /* add more sensors here if needed */
}

bool sendDataToThingSpeak(int maxRetries) {
  int postResponse = 0;
  int retries = 0;

  while (postResponse != 200 && retries < maxRetries) {
    setFields();
    thingSpeakHandler.setStatus("Number of retries: " + String(retries) + " - " + getStatus());

    postResponse = thingSpeakHandler.post();
    Serial.println("Post response: " + String(postResponse));

    retries++;
  }
  if (postResponse == 200) {
    led.blinkOK();
    return true;
  } else {
    led.blinkError();
    Serial.println("Failed to send data to ThingSpeak");
  }

  return false;
}

/**
 * @brief Calculates the sleep time based on the battery voltage.
 *
 * The sleep time is calculated using the following formula:
 * f(x)=((((1)/(1+ℯ^(-(4.7 (x-3.3))))))^(25.6) (-59)+60)*60
 *
 * The formula is based on the following graph: https://www.geogebra.org/calculator/sehbwmcd
 *
 * @param batteryVoltage The battery voltage.
 * @return uint64_t The sleep time in seconds. NOTE: Must be converted to microseconds before passing it to
 * esp_sleep_enable_timer_wakeup().
 */
long calcSleepTime(float batteryVoltage) {
  double e = 2.71828;
  return (long)(pow((1.0 / (1.0 + pow(e, -(4.7 * (batteryVoltage - 3.3))))), 25.6) * (-59L) + 60L) * 60L;
}

void setup() {
  // check if voltage is below 4V
  Serial.begin(9600);
  bootCount++;

  if (voltage.read(NUMBER_OF_READINGS) < 4.0) {
    led.blinkError();
    deepSleepTime = 3600L;
    Serial.println("Battery voltage is below 4V");
  } else {
    btStop();

    waterLevel.begin();
    dhtSensor.begin();
    voltage.begin();

    led.turnOff();
    wifiHandler.setConnectionTimeout(WIFI_CONNECTION_TIMEOUT);
    wifiHandler.setHostname(hostname);
    currentWifiConnectionTime = wifiHandler.connect(SECRET_SSID, SECRET_PASS) / 1000.0;
    led.turnOn();

    if (wifiHandler.isConnected()) {
      led.turnOff();

      readSensors();
      deepSleepTime = calcSleepTime(voltage.getVoltage());

      sendDataToThingSpeak(MAX_RETRIES);
    } else {
      led.blinkError();
      Serial.println("Not connected to WiFi");
    }
    led.turnOff();
  }

  Serial.println(getStatus());
  gpio_deep_sleep_hold_en();
  esp_sleep_enable_timer_wakeup(deepSleepTime * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {}
