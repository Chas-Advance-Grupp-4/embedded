# Chas Advance - Group 4 - Embedded

Chas Advance is a logistics project, developed by students as part of the education at Chas Academy  
  
This repo is for the IoT parts of the project.  
It consists of two separate codebases  

## Cloning the repository

This project uses Git submodules. You have to initialize them when cloning.  

### Option 1 Clone and initialize  

```bash
git clone --recurse-submodules https://github.com/Chas-Advance-Grupp-4/embedded
```

### Option 2 If you already cloned without initialization

Navigate to the repo folder and run this in the terminal:  

```bash
git submodule update --init --recursive
```

## 1. Control Unit

`controlunit/` folder  

ESP32 based Gateway that handles connections both with the Backend Server and the Sensor Units.  

Board: Waveshare ESP32-S3-Zero  
Framework: ESP-IDF

## 2. Sensor Unit

`sensorunit/` folder  

Arduino based controller that collects sensor readings and forwards them to the Control Unit.  

Board: Arduino UNO R4 Wifi  
Framework: PlatformIO/Arduino  
