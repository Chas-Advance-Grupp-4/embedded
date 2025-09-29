# ESP32 Control Unit

This directory contains the source code for the ESP32-based Control Unit, which acts as a gateway between sensor units and a backend server. It provides a lightweight web server interface for handling RESTful API requests and managing sensor data.

## Overview

The Control Unit is responsible for:

- Establishing and maintaining connections with multiple sensor units.
- Communicating with a backend server to relay sensor data and receive commands.
- Hosting a RESTful web server to expose endpoints for diagnostics, configuration, and data access.

## Example Endpoints

- `GET /hello` — Simple greeting endpoint
- `POST /postexample` — Example POST endpoint

## Getting Started

1. Clone the repository and navigate to `controlunit/`.
2. Configure your ESP-IDF environment (more details coming soon)
3. Build and flash the firmware:

```bash
source /your-installed-path-to/esp/v5.5.1/esp-idf/export.sh
idf.py build
idf.py flash monitor
```
