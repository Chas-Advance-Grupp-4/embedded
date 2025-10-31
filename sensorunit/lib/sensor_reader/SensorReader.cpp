/**
 * @file SensorReader.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of the SensorReader class
 * 
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "SensorReader.h"

SensorReader::SensorReader(int dht11Pin) : m_dht11 ( dht11Pin, DHT11 )  {}

void SensorReader::init() {
    LOG_INFO(TAG, "Initializing DHT11 SensorReader");
    m_dht11.begin();
}

/**
 * @brief Basic read implementation using the DHT library
 * Suitable for testing purposes
 * 
 * Possible improvements: 
 * - Read multiple samples and return an average
 * - Filter out readings that deviate more than 10% to avoid spikes
 * 
 * @return RawSensorReading containg temperature in celsius and humidity
 */
RawSensorReading SensorReader::read() {
    RawSensorReading reading;
    reading.temperature = m_dht11.readTemperature();
    reading.humidity = m_dht11.readHumidity();
    return reading;
}