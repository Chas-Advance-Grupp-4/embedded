# Library Directory

This folder corresponds to the Component Directory in ESP-IDF. It contains modular components used across the project.

Each subfolder (e.g. `logging/`) is a component. For PlatformIO to find all subdirectories within the folder you need to set `lib_ldf_mode = deep+` in `platformio.ini`  

## Sensor Unit components

`connection_manager`  Handles connections to Sensor Units  
`json_parser`  Parses and composes JSON  
`logging`  ESP-IDF style logging for the Arduino  
`reading_pipeline`  Reading buffer, Reading processor and Readings Dispatcher  
`rest_client`  REST client for control unit communication  
`scheduler`  Handles scheduling events for the unit  
`sensor_reader`  Reading sensor values  
`time_sync_manager`  Synchronizes the clock using the rest client  
