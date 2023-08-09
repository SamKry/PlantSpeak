#include <Arduino.h>

#include "actors/Led.h"
#include "cloud/ThingSpeakHandler.h"
#include "cloud/WifiHandler.h"
#include "secrets.h"
#include "sensors/CapacitiveWaterLevel.h"
#include "sensors/DhtSensor.h"
#include "sensors/Voltage.h"

const int NUMBER_OF_READINGS = 10;  // the number of readings to take from each sensor.
int DEEP_SLEEP_TIME;                // the time to sleep between readings. (in seconds)

Led led(16);                          // internal LED
Voltage voltage(35);                  // check README for more info about the voltage sensor installation
CapacitiveWaterLevel waterLevel(32);  // built-in capacitive water level sensor
DhtSensor dhtSensor(22);              // built-in DHT sensor

WifiHandler wifiHandler;
ThingSpeakHandler thingSpeakHandler(SECRET_CH_ID, SECRET_WRITE_API_KEY);  // deined in src/secrets.h
RTC_DATA_ATTR int bootCount;                                              // track boot count
double wifiConnectionTime = 0;

void setFields() {
  thingSpeakHandler.setField(1, String(waterLevel.read(NUMBER_OF_READINGS)));
  thingSpeakHandler.setField(2, String(dhtSensor.getTemperature()));
  thingSpeakHandler.setField(3, String(dhtSensor.getHumidity()));
  thingSpeakHandler.setField(4, String(voltage.read(NUMBER_OF_READINGS)));
  thingSpeakHandler.setField(5, String(wifiHandler.getRssi()));
  /* add more fields here if needed */
}

bool sendDataToThingSpeak(int maxRetries) {
  int postResponse = 0;
  int retries = 0;

  while (postResponse != 200 && retries < maxRetries) {
    setFields();
    thingSpeakHandler.setStatus("Boot Count: " + String(bootCount) + ", Number of retries: " + String(retries) + ", WiFi connection time: " + String(wifiConnectionTime) + "s, Sleep time: " + String(DEEP_SLEEP_TIME) + "s");
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
 * @brief Sets the deep sleep time in Seconds based on the voltage sensor reading.
 * 1 minute if over 4.1V, 20 minutes if under 2.4V.
 */
void setDEEP_SLEEP_TIME(float _voltage) {
  double e = 2.71828;
  DEEP_SLEEP_TIME = (pow((1.0 / (1.0 + pow(e, -(4.7 * (_voltage - 3.3))))), 25.6) * (-19) + 20) * 60;
}

void setup() {
  bootCount++;
  Serial.begin(9600);

  btStop();

  waterLevel.begin();
  dhtSensor.begin();
  voltage.begin();

  led.turnOff();
  wifiConnectionTime = wifiHandler.connect(SECRET_SSID, SECRET_PASS, NUMBER_OF_READINGS * 2) / 1000.0;
  led.turnOn();
}

void loop() {
  if (wifiHandler.isConnected()) {
    led.turnOff();

    dhtSensor.read(NUMBER_OF_READINGS);

    sendDataToThingSpeak(NUMBER_OF_READINGS);
  } else {
    led.blinkError();
    Serial.println("Not connected to WiFi");
  }
  led.turnOff();

  setDEEP_SLEEP_TIME(voltage.read(NUMBER_OF_READINGS));
  Serial.println("Going to sleep for " + String(DEEP_SLEEP_TIME) + " seconds");
  gpio_deep_sleep_hold_en();
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 1000000);
  esp_deep_sleep_start();
}
