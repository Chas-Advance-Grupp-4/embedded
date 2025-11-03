/**
 * @file ReadingProcessor.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of the ReadingProcessor class
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#include "ReadingProcessor.h"
#include "logging.h"
#include "sensor_data_types.h"

ReadingProcessor::ReadingProcessor(ISensorReader&    sensorReader,
                                   ITimeSyncManager& timeSyncManager,
                                   ReadingBuffer&    readingBuffer)
    : m_sensorReader(sensorReader), m_timeSyncManager(timeSyncManager),
      m_readingBuffer(readingBuffer) {}

void ReadingProcessor::process() {
    LOG_INFO(TAG, "Processing Sensor Reading");
    if (!m_timeSyncManager.isTimeSynced()) {
        LOG_WARN(TAG, "Time is not synced. Aborting reading");
        return;
    }
    uint32_t         timestamp  = m_timeSyncManager.getUnixTimeNow();
    RawSensorReading rawReading = m_sensorReader.read();
    LOG_INFO(
        TAG, "Temperature: %.2f°C Humidity: %.0f%%", rawReading.temperature, rawReading.humidity);
    m_readingBuffer.push(
        CaSensorunitReading{timestamp, rawReading.temperature, rawReading.humidity});
    LOG_INFO(TAG, "Reading added to SensorBuffer with timestamp %lu", timestamp);
}
