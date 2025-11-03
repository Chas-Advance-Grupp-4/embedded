# Sensor Unit (Arduino UNO R4 WiFi)

The `sensorunit/` directory contains the source code for the Arduino-based Sensor Unit, which reads sensor data and sends them to the control unit server  

## Overview

The Sensor Unit handles:

- Reading sensor values temperature and humidity
- Synchronizing the clock and adding timestamps to readings  
- Scheduling internal events  
- Sending readings to control unit through rest client API  
- Searching for available control units and connecting to them  

## Getting Started

Prerequisites:  

- You will need an Arduino UNO R4 WiFi or equivalent board  
- You will need a DHT11 temperature and humidity sensor  

---

- Clone the repository as detailed in root folder [README.md](../README.md)  

- Configure your PlatformIO environment:  
[Install the PlatformIO VS Code extension](https://platformio.org/install/ide?install=vscode)  

- Setup your `sensorunit/include/config.h`  
Copy `config_example.h` and make modifications as needed  

- When working you should start VS Code from the folder `sensorunit/`  

- Build and flash the firmware:

```bash
pio run  
pio run -t upload && pio device monitor --raw
```

For convenience there is a Makefile with commands for building and flashing  
Type `make` in the `sensorunit/` folder for instructions  
