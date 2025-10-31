/**
 * @file ReadingsDispatcher.h
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Class for dispatching batches of sensor readings
 * from the Reading buffer and sending them through Rest Client
 *
 * @date 2025-10-24
 *
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 *
 */
#pragma once
#include "IReadingsDispatcher.h"
#include "IRestClient.h"
#include "ReadingBuffer.h"
#include "config.h"

struct DispatchResponse {
    int restStatus;
    bool connected;
};

class ReadingsDispatcher : public IReadingsDispatcher {
  public:
    /**
     * @brief Construct a new Readings Dispatcher object
     *
     * @param restClient reference to RestClient interface
     * @param readingBuffer reference to concrete readingBuffer
     * @param sensorUnitId sensorUnitId is set in config.h
     */
    ReadingsDispatcher(IRestClient&   restClient,
                       ReadingBuffer& readingBuffer,
                       const char*    sensorUnitId = SENSOR_UNIT_ID);
    /**
     * @brief Function for dispatching sensor readings
     * Max batch size is set in ReadingBuffer.h
     * If buffer is larger than that dispatch will POST
     * several batches of max_batch_size until buffer is empty
     *
     * @returns false if response payload disconnected
     * @returns true otherwise
     */
    bool dispatch() override;

  private:
    /**
     * @brief Dispatch a single batch of max_batch_size
     * max_natch_size is set in ReadingBuffer.h
     *
     * @return ConnectResponse with restStatus
     * restStatus 200 means successful POST
     * bool connected used for passing on disconnect message from backend
     */
    DispatchResponse             dispatchBatch();
    IRestClient&                 m_restClient;
    ReadingBuffer&               m_readingBuffer;
    const char*                  m_sensorUnitId;
    static constexpr const char* TAG = "ReadingsDispatcher";
};