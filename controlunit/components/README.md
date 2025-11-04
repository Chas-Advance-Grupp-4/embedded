# Components Directory

This folder contains modular components used across the ESP-IDF project.

Each subfolder (e.g. `net_utils/`) is a standalone component with its own `CMakeLists.txt`, source files, and headers  
Components are automatically discovered and built by ESP-IDF  

## Control Unit components

`connection_data`  Data types for connection communication  
`control_unit_manager`  Holds all system state  
`json_parser`  Parses and composes JSON  
`mock_data`  Generates mocked readings for testing  
`net_utils`  Wi-Fi setup and utilities  
`readings_dispatcher`  Trigger and task for sending readings to backend  
`rest_client`  REST client for backend communication  
`rest_server`  REST server for sensor unit communication  
`sensor_data`  data types for storing sensor readings  
`sensor_unit_link_syncer`  Trigger and task for status polling  
`sensor_unit_manager`  Holds all sensor readings and sensor unit state  
`time_sync_manager`  Handles time synchronization with SNTP  
