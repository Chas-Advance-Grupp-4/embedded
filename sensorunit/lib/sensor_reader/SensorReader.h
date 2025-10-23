/**
 * @file SensorReader.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Concrete implementation of ISensorReader that uses a DHT11 sensor
 * and the DHT library by Adafruit to read temperature in celcius and
 * humidity in percent
 * 
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include "ISensorReader.h"
#include "logging.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"

/**
 * @class SensorReader
 * @brief Reads values from a DHT11 sensor
 * This class acts as a wrapper around the DHT library by Adafruit
 * 
 */
class SensorReader : public ISensorReader {
  public:
    /**
     * @brief Construct for the Sensor Reader
     * 
     * @param dht11Pin The Arduino Digital Pin the sensor is connected to. Default value = 2
     */
    SensorReader(int dht11Pin = DHT11_PIN);
    /**
     * @brief The sensor needs to be initialized in setup using this function
     */
    void init();
    /**
     * @brief This function reads values using the DHT11 sensor
     * 
     * @return RawSensorReading containing temperature in celsius and humidity in percent
     */
    RawSensorReading read() override;
  private:
    DHT m_dht11;
    static constexpr const int DHT11_PIN = 2;    
    static constexpr const char* TAG = "SensorReader";
};