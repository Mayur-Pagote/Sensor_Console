# Sensor Console

**A Plug-and-Play STEM Learning Platform for School Education Using NodeMCU ESP8266**

## Overview

Sensor Console is an open-source educational hardware platform designed to promote experiential STEM learning through real-world sensor experimentation.

Built around the NodeMCU ESP8266 microcontroller, the platform combines a modular sensor architecture, OLED-based graphical interface, and plug-and-play sensor connectivity to create an affordable and classroom-friendly learning system.

The platform enables students and educators to perform practical experiments without requiring programming knowledge, external computers, or complex laboratory equipment.



## Key Features

* Standalone operation without a computer
* OLED-based interactive user interface
* Plug-and-play sensor architecture
* Runtime sensor detection and validation
* Modular JST sensor interface
* Open-source hardware and firmware
* Low-cost implementation
* Designed for STEM and CBSE-aligned education
* Expandable architecture for future sensor modules



## Supported Experiments

### Distance Measurement

Uses the HC-SR04 ultrasonic sensor to measure object distance using time-of-flight principles.


### Heart Rate Monitoring

Uses a photoplethysmography (PPG) pulse sensor to measure heart rate in beats per minute (BPM).


### Soil Moisture Analysis

Uses a capacitive soil moisture sensor to estimate soil water content.


# Hardware Components

| Component                       | Quantity |
| ------------------------------- | -------- |
| NodeMCU ESP8266 (ESP-12E)       | 1        |
| SSD1306 OLED Display (128×64)   | 1        |
| Tactile Push Buttons            | 2        |
| HC-SR04 Ultrasonic Sensor       | 1        |
| Pulse Sensor                    | 1        |
| Capacitive Soil Moisture Sensor | 1        |
| 6-Pin JST Connectors            | 4        |
| Breadboard                      | 2        |
| Jumper Wires                    | Assorted |
| Micro-USB Cable                 | 1        |
| Li-Po Battery                   | 1        |



# Getting Started

## Software Requirements

* Arduino IDE 2.x
* ESP8266 Board Package

## Required Libraries

```text
Adafruit GFX Library
Adafruit SSD1306 Library
Wire Library
```

## Firmware Upload

1. Install Arduino IDE.
2. Install ESP8266 Board Support Package.
3. Install required libraries.
4. Open `sensor_console.ino`.
5. Select:

```text
Board: NodeMCU 1.0 (ESP-12E Module)
```

6. Connect the board via Micro-USB.
7. Compile and upload.


# User Interface

The system uses two push buttons:

| Button | Function        |
| ------ | --------------- |
| D5     | Menu Navigation |
| D6     | Select / Back   |

Main Menu:

```text
1. Measure Distance
2. Measure Heart Rate
3. Measure Soil Moisture
```

The firmware automatically checks whether the correct sensor is connected before starting an experiment.



# Future Development

Planned enhancements include:

* DHT22 Temperature & Humidity Sensor
* pH Sensor Module
* UV Sensor Module
* CO₂ Monitoring Module
* ESP32-S3 Upgrade
* Bluetooth Connectivity
* Wi-Fi Data Logging
* Mobile Application Integration
* Custom PCB Design
* Multilingual Interface Support



# License

This project is released under the MIT License.



# Author

**Mayur Pagote**

GitHub: https://github.com/Mayur-Pagote


# Acknowledgements

This project was developed to support affordable and accessible STEM education through practical experimentation and open-source hardware. The project aims to encourage hands-on learning, engineering exploration, and scientific inquiry in schools and educational institutions.
