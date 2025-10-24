/**
 * @file ReadingsDispatcher.cpp
 * @author Erik Dahl (erik@iunderlandet.se)
 * @brief Implementation of ReadingsDispatcher class
 * 
 * @date 2025-10-24
 * 
 * @copyright Copyright (c) 2025 Erik Dahl
 * @license MIT
 * 
 */
#include "ReadingsDispatcher.h"
#include "JsonParser.h"
#include "logging.h"

ReadingsDispatcher::ReadingsDispatcher(IRestClient&   restClient,
                                       ReadingBuffer& readingBuffer,
                                       const char*    sensorUnitId)
    : m_restClient(restClient), m_readingBuffer(readingBuffer), m_sensorUnitId(sensorUnitId) {}

void ReadingsDispatcher::dispatch() {
    LOG_INFO(TAG, "Dispatching readings...");
    while (m_readingBuffer.hasReadings()) {
        int restResponse = dispatchBatch();
        if (restResponse != 200) {
            LOG_WARN(TAG, "Rest Server error %d, Aborting dispatch", restResponse);
            break;
        }
    }
}

int ReadingsDispatcher::dispatchBatch() {
    if (!m_readingBuffer.hasReadings()) {
        LOG_WARN(TAG, "Reading buffer already empty. Aborting dispatchBatch");
        return -7;
    }
    etl::vector<CaSensorunitReading, buffer_config::max_batch_size> batch =
        m_readingBuffer.getBatch();
    size_t batchSize = batch.size();

    etl::string<json_config::max_json_size> payload =
        JsonParser::composeSensorSnapshotGroup(batch, m_sensorUnitId);

    LOG_INFO(TAG, "Dispatching batch with %zu readings", batchSize);
    RestResponse response = m_restClient.postTo("/readings", payload);

    if (response.status != 200) {
        LOG_WARN(TAG, "Posting batch to /readings failed with status code %d", response.status);
    } else {
        m_readingBuffer.removeBatch(batchSize);
        LOG_INFO(TAG, "%zu readings dispatched and removed from buffer", batchSize);
    }

    return response.status;
}