/**
 * @file ISensorReader.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Interface for Sensor Reader
 * Defines a return type and interface for reading sensor values
 * 
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once

/**
 * @brief Represents the raw results of a sensor reading
 * Note that timestamps are not handled by the SensorReader
 */
struct RawSensorReading {
    float temperature;      /**< Temperature in celsius */
    float humidity;         /**< Humidity as percentage */
};

class ISensorReader {
public:
    virtual RawSensorReading read() = 0;
    virtual ~ISensorReader() = default;
};
