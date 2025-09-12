# Chas Advance - Group 4 - Embedded

Chas Advance is a logistics project, developed by students as part of the education at Chas Academy  
  
This repo is for the IoT parts of the project.  
It consists of two separate codebases  

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
