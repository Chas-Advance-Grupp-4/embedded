# ESP32 Control Unit

This directory contains the source code for the ESP32-based Control Unit, which acts as a gateway between sensor units and a backend server. It provides a lightweight web server interface for handling RESTful API requests and managing sensor data.

## Overview

The Control Unit is responsible for:

- Establishing and maintaining connections with multiple sensor units.
- Hosting a RESTful web server over a local AP (Access Point)
- Communicating with a backend server to relay sensor data and receive commands.

## Endpoints available for Sensor Unit

- `GET /time` - Sync clock  
- `POST /connect` - Connect the Sensor Unit  
- `POST /readings` - Post readings  

## Getting Started

Clone the repository and navigate to `controlunit/`  

Configure your ESP-IDF environment (more details coming soon)  

Setup your wifi_config.h i net_utils  
   Copy wifi_config_example.h and make modifications as needed  

Build and flash the firmware:

```bash
source /your-installed-path-to/esp/v5.5.1/esp-idf/export.sh
idf.py build
idf.py flash monitor
```
