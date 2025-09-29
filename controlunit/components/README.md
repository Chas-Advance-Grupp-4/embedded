# Components Directory

This folder contains modular components used across the ESP-IDF project.

Each subfolder (e.g. `net_utils/`) is a standalone component with its own `CMakeLists.txt`, source files, and headers  
Components are automatically discovered and built by ESP-IDF  

## Structure

`net_utils/` Wi-Fi setup and utilities  
`rest_server/` REST server for backend and sensor unit communication  

To add a new component, create a subfolder with source files and a valid `CMakeLists.txt`
