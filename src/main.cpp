#include <Arduino.h>

#include "actors/Led/Led.h"
#include "cloud/ThingSpeakHandler/ThingSpeakHandler.h"
#include "cloud/WifiHandler/WifiHandler.h"
#include "secrets.h"
#include "sensors/CapacitiveWaterLevel/CapacitiveWaterLevel.h"
#include "sensors/DhtSensor/DhtSensor.h"
#include "sensors/Voltage/Voltage.h"

const int NUMBER_OF_READINGS = 10;       // the number of readings to take from each sensor.
const int WIFI_CONNECTION_TIMEOUT = 20;  // the time to wait for a WiFi connection (in seconds)
int DEEP_SLEEP_TIME;                     // the time to sleep between readings. (in seconds)

Led led(16);  // internal LED

Voltage voltage(35);                  // check README for more info about the voltage sensor installation
CapacitiveWaterLevel waterLevel(32);  // built-in capacitive water level sensor
DhtSensor dhtSensor(22);              // built-in DHT sensor

WifiHandler wifiHandler;
ThingSpeakHandler thingSpeakHandler(SECRET_CH_ID, SECRET_WRITE_API_KEY);  // deined in src/secrets.h
RTC_DATA_ATTR int bootCount;                                              // track boot count
double currentWifiConnectionTime = 0;

void setFields() {
  thingSpeakHandler.setField(1, String(waterLevel.getWaterLevel()));
  thingSpeakHandler.setField(2, String(dhtSensor.getTemperature()));
  thingSpeakHandler.setField(3, String(dhtSensor.getHumidity()));
  thingSpeakHandler.setField(4, String(voltage.getVoltage()));
  thingSpeakHandler.setField(5, String(wifiHandler.getRssi()));
  /* add more fields here if needed */
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
    thingSpeakHandler.setStatus("Boot Count: " + String(bootCount) + ", Number of retries: " + String(retries) + ", WiFi connection time: " + String(currentWifiConnectionTime) + "s, Sleep time: " + String(DEEP_SLEEP_TIME) + "s");
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

float calcSleepTime(float batteryVoltage) {
  double e = 2.71828;
  return (pow((1.0 / (1.0 + pow(e, -(4.7 * (batteryVoltage - 3.3))))), 25.6) * (-19) + 20) * 60;
}

void setup() {
  bootCount++;
  Serial.begin(9600);

  btStop();

  waterLevel.begin();
  dhtSensor.begin();
  voltage.begin();

  led.turnOff();
  wifiHandler.setConnectionTimeout(WIFI_CONNECTION_TIMEOUT);
  currentWifiConnectionTime = wifiHandler.connect(SECRET_SSID, SECRET_PASS) / 1000.0;
  led.turnOn();
}

void loop() {
  if (wifiHandler.isConnected()) {
    led.turnOff();

    readSensors();

    sendDataToThingSpeak(NUMBER_OF_READINGS);
  } else {
    led.blinkError();
    Serial.println("Not connected to WiFi");
  }
  led.turnOff();

  DEEP_SLEEP_TIME = calcSleepTime(voltage.getVoltage());
  Serial.println("Going to sleep for " + String(DEEP_SLEEP_TIME) + " seconds");
  gpio_deep_sleep_hold_en();
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 1000000);
  esp_deep_sleep_start();
}
