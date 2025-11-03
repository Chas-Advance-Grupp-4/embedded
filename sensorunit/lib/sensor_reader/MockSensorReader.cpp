#include "MockSensorReader.h"

MockSensorReader::MockSensorReader(float temperature, float humidity) 
: m_temperature ( temperature ), m_humidity ( humidity) {}

RawSensorReading MockSensorReader::read()
{
        return { .temperature = m_temperature, .humidity = m_humidity };
}