/**
 * @file MockSensorReader.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Mock Sensor Reader
 * Returns static values for testing purposes
 * 
 * @date 2025-10-23
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#pragma once
#include "ISensorReader.h"

class MockSensorReader : public ISensorReader {
  public:
    MockSensorReader(float temperature = 22.5f, float humidity = 45.0f);
    RawSensorReading read() override; 
  private:
    float m_temperature;
    float m_humidity;
};