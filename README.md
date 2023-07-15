# PlantSpeak

PlantSpeak is an IoT project that publishes sensor data from an "ESP-32 Soil Moisture Sensor Module" to ThingSpeak. It collects data related to soil humidity, air temperature, air humidity, voltage, and RSSI, and sends it to ThingSpeak for further analysis and visualization.

This project was inspired by:

- [DIY MORE ESP32 DHT11 Soil Sensor](https://templates.blakadder.com/diymore_012592.html)
- [ESP32 Soil Moisture Sensor](https://judelabs.com/esp32-soil-moisture-sensor)

## Installation

To install and set up PlantSpeak, please follow these steps:

### 1. Create a ThingSpeak Account

If you haven't already, create an account on [ThingSpeak](https://thingspeak.com) to host your sensor data.

### 2. Create a New Channel on ThingSpeak

Create a new channel on ThingSpeak with at least five fields. The default fields for PlantSpeak are as follows:

1. Soil Humidity
2. Air Temperature
3. Air Humidity
4. Voltage
5. RSSI

### 3. Clone the Repository

Clone the PlantSpeak repository to your local machine.

```bash
git clone https://github.com/SamKry/PlantSpeak.git
```

### 4. Adjust the `secrets.h` File in the `src` Directory

Modify the `secrets.h` file in the repository to include your network and ThingSpeak credentials. This file should not be committed to version control, as it contains sensitive information.

### 5. Hardware Setup

Follow the steps below to set up the hardware components for PlantSpeak.

#### 5.1 Solder the Voltage Sensor

![Voltage Sensor Installation](resources/imgs/voltage-sensor.png)

Solder the resistor to the ESP-32 board according to the provided diagram.

![Resistor Installation](https://link.to.your/resistor/image)

#### 5.2 (Optional) Add Solar Panel

![Solar Panel Installation](https://link.to.your/solar/panel/image)

If you want to power the system with a solar panel, solder the solar panel to the ESP-32 board following the provided diagram. A small battersie is sufficient to survive a day, but I recommend using a slightly larger LiPo, as it is not sunny every day in my region.

### 6. Upload Code to the ESP-32

Use your preferred development environment (e.g., Visual Studio Code with the PlatformIO plugin or Arduino IDE) to upload the code to the ESP-32 board.

## Used Libraries

The following libraries are used in the PlantSpeak project:

- [mathworks/thingspeak-arduino @^2.0.0](https://github.com/mathworks/thingspeak-arduino)
- [adafruit/DHT sensor library@^1.4.4](https://github.com/adafruit/DHT-sensor-library)
- [adafruit/Adafruit Unified Sensor@^1.1.9](https://github.com/adafruit/Adafruit_Sensor)

Make sure to install these libraries before uploading the code to the ESP-32.

## Contributing

Contributions to PlantSpeak are welcome! If you have any ideas, improvements, or bug fixes, please open an issue or submit a pull request on this repository.

## License

[MIT License](https://opensource.org/licenses/MIT)
