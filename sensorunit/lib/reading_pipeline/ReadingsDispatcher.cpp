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

bool ReadingsDispatcher::dispatch() {
    LOG_INFO(TAG, "Dispatching readings...");
    DispatchResponse dispatchResponse;
    while (m_readingBuffer.hasReadings()) {
        dispatchResponse = dispatchBatch();
        
        if (dispatchResponse.restStatus != 200) {
            LOG_WARN(TAG, "Rest Server error %d, Aborting dispatch", (dispatchResponse.restStatus));
            return true;
        }
    }
    if (!dispatchResponse.connected) {
        LOG_INFO(TAG, "Sensor Unit received disconnect status");
    }
    return dispatchResponse.connected;
}

DispatchResponse ReadingsDispatcher::dispatchBatch() {
    if (!m_readingBuffer.hasReadings()) {
        LOG_WARN(TAG, "Reading buffer already empty. Aborting dispatchBatch");
        return { -7, true };
    }
    etl::vector<CaSensorunitReading, buffer_config::max_batch_size> batch =
        m_readingBuffer.getBatch();
    size_t batchSize = batch.size();

    etl::string<json_config::max_json_size> payload =
        JsonParser::composeSensorSnapshotGroup(batch, m_sensorUnitId);

    LOG_INFO(TAG, "Dispatching batch with %zu readings", batchSize);
    RestResponse restResponse = m_restClient.postTo("/readings", payload);
    DispatchResponse response { restResponse.status, true } ;

    if (restResponse.status != 200) {
        LOG_WARN(TAG, "Posting batch to /readings failed with status code %d", restResponse.status);
    } else {
        m_readingBuffer.removeBatch(batchSize);
        LOG_INFO(TAG, "%zu readings dispatched and removed from buffer", batchSize);
        response.connected = JsonParser::parseDispatchResponse(restResponse.payload);
    } 

    return response;
}