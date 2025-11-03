# Control Unit (ESP32-S3-Zero)

The `controlunit/` directory contains the source code for the ESP32-based Control Unit, which acts as a gateway between sensor units and a backend server. It provides a lightweight web server interface for handling RESTful API requests and managing sensor data.

## Overview

The Control Unit is responsible for:

- Setting up a local AP (Access Point) for Sensor Units
- Hosting a RESTful web server
- Communicating with a backend server to relay sensor data and receive commands.
- Maintaining a backend orchestrated list of connected Sensor Units

## Getting Started

Prerequisites: You will need a ESP32-S3-Zero or equivalent board  

---

- Clone the repository as detailed in root folder [README.md](../README.md)  

- Configure your ESP-IDF environment:  
If you're on mac or linux some [prerequisites need to be met](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html)  
Then [install the ESP-IDF VS Code extension](https://github.com/espressif/vscode-esp-idf-extension/blob/master/README.md)  

- Setup your `controlunit/components/net_utils/wifi_config.h`  
Copy `wifi_config_example.h` and make modifications as needed  

- When working you should start VS Code from the folder `controlunit/`  

- You will need to source `idf.py` every time you open the project:  
navigate to `controlunit/`  
`source /your-installed-path-to/esp/v5.5.1/esp-idf/export.sh`   

- When setting up you need to edit some settings in menuconfig  
If you haven't source `idf.py` then run `idf.py menuconfig`

Edit these settings:  

```bash
-> Component Config  
  -> mbedTLS  
    -> Certificate Bundle  
      [*] Enable trusted root certificate bundle  
```

```bash
-> Component Config  
  -> mbedTLS  
    -> Certificate Bundle  
      -> Default certificate bundle options  
        [*] Use only the most common certificates from the default bundles  
```

- Build and flash the firmware:

```bash
idf.py build
idf.py flash monitor
```

For convenience there is a Makefile with commands for building and flashing  
Type `make` in the `controlunit/` folder for instructions  

---

## Endpoints available for Sensor Unit

- `GET /time` - Sync clock  
- `POST /connect` - Connect the Sensor Unit  
- `POST /readings` - Post readings  

Details about the embedded [API](../API.md)  
