/**
 * @file ReadingProcessor.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for processing sensor readings
 * Calling process adds a timestamp to a raw reading and pushes it to the buffer
 *
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "ISensorReader.h"
#include "ITimeSyncManager.h"
#include "ReadingBuffer.h"

class ReadingProcessor {
  public:
    /**
     * @brief Constructor for ReadingProcessor
     *
     * @param sensorReader reference to ISensorReader interface
     * @param timeSyncManager reference to ITimeSyncManager interface
     * @param readingBuffer reference to concrete ReadingBuffer
     */
    ReadingProcessor(ISensorReader&    sensorReader,
                     ITimeSyncManager& timeSyncManager,
                     ReadingBuffer&    readingBuffer);
    /**
     * @brief This function combines the following functionality:
     *   - get a raw sensor reading
     *   - add a Unix timestamp
     *   - push resulting reading to sensor buffer
     */
    void process();

  private:
    ISensorReader&               m_sensorReader;
    ITimeSyncManager&            m_timeSyncManager;
    ReadingBuffer&               m_readingBuffer;
    static constexpr const char* TAG = "ReadingProcessor";
};
